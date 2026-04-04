# TBC Porting Guide

Complete guide to finding TBC 2.4.3 (build 8606) addresses for TBC-Extensions.

---

## Required Tools

| Tool | Purpose | Link |
|------|---------|------|
| **IDA Pro** (Free) | Disassembly, cross-references, renaming | https://hex-rays.com/ida-free/ |
| **Ghidra** | Free alternative to IDA | https://ghidra-sre.org/ |
| **x32dbg** | Dynamic debugger (attach to running WoW) | https://x64dbg.com/ |
| **CFF Explorer** | PE header viewer/editor | https://ntcore.com/?page_id=388 |
| **HxD** | Hex editor for patcher offsets | https://mh-nexus.de/en/hxd/ |

---

## Methodology Overview

1. Open `WoW.exe` (TBC 2.4.3, build 8606) in IDA Pro or Ghidra
2. Find each function using the techniques below
3. Record the **virtual address** (shown in IDA as hex like `0x400000 + offset`)
4. Replace every `0x000000 /* TODO_TBC */` comment in the source code
5. Rebuild and test

---

## Finding Core Addresses

### FrameScript / Lua Functions

| Function | How to Find |
|----------|------------|
| `FrameScript__RegisterFunction` | Search for string `"FrameScript"`, trace to function that takes (name, ptr) |
| `FrameScript__LoadFunctions` | Find the function called early at startup that registers all built-in Lua functions |
| `lua_pushstring` | Find code pushing string literals to Lua stack — cross-ref any `lua_State*` usage |
| `lua_pushnumber` | Find numeric push calls near combat rating or health display code |
| `lua_tostring` | Find GetSpellInfo or similar that reads string argument from Lua |
| `FrameScript__SignalEvent` | Search for `"PLAYER_ENTERING_WORLD"` string, trace call |

### ClientServices / Object Manager

| Function | How to Find |
|----------|------------|
| `GetActivePlayer` | Search for `"player"` unit token string in Lua unit functions, trace GUID getter |
| `GetObjectPtr` | Cross-reference from any code that calls GetActivePlayer then uses the result |
| `GetCharacterClass` | Find where UnitClass is read from player unit fields |

### DBClient

| Function | How to Find |
|----------|------------|
| `GetLocalizedRow` | Find Spell.dbc row access in tooltip rendering code |
| `GetRow` | Find the faster non-localized DBC row accessor |
| `GetGameTableValue` | Locate gt*.dbc files (gtCombatRatings etc.) read in combat formulas |

### DBC Addresses

Open IDA's Imports/Data view and search for patterns like:
- `DB2Client` struct alignment (vtable ptr, bool, int, int, int, char*, void*, int*, int*)
- Or search for DBC loading code with string `"DBFilesClient\\"` to find the DB array

---

## Finding Patch Addresses

### OOBLUAFUNCTIONS_PATCH (Out-of-Binary Lua Functions)

1. In IDA, find `FrameScript__LoadFunctions` 
2. Look for two consecutive 32-bit values initialized at the start  
   (WotLK 3.3.5: `0xD415B8` = 1, `0xD415BC` = 0x7FFFFFFF)
3. These are the "function table extent" limits for Lua function registration

### CustomLua::Apply Hook

1. Find where `FrameScript__LoadFunctions` is called during startup
2. Patch the call to redirect to `LoadScriptFunctionsCustom` first
3. `LoadScriptFunctionsCustom` registers custom functions then calls the original

### CUSTOMPACKETS_PATCH

1. Find `NetClient::ProcessMessage` — the main incoming packet handler
2. Find `NetClient::SetMessageHandler` — where opcode handlers are registered
3. Patch call sites to our `ProcessMessageEx` / `SetMessageHandlerEx` wrappers

---

## Finding Patcher Offsets

Patcher offsets are **file offsets** (not virtual addresses). Use HxD:

1. Open `WoW.exe` in HxD
2. Use Ctrl+G to go to a known virtual address
3. Convert: `file_offset = VA - ImageBase` (TBC ImageBase is usually `0x00400000`)
4. For the DLL name string: search for `WoW.exe` or a blank area in `.rdata`

---

## Pattern Scanning

For finding addresses in a new TBC build:

```cpp
// Example: Find lua_pushstring by pattern
// Typical signature: 55 8B EC 8B 45 08 85 C0 74 ...
uint8_t pattern[] = { 0x55, 0x8B, 0xEC, 0x8B, 0x45, 0x08, 0x85, 0xC0, 0x74, 0x?? };
```

Tools like Sig Maker (IDA plugin) can generate patterns automatically.

---

## Testing Strategy

1. Start with `OOBLUAFUNCTIONS_PATCH` only — verify `/script print("Hello")` works
2. Enable `DEVHELPER_LUA` — test `ToggleTerrain()` or `ReloadMap()`  
3. Enable `ACTIONBAR_LUA` — test `FindSpellActionBarSlots(spellID)`
4. Enable `CUSTOMPACKETS_PATCH` — verify no crash on login
5. Enable `CUSTOM_DBC` — test with a simple custom DBC file

---

## Debugging Tips

- Use `LOG_DEBUG << "Address value: " << somePtr;` to log from the DLL  
  (logs appear in `DLL_Logs/` folder)
- Attach x32dbg to WoW.exe BEFORE login screen appears
- Set breakpoints at your patched addresses to verify control flow
- If WoW crashes immediately: the patch address is wrong
- If nothing happens: `Apply()` hook address is wrong
- If Lua returns nil: function registered but implementation needs work

---

## TBC-Specific Notes

- **No LFD/Dungeon Finder**: Classes have no role mask in base TBC
- **No ZoneLight DBC**: Zone lighting handled differently in TBC
- **No Rune/Glyph system**: TBC talent system is simpler (3 trees, 51 points max)
- **Gems exist in TBC**: Socket bonus, gem properties are fully present
- **TBC has 10 classes max**: No Death Knight (classID 6 is empty)
- **Combo points work differently**: Rogue/Druid only by default
