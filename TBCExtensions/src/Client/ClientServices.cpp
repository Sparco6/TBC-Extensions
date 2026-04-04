#include <Client/ClientServices.hpp>

WoWGUID ClientServices::GetActivePlayer()
{
    // TODO_TBC: Find GetActivePlayer / ClntObjMgr__GetActivePlayer address
    // WotLK 3.3.5 address was: 0x4D3790
    // How to find: Search for "ClntObjMgr" string, trace to active player GUID getter
    return reinterpret_cast<WoWGUID (__cdecl*)()>(0x000000 /* TODO_TBC */)();
}

uint8_t ClientServices::GetCharacterClass()
{
    // TODO_TBC: Find GetCharacterClass / CGUnit__GetClass address
    // WotLK 3.3.5 address was: 0x6B1080
    return reinterpret_cast<uint8_t (__cdecl*)()>(0x000000 /* TODO_TBC */)();
}

void* ClientServices::GetObjectPtr(WoWGUID objGUID, uint32_t typeMask)
{
    // TODO_TBC: Find GetObjectPtr / ClntObjMgr__GetObjectPtr address
    // WotLK 3.3.5 address was: 0x4D4DB0
    return reinterpret_cast<void* (__cdecl*)(WoWGUID, uint32_t)>(0x000000 /* TODO_TBC */)(objGUID, typeMask);
}

uint32_t ClientServices::GetPowerDivisor(int32_t powerType)
{
    switch (powerType)
    {
        case 1:
        case 6:
            return 10;
        case 4:
            return 1000;
        case 0:
        case 2:
        case 3:
        case 5:
        default:
            return 1;
    }
}

void ClientServices::InitializePlayer()
{
    // TODO_TBC: Find InitializePlayer address
    // WotLK 3.3.5 address was: 0x6E83B0
    reinterpret_cast<void (__cdecl*)()>(0x000000 /* TODO_TBC */)();
}

void ClientServices::SendPacket(CDataStore* packet)
{
    // TODO_TBC: Find NetClient__SendPacket address
    // WotLK 3.3.5 address was: 0x6B0B50
    reinterpret_cast<void (__cdecl*)(CDataStore*)>(0x000000 /* TODO_TBC */)(packet);
}

SkillLineAbilityRow* ClientServices::SpellTableLookupAbility(uint32_t raceID, uint32_t classID, int32_t spellID)
{
    // TODO_TBC: Find SpellTableLookupAbility address
    // WotLK 3.3.5 address was: 0x812410
    return reinterpret_cast<SkillLineAbilityRow* (__cdecl*)(uint32_t, uint32_t, uint32_t)>(0x000000 /* TODO_TBC */)(raceID, classID, spellID);
}
