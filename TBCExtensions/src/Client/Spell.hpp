#pragma once

#include <cstdint>

class Spell
{
public:
    static void ApplyPatches();

    static int32_t GetDefaultMinRange(struct SpellRow* spell, float* minRange);
    static int32_t GetMinMaxRange(struct CGUnit* unit, struct SpellRow* spell, float* minRange, float* maxRange, int32_t index, float a6);
    static int32_t GetPowerCost(struct SpellRow* spell, struct CGUnit* unit);
    static int32_t GetPowerCostPerSecond(struct SpellRow* spell, struct CGUnit* unit);
    static bool    IsModifiedStat(struct SpellRow* spell, int32_t stat);
    static bool    UsesDefaultMinRange(struct SpellRow* spell);
    static bool    UsesMeleeRange(struct SpellRow* spell);

private:
    Spell() = delete;
    ~Spell() = delete;
};
