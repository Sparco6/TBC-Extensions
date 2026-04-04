#include <Client/CVar.hpp>

void CVar::Set(void* cVar, int32_t value, bool a3, bool a4, bool a5, bool a6)
{
    // TODO_TBC: Find CVar::Set address
    // WotLK 3.3.5 address was: 0x766940
    reinterpret_cast<void (__thiscall*)(void*, int32_t, bool, bool, bool, bool)>(0x000000 /* TODO_TBC */)(cVar, value, a3, a4, a5, a6);
}
