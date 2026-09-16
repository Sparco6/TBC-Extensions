#include <Main.hpp>
#include <Client/FrameScript.hpp>
#include <Offsets/ClientOffsets_8606.hpp>
#include <Graphics/Textures/BLPRuntimeTrace.hpp>
#include <Graphics/Textures/NativeBLPCorrelation.hpp>
#include <MinHook.h>

namespace
{
HMODULE g_module = nullptr;
CustomLua::SafeRegistrar g_originalRegister = nullptr;
lua_State* g_registeredState = nullptr;
using ValidateFunction = void(__cdecl*)(std::uintptr_t);
ValidateFunction g_originalValidate = nullptr;
constexpr bool EnableNativeLuaCallbacks = true;

void DiagnosticLog(HMODULE module, const char* message)
{
    char path[MAX_PATH] = {};
    const DWORD length = GetModuleFileNameA(module, path, MAX_PATH);
    if (length == 0 || length >= MAX_PATH)
        return;
    for (DWORD i = length; i > 0; --i)
    {
        if (path[i - 1] == '\\' || path[i - 1] == '/')
        {
            path[i] = '\0';
            break;
        }
    }
    if (strlen(path) + strlen("TBCExtensions.log") + 1 >= MAX_PATH)
        return;
    strcat_s(path, "TBCExtensions.log");
    HANDLE file = CreateFileA(path, FILE_APPEND_DATA, FILE_SHARE_READ | FILE_SHARE_WRITE,
        nullptr, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
    if (file == INVALID_HANDLE_VALUE)
        return;
    SYSTEMTIME now{};
    GetLocalTime(&now);
    char line[256] = {};
    const int written = sprintf_s(line, "[%04u-%02u-%02u %02u:%02u:%02u] %s\r\n",
        now.wYear, now.wMonth, now.wDay, now.wHour, now.wMinute, now.wSecond, message);
    DWORD ignored = 0;
    if (written > 0)
        WriteFile(file, line, static_cast<DWORD>(written), &ignored, nullptr);
    CloseHandle(file);
}

int32_t __cdecl RegisterFunctionLifecycle(const char* name, void* callback)
{
    const int32_t result = g_originalRegister(name, callback);
    lua_State* state = FrameScript::GetState();
    bool missing = false;
    if (state != nullptr)
    {
        const int top = FrameScript::GetTop(state, 0);
        FrameScript::GetField(state, -10002, "TBCExt_GetVersion");
        missing = FrameScript::LuaType(state, -1) != 6;
        FrameScript::SetTop(state, top);
    }
    if (state != nullptr && (state != g_registeredState || missing))
    {
        g_registeredState = state;
        DiagnosticLog(g_module, "Lua bridge refresh: changed state or missing global on FrameScript thread");
        CustomLua::ApplySafeResearchApiWithRegistrar(g_originalRegister);
        DiagnosticLog(g_module, "Safe Lua API registered into new Lua state");
    }
    return result;
}

void __cdecl ValidateFunctionPointer(std::uintptr_t callback)
{
    if (CustomLua::IsSafeResearchCallback(callback))
        return;
    g_originalValidate(callback);
}

bool HasBytes(std::uintptr_t address, const unsigned char* expected, std::size_t count)
{
    return memcmp(reinterpret_cast<const void*>(address), expected, count) == 0;
}

DWORD WINAPI InstallLuaLifecycleHook(LPVOID)
{
    void* target = reinterpret_cast<void*>(Offsets8606::FrameScriptRegisterFunction);
    void* validator = reinterpret_cast<void*>(Offsets8606::ValidateFunctionPointer);
    static const unsigned char registerPrologue[] = {0x55,0x8B,0xEC,0x56,0xE8,0x07,0xFE,0xFF,0xFF};
    static const unsigned char validatorPrologue[] = {0x55,0x8B,0xEC,0x83,0xEC,0x44,0xA1,0x30,0xF5,0x90,0x00};
    if (!HasBytes(Offsets8606::FrameScriptRegisterFunction, registerPrologue, sizeof(registerPrologue)) ||
        !HasBytes(Offsets8606::ValidateFunctionPointer, validatorPrologue, sizeof(validatorPrologue)))
    {
        DiagnosticLog(g_module, "Lua bridge signatures do not match build 8606; hooks disabled");
        return 0;
    }
    MH_STATUS status = MH_Initialize();
    if (status != MH_OK && status != MH_ERROR_ALREADY_INITIALIZED)
    {
        DiagnosticLog(g_module, "Failed to initialize Lua lifecycle hook");
        return 0;
    }
    status = MH_CreateHook(validator, reinterpret_cast<void*>(&ValidateFunctionPointer),
        reinterpret_cast<void**>(&g_originalValidate));
    if (status != MH_OK)
    {
        DiagnosticLog(g_module, "Failed to create callback validator hook");
        return 0;
    }
    status = MH_CreateHook(target, reinterpret_cast<void*>(&RegisterFunctionLifecycle),
        reinterpret_cast<void**>(&g_originalRegister));
    if (status != MH_OK)
    {
        DiagnosticLog(g_module, "Failed to create Lua lifecycle hook");
        return 0;
    }
    const bool observationCreated = BlpRuntimeTrace::CreateObservationHook();
    const bool correlationCreated = NativeBLPCorrelation::CreateHooks();
    if (!correlationCreated)
        DiagnosticLog(g_module, "Native BLP correlation signatures/sections did not qualify; stock path unchanged");
    if (!observationCreated)
        DiagnosticLog(g_module, "BLP observation prologue/section did not qualify; stock path unchanged");
    // The client can rebuild the global table without replacing lua_State.
    // Force exactly one install on the first registration after enabling.
    g_registeredState = nullptr;
    status = MH_EnableHook(MH_ALL_HOOKS);
    BlpRuntimeTrace::SetEnabled(status == MH_OK && observationCreated);
    NativeBLPCorrelation::SetEnabled(status == MH_OK && correlationCreated);
    if (status == MH_OK)
    {
        DiagnosticLog(g_module, "Lua lifecycle and exact-callback validator hooks enabled; next registration will refresh API");
        if (observationCreated)
            DiagnosticLog(g_module, "BLP one-shot observation hook enabled but unarmed; runtime replacement disabled");
    }
    else
    {
        DiagnosticLog(g_module, "Failed to enable Lua bridge hooks");
    }
    return 0;
}

}

void Main::OnAttach()
{
#if !UNSAFE_LEGACY_PORT
    // The injection thread can run before FrameScript has created its state.
    // Wait only for that confirmed pointer; do not scan or patch client memory.
    for (int attempt = 0; attempt < 600 && FrameScript::GetState() == nullptr; ++attempt)
        Sleep(100);
    if (FrameScript::GetState() == nullptr)
        return;
#endif
    Init();

#if UNSAFE_LEGACY_PORT
    // WoWTime patches - fixes time-related bugs (non-optional)
    WoWTime::ApplyWoWTimePatches();

    // Prevents crashes from missing M2 model files
    CMap::Apply();

    // Optional patches
    Misc::ApplyPatches();
    CGPlayer::ApplyPatches();
    CGTooltip::ApplyPatches();
    Spell::ApplyPatches();

    // Custom DBC loader (no LFGRoles or ZoneLight in TBC)
#if CUSTOM_DBC
    CDBCMgr::PatchAddress();
#endif
#endif
}

void Main::Init()
{
#if UNSAFE_LEGACY_PORT
    DataContainer& dc = DataContainer::GetInstance();
    dc.SetYearOffsetMultiplier();

#if CUSTOMPACKETS_PATCH
    CNetClient::Apply();
#endif

    // TODO_TBC: Replace 0xD415B8 / 0xD415BC with TBC 2.4.3 invalid function pointer hack addresses
    // WotLK 3.3.5 addresses were: 0xD415B8, 0xD415BC
    // How to find: Look for FrameScript__LoadFunctions bootstrap area (AwesomeWotLK technique)
#if OOBLUAFUNCTIONS_PATCH || CUSTOM_DBC || CUSTOMPACKETS_PATCH
    *reinterpret_cast<uint32_t*>(0x000000 /* TODO_TBC */) = 1;
    *reinterpret_cast<uint32_t*>(0x000004 /* TODO_TBC */) = 0x7FFFFFFF;
#endif

#if OOBLUAFUNCTIONS_PATCH || CUSTOMPACKETS_PATCH
    CustomLua::Apply();
#endif
#else
    CustomLua::ApplySafeResearchApi();
#endif
}

static bool __stdcall DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpReserved)
{
    if (fdwReason == DLL_PROCESS_ATTACH)
    {
        DisableThreadLibraryCalls(hinstDLL);
        g_module = hinstDLL;
        DiagnosticLog(g_module, "DLL loaded on loader thread");
        if (!CustomLua::CaptureClientFingerprint())
        {
            DiagnosticLog(g_module, "Client fingerprint mismatch: safe Lua bridge disabled; no registration or hooks attempted");
            return true;
        }
        DiagnosticLog(g_module, "API v7 / 0.1.7 fingerprint VALID: native BLP correlation unarmed; runtime texture transform disabled");
        if (!EnableNativeLuaCallbacks)
        {
            DiagnosticLog(g_module, "Native Lua callbacks quarantined after unsafe dispatch was observed");
        }
        else if (FrameScript::GetState() != nullptr)
        {
            DiagnosticLog(g_module, "Lua state ready on loader thread; registering safe API");
            Main::Init();
            g_registeredState = FrameScript::GetState();
            DiagnosticLog(g_module, "Safe Lua API registration returned");
        }
        else
        {
            DiagnosticLog(g_module, "Lua state was null at DLL load; registration skipped");
        }
        if (EnableNativeLuaCallbacks)
        {
            HANDLE worker = CreateThread(nullptr, 0, InstallLuaLifecycleHook, nullptr, 0, nullptr);
            if (worker != nullptr)
                CloseHandle(worker);
        }
    }

    return true;
}
