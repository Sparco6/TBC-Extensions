#include <Client/SFile.hpp>

bool SFile::OpenFile(const char* filename, HANDLE* fileBlock)
{
    return OpenFileEx(nullptr, filename, 0, fileBlock);
}

bool SFile::OpenFileEx(HANDLE handle, const char* filename, uint32_t flags, HANDLE* fileBlock)
{
    // Found in func.sym: SFile__OpenFile = 0x006755A0
    return reinterpret_cast<bool (__stdcall*)(HANDLE, const char*, uint32_t, HANDLE*)>(0x006755A0)(handle, filename, flags, fileBlock);
}

bool SFile::ReadFile(HANDLE handle, void* data, uint32_t bytesToRead, uint32_t* bytesRead, uint32_t* overlap, uint32_t unk)
{
    // Found in func.sym: SFile__ReadFile = 0x0067FF90
    return reinterpret_cast<bool (__stdcall*)(HANDLE, void*, uint32_t, uint32_t*, uint32_t*, uint32_t)>(0x0067FF90)(handle, data, bytesToRead, bytesRead, overlap, unk);
}

void SFile::CloseFile(HANDLE handle)
{
    // TODO_TBC: Find SFile::CloseFile address
    // WotLK 3.3.5 address was: 0x422910
    reinterpret_cast<void(__stdcall*)(HANDLE)>(0x000000 /* TODO_TBC */)(handle);
}
