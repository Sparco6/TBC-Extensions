// TBC-Extensions Patcher
// Patches wow.exe (TBC 2.4.3 build 8606) to load TBCExtensions.dll
//
// TODO_TBC: All offsets below are WotLK 3.3.5 placeholders.
// You MUST find the correct TBC 2.4.3 (build 8606) file offsets before use.
// See TBC_PORTING_GUIDE.md for instructions.
//
// Key things to find in TBC wow.exe:
//  1. Large Address Aware flag position (PE header, usually 0x126)
//  2. ScanDllStart / Lua_ScanDllStart routines (or equivalent entrypoints)
//  3. The DLL name string location in .rdata
//  4. Custom GlueXML validation bypass locations (optional)

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
    bool unlockCustomGluexml = true;

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

    // TODO_TBC: Applies Large Address Aware flag (4GB patch)
    // WotLK offset was: 0x126
    // For TBC 2.4.3: verify using CFF Explorer - open WoW.exe, go to NT Headers ->
    //   File Header -> Characteristics field. The file offset for this field is 0x126
    //   in most Win32 PE executables. Confirm by looking for the 2-byte value at that
    //   offset; setting bit 0x20 enables IMAGE_FILE_LARGE_ADDRESS_AWARE.
    write_pos<uint8_t>(0x126 /* TODO_TBC: verify using CFF Explorer as noted above */, 0x23);

    // TODO_TBC: Custom Glue XML bypass
    // WotLK offsets were: 0x1F41BF, 0x415A25, 0x415A3F, etc.
    // How to find: Search for "Interface\\GlueXML" string in IDA, trace to validation code
    if (unlockCustomGluexml)
    {
        // TODO_TBC: Replace with TBC 2.4.3 file offsets
        /*
        write_pos<uint8_t>(0x000000, 0xEB);  // TODO_TBC
        write_pos<uint8_t>(0x000000, 0xEB);  // TODO_TBC
        write_pos<uint8_t>(0x000000, 0x03);  // TODO_TBC
        write_pos<uint8_t>(0x000000, 0x03);  // TODO_TBC
        write_pos<uint8_t>(0x000000, 0xEB);  // TODO_TBC
        write_pos(0x000000, { 0xB8, 0x03, 0x00, 0x00, 0x00, 0xEB, 0xED }); // TODO_TBC
        */
    }

    // TODO_TBC: PE header fixups after patch
    // WotLK offsets were: 0x168, 0x1A9, 0x210, 0x238, 0x260, 0x2B0
    /*
    if (unlockCustomGluexml)
        write_pos(0x168, { 0x00, 0x00, 0x00 }); // TODO_TBC - recalculate checksum
    write_pos(0x1A9, { 0x00, 0x00, 0x00, 0x00, 0x00 });
    write_pos(0x210, { 0x00, 0xE0 });
    write_pos(0x238, { 0x00, 0x70 });
    write_pos(0x260, { 0x00, 0xB0 });
    write_pos(0x2B0, { 0x00, 0x10 });
    */

    // TODO_TBC: StartAddress patch - redirect DLL loader to TBCExtensions.dll
    // WotLK offset was: 0xABD0
    // How to find: Locate the EXE entrypoint stub code, patch jump to ScanDllStart
    /*
    write_pos(0x000000, { 0xE9, 0x00, 0x00, 0x00, 0x00, 0x90, 0x90, 0x90 }); // TODO_TBC
    */

    // TODO_TBC: ScanDllStart patch - inject DLL load stub
    // WotLK offset was: 0xE50B0
    // How to find: Trace DLL loading code from WinMain
    // The shellcode below loads TBCExtensions.dll via LoadLibraryA
    /*
    write_pos(0x000000, { // TODO_TBC
        0xB8, 0x01, 0x00, 0x00, 0x00,       // mov eax, 1
        0xA3, 0x00, 0x00, 0x00, 0x00,       // mov [g_DllLoaded], eax  - TODO_TBC address
        0x68, 0x00, 0x00, 0x00, 0x00,       // push "TBCExtensions.dll" ptr - TODO_TBC address
        0xE8, 0x00, 0x00, 0x00, 0x00,       // call LoadLibraryA - TODO_TBC offset
        0x83, 0xC4, 0x04,                   // add esp, 4
        // ... original code + jmp back
    });
    */

    // TODO_TBC: Lua_ScanDllStart bypass (make it return immediately)
    // WotLK offset was: 0xDC0F0
    /*
    write_pos(0x000000, { 0xB8, 0x01, 0x00, 0x00, 0x00, 0xC3 }); // TODO_TBC
    */

    // TODO_TBC: Write DLL name "TBCExtensions.dll" into the exe .rdata section
    // WotLK offset was: 0x5E2A70
    // How to find: Find an unused area in .rdata, or overwrite old DLL name string
    /*
    write_pos(0x000000, { // TODO_TBC
        // "TBCExtensions.dll\0"
        0x54, 0x42, 0x43, 0x45, 0x78, 0x74, 0x65, 0x6E,
        0x73, 0x69, 0x6F, 0x6E, 0x73, 0x2E, 0x64, 0x6C,
        0x6C, 0x00
    });
    */

    cout << "TODO_TBC: All patch offsets need to be found for TBC 2.4.3 (build 8606)!\n";
    cout << "See TBC_PORTING_GUIDE.md for instructions on finding these addresses.\n";
    cout << "Once all TODO_TBC entries are filled in, re-compile and run this patcher.\n";
    cout << "\nWhen complete, World of Warcraft exe will be patched.\n";
    cout << "Remember to copy TBCExtensions.dll to your game folder!\n";

    return 0;
}
