#include <CDBCMgr/CDBCMgr.hpp>
#include <Misc/DataContainer.hpp>
#include <Misc/Util.hpp>

#include <PatchConfig.hpp>

void CDBCMgr::Load()
{
    // TBC: No LFGRoles or ZoneLight DBCs (WotLK-only features).
    // Add custom TBC DBC loaders here if needed.
}

int __declspec(naked) CDBCMgr::RegisterDBCEx()
{
    CDBCMgr::Load();

    __asm
    {
        // TODO_TBC: Replace 0x000000 with TBC 2.4.3 original RegisterDBC function address
        // func.sym does NOT have RegisterDBC labeled directly.
        // StaticDBLoadAll (0x00573C90) calls all WowClientDB::Load__* functions.
        // Disassemble StaticDBLoadAll in IDA/Ghidra to find the RegisterDBC call target.
        // WotLK 3.3.5 address was: 0x6337D0
        mov ecx, 0x000000; // TODO_TBC
        call ecx;
        ret;
    }
}

void CDBCMgr::PatchAddress()
{
    // TODO_TBC: Replace 0x000000 with TBC 2.4.3 DBC registration patch address
    // func.sym does NOT have this patch site labeled.
    // StaticDBLoadAll (0x00573C90) contains the CALL instruction to patch.
    // Disassemble StaticDBLoadAll to find the exact call-site offset.
    // WotLK 3.3.5 was: 0x634E30 (patch site), offset = fn - 0x634E34
    Util::OverwriteUInt32AtAddress(0x000000 /* TODO_TBC */, (uint32_t)&CDBCMgr::RegisterDBCEx - 0x000004 /* TODO_TBC */);
}
