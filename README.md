# TBC-Extensions

A comprehensive extension DLL for World of Warcraft TBC 2.4.3 (build 8606), ported from [WotLK-Extensions](https://github.com/Alyst3r/WotLK-Extensions).

> ⚠️ **STATUS: IN PROGRESS — Framework complete; several addresses confirmed from sym files, rest still TODO_TBC**
>
> The full code framework is in place with 85+ source files. A growing set of TBC 2.4.3 addresses has been confirmed
> from `Docs/symbols/func.sym` and `Docs/symbols/label.sym`. All remaining `0x000000 /* TODO_TBC */` placeholders
> must still be located via IDA/Ghidra before those code paths will work.
> See [TBC_PORTING_GUIDE.md](TBC_PORTING_GUIDE.md) for instructions.

---

Contact: Discord: sparco6 if you want help us! make TBC great AGAIN
## What Has Been Done

### ✅ Framework & Porting (initial port)

- Full project structure with CMake, Win32 DLL target, configurable feature flags
- **85+ C++ source files** ported from WotLK-Extensions and adapted for TBC 2.4.3 field layouts
- All WotLK-only features removed (LFD, ZoneLight DBC, Glyphs, Vehicles)
- Custom DBC loader (`CDBCMgr`) — `Load()` / `PatchAddress()` / `RegisterDBCEx()` class interface
- Custom Lua function registration system (`CustomLua`, `FrameScript` wrappers)
- Custom packet system (`CNetClient`, `CDataStore`)
- WoWTime patch, CMap safe-load, CGTooltip, Spell, Misc, CGPlayer patches
- `DataContainer` singleton for runtime configuration
- Logger, pattern scanner, utility helpers
- Patcher binary (writes DLL-load stub into WoW.exe)
- Symbol files: `Docs/symbols/func.sym` and `Docs/symbols/label.sym` for TBC 2.4.3

### ✅ CDBCMgr — `RegisterDBCEx` class member

- `static int RegisterDBCEx()` added to `CDBCMgr.hpp` public interface
- Implementation promoted from anonymous free function to proper class member in `CDBCMgr.cpp`
- `PatchAddress()` updated to reference `&CDBCMgr::RegisterDBCEx`

### ✅ Addresses confirmed from func.sym / label.sym

The following TBC 2.4.3 addresses have been filled in from the symbol files (no more `TODO_TBC` for these):

| File | Function | Address |
|------|----------|---------|
| `Client/CNetClient.cpp` | `NetClient::ProcessMessage` | `0x0055F440` |
| `Client/CNetClient.cpp` | `NetClient::SetMessageHandler` | `0x0055F400` |
| `Client/CDataStore.cpp` | `CDataStore::IsRead` | `0x00425BB0` |
| `Client/SFile.cpp` | `SFile::OpenFileEx` (`SFile__OpenFile`) | `0x006755A0` |
| `Client/SFile.cpp` | `SFile::ReadFile` | `0x0067FF90` |
| `Client/DBClient.cpp` | `WowClientDB::GetRow` | `0x004047C0` |

### 🔍 Additional confirmed symbols (available for future use)

These are confirmed in `func.sym` / `label.sym` and can be used when implementing their call sites:

| Symbol | Address | Relevant for |
|--------|---------|-------------|
| `NetClient__Init` | `0x0055F220` | Network init hook |
| `NetClient__Send` | `0x0055F9A0` | Packet send |
| `NetClient__Disconnect` | `0x0055F7B0` | Disconnect hook |
| `CDataStore__Init` | `0x0041F0E0` | Packet construction |
| `CDataStore__Alloc` | `0x0041F140` | Packet construction |
| `CDataStore__Reset` | `0x00425C30` | Packet reset |
| `CDataStore__GrowBuffer` | `0x00422DA0` | Buffer growth |
| `SFile__Free` | `0x0065D4E0` | File memory free |
| `CMap__SafeOpen` | `0x006B8D10` | Map safe-open |
| `CMap__SafeRead` | `0x006B8EF0` | Map safe-read |
| `CMap__LoadWdt` | `0x006BB6A0` | WDT load |
| `WowClientDB__BuildIndex` | `0x00405060` | DBC index build |
| `StaticDBLoadAll` | `0x00573C90` | DBC load entry point |
| `ObjectMgrClient__RegisterHandlers` | `0x0046E060` | Object manager |
| `Script_GetItemInfo` | `0x0049B6F0` | GetItemInfo Lua impl |
| `Script_GetTalentTabInfo` | `0x0050A0E0` | GetNumTalentTabs |
| `Script_GetNumTalents` | `0x0050A250` | GetNumTalents |
| `Script_LearnTalent` | `0x0050A410` | LearnTalent |
| `Script_GetTalentInfo` | `0x0050A910` | GetTalentInfo |
| `Script_GetSpellTabInfo` | `0x004C1AE0` | GetSpellTabInfo |
| `Script_CastSpellByName` | `0x004C42E0` | CastSpellByName |
| `Script_GetActionInfo` | `0x00526550` | GetActionInfo |
| `Script_GetActionCount` | `0x00525890` | GetActionCount |
| `Script_GetActionText` | `0x00525900` | GetActionText |
| `Script_GetActionCooldown` | `0x00526690` | GetActionCooldown |
| `Script_GetActionTexture` | `0x005271F0` | GetActionTexture |
| `Script_UnitClass` | `0x005451F0` | UnitClass |
| `Script_UnitGUID` | `0x00543CA0` | UnitGUID |
| `Script_GetWorldLocMapPosition` | `0x004B2750` | GetPlayerMapPosition |
| `g_netClientList` | `0x00B9F940` | Net client list |
| `g_pendingMapID` | `0x00BDB05C` | Pending map ID |
| `g_glueMgrState` | `0x00C07D2C` | Glue manager state |

### ❌ Still TODO_TBC (addresses not yet in sym files)

| Module | What is needed |
|--------|----------------|
| `FrameScript.cpp` | All Lua function addresses (`lua_pushstring`, `lua_tonumber`, `lua_toboolean`, `lua_settop`, `FrameScript__RegisterFunction`, `FrameScript__SignalEvent`, `FrameScript__LoadFunctions`, global `lua_State*`, etc.) |
| `CDataStore.cpp` | `CDataStore::Put*` and `CDataStore::Get*` method addresses (packet read/write) |
| `SFile.cpp` | `SFile::CloseFile` address |
| `Client/CNetClient.cpp` | `SendActionButton` call site, `ProcessMessage` patch addresses, group spell launch handler |
| `Client/CustomLua.cpp` | `Apply()` patch call site, `FrameScript::GetState()` global address, FPS global |
| `Client/WoWTime.cpp` | All WoWTime patch call sites |
| `Client/CGPlayer.cpp` | Character creation race table patch addresses |
| `Client/CGTooltip.cpp` | Tooltip patch addresses |
| `Data/MiscAddresses.hpp` | Render flags, `g_currentMapID`, game window HWND, action button arrays, UI coordinate multipliers |
| `Data/DBCAddresses.hpp` | All DBC global instance pointers (50+ tables) |
| `Main.cpp` | Invalid-function-pointer hack addresses (`FrameScript__LoadFunctions` bootstrap area) |
| `CDBCMgr/CDBCMgr.cpp` | `RegisterDBCEx` call target + patch site in `StaticDBLoadAll` |

---

## Features

- **200+ Backported Lua API functions** from WotLK, Cataclysm, and MoP
- **Compatibility stubs** for WotLK+ features that return safe defaults in TBC
- **Modern C_ namespace APIs** (C_QuestLog, C_Map, C_Container, C_ChatInfo)
- **Custom DBC loader** (`CDBCMgr`) for adding new database tables
- **Custom packet system** for server↔client communication
- **CVar system** — read and write client CVars from Lua
- **Spell description parser** (`SpellParser`) — custom variables in spell tooltips (`$hp`, `$power1`, etc.)
- **Formula computation** (`CFormula`) — evaluate stat/damage formulas at runtime
- **Action bar extensions** — extra Lua functions such as `ReplaceActionBarSpell`
- **Developer helper functions** — `ReloadMap`, toggle rendering of liquids, terrain, and more
- **NOAMMO patch** — optionally removes the ammo requirement for ranged weapons
- **Combo-point fix** — makes combo points work for all classes, not just Rogues/Druids
- **Character-creation race fix** — prevents crash when more than 21 playable races are defined
- **Item-mod extension** — expands the `ITEM_MOD` table to display custom tooltip stats
- **All WotLK-only features removed**: No LFD, no ZoneLight DBCs, no Glyphs, no Vehicles
- **Full TBC 2.4.3 field layouts** for all game objects (build 8606)

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

1. **Find remaining TODO_TBC addresses** — see [TBC_PORTING_GUIDE.md](TBC_PORTING_GUIDE.md)
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
- [Docs/symbols/func.sym](Docs/symbols/func.sym) — TBC 2.4.3 function symbol addresses
- [Docs/symbols/label.sym](Docs/symbols/label.sym) — TBC 2.4.3 data label addresses

---

## Credits

- **Original WotLK-Extensions**: [Alyst3r](https://github.com/Alyst3r/WotLK-Extensions)
- **TBC Port**: TBC-Extensions contributors
- Techniques inspired by AwesomeWotLK and TSWoW projects

---

## Special Thanks

A **big thank you** to **Alyst3r** for his invaluable help and support throughout the development of this project. 🙏
A **big thank you** to **bloodfangx** for his invaluable help and support throughout the development of this project. (he give us adresses) 🙏