/**
 * test_addresses.cpp
 * ==================
 * Minimal test harness to verify that TODO_TBC addresses are correct.
 *
 * Compile as a standalone DLL and inject it into WoW.exe (TBC 2.4.3 build 8606)
 * using your preferred DLL injector (e.g. Process Hacker, RemoteDLL).
 *
 * The DLL attempts to call each patched function with safe arguments and logs
 * whether the call succeeded (no crash) or failed (access violation).
 *
 * Build (MSVC, x86):
 *   cl /nologo /W3 /O2 /LD /MD /Fe:test_addresses.dll test_addresses.cpp
 *
 * Build (MinGW/GCC, x86 cross):
 *   i686-w64-mingw32-g++ -shared -o test_addresses.dll test_addresses.cpp
 *
 * NOTE: This file intentionally does NOT include the rest of TBC-Extensions.
 *       Add addresses directly in the ADDR_* constants below, then inject.
 */

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <cstdio>
#include <cstdint>
#include <cstring>

// ---------------------------------------------------------------------------
// Replace TODO values with the TBC 2.4.3 addresses you want to test.
// ---------------------------------------------------------------------------

// CRITICAL
static const uintptr_t ADDR_FrameScript__LoadFunctions    = 0x000000; // TODO_TBC
static const uintptr_t ADDR_FrameScript__RegisterFunction = 0x000000; // TODO_TBC
static const uintptr_t ADDR_FrameScript__GetState         = 0x000000; // TODO_TBC (pointer to lua_State*)
static const uintptr_t ADDR_CVar__Set                     = 0x000000; // TODO_TBC
static const uintptr_t ADDR_SErr__PrepareAppFatal         = 0x000000; // TODO_TBC

// HIGH — Lua C-API
static const uintptr_t ADDR_lua_gettop      = 0x000000; // TODO_TBC
static const uintptr_t ADDR_lua_settop      = 0x000000; // TODO_TBC
static const uintptr_t ADDR_lua_type        = 0x000000; // TODO_TBC
static const uintptr_t ADDR_lua_pushnil     = 0x000000; // TODO_TBC
static const uintptr_t ADDR_lua_pushnumber  = 0x000000; // TODO_TBC
static const uintptr_t ADDR_lua_pushstring  = 0x000000; // TODO_TBC
static const uintptr_t ADDR_lua_toboolean   = 0x000000; // TODO_TBC
static const uintptr_t ADDR_lua_tonumber    = 0x000000; // TODO_TBC
static const uintptr_t ADDR_lua_tolstring   = 0x000000; // TODO_TBC

// MEDIUM
static const uintptr_t ADDR_SStr__Printf              = 0x000000; // TODO_TBC
static const uintptr_t ADDR_CGChat__AddChatMessage    = 0x000000; // TODO_TBC

// ---------------------------------------------------------------------------
// Logging helpers
// ---------------------------------------------------------------------------

static FILE* g_log = nullptr;

static void log_open()
{
    g_log = fopen("C:\\tbc_addr_test.log", "w");
    if (!g_log) g_log = stderr;
    fprintf(g_log, "=== TBC Address Test Harness ===\n\n");
    fflush(g_log);
}

static void log_close()
{
    if (g_log && g_log != stderr) fclose(g_log);
    g_log = nullptr;
}

#define LOG(fmt, ...) \
    do { if (g_log) { fprintf(g_log, fmt "\n", ##__VA_ARGS__); fflush(g_log); } } while (0)

// ---------------------------------------------------------------------------
// Structured Exception Handler — catches access violations during tests
// ---------------------------------------------------------------------------

static bool call_and_catch(const char* label, void (*fn)())
{
    __try {
        fn();
        LOG("[PASS] %s", label);
        return true;
    }
    __except (EXCEPTION_EXECUTE_HANDLER) {
        LOG("[FAIL] %s  (SEH 0x%08X)", label, GetExceptionCode());
        return false;
    }
}

// ---------------------------------------------------------------------------
// Individual address tests
// ---------------------------------------------------------------------------

static void test_FrameScript__GetState()
{
    if (!ADDR_FrameScript__GetState) { LOG("[SKIP] FrameScript__GetState — address not set"); return; }
    // Read the lua_State pointer (it is a global pointer, not a function)
    void* pState = *reinterpret_cast<void**>(ADDR_FrameScript__GetState);
    if (pState) {
        LOG("[PASS] FrameScript__GetState → lua_State* = 0x%p (non-null)", pState);
    } else {
        LOG("[WARN] FrameScript__GetState → lua_State* is NULL (is the game fully loaded?)");
    }
}

static void test_lua_gettop()
{
    if (!ADDR_lua_gettop || !ADDR_FrameScript__GetState) {
        LOG("[SKIP] lua_gettop — dependency address not set"); return;
    }
    // lua_gettop: int __cdecl lua_gettop(lua_State *L)
    using fn_t = int(__cdecl*)(void*);
    void* L = *reinterpret_cast<void**>(ADDR_FrameScript__GetState);
    if (!L) { LOG("[SKIP] lua_gettop — lua_State* is NULL"); return; }

    __try {
        int top = reinterpret_cast<fn_t>(ADDR_lua_gettop)(L);
        LOG("[PASS] lua_gettop → stack top = %d", top);
    }
    __except (EXCEPTION_EXECUTE_HANDLER) {
        LOG("[FAIL] lua_gettop (SEH 0x%08X)", GetExceptionCode());
    }
}

static void test_lua_pushnumber()
{
    if (!ADDR_lua_pushnumber || !ADDR_lua_gettop || !ADDR_lua_settop ||
        !ADDR_FrameScript__GetState) {
        LOG("[SKIP] lua_pushnumber — dependency address not set"); return;
    }
    // lua_pushnumber: void __cdecl lua_pushnumber(lua_State *L, lua_Number n)
    // lua_gettop:    int  __cdecl lua_gettop(lua_State *L)
    // lua_settop:    void __cdecl lua_settop(lua_State *L, int idx)
    using push_t   = void(__cdecl*)(void*, double);
    using gettop_t = int(__cdecl*)(void*);
    using settop_t = void(__cdecl*)(void*, int);

    void* L = *reinterpret_cast<void**>(ADDR_FrameScript__GetState);
    if (!L) { LOG("[SKIP] lua_pushnumber — lua_State* is NULL"); return; }

    __try {
        int before = reinterpret_cast<gettop_t>(ADDR_lua_gettop)(L);
        reinterpret_cast<push_t>(ADDR_lua_pushnumber)(L, 42.0);
        int after  = reinterpret_cast<gettop_t>(ADDR_lua_gettop)(L);
        if (after == before + 1) {
            LOG("[PASS] lua_pushnumber — stack grew by 1 (before=%d after=%d)", before, after);
        } else {
            LOG("[WARN] lua_pushnumber — unexpected stack change (before=%d after=%d)", before, after);
        }
        // clean up stack
        reinterpret_cast<settop_t>(ADDR_lua_settop)(L, before);
    }
    __except (EXCEPTION_EXECUTE_HANDLER) {
        LOG("[FAIL] lua_pushnumber (SEH 0x%08X)", GetExceptionCode());
    }
}

static void test_SStr__Printf()
{
    if (!ADDR_SStr__Printf) { LOG("[SKIP] SStr__Printf — address not set"); return; }
    using fn_t = void(__cdecl*)(char*, size_t, const char*, ...);

    __try {
        char buf[64] = {};
        reinterpret_cast<fn_t>(ADDR_SStr__Printf)(buf, sizeof(buf), "Hello %s %d", "TBC", 8606);
        if (strncmp(buf, "Hello TBC 8606", 14) == 0) {
            LOG("[PASS] SStr__Printf → \"%s\"", buf);
        } else {
            LOG("[WARN] SStr__Printf → unexpected result \"%s\"", buf);
        }
    }
    __except (EXCEPTION_EXECUTE_HANDLER) {
        LOG("[FAIL] SStr__Printf (SEH 0x%08X)", GetExceptionCode());
    }
}

// ---------------------------------------------------------------------------
// Test runner
// ---------------------------------------------------------------------------

static DWORD WINAPI run_tests(LPVOID)
{
    // Wait briefly for the game to be fully initialized
    Sleep(3000);

    log_open();
    LOG("WoW.exe base: 0x%p", GetModuleHandleA(nullptr));
    LOG("");

    test_FrameScript__GetState();
    test_lua_gettop();
    test_lua_pushnumber();
    test_SStr__Printf();

    // Basic null-address checks for remaining entries
    struct { const char* name; uintptr_t addr; } checks[] = {
        { "FrameScript__LoadFunctions",    ADDR_FrameScript__LoadFunctions },
        { "FrameScript__RegisterFunction", ADDR_FrameScript__RegisterFunction },
        { "CVar__Set",                     ADDR_CVar__Set },
        { "SErr__PrepareAppFatal",         ADDR_SErr__PrepareAppFatal },
        { "lua_type",                      ADDR_lua_type },
        { "lua_pushnil",                   ADDR_lua_pushnil },
        { "lua_pushstring",                ADDR_lua_pushstring },
        { "lua_toboolean",                 ADDR_lua_toboolean },
        { "lua_tonumber",                  ADDR_lua_tonumber },
        { "lua_tolstring",                 ADDR_lua_tolstring },
        { "CGChat__AddChatMessage",        ADDR_CGChat__AddChatMessage },
    };
    for (const auto& c : checks) {
        if (c.addr) {
            // Quick readability check: first byte should be a valid instruction
            // (not 0x00 or 0xCC padding)
            uint8_t first_byte = *reinterpret_cast<const uint8_t*>(c.addr);
            if (first_byte != 0x00 && first_byte != 0xCC) {
                LOG("[ADDR] %-40s 0x%08X  first_byte=0x%02X (plausible)", c.name, c.addr, first_byte);
            } else {
                LOG("[WARN] %-40s 0x%08X  first_byte=0x%02X (padding/zeroes — wrong addr?)", c.name, c.addr, first_byte);
            }
        } else {
            LOG("[TODO] %-40s address not set", c.name);
        }
    }

    LOG("\n=== Test complete ===");
    log_close();
    return 0;
}

// ---------------------------------------------------------------------------
// DLL entry point
// ---------------------------------------------------------------------------

BOOL APIENTRY DllMain(HMODULE hModule, DWORD reason, LPVOID /*reserved*/)
{
    if (reason == DLL_PROCESS_ATTACH) {
        DisableThreadLibraryCalls(hModule);
        CreateThread(nullptr, 0, run_tests, nullptr, 0, nullptr);
    }
    return TRUE;
}
