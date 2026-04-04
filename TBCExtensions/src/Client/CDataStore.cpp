#include <Client/CDataStore.hpp>

void CDataStore::GenPacket(CDataStore* thisPacket)
{
    // TODO_TBC: Find CDataStore::GenPacket address
    // WotLK 3.3.5 address was: 0x401050
    reinterpret_cast<void (__thiscall*)(CDataStore*)>(0x000000 /* TODO_TBC */)(thisPacket);
}

void CDataStore::GetCString(CDataStore* thisPacket, char* cString, int32_t size)
{
    // TODO_TBC: Find CDataStore::GetCString address
    // WotLK 3.3.5 address was: 0x47B480
    reinterpret_cast<void (__thiscall*)(CDataStore*, char*, int32_t)>(0x000000 /* TODO_TBC */)(thisPacket, cString, size);
}

void CDataStore::GetInt8(CDataStore* thisPacket, int8_t* var)
{
    // TODO_TBC: Find CDataStore::GetInt8 address
    // WotLK 3.3.5 address was: 0x47B340
    reinterpret_cast<void (__thiscall*)(CDataStore*, int8_t*)>(0x000000 /* TODO_TBC */)(thisPacket, var);
}

void CDataStore::GetInt16(CDataStore* thisPacket, int16_t* var)
{
    // TODO_TBC: Find CDataStore::GetInt16 address
    // WotLK 3.3.5 address was: 0x47B380
    reinterpret_cast<void (__thiscall*)(CDataStore*, int16_t*)>(0x000000 /* TODO_TBC */)(thisPacket, var);
}

void CDataStore::GetInt32(CDataStore* thisPacket, int32_t* var)
{
    // TODO_TBC: Find CDataStore::GetInt32 address
    // WotLK 3.3.5 address was: 0x47B3C0
    reinterpret_cast<void (__thiscall*)(CDataStore*, int32_t*)>(0x000000 /* TODO_TBC */)(thisPacket, var);
}

void CDataStore::GetInt64(CDataStore* thisPacket, int64_t* var)
{
    // TODO_TBC: Find CDataStore::GetInt64 address
    // WotLK 3.3.5 address was: 0x47B400
    reinterpret_cast<void (__thiscall*)(CDataStore*, int64_t*)>(0x000000 /* TODO_TBC */)(thisPacket, var);
}

void CDataStore::PutCString(CDataStore* thisPacket, const char* cString)
{
    // TODO_TBC: Find CDataStore::PutCString address
    // WotLK 3.3.5 address was: 0x47B300
    reinterpret_cast<void (__thiscall*)(CDataStore*, const char*)>(0x000000 /* TODO_TBC */)(thisPacket, cString);
}

void CDataStore::PutInt8(CDataStore* thisPacket, int8_t value)
{
    // TODO_TBC: Find CDataStore::PutInt8 address
    // WotLK 3.3.5 address was: 0x47AFE0
    reinterpret_cast<void (__thiscall*)(CDataStore*, int8_t)>(0x000000 /* TODO_TBC */)(thisPacket, value);
}

void CDataStore::PutInt16(CDataStore* thisPacket, int16_t value)
{
    // TODO_TBC: Find CDataStore::PutInt16 address
    // WotLK 3.3.5 address was: 0x47AFE0
    reinterpret_cast<void (__thiscall*)(CDataStore*, int16_t)>(0x000000 /* TODO_TBC */)(thisPacket, value);
}

void CDataStore::PutInt32(CDataStore* thisPacket, int32_t value)
{
    // TODO_TBC: Find CDataStore::PutInt32 address
    // WotLK 3.3.5 address was: 0x47B0A0
    reinterpret_cast<void (__thiscall*)(CDataStore*, int32_t)>(0x000000 /* TODO_TBC */)(thisPacket, value);
}

void CDataStore::PutInt64(CDataStore* thisPacket, int64_t value)
{
    // TODO_TBC: Find CDataStore::PutInt64 address
    // WotLK 3.3.5 address was: 0x47B100
    reinterpret_cast<void (__thiscall*)(CDataStore*, int64_t)>(0x000000 /* TODO_TBC */)(thisPacket, value);
}

void CDataStore::Release(CDataStore* thisPacket)
{
    // TODO_TBC: Find CDataStore::Release address
    // WotLK 3.3.5 address was: 0x403880
    reinterpret_cast<void (__thiscall*)(CDataStore*)>(0x000000 /* TODO_TBC */)(thisPacket);
}

bool CDataStore::IsRead(CDataStore* thisPacket)
{
    // TODO_TBC: Find CDataStore::IsRead address
    // WotLK 3.3.5 address was: 0x4010D0
    return reinterpret_cast<bool (__thiscall*)(CDataStore*)>(0x000000 /* TODO_TBC */)(thisPacket);
}
