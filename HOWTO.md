# HOWTO - Step by Step Usage Guide

## Prerequisites

- Visual Studio 2022 with C++ Desktop Development workload
- CMake 3.25+
- IDA Pro Free or Ghidra (for address hunting)
- x32dbg (for runtime debugging)
- A TBC 2.4.3 (build 8606) WoW client

---

## Step 1: Clone and Explore

```bash
git clone https://github.com/Sparco6/TBC-Extensions.git
cd TBC-Extensions
```

Read the key files:
- `TBC_PORTING_GUIDE.md` — understand the address finding process
- `Docs/BACKPORTED_API.md` — understand what functions are available
- `TBCExtensions/src/Client/CustomLua.cpp` — the main Lua function file

---

## Step 2: Find Addresses

Open `WoW.exe` (TBC 2.4.3 build 8606) in IDA Pro:

1. **Find FrameScript addresses** (highest priority):
   - `FrameScript__RegisterFunction`
   - `FrameScript__LoadFunctions`
   - `lua_pushstring`, `lua_pushnumber`, `lua_pushboolean`, `lua_pushnil`
   - `lua_tostring`, `lua_tonumber`, `lua_toboolean`
   - `lua_gettop`, `lua_settop`

2. **Find ClientServices addresses**:
   - `ClntObjMgr__GetActivePlayer`
   - `ClntObjMgr__GetObjectPtr`
   - `GetCharacterClass`

3. **Find the OOBLUAFUNCTIONS hook site**:
   - The address where `FrameScript__LoadFunctions` is called at startup
   - The two adjacent dwords used for function table limits

4. Replace every `0x000000 /* TODO_TBC */` in the source files with your values.

See [TBC_PORTING_GUIDE.md](TBC_PORTING_GUIDE.md) for detailed methodology.

---

## Step 3: Configure CMake

```bash
mkdir build && cd build

# Minimal config to start (just custom Lua functions)
cmake -A Win32 -DOOBLUAFUNCTIONS_PATCH=ON ..

# Or with more features:
cmake -A Win32 \
  -DOOBLUAFUNCTIONS_PATCH=ON \
  -DACTIONBAR_LUA=ON \
  -DCHARCREATIONRACE_FIX=ON \
  -DCOMBOPOINT_FIX=ON \
  ..
```

---

## Step 4: Build

```bash
cmake --build . --config Release
```

Output: `build/bin/TBCExtensions.dll` and `build/bin/Patcher.exe`

---

## Step 5: Patch WoW.exe

> ⚠️ **Back up your WoW.exe first!**

Once Patcher addresses are filled in:

```bash
./Patcher.exe "C:\path\to\WoW.exe"
```

---

## Step 6: Install the DLL

Copy `TBCExtensions.dll` to the same folder as `WoW.exe`.

---

## Step 7: Test

Start WoW and open the Lua console:

```lua
-- Test basic function registration
/script print(GetBuildInfo())
-- Expected: "2.4.3", "8606", date, time

-- Test a backported function
/script local n = GetSpellNameById(1)
/script print(n)
-- Expected: spell name or nil if DBC not loaded

-- Test a stub function
/script print(GetNumGlyphSockets())
-- Expected: 0 (stub)

-- Test dev helper (if DEVHELPER_LUA=ON)
/script ToggleTerrain()
-- Expected: terrain disappears/reappears
```

---

## Troubleshooting

| Problem | Solution |
|---------|----------|
| WoW crashes on startup | Hook address is wrong — re-check OOBLUAFUNCTIONS patch site |
| Functions return nil | Check that `FrameScript__RegisterFunction` address is correct |
| No log files created | Check `DLL_Logs/` folder is writable |
| DLL not loaded at all | Run Patcher again; verify DLL name string offset |
| Lua errors on load | Check `GetTop`/`IsNumber`/`GetString` addresses |

---

## Adding New Lua Functions

1. Declare in `TBCExtensions/src/Client/CustomLua.hpp`:
   ```cpp
   static int32_t MyNewFunction(lua_State* L);
   ```

2. Implement in `TBCExtensions/src/Client/CustomLua.cpp`:
   ```cpp
   int32_t CustomLua::MyNewFunction(lua_State* L)
   {
       // read arguments
       double val = FrameScript::GetNumber(L, 1);
       // push return values
       FrameScript::PushNumber(L, val * 2.0);
       return 1;
   }
   ```

3. Register in `CustomLua::RegisterFunctions()`:
   ```cpp
   AddToFunctionMap("MyNewFunction", (void*)&MyNewFunction);
   ```

4. Rebuild and test in Lua:
   ```lua
   /script print(MyNewFunction(5))  -- should print 10
   ```
