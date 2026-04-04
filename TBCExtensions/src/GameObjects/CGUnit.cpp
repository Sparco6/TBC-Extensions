#include <GameObjects/CGUnit.hpp>

uint32_t CGUnit::GetShapeshiftFormID(CGUnit* thisUnit)
{
    // TODO_TBC: Find CGUnit::GetShapeshiftFormID address
    // WotLK 3.3.5 address was: 0x71AF70
    return reinterpret_cast<uint32_t (__thiscall*)(CGUnit*)>(0x000000 /* TODO_TBC */)(thisUnit);
}

uint8_t CGUnit::GetClass() const
{
    return m_unitFields->m_bytes0.m_unitClass;
}

uint8_t CGUnit::GetGender() const
{
    return m_unitFields->m_bytes0.m_unitGender;
}

uint8_t CGUnit::GetPowerType() const
{
    return m_unitFields->m_bytes0.m_unitPowerType;
}

uint8_t CGUnit::GetRace() const
{
    return m_unitFields->m_bytes0.m_unitRace;
}
