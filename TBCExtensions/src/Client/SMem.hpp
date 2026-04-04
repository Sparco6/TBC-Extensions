#pragma once

#include <cstdint>

class SMem
{
public:
    static void* Alloc(uint32_t size, const char* filepath, uint32_t flags, uint32_t smth);
    static bool  Free(void* memory, const char* func, uint32_t flags, uint32_t smth);

private:
    SMem() = delete;
    ~SMem() = delete;
};
