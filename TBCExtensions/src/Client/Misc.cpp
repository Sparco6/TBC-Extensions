#include <Client/Misc.hpp>
#include <Misc/Util.hpp>

#include <PatchConfig.hpp>

void Misc::ApplyPatches()
{
#if NOAMMO_PATCH
    // TODO_TBC: Find NoAmmo patch address
    // WotLK 3.3.5 address was: 0x809540
    // How to find: Search for the ammo check in projectile/ranged attack code
    uint8_t byteArray[] = { 0xE9, 0xBA, 0x00, 0x00, 0x00 };
    Util::OverwriteBytesAtAddress(0x000000 /* TODO_TBC */, byteArray, sizeof(byteArray));
#endif

#if ITEMMODEXT_PATCH
    // TODO_TBC: Replace ALL item mod table addresses with TBC 2.4.3 equivalents
    // WotLK 3.3.5 addresses: 0xAD6640 (item mod table), 0xA25F78 (item mod value table)
    // And patch addresses: 0x5DC1EB, 0x629211, 0x6292E6, 0x62BC60, etc.
    // How to find: Search for ITEM_MOD_ string references in tooltip rendering code
    memcpy(&itemModTable, (const void*)0x000000 /* TODO_TBC */, 0xC4);
    Util::OverwriteUInt32AtAddress(0x000000 /* TODO_TBC */, (uint32_t)&itemModTable);
    Util::OverwriteUInt32AtAddress(0x000000 /* TODO_TBC */, (uint32_t)&itemModTable);
    Util::OverwriteUInt32AtAddress(0x000000 /* TODO_TBC */, (uint32_t)&itemModTable);
    Util::OverwriteUInt32AtAddress(0x000000 /* TODO_TBC */, (uint32_t)&itemModTable);
    Util::OverwriteUInt32AtAddress(0x000000 /* TODO_TBC */, (uint32_t)&itemModTable);
    Util::OverwriteUInt32AtAddress(0x000000 /* TODO_TBC */, (uint32_t)&itemModTable[45]);

    memcpy(&itemModTableVal, (const void*)0x000000 /* TODO_TBC */, 0x94);
    Util::OverwriteUInt32AtAddress(0x000000 /* TODO_TBC */, (uint32_t)&itemModTableVal);
    Util::SetByteAtAddress((void*)0x000000 /* TODO_TBC */, (uint8_t)(sizeof(itemModTableVal) / 4));
    Util::OverwriteUInt32AtAddress(0x000000 /* TODO_TBC */, (uint32_t)&itemModTableVal);
    Util::OverwriteUInt32AtAddress(0x000000 /* TODO_TBC */, (uint32_t)&itemModTableVal);
    Util::SetByteAtAddress((void*)0x000000 /* TODO_TBC */, (uint8_t)(sizeof(itemModTableVal) / 4));
    Util::OverwriteUInt32AtAddress(0x000000 /* TODO_TBC */, (uint32_t)&itemModTableVal);
#endif
}

void Misc::SetYearOffsetMultiplier()
{
    // No-op for TBC; year offset multiplier was a WotLK-specific fix
}
