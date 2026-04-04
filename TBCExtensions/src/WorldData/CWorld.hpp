#pragma once

#include <Data/Math.hpp>

class CWorld
{
public:
    static void    LoadMap(const char* directory, Vec3* pos, uint32_t mapID);
    static void    UnloadMap();
    static int32_t Pos3Dto2D(void* thisWorld, Vec3* pos3d, Vec3* pos2d, uint32_t* flags);

private:
    CWorld() = delete;
    ~CWorld() = delete;
};
