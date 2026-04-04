#include <Client/CMap.hpp>
#include <Client/SFile.hpp>
#include <Misc/Util.hpp>

void CMap::Apply()
{
    // TODO_TBC: Replace all CMap patch addresses with TBC 2.4.3 equivalents
    // WotLK 3.3.5 addresses were: 0x7D7162, 0x7D80E9, 0x7D8100, 0x7D8670
    // How to find: Find the M2 model loading code that calls SFile::OpenEx, patch each call site
    Util::OverwriteUInt32AtAddress(0x000000 /* TODO_TBC */, reinterpret_cast<uint32_t>(&SafeOpenEx) - 0x000004 /* TODO_TBC */);
    Util::OverwriteUInt32AtAddress(0x000000 /* TODO_TBC */, reinterpret_cast<uint32_t>(&SafeOpenEx) - 0x000004 /* TODO_TBC */);
    Util::OverwriteUInt32AtAddress(0x000000 /* TODO_TBC */, reinterpret_cast<uint32_t>(&SafeOpenEx) - 0x000004 /* TODO_TBC */);
    Util::OverwriteUInt32AtAddress(0x000000 /* TODO_TBC */, reinterpret_cast<uint32_t>(&SafeOpenEx) - 0x000004 /* TODO_TBC */);
}

bool CMap::SafeOpenEx(const char* filename, HANDLE* a2)
{
    for (int i = 0; i < 10; i++)
        if (SFile::OpenFile(filename, a2))
            return true;

    return SFile::OpenFile("Spells\\ErrorCube.mdx", a2);
}
