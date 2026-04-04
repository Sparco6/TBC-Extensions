#include <GameObjects/CGPetInfo.hpp>

WoWGUID CGPetInfo::GetPet(int32_t index)
{
    // TODO_TBC: Find CGPetInfo::GetPet address
    // WotLK 3.3.5 address was: 0x5D3390
    return reinterpret_cast <WoWGUID (__cdecl*)(int32_t)>(0x000000 /* TODO_TBC */)(index);
}
