#include <Client/DBClient.hpp>

double DBClient::GetGameTableValue(uint32_t tableID, uint32_t classID, uint32_t combatRating)
{
    // TODO_TBC: Find GetGameTableValue address
    // WotLK 3.3.5 address was: 0x7F6990
    // How to find: Search for game table DBC load, track function reading combat rating data
    return reinterpret_cast<double (__cdecl*)(uint32_t, uint32_t, uint32_t)>(0x000000 /* TODO_TBC */)(tableID, classID, combatRating);
}

int32_t DBClient::GetLocalizedRow(void* thisDBC, uint32_t rowIndex, void* row)
{
    // TODO_TBC: Find GetLocalizedRow (thiscall DBC lookup) address
    // WotLK 3.3.5 address was: 0x4CFD20
    // How to find: Trace DBC row access in spell/item tooltip code
    return reinterpret_cast<int (__thiscall*)(void*, uint32_t, void*)>(0x000000 /* TODO_TBC */)(thisDBC, rowIndex, row);
}

void* DBClient::GetRow(void* vtable2, uint32_t rowIndex)
{
    // TODO_TBC: Find GetRow address
    // WotLK 3.3.5 address was: 0x65C290
    // How to find: Look for DBC row read function in item/creature lookup code
    return reinterpret_cast<void* (__thiscall*)(void*, uint32_t)>(0x000000 /* TODO_TBC */)(vtable2, rowIndex);
}
