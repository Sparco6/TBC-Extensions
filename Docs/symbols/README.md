# TBC Client Symbol Files

This directory contains reverse-engineered symbol information for the World of Warcraft TBC (2.4.3) client.

## Files

### func.sym
**Format:** `FunctionName Address f`  
**Size:** 110 KB  
**Count:** 1,744 functions  
**Purpose:** Manually curated, human-readable function names

Example:
```
PacketHandler_SMSG_CHAR_ENUM 0041C920 f
Script_GetRaidTargetIndex 004BA8A0 f
CDataStore__Init 0041F0E0 f
```

### wow_tbc.sym
**Format:** `FunctionName Address f`  
**Size:** 546 KB  
**Count:** 9,383 functions  
**Purpose:** Complete Ghidra export (all discovered functions)

Contains both named and auto-generated names (FUN_*):
```
entry 00401000 f
FUN_00401010 00401010 f
PacketHandler_SMSG_CHAR_ENUM 0041C920 f
```

### label.sym
**Format:** `LabelName Address l`  
**Size:** 121 KB  
**Purpose:** Jump tables, lookup tables, and code labels

Example:
```
jump_table_40177c 0040177C l
lookup_table_405b4c 0040B4C l
```

### Functions.csv
**Format:** CSV with columns: Name, Location, Function Signature, Function Size  
**Size:** 1.9 MB  
**Purpose:** Detailed function information from Ghidra analysis

Example:
```csv
"PacketHandler_SMSG_CHAR_ENUM","0041c920","undefined4 FUN_0041c920(void)","688"
```

## Usage

### Search for Functions
```bash
# Find all packet handlers
grep "PacketHandler" func.sym

# Find functions by address
grep "0041C920" func.sym

# Search CSV for specific patterns
grep -i "script_get" Functions.csv
```

### Import into Tools

**IDA Pro / Ghidra:**
- File → Load File → Script File
- Select `.sym` file
- Symbols will be imported at specified addresses

**x64dbg:**
- Symbols → Import → Select `.sym` file

## Statistics

- **Total functions found:** 9,383
- **Named functions:** 1,744 (18.6%)
- **Unnamed functions:** 7,639 (81.4%)

## Address Ranges

- `0x400000 - 0x430000` - Core engine, packet handlers
- `0x430000 - 0x490000` - UI/Script system
- `0x490000 - 0x510000` - Game mechanics
- `0x510000+` - Graphics, rendering, world

## Address Finding

The TBC-Extensions codebase contains 100+ `TODO_TBC` placeholder addresses that must
be replaced with the correct TBC 2.4.3 (build 8606) addresses before the extension
will build and run.

### Quick Start

```bash
# Search symbol files and generate a prioritised candidate report
python Tools/find_addresses.py

# Output:
#   Docs/ADDRESS_FINDINGS.md  — ranked candidates for every TODO_TBC function
#   Docs/symbols/address_map.json  — machine-readable results (auto-updated)
```

### Files

| File | Purpose |
|------|---------|
| [`priority_functions.txt`](priority_functions.txt) | All TODO_TBC functions sorted by priority |
| [`address_map.json`](address_map.json) | Structured data: candidates, confidence, verification status |
| [`Docs/ADDRESS_FINDINGS.md`](../ADDRESS_FINDINGS.md) | Auto-generated candidate report (run script to refresh) |
| [`Docs/ADDRESS_STATUS.md`](../ADDRESS_STATUS.md) | Manual progress dashboard (tick boxes as you verify) |
| [`Docs/GHIDRA_ADDRESS_FINDING.md`](../GHIDRA_ADDRESS_FINDING.md) | Step-by-step Ghidra analysis guide |
| [`Tools/find_addresses.py`](../../Tools/find_addresses.py) | Automated search script |
| [`Tools/test_addresses.cpp`](../../Tools/test_addresses.cpp) | DLL test harness to verify found addresses |

### Workflow

1. Run `python Tools/find_addresses.py` — get candidate addresses
2. Follow `Docs/GHIDRA_ADDRESS_FINDING.md` — verify candidates
3. Update `Docs/symbols/address_map.json` with verified addresses
4. Replace `0x000000 /* TODO_TBC */` in source code
5. Re-run the script to refresh reports
6. Track overall progress in `Docs/ADDRESS_STATUS.md`

### Confidence Levels

| Level | Meaning |
|-------|---------|
| 95 % | Exact name match in func.sym |
| 90 % | Case-insensitive name match |
| 75 % | Substring match in named symbols |
| 50 % | Substring match in auto-named symbols |
| < 40 % | Proximity to WotLK address only — manual verification required |

### Troubleshooting

**No candidates found for a function:**
- Open Ghidra and use the techniques in `GHIDRA_ADDRESS_FINDING.md`
- Check if a `FUN_XXXXXXXX` at the expected address region matches the signature

**Candidate crashes the game:**
- Verify calling convention (`__cdecl` vs `__thiscall` vs `__stdcall`)
- Check whether the address is a JMP trampoline — use the real destination
- Ensure WoW.exe is fully loaded before calling (use DLL injection timing)

**Address range looks wrong:**
- TBC 2.4.3 `.text` section spans roughly `0x401000` – `0x88A000`
- Addresses outside this range are likely data, not code

## Contributing

When adding new function names to `func.sym`:
1. Use descriptive names: `ClassName__MethodName` or `Script_FunctionName`
2. Verify addresses are correct
3. Keep alphabetical or address order
4. Add entry in this README if discovering new subsystem
5. Update `address_map.json` with `"verification_status": "verified"`

## Tools Used

- **Ghidra 11.0+** - Function discovery and analysis
- **IDA Pro** - Cross-referencing
- **x64dbg / x32dbg** - Runtime debugging and address verification
- **Cheat Engine** - Live memory inspection

## Resources

- [TrinityCore](https://github.com/TrinityCore/TrinityCore) - Server reference
- [WoW Dev Wiki](https://wowdev.wiki/) - Protocol documentation
- [OpcodeTable 2.4.3](https://gtker.com/wow_messages/docs/opcodes_2_4_3.html) - Packet opcodes
