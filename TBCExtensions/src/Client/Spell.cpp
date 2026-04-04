#include <Client/Spell.hpp>
#include <Data/Structs.hpp>
#include <GameObjects/CGUnit.hpp>

void Spell::ApplyPatches()
{
}

int32_t Spell::GetDefaultMinRange(SpellRow* spell, float* minRange)
{
    // TODO_TBC: Find Spell::GetDefaultMinRange address
    // WotLK 3.3.5 address was: 0x7FF400
    return reinterpret_cast<int32_t (__cdecl*)(SpellRow*, float*)>(0x000000 /* TODO_TBC */)(spell, minRange);
}

int32_t Spell::GetMinMaxRange(CGUnit* unit, SpellRow* spell, float* minRange, float* maxRange, int32_t index, float a6)
{
    // TODO_TBC: Find Spell::GetMinMaxRange address
    // WotLK 3.3.5 address was: 0x7FF480
    return reinterpret_cast<int32_t (__cdecl*)(CGUnit*, SpellRow*, float*, float*, int32_t, float)>(0x000000 /* TODO_TBC */)(unit, spell, minRange, maxRange, index, a6);
}

int32_t Spell::GetPowerCost(SpellRow* spell, CGUnit* unit)
{
    // TODO_TBC: Find Spell::GetPowerCost address
    // WotLK 3.3.5 address was: 0x8012F0
    return reinterpret_cast<int32_t (__cdecl*)(SpellRow*, CGUnit*)>(0x000000 /* TODO_TBC */)(spell, unit);
}

int32_t Spell::GetPowerCostPerSecond(SpellRow* spell, CGUnit* unit)
{
    // TODO_TBC: Find Spell::GetPowerCostPerSecond address
    // WotLK 3.3.5 address was: 0x7FF100
    return reinterpret_cast<int32_t (__cdecl*)(SpellRow*, CGUnit*)>(0x000000 /* TODO_TBC */)(spell, unit);
}

bool Spell::IsModifiedStat(SpellRow* spell, int32_t stat)
{
    // TODO_TBC: Find Spell::IsModifiedStat address
    // WotLK 3.3.5 address was: 0x800770
    return reinterpret_cast <bool (__cdecl*)(SpellRow*, int32_t)>(0x000000 /* TODO_TBC */)(spell, stat);
}

bool Spell::UsesDefaultMinRange(SpellRow* spell)
{
    // TODO_TBC: Find Spell::UsesDefaultMinRange address
    // WotLK 3.3.5 address was: 0x7FF3C0
    return reinterpret_cast<bool (__cdecl*)(SpellRow*)>(0x000000 /* TODO_TBC */)(spell);
}

bool Spell::UsesMeleeRange(SpellRow* spell)
{
    // TODO_TBC: Find Spell::UsesMeleeRange address
    // WotLK 3.3.5 address was: 0x7FF380
    return reinterpret_cast<bool (__cdecl*)(SpellRow*)>(0x000000 /* TODO_TBC */)(spell);
}
