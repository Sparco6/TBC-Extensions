# TBC-Extensions

A comprehensive extension DLL for World of Warcraft TBC 2.4.3 (build 8606), ported from [WotLK-Extensions](https://github.com/Alyst3r/WotLK-Extensions).

> ⚠️ **STATUS: WORK IN PROGRESS - All memory addresses are TODO_TBC placeholders**
>
> This project provides the complete code framework, but every hardcoded address must be found and filled in for TBC 2.4.3 before the DLL will work.  
> See [TBC_PORTING_GUIDE.md](TBC_PORTING_GUIDE.md) for address-finding instructions.

---

## Features

- **200+ Backported Lua API functions** from WotLK, Cataclysm, and MoP
- **Compatibility stubs** for WotLK+ features that return safe defaults in TBC
- **Modern C_ namespace APIs** (C_QuestLog, C_Map, C_Container, C_ChatInfo)
- **Custom DBC loader** for adding new database tables
- **Custom packet system** for server↔client communication
- **All WotLK-only features removed**: No LFD, no ZoneLight DBCs, no Glyphs, no Vehicles
- **Full TBC 2.4.3 field layouts** for all game objects

---

## What's Included

| Category | Functions |
|----------|-----------|
| Spell API | 20+ backported functions |
| Talent API | 11 backported functions |
| Item API | 30+ backported functions |
| Unit API | 60+ backported functions |
| Quest API | 15 backported functions |
| Map/Position API | 14 backported functions |
| Camera API | 14 backported functions |
| System/Utility API | 25+ backported functions |
| Chat/Social API | 13 backported functions |
| Inventory/Container | 10 backported functions |
| Tooltip API | 10 backported functions |
| Merchant/Trade API | 7 backported functions |
| Combat Log API | 2 backported functions |
| Compatibility Stubs | 15 stub functions |
| Action Bar API | 13 functions |
| Dev Helper API | 9 functions |

---

## Build Requirements

- Visual Studio 2022 (or later) with C++17
- CMake 3.25+
- Windows SDK
- **Target platform**: Win32 (x86) — TBC client is 32-bit

---

## Build Instructions

```bash
# Create build directory
mkdir build && cd build

# Configure (Win32 is required for TBC)
cmake -A Win32 ..

# Build
cmake --build . --config Release
```

The output DLL will be in `build/bin/TBCExtensions.dll`.

---

## Installation

1. **Find all TODO_TBC addresses** — see [TBC_PORTING_GUIDE.md](TBC_PORTING_GUIDE.md)
2. **Build the DLL** — see Build Instructions above
3. **Run the Patcher** on your `WoW.exe`:
   ```
   Patcher.exe "C:\WoW\WoW.exe"
   ```
4. **Copy** `TBCExtensions.dll` to your WoW game folder
5. **Start WoW** — the DLL loads automatically

---

## Configuration Options (CMake)

| Option | Default | Description |
|--------|---------|-------------|
| `CUSTOM_DBC` | OFF | Enable custom DBC table loader |
| `NOAMMO_PATCH` | OFF | Remove ammo requirement for ranged weapons |
| `CHARCREATIONRACE_FIX` | ON | Fix crash with >21 playable races |
| `COMBOPOINT_FIX` | ON | Enable combo points for all classes |
| `OOBLUAFUNCTIONS_PATCH` | ON | Register Lua functions from outside WoW.exe |
| `CUSTOMPACKETS_PATCH` | OFF | Enable custom server↔client packets |
| `ITEMMODEXT_PATCH` | OFF | Expand ITEM_MOD table for custom tooltip stats |
| `SPELLDESCRIPTIONEXT_PATCH` | OFF | Add custom variables to spell descriptions |
| `ACTIONBAR_LUA` | OFF | Enable additional action bar Lua functions |
| `DEVHELPER_LUA` | OFF | Enable developer helper Lua functions |

---

## Documentation

- [TBC_PORTING_GUIDE.md](TBC_PORTING_GUIDE.md) — How to find TBC addresses
- [Docs/BACKPORTED_API.md](Docs/BACKPORTED_API.md) — Full API reference
- [Docs/API_QUICK_REFERENCE.md](Docs/API_QUICK_REFERENCE.md) — Quick lookup table
- [HOWTO.md](HOWTO.md) — Step-by-step usage guide

---

## Credits

- **Original WotLK-Extensions**: [Alyst3r](https://github.com/Alyst3r/WotLK-Extensions)
- **TBC Port**: TBC-Extensions contributors
- Techniques inspired by AwesomeWotLK and TSWoW projects