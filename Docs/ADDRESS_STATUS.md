# TBC Address Finding Progress

> Updated by running `python Tools/find_addresses.py`.  
> Tick a box only after the address has been **verified** in Ghidra / x32dbg
> and updated in the source code.

---

## Overall Progress: 0 / 44 (0%)

```
████░░░░░░░░░░░░░░░░░░░░░░░░░░░░  0%
```

---

## 🔴 CRITICAL Functions (0 / 5 complete)

These must be found first — the extension will not load without them.

| Status | Function | WotLK Addr | TBC Addr | Notes |
|--------|----------|-----------|---------|-------|
| [ ] | `FrameScript__LoadFunctions` | `0x5120E0` | TODO_TBC | Registers all Lua API functions |
| [ ] | `FrameScript__RegisterFunction` | `0x817F90` | TODO_TBC | Maps name→fn pointer |
| [ ] | `FrameScript__GetState` | `*(0x884BAC)` | TODO_TBC | Global lua_State* |
| [ ] | `CVar__Set` | `0x766940` | TODO_TBC | Set a CVar value |
| [ ] | `SErr__PrepareAppFatal` | `0x772A80` | TODO_TBC | Fatal error handler |

---

## 🟠 HIGH Priority — Lua C-API (0 / 18 complete)

Core Lua C-API functions embedded in WoW.exe.

| Status | Function | WotLK Addr | TBC Addr |
|--------|----------|-----------|---------|
| [ ] | `lua_error` | `0x84F280` | TODO_TBC |
| [ ] | `lua_toboolean` | `0x84E0B0` | TODO_TBC |
| [ ] | `lua_tonumber` | `0x84E030` | TODO_TBC |
| [ ] | `FrameScript__GetParam` | `0x815500` | TODO_TBC |
| [ ] | `lua_tolstring` | `0x84E0E0` | TODO_TBC |
| [ ] | `FrameScript__GetText` | `0x819D40` | TODO_TBC |
| [ ] | `lua_gettop` | `0x84DBD0` | TODO_TBC |
| [ ] | `lua_isnumber` | `0x84DF20` | TODO_TBC |
| [ ] | `lua_isstring` | `0x84DF60` | TODO_TBC |
| [ ] | `lua_pushboolean` | `0x84E4D0` | TODO_TBC |
| [ ] | `lua_pushnil` | `0x84E280` | TODO_TBC |
| [ ] | `lua_pushnumber` | `0x84E2A0` | TODO_TBC |
| [ ] | `lua_pushstring` | `0x84E350` | TODO_TBC |
| [ ] | `lua_settop` | `0x84DBF0` | TODO_TBC |
| [ ] | `FrameScript__SignalEvent` | `0x81B530` | TODO_TBC |
| [ ] | `lua_getfield` | `0x84E620` | TODO_TBC |
| [ ] | `lua_setfield` | `0x84E680` | TODO_TBC |
| [ ] | `lua_pushcclosure` | `0x84E460` | TODO_TBC |
| [ ] | `lua_type` | `0x84DEC0` | TODO_TBC |

## 🟠 HIGH Priority — Native Lua API (0 / 5 complete)

Script_ functions already named in func.sym — verify addresses are correct
and update source if not yet done.

| Status | Function | TBC Addr (func.sym) | Verified? |
|--------|----------|---------------------|-----------|
| [ ] | `Script_GetSpellInfo` | *(not found — search needed)* | No |
| [ ] | `Script_UnitGUID` | `0x00543CA0` | No |
| [ ] | `Script_GetItemInfo` | `0x0049B6F0` | No |
| [ ] | `Script_UnitHealth` | `0x00544190` | No |
| [ ] | `Script_GetCameraPosition` | *(not found — search needed)* | No |

---

## 🟡 MEDIUM Priority — Utility Functions (0 / 13 complete)

| Status | Function | WotLK Addr | TBC Addr |
|--------|----------|-----------|---------|
| [ ] | `SStr__Printf` | `0x76F070` | TODO_TBC |
| [ ] | `SStr__Append` | `0x76EF70` | TODO_TBC |
| [ ] | `SStr__Copy` | `0x76ED20` | TODO_TBC |
| [ ] | `SFile__CloseFile` | `0x422910` | TODO_TBC |
| [ ] | `CFormula__GetVariableValue` | `0x5782D0` | TODO_TBC |
| [ ] | `DBClient__GetGameTableValue` | `0x7F6990` | TODO_TBC |
| [ ] | `DBClient__GetLocalizedRow` | `0x4CFD20` | TODO_TBC |
| [ ] | `CGChat__AddChatMessage` | `0x509DD0` | TODO_TBC |
| [ ] | `SpellParser__ParseText` | *(unknown)* | TODO_TBC |
| [ ] | `CGUnit__GetShapeshiftFormID` | *(unknown)* | TODO_TBC |
| [ ] | `CGPlayer__IsDeadOrGhost` | *(unknown)* | TODO_TBC |
| [ ] | `CGPetInfo__GetPet` | *(unknown)* | TODO_TBC |

---

## 🟢 LOW Priority — Global Variables & Misc (0 / 8 complete)

| Status | Variable / Function | WotLK Addr | TBC Addr | Notes |
|--------|---------------------|-----------|---------|-------|
| [ ] | `renderFlags1–4` | `0xCD774C` | TODO_TBC | Bool array near rendering CVars |
| [ ] | `g_currentMapID` | `0xBD088C` | TODO_TBC | Global int32 map ID |
| [ ] | `g_window` | `0xD41620` | TODO_TBC | Game window HWND |
| [ ] | `g_actionButtonsArray` | `0xC1DED8` | TODO_TBC | Action bar button array |
| [ ] | `g_actionBarSpellIDArray` | `0xC1E358` | TODO_TBC | Action bar spell ID array |
| [ ] | `DNInfo__AddZoneLight` | *(unknown)* | TODO_TBC | |
| [ ] | `DNInfo__GetDNInfoPtr` | *(unknown)* | TODO_TBC | |
| [ ] | Bootstrap hack addresses | `0xD415B8` / `0xD415BC` | TODO_TBC | Invalid function pointer hack |

---

## Workflow

```
1. Run:  python Tools/find_addresses.py
         → generates Docs/ADDRESS_FINDINGS.md with candidates

2. Open  Docs/GHIDRA_ADDRESS_FINDING.md
         → follow step-by-step guide for each function

3. Verify candidate address in Ghidra / x32dbg

4. Update Docs/symbols/address_map.json:
         "verification_status": "verified"
         "tbc_address": "0x00XXXXXX"

5. Replace 0x000000 /* TODO_TBC */ in source code

6. Tick the box in this file

7. Re-run the script to regenerate ADDRESS_FINDINGS.md
```

---

## Resources

- [`Tools/find_addresses.py`](../Tools/find_addresses.py) — automated search
- [`Docs/ADDRESS_FINDINGS.md`](ADDRESS_FINDINGS.md) — generated candidate report
- [`Docs/GHIDRA_ADDRESS_FINDING.md`](GHIDRA_ADDRESS_FINDING.md) — analysis guide
- [`Docs/symbols/address_map.json`](symbols/address_map.json) — structured data
- [`Docs/symbols/priority_functions.txt`](symbols/priority_functions.txt) — flat list
- [`TBC_PORTING_GUIDE.md`](../TBC_PORTING_GUIDE.md) — broader porting context
