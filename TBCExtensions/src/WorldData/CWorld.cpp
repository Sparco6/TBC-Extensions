#include <WorldData/CWorld.hpp>

void CWorld::LoadMap(const char* directory, Vec3* pos, uint32_t mapID)
{
    // TODO_TBC: Find CWorld::LoadMap address
    // WotLK 3.3.5 address was: 0x781430
    reinterpret_cast<void (__cdecl*)(const char*, Vec3*, uint32_t)>(0x000000 /* TODO_TBC */)(directory, pos, mapID);
}

void CWorld::UnloadMap()
{
    // TODO_TBC: Find CWorld::UnloadMap address
    // WotLK 3.3.5 address was: 0x783180
    reinterpret_cast<void (__cdecl*)()>(0x000000 /* TODO_TBC */)();
}

int32_t CWorld::Pos3Dto2D(void* thisWorld, Vec3* pos3d, Vec3* pos2d, uint32_t* flags)
{
    // TODO_TBC: Find CWorld::Pos3Dto2D address
    // WotLK 3.3.5 address was: 0x4F6D20
    return reinterpret_cast<int32_t (__thiscall*)(void*, Vec3*, Vec3*, uint32_t*)>(0x000000 /* TODO_TBC */)(thisWorld, pos3d, pos2d, flags);
}
