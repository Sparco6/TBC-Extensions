#include <Main.hpp>

void Main::OnAttach()
{
    Init();

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
}

void Main::Init()
{
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
}

static bool __stdcall DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpReserved)
{
    if (fdwReason == DLL_PROCESS_ATTACH)
    {
        DisableThreadLibraryCalls(hinstDLL);
        CreateThread(nullptr, 0, [](LPVOID) -> DWORD
        {
            Main::OnAttach();
            return 0;
        }, nullptr, 0, nullptr);
    }

    return true;
}
