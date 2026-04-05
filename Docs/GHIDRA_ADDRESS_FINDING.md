# Ghidra Address Finding Guide

> **Target binary:** World of Warcraft 2.4.3 (build 8606) — `WoW.exe` (x86, 32-bit)  
> **Ghidra version:** 11.0 or later recommended  
> This guide explains how to find the `TODO_TBC` addresses needed to build and
> run TBC-Extensions.

---

## Table of Contents

1. [Setup](#1-setup)
2. [Technique A — String Reference Method](#2a-string-reference-method)
3. [Technique B — Cross-Reference Method](#2b-cross-reference-method)
4. [Technique C — Signature Scanning](#2c-signature-scanning)
5. [Technique D — DBC / Data-Structure Method](#2d-dbc--data-structure-method)
6. [Verification Checklist](#3-verification-checklist)
7. [Step-by-Step Guides for Critical Functions](#4-step-by-step-guides)

---

## 1. Setup

### 1.1 Load WoW.exe into Ghidra

1. Create a new Ghidra project: **File → New Project → Non-Shared Project**.
2. Import the binary: **File → Import File → select `WoW.exe`**.
3. In the import dialog:
   - **Language:** `x86:LE:32:default` (auto-detected for a PE binary)
   - **Compiler:** `windows`
4. Double-click the imported file to open it in the Code Browser.
5. When prompted to run auto-analysis, click **Yes** (or **Analyze Later** if
   you want to import symbols first — recommended).

### 1.2 Import Existing Symbol File (`func.sym`)

Use the bundled Ghidra script or manual import:

**Option A — Script (recommended):**
```
Window → Script Manager → search "ImportSymbolsScript"
Run it → point it at Docs/symbols/func.sym
```

**Option B — Manual via Batch Import:**
```
File → Parse C Source  (only for headers)
```

For `.sym` files the easiest approach is a short Python script inside Ghidra:

```python
# Ghidra Script: ImportFuncSym.py
# Run from Window → Script Manager → New Script → Python
from ghidra.program.model.symbol import SourceType

lines = open("C:/path/to/Docs/symbols/func.sym").readlines()
for line in lines:
    parts = line.strip().split()
    if len(parts) < 2:
        continue
    name, addr_str = parts[0], parts[1]
    addr = toAddr(int(addr_str, 16))
    createLabel(addr, name, True, SourceType.IMPORTED)

print("Import complete")
```

### 1.3 Recommended Ghidra Plugins / Scripts

| Plugin / Script | Purpose |
|----------------|---------|
| `DecompilePlugin` (built-in) | C-like decompilation view |
| `FindReferencesAction` | Find all XREFs to an address |
| `FindStrings` (built-in) | List embedded ASCII / UTF-16 strings |
| `SymbolTablePlugin` (built-in) | Browse all labels |
| `PEFunctionsFinder` (community) | Locate functions by PE export table |
| `BinDiff` / `BinExport` | Diff TBC binary against WotLK binary |

---

## 2. Techniques

### 2A. String Reference Method

Best for: functions with distinctive error/log strings.

**Example: Finding `SErr::PrepareAppFatal`**

```
1. Search → For Strings → type "fatal" (case-insensitive)
2. Locate string like "FATAL ERROR" or "AppFatal"
3. Right-click the string → References → Show References to Address
4. Follow the XREF to the calling function
5. Decompile the calling function (press F → decompile window)
6. Verify it matches the WotLK signature: void __cdecl SErr__PrepareAppFatal(...)
7. Note the address in the symbol table — this is your TBC address
```

**Example: Finding `CGChat::AddChatMessage`**

```
1. Search → For Strings → "ChatFrame"
2. Find strings like "ChatFrame%d" or message-type strings
3. Follow XREFs to callers
4. Look for a function that takes (channel_type, message, sender, ...)
5. Verify by checking the decompiled code writes to the chat frame list
```

### 2B. Cross-Reference Method

Best for: functions that are called from already-known functions.

**Example: Finding `FrameScript::RegisterFunction`**

```
1. Start from a known address: WowClientDB__GetRow (0x004047C0)
   (already in func.sym)
2. In Ghidra: Navigate → Go To → 0x004047C0
3. Press X to show "References TO" this function
4. Trace callers upward until you find initialization code
   (look for a loop that calls the same function many times in sequence)
5. That loop is likely the Lua API registration routine
6. The function being looped is FrameScript::RegisterFunction
```

**ASCII call-graph pattern to look for:**

```
init_function()
  └─ FrameScript__LoadFunctions()
       ├─ RegisterFunction("GetSpellInfo",  Script_GetSpellInfo)
       ├─ RegisterFunction("UnitHealth",    Script_UnitHealth)
       ├─ RegisterFunction("UnitGUID",      Script_UnitGUID)
       └─ ... (60+ more)
```

### 2C. Signature Scanning

Best for: functions whose first ~20 bytes rarely change between versions.

**Example: Scanning for `CVar::Set`**

```
1. Open WotLK 3.3.5 WoW.exe in a second Ghidra project
2. Navigate to 0x766940 (WotLK CVar::Set address)
3. Copy the disassembly of the first 15-20 instructions
4. In the TBC project: Search → For Instruction Patterns
5. Enter the byte sequence (wildcarding register-relative offsets)
6. Review matches — the one in the CVar address range is your target
```

**Typical CVar::Set prologue pattern (x86):**
```asm
55          PUSH   EBP
8B EC       MOV    EBP, ESP
83 EC ??    SUB    ESP, imm8     ; stack frame — wildcard the size
56          PUSH   ESI
57          PUSH   EDI
8B ?? ??    MOV    ESI/EDI, [EBP+arg]
```

Search in Ghidra using the **Instruction Pattern Search**:
```
Window → Search → For Instruction Patterns
Enter bytes with wildcards, e.g.: 55 8B EC 83 EC .. 56 57
```

### 2D. DBC / Data-Structure Method

Best for: Lua API functions (`Script_*`) that are registered in a table.

**Example: Finding unmapped Lua API functions**

```
1. grep "Script_" Docs/symbols/func.sym   (these are anchor points)
2. In Ghidra, navigate to a known Script_ function, e.g. Script_GetItemInfo (0x0049B6F0)
3. Find the function that CALLs Script_GetItemInfo — this is the registration table builder
4. In that caller, look for a pattern of:
     PUSH offset "FunctionName"
     PUSH offset Script_FunctionAddress
     CALL FrameScript__RegisterFunction
5. Each such pair is a TODO_TBC function waiting to be mapped
6. The string pushed before the call gives you the Lua API name
```

**Finding the Lua C-API functions (lua_*):**

```
1. Navigate to any Script_ function (e.g. Script_UnitHealth at 0x00544190)
2. Open Decompiler view
3. Look for calls to unnamed FUN_008??????? — these are likely lua_pushinteger,
   lua_tostring, etc.
4. Count arguments to identify:
   - 1 arg (lua_State*) → lua_gettop, lua_pushnil
   - 2 args (lua_State*, int) → lua_toboolean, lua_isnumber, lua_type
   - 2 args (lua_State*, double) → lua_pushnumber
   - 2 args (lua_State*, char*) → lua_pushstring
```

---

## 3. Verification Checklist

Before updating a `TODO_TBC` address in source code:

- [ ] **Signature match:** Decompiled parameters match the WotLK version.
- [ ] **Return type match:** Same return type (void / int / bool / pointer).
- [ ] **Calling convention:** Check Ghidra's calling convention label
      (`__cdecl`, `__thiscall`, `__stdcall`).
- [ ] **Address range plausibility:** Address is inside the `.text` section
      of WoW.exe (typically `0x401000` – `0x88A000` for TBC 2.4.3).
- [ ] **Live test:** Use x32dbg or Cheat Engine to set a breakpoint at the
      candidate address while the game runs, then trigger the relevant
      functionality to confirm execution hits there.
- [ ] **Update address_map.json:** Set `"verification_status": "verified"`.

### Common Pitfalls

| Pitfall | Symptom | Fix |
|---------|---------|-----|
| Wrong calling convention | Game crashes on call | Check `__thiscall` vs `__cdecl` |
| Off-by-one (jmp stub) | Function executes but returns garbage | Check if address is a `JMP` trampoline; use the real target |
| Inlined function | Can't find the function | The compiler inlined it; search for the call site instead |
| Different Lua version | Lua stack corruption | TBC uses Lua 5.1; verify LUA_REGISTRYINDEX = -10000 |
| Version mismatch | Game crashes at load | Double-check build number is 8606 |

---

## 4. Step-by-Step Guides

### 4.1 `FrameScript__LoadFunctions` (CRITICAL)

**WotLK address:** `0x5120E0`

```
Step 1: In func.sym we have FrameScript_EventObject__ctor at 0x00708060.
        Navigate there in Ghidra.

Step 2: Search for a function that calls FrameScript_EventObject__ctor AND
        also contains a large loop registering string→function pairs.

Step 3: Look for a function in the 0x004XXXXX–0x005XXXXX range that:
        - Has 60+ direct CALL instructions
        - Each call is preceded by two PUSH instructions (string, pointer)
        - Contains no large data processing logic (pure registration)

Step 4: Verify by checking if Script_GetItemInfo (0x0049B6F0) is one of the
        values pushed inside the function.

Step 5: That function IS FrameScript__LoadFunctions.
```

### 4.2 `FrameScript__RegisterFunction` (CRITICAL)

**WotLK address:** `0x817F90`

```
Step 1: Find FrameScript__LoadFunctions (see 4.1 above).

Step 2: Inside that function, every registration call follows the pattern:
        PUSH  <function_ptr>   ; e.g. offset Script_GetItemInfo
        PUSH  <string_ptr>     ; e.g. offset "GetItemInfo"
        CALL  <some_function>

Step 3: The <some_function> is FrameScript__RegisterFunction.
        It typically lives near 0x007XXXXX–0x008XXXXX.

Step 4: Verify its signature:
        int __cdecl FrameScript__RegisterFunction(const char* name, void* funcPtr)
        It should store the pair in a hash table or sorted array.
```

### 4.3 `FrameScript__GetState` / Global `lua_State*` (CRITICAL)

**WotLK address:** `*(lua_State**)0x00884BAC`

```
Step 1: Navigate to any Script_ function (e.g. Script_UnitHealth 0x00544190).

Step 2: In the decompiler, look for a global pointer dereference at the
        top of the function — something like:
        pLState = DAT_00??????

Step 3: That DAT_00?????? is the global lua_State* pointer.
        The address of that global IS the value you put in FrameScript::GetState.

Step 4: Verify: the pointer should change each time you /reload in WoW.
```

### 4.4 `CVar__Set` (CRITICAL)

**WotLK address:** `0x766940`

```
Step 1: Navigate to Script_SetCVar (already in func.sym at 0x00472320).

Step 2: Inside Script_SetCVar, locate the call that passes the CVar name and
        new value strings — this call is CVar__Set.

Step 3: Alternatively, use signature scanning (see Technique C):
        Look for the pattern near 0x006400E0 (CVar__ctor, already named).

Step 4: Verify CVar::Set signature:
        void __thiscall CVar__Set(CVar* this, const char* value, bool persist)
```

### 4.5 `SErr__PrepareAppFatal` (CRITICAL)

**WotLK address:** `0x772A80`

```
Step 1: Search → For Strings → "AppFatal" or "Fatal Error"

Step 2: Follow the XREF to the calling site. The caller is likely a wrapper
        that formats the error message and then calls SErr__PrepareAppFatal.

Step 3: Verify signature:
        void __cdecl SErr__PrepareAppFatal(const char* file, int line,
                                           const char* fmt, ...)
        It should end with a call to ExitProcess or TerminateProcess.
```

### 4.6 `lua_*` C-API Functions (HIGH)

The Lua C-API functions are embedded directly into the WoW.exe binary.
They are tightly clustered around address `0x0084XXXX` in TBC (based on
WotLK evidence at `0x84D000–0x84F000`).

```
Step 1: Navigate to any named Script_ function that you know uses the Lua stack.
        Good example: Script_UnitHealth (0x00544190) — pushes an integer result.

Step 2: In the decompiler, find the call that looks like:
        FUN_008?????(lua_state_ptr, int_value)
        This is likely lua_pushinteger or lua_pushnumber.

Step 3: Navigate to that FUN_008????? and check its size (~20–50 bytes for
        basic Lua stack functions) and behaviour (modifies a struct at offset
        0x8 from the lua_State pointer).

Step 4: Cross-reference with the known WotLK addresses to confirm the mapping:
        lua_gettop   0x84DBD0  → look near 0x0084XXXX
        lua_settop   0x84DBF0  → immediately after lua_gettop
        lua_toboolean 0x84E0B0 → look in the same cluster
        lua_tonumber  0x84E030 → near lua_toboolean
```

### 4.7 `CGChat__AddChatMessage` (MEDIUM)

**WotLK address:** `0x509DD0`

```
Step 1: Search → For Strings → "ChatFrame" or "System"

Step 2: Find the function called when an in-game chat message appears.
        It should take: (CGChat* this, int type, const char* msg,
                         const char* sender, int language, ...)

Step 3: Verify by checking — the function near 0x00509??? should update a
        linked list / ring buffer of chat messages.
```

### 4.8 `SFile__CloseFile` (MEDIUM)

**WotLK address:** `0x422910`

```
Step 1: SFile__OpenFile is already named at 0x006755A0 in func.sym.

Step 2: Find functions nearby (search wow_tbc.sym in the 0x006755 range).

Step 3: SFile__CloseFile should be a small function (~30-60 bytes) that calls
        the OS CloseHandle and frees a Storm file handle structure.

Step 4: Verify: it takes one HANDLE/HSFILE argument and returns bool.
```

---

> **Tip:** After finding any function, always update `Docs/symbols/address_map.json`
> and re-run `python Tools/find_addresses.py` to regenerate the findings report.
