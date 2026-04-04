#include <Client/SMem.hpp>

void* SMem::Alloc(uint32_t size, const char* filepath, uint32_t flags, uint32_t smth)
{
    // TODO_TBC: Find SMem::Alloc address
    // WotLK 3.3.5 address was: 0x76E540
    return reinterpret_cast<void* (__stdcall*)(uint32_t, const char*, uint32_t, uint32_t)>(0x000000 /* TODO_TBC */)(size, filepath, flags, smth);
}

bool SMem::Free(void* memory, const char* func, uint32_t flags, uint32_t smth)
{
    // TODO_TBC: Find SMem::Free address
    // WotLK 3.3.5 address was: 0x76E5A0
    return reinterpret_cast<bool (__stdcall*)(void*, const char*, uint32_t, uint32_t)>(0x000000 /* TODO_TBC */)(memory, func, flags, smth);
}
