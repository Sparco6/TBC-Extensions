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

# TBC-Extensions

A native x86 extension framework for **World of Warcraft: The Burning Crusade 2.4.3 (build 8606)**.

TBC-Extensions extends the original client through `TBCExtensions.dll` and provides a controlled foundation for native Lua APIs, custom client data, developer tooling, reverse engineering, texture compatibility research, and future client-side feature development.

The project originally started as a TBC port of [WotLK-Extensions](https://github.com/Alyst3r/WotLK-Extensions), but the current codebase increasingly contains TBC-specific architecture, independently verified build-8606 addresses, runtime safety gates, research tooling, and native compatibility work.

> **Status: ACTIVE DEVELOPMENT / RESEARCH**
>
> The safe TBCExtensions foundation is operational on the target TBC 2.4.3 build 8606 client.
>
> Several legacy WotLK-derived modules still contain unverified TBC addresses and are therefore isolated behind:
>
> `UNSAFE_LEGACY_PORT=OFF`
>
> Do not interpret the presence of legacy source code as proof that every historical feature is production-ready.

---

## Target Client

| Property | Value |
|---|---|
| Game | World of Warcraft: The Burning Crusade |
| Version | 2.4.3 |
| Build | 8606 |
| Architecture | Win32 / x86 |
| DLL | `TBCExtensions.dll` |
| Language | C++17 |
| Hook library | MinHook |
| Build system | CMake |
| Primary compiler | Visual Studio / MSVC |

TBC-Extensions is tightly coupled to the exact client build.

Addresses and structures from WotLK 3.3.5a or other WoW versions must **not** be reused without independent TBC 8606 verification.

---

# Project Architecture

The current safe runtime architecture is:

```text
WoW.exe 2.4.3.8606
        │
        ▼
custom executable DLL loader
        │
        ▼
TBCExtensions.dll
        │
        ├── client fingerprint validation
        ├── executable/signature validation
        ├── centralized build-8606 offset registry
        ├── MinHook lifecycle hooks
        ├── callback validator
        ├── native Lua bridge
        ├── Developer Toolkit bridge
        ├── DLL-owned Custom DBC subsystem
        ├── BLP / texture research subsystem
        └── legacy modules
                │
                └── disabled unless explicitly enabled
```

The existing `TBCExtensions.dll` is the project foundation. New client research and compatibility work should extend this architecture rather than creating a second loader or parallel extension DLL.

---

# Runtime Safety

A major focus of the current project is making reverse-engineered client functionality **fail closed**.

The safe runtime performs client validation before enabling sensitive native functionality.

Current protections include:

- exact x86 client expectations
- expected PE image base
- expected PE timestamp
- expected image size
- critical code-byte validation
- hook target signature validation
- executable/readable memory validation
- callback allowlisting
- build-specific address registry
- ambiguity shutdown in research hooks
- controlled developer-only experimental gates
- stock behavior passthrough when a research condition does not match

Experimental native hooks should never silently activate on an unknown client build.

---

# Centralized TBC 8606 Address Registry

Verified safe-core addresses are centralized in:

```text
TBCExtensions/src/Offsets/ClientOffsets_8606.hpp
```

This replaces scattering build-specific absolute addresses throughout runtime code.

Examples currently represented by the registry include the TBC Lua API, FrameScript registration, global Lua state access, and callback validation infrastructure.

```cpp
namespace Offsets8606
{
    constexpr std::uintptr_t LuaLError                   = 0x0072F5C0;
    constexpr std::uintptr_t LuaToBoolean                = 0x0072DFC0;
    constexpr std::uintptr_t LuaToNumber                 = 0x0072DF40;
    constexpr std::uintptr_t LuaToLString                = 0x0072DFF0;
    constexpr std::uintptr_t LuaGetTop                   = 0x0072DAE0;
    constexpr std::uintptr_t LuaSetTop                   = 0x0072DB00;
    constexpr std::uintptr_t LuaIsNumber                 = 0x0072DE30;
    constexpr std::uintptr_t LuaIsString                 = 0x0072DE70;
    constexpr std::uintptr_t LuaType                     = 0x0072DDC0;
    constexpr std::uintptr_t LuaPushNil                  = 0x0072E180;
    constexpr std::uintptr_t LuaPushNumber               = 0x0072E1A0;
    constexpr std::uintptr_t LuaPushString               = 0x0072E250;
    constexpr std::uintptr_t LuaPushBoolean              = 0x0072E3B0;
    constexpr std::uintptr_t LuaPushCClosure             = 0x0072E2F0;
    constexpr std::uintptr_t LuaGetField                 = 0x0072E550;
    constexpr std::uintptr_t LuaSetField                 = 0x0072E7E0;

    constexpr std::uintptr_t FrameScriptRegisterFunction = 0x007059B0;
    constexpr std::uintptr_t GlobalLuaStatePointer       = 0x00E1DB84;
    constexpr std::uintptr_t ValidateFunctionPointer     = 0x0074A160;
}
```

These addresses are specific to the project's target **TBC 2.4.3 build 8606 client**.

The repository still contains legacy modules with unresolved `TODO_TBC` addresses. Those must not be confused with the verified safe-core registry.

---

# Native Lua Bridge

TBC-Extensions contains a native Lua bridge capable of registering DLL-owned functions with the TBC client.

The bridge includes lifecycle handling because the client can rebuild or replace its Lua global environment during normal operation.

The runtime can detect the relevant Lua state lifecycle and re-register the safe TBCExtensions API when necessary.

The safe research API includes functionality for querying:

- TBCExtensions version
- client build
- module base
- Lua state
- native API version
- active client profile
- address information
- executable/readable address state
- callback validator state
- native tracing state
- model/research hook state
- Custom DBC state

The bridge is deliberately separated from unfinished legacy Lua patches.

---

# Developer Toolkit

The project includes a large in-game **MasterWoW Developer Toolkit** used to inspect the TBC client and TBCExtensions runtime.

Current areas include tooling for:

```text
Lua Console
API / Globals
CVars
Frames
Events
Unit / Target
Items
Spells
Actions / Macros
Textures
Watch
Performance
Capabilities
Addons / Snippets
Research / Export
Native / DLL
Native Functions
Client Internals
DBC Inspector
Model Browser
```

The Toolkit is an important part of the reverse-engineering workflow.

It allows research features to expose their state without automatically executing arbitrary candidate addresses.

An address being visible in the Toolkit is **not** by itself proof that the address has been runtime verified.

---

# Custom DBC Support

TBC-Extensions contains a DLL-owned WDBC loader.

Custom tables are loaded into TBCExtensions-owned memory rather than being blindly injected into the stock client DBC manager.

Supported operations include:

```text
load
info
row
field
unload
```

Supported field interpretations include:

```text
uint32
float
string
```

The loader performs structural validation including:

- WDBC magic validation
- bounded file size
- bounded row count
- bounded field count
- fixed 4-byte field validation
- exact payload length validation
- string-block validation
- duplicate ID rejection
- safe filename/path validation

Custom DBC files are expected under:

```text
<TBC Client>\TBCExtensions\DBFilesClient\
```

Custom table names use the project namespaces:

```text
AoR_*
TBCExt_*
```

The Custom DBC subsystem does **not** imply automatic integration with every stock client database or rendering system.

For example, loading a custom creature display row does not automatically make the stock TBC renderer understand modern retail models.

---

# BLP / Texture Compatibility Research

One of the most developed current research areas is extending the TBC texture pipeline while preserving as much of the original client renderer and resource lifetime system as possible.

The texture subsystem includes code under:

```text
TBCExtensions/src/Graphics/Textures/
```

including components for:

```text
BLPReader
BLPInspector
BLP2Decoder
BLPFormat
NativeTextureData
BLPRuntimeTrace
NativeBLPCorrelation
```

The project uses both offline parsing and narrowly gated runtime observation.

---

## Stock TBC BLP2 Support

Runtime testing has established working stock paths for controlled:

- Palette textures
- DXT1
- DXT3
- DXT5

DXT5 has been traced through the native TBC resource and GPU upload pipeline.

A verified controlled DXT5 example uses:

```text
BLP2
encoding:      2
alpha depth:   8
alpha encoding: 7

resource selectors: 7/7
GPU format: DXT5
copy class: 7/7
```

For the controlled 64×64 test:

```text
source pitch:      256
destination pitch: 256
block rows:         16
payload:          4096 bytes
```

The resulting texture renders correctly through the stock client.

---

# Native BGRA8 Compatibility Research

TBC build 8606 also recognizes the tested BLP2 uncompressed BGRA8 header but does not classify the controlled file correctly by default.

The controlled source is:

```text
BLP2
encoding:        3
alpha depth:     8
alpha encoding:  0
dimensions:      64x64
```

The unmodified client classifies this controlled resource as:

```text
resource selectors: 5/0
descriptor class:    5/5
GPU format:          DXT1
```

This produces corrupted/striped output because raw BGRA data is routed through DXT1-shaped upload geometry.

Runtime research established a coherent native TBC route:

```text
resource selectors: 1/2
descriptor class:    1/1
GPU format:          A8R8G8B8
copy path:           32-bit row copy
```

The controlled compatibility prototype forwards the classifier from:

```text
5/0
```

to:

```text
1/2
```

for the exact developer-controlled resource.

The verified 64×64 runtime result is:

```text
BLP2: encoding 3 / alpha 8/0

classifier:
    incoming  5/0
    forwarded 1/2

resource:
    selectors 1/2

descriptor:
    class 1/1

GPU:
    D3DFMT_A8R8G8B8

source pitch:
    256

destination pitch:
    256

rows:
    64

copied bytes:
    16384
```

And:

```text
64 × 64 × 4 = 16384 bytes
```

The controlled visual renders correctly.

This result is classified as:

```text
CONTROLLED_NATIVE_BGRA8_1_2_VERIFIED_RUNTIME
```

No custom D3D texture is required for this verified case.

No runtime BGRA→DXT5 conversion is required.

No replacement pixel buffer is required.

The compatibility prototype uses the stock TBC descriptor, texture creation, upload, and resource lifetime pipeline after correcting the controlled format classification.

---

## Important BGRA8 Limitation

The native BGRA8 compatibility mechanism is still **developer research**, not a globally enabled production feature.

The verified result currently applies to the controlled test case.

Research is continuing into:

- full mip chains
- dimensions other than 64×64
- non-square raw BGRA textures
- small mip levels down to 1×1
- resource/cache reuse
- descriptor recreation
- device-reset lifetime behavior
- larger real-world BLP corpora

TBC-Extensions must **not** globally reinterpret every BLP2 encoding-3 texture until these cases are sufficiently verified.

The current design intentionally prefers narrow allowlists, metadata validation, and fail-closed behavior.

---

# BLP Offline Tooling

The repository also contains independent BLP parsing/inspection functionality.

The parser validates:

- BLP headers
- dimensions
- encoding
- alpha depth
- alpha encoding
- mip offsets
- mip sizes
- payload bounds
- mip progression
- overlapping ranges
- decoded memory budgets
- malformed/truncated data

The classifier is intended to distinguish between categories such as:

```text
STOCK_TBC_SUPPORTED
NATIVE_BGRA_1_2_STRUCTURAL_CANDIDATE
TBCExt_DECODE_ONLY
TRANSCODE_REQUIRED
UNSUPPORTED
MALFORMED
UNKNOWN
```

Compatibility is determined from actual file structure rather than expansion name or filename.

---

# Real-World BLP Corpus Testing

Real-world UI asset classification has shown that many newer BLP2 assets still use formats already understood by the stock TBC client.

A large number of tested UI assets use:

```text
BLP2
encoding 2
alpha 8/7
DXT5
```

including:

- large textures
- non-square textures
- multi-mip textures
- long mip chains

These remain on the stock TBC texture path.

The BGRA compatibility research is intentionally isolated from stock DXT behavior.

---

# Research Evidence Levels

The project uses explicit evidence labels.

### `VERIFIED_RUNTIME`

Observed in the target TBC 2.4.3 build 8606 client during a controlled runtime test.

### `VERIFIED_STATIC`

Supported by disassembly, signatures, control flow, or other static analysis, but not necessarily exercised by the controlled runtime path.

### `CONTROLLED_PROTOTYPE`

Implemented only for a narrowly defined developer-controlled experiment.

### `CONTROLLED_STRUCTURAL_CANDIDATE`

File structure satisfies the currently understood compatibility contract but has not yet passed owner runtime testing.

### `CANDIDATE`

Plausible interpretation requiring additional evidence.

### `UNKNOWN`

Insufficient evidence.

### `DEPRECATED / SUPERSEDED`

An older conclusion that has been replaced by stronger evidence.

This distinction is important throughout the repository.

A mapped address, readable pointer, parser success, or static code path should not automatically be described as runtime verified.

---

# Legacy Port Isolation

The repository still contains substantial code originating from the original WotLK extension framework.

Some modules remain incomplete for TBC.

They may contain:

```text
TODO_TBC
unverified addresses
WotLK-specific assumptions
unverified object layouts
unverified calling conventions
unfinished patch sites
```

These paths are isolated by:

```cmake
UNSAFE_LEGACY_PORT=OFF
```

The recommended development configuration keeps this option disabled.

Do not enable legacy patches simply because the project compiles.

---

# Current CMake Options

| Option | Default | Purpose |
|---|---:|---|
| `CUSTOM_DBC` | OFF | Custom DBC loader |
| `NOAMMO_PATCH` | OFF | Remove ranged ammo requirement |
| `CHARCREATIONRACE_FIX` | ON | Legacy character creation race fix |
| `COMBOPOINT_FIX` | ON | Legacy combo-point extension |
| `OOBLUAFUNCTIONS_PATCH` | ON | Allow DLL-owned Lua callbacks |
| `CUSTOMPACKETS_PATCH` | OFF | Custom packet framework |
| `ITEMMODEXT_PATCH` | OFF | Extended custom item tooltip mods |
| `SPELLDESCRIPTIONEXT_PATCH` | OFF | Extended spell-description variables |
| `ACTIONBAR_LUA` | OFF | Additional action-bar Lua API |
| `DEVHELPER_LUA` | OFF | Developer-oriented Lua helpers |
| `UNSAFE_LEGACY_PORT` | **OFF** | Incomplete WotLK-derived code paths |

For research and normal development:

```text
UNSAFE_LEGACY_PORT=OFF
```

should remain the baseline.

---

# Build Requirements

Recommended environment:

- Windows
- Visual Studio 2022 or compatible MSVC toolchain
- C++17
- CMake 3.25+
- Windows SDK
- Win32/x86 target

The original TBC client is 32-bit, therefore the DLL must also be built as x86.

---

# Building

Example:

```bash
cmake -S . -B build -A Win32
cmake --build build --config Release
```

Depending on the selected build directory/layout, the resulting DLL will be available under the Release output directory.

Always verify that the resulting PE machine type is:

```text
0x14C / IMAGE_FILE_MACHINE_I386
```

Do not deploy an x64 build into the TBC client.

---

# Installation

TBC-Extensions requires the project's compatible custom TBC executable loader.

Typical runtime layout:

```text
World of Warcraft\
│
├── WoW.exe
├── TBCExtensions.dll
│
└── TBCExtensions\
    ├── DBFilesClient\
    └── ...
```

The current executable loader is an existing project baseline.

Texture, BLP, or retail-asset research should **not** modify the loader unless a separate loader-specific task explicitly requires it.

---

# Source Layout

Important current areas include:

```text
TBCExtensions/
│
├── src/
│   │
│   ├── CDBCMgr/
│   │   ├── CDBC
│   │   ├── CDBCMgr
│   │   ├── NativeDBC
│   │   └── WdbcTable
│   │
│   ├── Client/
│   │   ├── CustomLua
│   │   ├── FrameScript
│   │   ├── CDataStore
│   │   ├── CNetClient
│   │   ├── DBClient
│   │   ├── SFile
│   │   ├── Spell
│   │   └── ...
│   │
│   ├── Graphics/
│   │   └── Textures/
│   │       ├── BLPReader
│   │       ├── BLPInspector
│   │       ├── BLP2Decoder
│   │       ├── BLPRuntimeTrace
│   │       ├── NativeBLPCorrelation
│   │       └── NativeTextureData
│   │
│   ├── GameObjects/
│   ├── WorldData/
│   ├── Data/
│   ├── Misc/
│   │
│   └── Offsets/
│       └── ClientOffsets_8606.hpp
│
├── include/
├── cmake/
└── CMakeLists.txt
```

---

# Future Retail Asset Work

Long-term research aims to investigate compatibility with newer WoW client assets while keeping the TBC client as the runtime foundation.

Potential future areas include:

```text
RetailAssets/
    FileData/
    M2/
    SKIN/
    Anim/
    DBC/

Renderer/
    D3D9/
    BonePalette/
    Attachments/
    Particles/
    Ribbons/

Graphics/
    Textures/
        BLP1
        BLP2
        DXT
        TextureCache
```

Portable parsing/rendering concepts from newer clients or reference projects may be studied, but all integration with TBC must use independently verified build-8606 addresses, structures, ABI, and lifetime behavior.

Modern M2/SKIN rendering is **not currently a production-ready feature**.

---

# Current Development Priorities

Current research is progressing approximately through:

```text
Safe TBCExtensions foundation
        ↓
Native Lua / Toolkit
        ↓
Custom DBC
        ↓
BLP format capability mapping
        ↓
Native BGRA8 compatibility research
        ↓
multi-mip / non-square / lifetime verification
        ↓
larger real-world texture corpus
        ↓
texture compatibility layer
        ↓
modern M2 / SKIN research
        ↓
isolated creature rendering prototype
        ↓
broader retail asset integration
```

The project intentionally favors incremental evidence over large speculative patches.

---

# Documentation

The `Docs/` directory contains detailed research notes and implementation status.

Recommended starting points:

- [`Docs/ARCHITECTURE.md`](Docs/ARCHITECTURE.md)
- [`Docs/DEVELOPER_HANDOFF.md`](Docs/DEVELOPER_HANDOFF.md)
- [`Docs/REVERSE_ENGINEERING_STATUS.md`](Docs/REVERSE_ENGINEERING_STATUS.md)
- [`Docs/TBC_8606_ADDRESS_MAP.md`](Docs/TBC_8606_ADDRESS_MAP.md)
- [`Docs/BLP_TEXTURE_RESEARCH.md`](Docs/BLP_TEXTURE_RESEARCH.md)
- [`Docs/TBC_BLP_CAPABILITY_AUDIT.md`](Docs/TBC_BLP_CAPABILITY_AUDIT.md)
- [`Docs/TBC_BLP2_RUNTIME_TRANSFORM.md`](Docs/TBC_BLP2_RUNTIME_TRANSFORM.md)
- [`Docs/TESTING_GUIDE.md`](Docs/TESTING_GUIDE.md)
- [`Docs/BUILD_AND_INSTALL.md`](Docs/BUILD_AND_INSTALL.md)
- [`Docs/ROADMAP.md`](Docs/ROADMAP.md)
- [`Docs/KNOWN_ISSUES.md`](Docs/KNOWN_ISSUES.md)
- [`Docs/CHANGELOG_RESEARCH.md`](Docs/CHANGELOG_RESEARCH.md)

Legacy address documentation may contain superseded information.

For the safe native bridge, prefer the current:

```text
ClientOffsets_8606.hpp
TBC_8606_ADDRESS_MAP.md
REVERSE_ENGINEERING_STATUS.md
DEVELOPER_HANDOFF.md
```

over older initial-port TODO reports.

---

# Development Rules

When contributing native client work:

1. Target the exact **TBC 2.4.3 build 8606** client.
2. Never copy WotLK absolute addresses into TBC runtime code.
3. Independently verify every new native address.
4. Prefer centralized address registries.
5. Verify hook prologues/signatures before installation.
6. Fail closed on mismatches.
7. Keep experimental behavior developer-gated.
8. Preserve stock behavior outside the exact experiment.
9. Distinguish static evidence from runtime evidence.
10. Keep `UNSAFE_LEGACY_PORT=OFF` unless explicitly researching a legacy module.

For texture research specifically:

```text
Do not globally patch format tables.
Do not globally reinterpret encoding 3.
Do not replace working DXT paths.
Do not assume parser success means renderer compatibility.
Do not assume one successful mip proves a complete mip chain.
```

---

# Project Philosophy

TBC-Extensions is no longer simply about filling `TODO_TBC` addresses.

The project is evolving into a build-specific native extension and reverse-engineering framework for the TBC 8606 client.

The preferred workflow is:

```text
identify
    ↓
statically verify
    ↓
instrument safely
    ↓
observe runtime behavior
    ↓
build controlled prototype
    ↓
owner runtime test
    ↓
regression test
    ↓
generalize only when evidence supports it
```

This keeps experimental client work isolated while gradually building a reliable foundation for larger compatibility features.

---

# Credits

### Original foundation

- [Alyst3r / WotLK-Extensions](https://github.com/Alyst3r/WotLK-Extensions)

### TBC development

- TBC-Extensions contributors
- MasterWoW client research and build-8606 testing

### Special thanks

Special thanks to **Alyst3r** for the original WotLK-Extensions project and development support.

Special thanks to **bloodfangx** for TBC client research and address assistance.

Additional architectural and reverse-engineering concepts have been studied from community WoW client projects and research implementations. Build-specific code, addresses, structures, and runtime behavior must always be independently validated for TBC 2.4.3 build 8606.

---

# Contact

Discord:

```text
sparco6
```

Contributions, build-8606 reverse-engineering results, reproducible runtime traces, and carefully validated client research are welcome.

---

> **Important**
>
> TBC-Extensions is an experimental client modification and reverse-engineering project.
>
> Keep backups of the original client executable and DLLs, use the exact supported build, and do not treat unfinished research modules as production-ready features.

## Special Thanks

A **big thank you** to **Alyst3r** for his invaluable help and support throughout the development of this project. 🙏
A **big thank you** to **bloodfangx** for his invaluable help and support throughout the development of this project. (he give us adresses) 🙏
