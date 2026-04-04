#include <Misc/Util.hpp>

void Util::SetByteAtAddress(void* address, uint8_t byte)
{
    DWORD flOldProtect = 0;
    VirtualProtect(address, 0x1, PAGE_EXECUTE_READWRITE, &flOldProtect);
    memset(address, byte, 0x1);
    VirtualProtect(address, 0x1, flOldProtect, &flOldProtect);
}

void Util::OverwriteBytesAtAddress(void* address, uint8_t byte, size_t numRepeats)
{
    DWORD flOldProtect = 0;
    VirtualProtect(address, numRepeats, PAGE_EXECUTE_READWRITE, &flOldProtect);
    memset(address, byte, numRepeats);
    VirtualProtect(address, numRepeats, flOldProtect, &flOldProtect);
}

void Util::OverwriteBytesAtAddress(uint32_t address, uint8_t byteArray[], size_t arraySize)
{
    for (size_t i = 0; i < arraySize; i++)
        SetByteAtAddress((void*)(address + i), byteArray[i]);
}

void Util::OverwriteUInt32AtAddress(uint32_t address, uint32_t newVal)
{
    DWORD flOldProtect = 0;
    void* vAddress = (void*)address;
    VirtualProtect(vAddress, sizeof(uint32_t), PAGE_EXECUTE_READWRITE, &flOldProtect);
    *(uint32_t*)address = newVal;
    VirtualProtect(vAddress, sizeof(uint32_t), flOldProtect, &flOldProtect);
}

void Util::PercToScreenPos(float x, float y, float* resX, float* resY)
{
    // TODO_TBC: Replace these multiplier addresses with TBC 2.4.3 equivalents
    // WotLK 3.3.5 addresses were: 0xAC0CB4, 0xAC0CBC
    // How to find: Look for global float pair used in UI coordinate scaling
    float g_UITexCoordAlphaMultiplier1 = *reinterpret_cast<float*>(0x000000 /* TODO_TBC */);
    float g_UITexCoordAlphaMultiplier3 = *reinterpret_cast<float*>(0x000000 /* TODO_TBC */);

    *resX = (x * (g_UITexCoordAlphaMultiplier3 * 1024.f)) / g_UITexCoordAlphaMultiplier1;
    *resY = (y * (g_UITexCoordAlphaMultiplier3 * 1024.f)) / g_UITexCoordAlphaMultiplier1;
}
