#include <Client/SStr.hpp>

#include <cstdarg>
#include <cstdio>

int32_t SStr::Printf(char* buf, uint32_t length, char* fmt, ...)
{
    char buffer[2048] = { 0 };
    va_list args;

    va_start(args, fmt);
    vsnprintf_s(buffer, sizeof(buffer), _TRUNCATE, fmt, args);
    va_end(args);

    // TODO_TBC: Find SStr::Printf address
    // WotLK 3.3.5 address was: 0x76F070
    return reinterpret_cast<int32_t(__cdecl*)(char*, uint32_t, char*, ...)>(0x000000 /* TODO_TBC */)(buf, length, "%s", buffer);
}

char* SStr::Append(char* dst, char* src, int32_t size)
{
    // TODO_TBC: Find SStrCopy/SStr::Append address
    // WotLK 3.3.5 address was: 0x76EF70
    return reinterpret_cast<char* (__stdcall*)(char*, char*, int32_t)>(0x000000 /* TODO_TBC */)(dst, src, size);
}

char* SStr::Copy(char* dst, char* src, int32_t size)
{
    // TODO_TBC: Find SStr::Copy address
    // WotLK 3.3.5 address was: 0x76ED20
    return reinterpret_cast<char* (__stdcall*)(char*, char*, int32_t)>(0x000000 /* TODO_TBC */)(dst, src, size);
}
