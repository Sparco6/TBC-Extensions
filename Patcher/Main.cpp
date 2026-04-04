// TBC-Extensions Patcher
// Patches wow.exe (TBC 2.4.3 build 8606) to load TBCExtensions.dll

#include <iostream>
#include <filesystem>
#include <fstream>
#include <array>
#include <iterator>
#include <vector>

using namespace std;

static fstream wow_exe;

template <typename T>
void write_pos(streampos pos, T value)
{
    wow_exe.clear();
    wow_exe.seekg(0);
    wow_exe.seekp(pos);
    wow_exe << value;
}

void write_pos(streampos pos, vector<uint8_t> const& values)
{
    wow_exe.clear();
    wow_exe.seekg(0);
    wow_exe.seekp(pos);
    for (uint8_t v : values)
        wow_exe << v;
}

void write_pos_n(streampos pos, uint8_t value, size_t n)
{
    wow_exe.clear();
    wow_exe.seekg(0);
    wow_exe.seekp(pos);
    for (size_t i = 0; i < n; i++)
        wow_exe << value;
}

int main(int argc, char** argv)
{
    string wow;

    if (argc < 2)
    {
        cout << "Usage: Patcher.exe <path/to/WoW.exe>\n";
        cout << "World of Warcraft exe not found!\n";
        return 1;
    }

    wow = argv[1];

    wow_exe = fstream(wow, ios::in | ios::out | ios::binary);

    if (!wow_exe)
    {
        cout << "World of Warcraft exe not found!\n";
        return 1;
    }

    // Applies Large Address Aware flag (4GB patch)
    write_pos<uint8_t>(0x126, 0x23);

    // StartAddress + 37: redirect to cave 1
    write_pos(0x244A17, { 0xE9, 0x65, 0x88, 0x0C, 0x00 }); // jmp cave 1

    // Cave 1: push TBCExtensions.dll string address, then jump to cave 2
    write_pos(0x30D281, { 0x68, 0x00, 0xD3, 0x88, 0x00,     // push TBCExtensions.dll
                          0xE9, 0xF6, 0x3A, 0x00, 0x00 });  // jmp cave 2

    // Cave 2: call LoadLibraryA via IAT, then jump to cave 3
    write_pos(0x310D81, { 0xFF, 0x15, 0x74, 0xA1, 0x88, 0x00, // call LoadLibraryA
                          0xE9, 0x55, 0x04, 0x00, 0x00 });     // jmp cave 3

    // Cave 3: restore original ecx value, then jump back
    write_pos(0x3111E1, { 0xB9, 0xCC, 0x51, 0xD7, 0x00,     // mov ecx, dword_D751CC
                          0xE9, 0x31, 0x38, 0xF3, 0xFF });   // jmp back

    // Write "TBCExtensions.dll\0" string into exe
    write_pos(0x48B900, { 0x54, 0x42, 0x43, 0x45, 0x78, 0x74, 0x65, 0x6E,
                          0x73, 0x69, 0x6F, 0x6E, 0x73, 0x2E, 0x64, 0x6C,
                          0x6C, 0x00 }); // TBCExtensions.dll

    // Additional edits to relocate "player visible"/"player hidden" strings,
    // freeing space used by the TBCExtensions.dll string above
    write_pos(0x5DD0, { 0xB8, 0x31, 0x1F, 0x71, 0x00 }); // mov eax, "player visible"
    write_pos(0x5DD7, { 0xB8, 0x01, 0x1E, 0x71, 0x00 }); // mov eax, "player hidden"

    write_pos(0x311201, { 0x70, 0x6C, 0x61, 0x79, 0x65, 0x72, 0x20, 0x68,
                          0x69, 0x64, 0x64, 0x65, 0x6E, 0x00 }); // player hidden
    write_pos(0x311331, { 0x70, 0x6C, 0x61, 0x79, 0x65, 0x72, 0x20, 0x76,
                          0x69, 0x73, 0x69, 0x62, 0x6C, 0x65, 0x00 }); // player visible

    cout << "World of Warcraft exe patched successfully.\n";
    cout << "Remember to copy TBCExtensions.dll to your game folder!\n";

    return 0;
}
