#include <WorldData/DNInfo.hpp>

void DNInfo::AddZoneLight(void* thisDNInfo, int32_t lightId, float f)
{
    // TODO_TBC: Find DNInfo::AddZoneLight address
    // WotLK 3.3.5 address was: 0x7ED150
    reinterpret_cast<void (__thiscall*)(void*, int32_t, float)>(0x000000 /* TODO_TBC */)(thisDNInfo, lightId, f);
}

void* DNInfo::GetDNInfoPtr()
{
    // TODO_TBC: Find DNInfo::GetDNInfoPtr address
    // WotLK 3.3.5 address was: 0x7ECEF0
    return reinterpret_cast<void* (__stdcall*)()>(0x000000 /* TODO_TBC */)();
}
