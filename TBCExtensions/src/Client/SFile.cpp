#include <Client/SFile.hpp>

bool SFile::OpenFile(const char* filename, HANDLE* fileBlock)
{
    return OpenFileEx(nullptr, filename, 0, fileBlock);
}

bool SFile::OpenFileEx(HANDLE handle, const char* filename, uint32_t flags, HANDLE* fileBlock)
{
    // TODO_TBC: Find SFile::OpenEx address
    // WotLK 3.3.5 address was: 0x424B50
    return reinterpret_cast<bool (__stdcall*)(HANDLE, const char*, uint32_t, HANDLE*)>(0x000000 /* TODO_TBC */)(handle, filename, flags, fileBlock);
}

bool SFile::ReadFile(HANDLE handle, void* data, uint32_t bytesToRead, uint32_t* bytesRead, uint32_t* overlap, uint32_t unk)
{
    // TODO_TBC: Find SFile::ReadFile address
    // WotLK 3.3.5 address was: 0x422530
    return reinterpret_cast<bool (__stdcall*)(HANDLE, void*, uint32_t, uint32_t*, uint32_t*, uint32_t)>(0x000000 /* TODO_TBC */)(handle, data, bytesToRead, bytesRead, overlap, unk);
}

void SFile::CloseFile(HANDLE handle)
{
    // TODO_TBC: Find SFile::CloseFile address
    // WotLK 3.3.5 address was: 0x422910
    reinterpret_cast<void(__stdcall*)(HANDLE)>(0x000000 /* TODO_TBC */)(handle);
}
