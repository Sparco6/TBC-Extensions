#include <Client/FrameScript.hpp>

#include <cstdarg>
#include <cstdio>

void FrameScript::DisplayError(lua_State* L, char* fmt, ...)
{
    char buffer[2048] = { 0 };
    va_list args;

    va_start(args, fmt);
    vsnprintf_s(buffer, sizeof(buffer), _TRUNCATE, fmt, args);
    va_end(args);

    // TODO_TBC: Find lua_error / FrameScript__Error address
    // WotLK 3.3.5 address was: 0x84F280
    // How to find: Search for error string "Lua Error" near script engine init
    reinterpret_cast<void(_cdecl*)(lua_State*, char*, ...)>(0x000000 /* TODO_TBC */)(L, "%s", buffer);
}

bool FrameScript::GetBoolean(lua_State* L, int32_t index)
{
    // TODO_TBC: Find lua_toboolean address
    // WotLK 3.3.5 address was: 0x84E0B0
    return reinterpret_cast<bool (__cdecl*)(lua_State*, int32_t)>(0x000000 /* TODO_TBC */)(L, index);
}

double FrameScript::GetNumber(lua_State* L, int32_t index)
{
    // TODO_TBC: Find lua_tonumber address
    // WotLK 3.3.5 address was: 0x84E030
    return reinterpret_cast<double(__cdecl*)(lua_State*, int32_t)>(0x000000 /* TODO_TBC */)(L, index);
}

bool FrameScript::GetParam(lua_State* L, int32_t index, bool p)
{
    // TODO_TBC: Find FrameScript__GetParam address
    // WotLK 3.3.5 address was: 0x815500
    return reinterpret_cast<bool(__cdecl*)(lua_State*, int32_t, bool)>(0x000000 /* TODO_TBC */)(L, index, p);
}

char* FrameScript::GetString(lua_State* L, int32_t index, bool p)
{
    // TODO_TBC: Find lua_tolstring address
    // WotLK 3.3.5 address was: 0x84E0E0
    return reinterpret_cast<char* (__cdecl*)(lua_State*, int32_t, bool)>(0x000000 /* TODO_TBC */)(L, index, p);
}

char* FrameScript::GetText(const char* str, int32_t a2, int32_t a3)
{
    // TODO_TBC: Find FrameScript__GetText (localization lookup) address
    // WotLK 3.3.5 address was: 0x819D40
    return reinterpret_cast<char* (__cdecl*)(const char*, int32_t, int32_t)>(0x000000 /* TODO_TBC */)(str, a2, a3);
}

int32_t FrameScript::GetTop(lua_State* L, int32_t p)
{
    // TODO_TBC: Find lua_gettop address
    // WotLK 3.3.5 address was: 0x84DBD0
    return reinterpret_cast <int32_t(__cdecl*)(lua_State*, int32_t)>(0x000000 /* TODO_TBC */)(L, p);
}

int32_t FrameScript::IsNumber(lua_State* L, int32_t index)
{
    // TODO_TBC: Find lua_isnumber address
    // WotLK 3.3.5 address was: 0x84DF20
    return reinterpret_cast<int32_t(__cdecl*)(lua_State*, int32_t)>(0x000000 /* TODO_TBC */)(L, index);
}

char* FrameScript::IsString(lua_State* L, int32_t index)
{
    // TODO_TBC: Find lua_isstring address
    // WotLK 3.3.5 address was: 0x84DF60
    return reinterpret_cast<char* (__cdecl*)(lua_State*, int32_t)>(0x000000 /* TODO_TBC */)(L, index);
}

int32_t FrameScript::LoadFunctions()
{
    // TODO_TBC: Find FrameScript__LoadFunctions address
    // WotLK 3.3.5 address was: 0x5120E0
    // How to find: Search for the function that registers all default Lua functions at startup
    return reinterpret_cast<int32_t(__cdecl*)()>(0x000000 /* TODO_TBC */)();
}

int32_t FrameScript::PushBoolean(lua_State* L, bool state)
{
    // TODO_TBC: Find lua_pushboolean address
    // WotLK 3.3.5 address was: 0x84E4D0
    return reinterpret_cast<int32_t(__cdecl*)(lua_State*, bool)>(0x000000 /* TODO_TBC */)(L, state);
}

int32_t FrameScript::PushNil(lua_State* L)
{
    // TODO_TBC: Find lua_pushnil address
    // WotLK 3.3.5 address was: 0x84E280
    return reinterpret_cast<int32_t(__cdecl*)(lua_State*)>(0x000000 /* TODO_TBC */)(L);
}

int32_t FrameScript::PushNumber(lua_State* L, double value)
{
    // TODO_TBC: Find lua_pushnumber address
    // WotLK 3.3.5 address was: 0x84E2A0
    return reinterpret_cast<int32_t(__cdecl*)(lua_State*, double)>(0x000000 /* TODO_TBC */)(L, value);
}

int32_t FrameScript::PushString(lua_State* L, const char* str)
{
    // TODO_TBC: Find lua_pushstring address
    // WotLK 3.3.5 address was: 0x84E350
    return reinterpret_cast<int32_t(__cdecl*)(lua_State*, const char*)>(0x000000 /* TODO_TBC */)(L, str);
}

int32_t FrameScript::RegisterFunction(const char* name, void* ptr)
{
    // TODO_TBC: Find FrameScript__RegisterFunction address
    // WotLK 3.3.5 address was: 0x817F90
    // How to find: Search for the function that maps string→function pointer for Lua API
    return reinterpret_cast<int32_t(__cdecl*)(const char*, void*)>(0x000000 /* TODO_TBC */)(name, ptr);
}

int32_t FrameScript::SetTop(lua_State* L, int32_t p)
{
    // TODO_TBC: Find lua_settop address
    // WotLK 3.3.5 address was: 0x84DBF0
    return reinterpret_cast<int32_t(__cdecl*)(lua_State * L, int32_t)>(0x000000 /* TODO_TBC */)(L, p);
}

int32_t FrameScript::SignalEvent(uint32_t event, char* fmt, ...)
{
    char buffer[2048] = { 0 };
    va_list args;

    va_start(args, fmt);
    vsnprintf_s(buffer, sizeof(buffer), _TRUNCATE, fmt, args);
    va_end(args);

    // TODO_TBC: Find FrameScript__SignalEvent address
    // WotLK 3.3.5 address was: 0x81B530
    // How to find: Trace event firing from Lua event handler code
    return reinterpret_cast<int32_t(__cdecl*)(uint32_t, char*, ...)>(0x000000 /* TODO_TBC */)(event, "%s", buffer);
}

// ---------------------------------------------------------------
// Extended Lua API helpers – used for Frame metatable injection
// ---------------------------------------------------------------

lua_State* FrameScript::GetState()
{
    // TODO_TBC: Find the address of the global lua_State pointer in wow.exe
    // WotLK 3.3.5 address was: *(lua_State**)0x00884BAC
    // How to find: Search for the global variable used in FrameScript__LoadFunctions
    //              to pass the Lua state to Lua C API calls.
    return *reinterpret_cast<lua_State**>(0x000000 /* TODO_TBC */);
}

void FrameScript::GetField(lua_State* L, int32_t idx, const char* k)
{
    // TODO_TBC: Find lua_getfield address
    // WotLK 3.3.5 address was: 0x84E620
    // How to find: Search for lua_getfield calls near FrameScript init
    reinterpret_cast<void(__cdecl*)(lua_State*, int32_t, const char*)>(0x000000 /* TODO_TBC */)(L, idx, k);
}

void FrameScript::SetField(lua_State* L, int32_t idx, const char* k)
{
    // TODO_TBC: Find lua_setfield address
    // WotLK 3.3.5 address was: 0x84E680
    // How to find: Search for lua_setfield calls near frame method registration
    reinterpret_cast<void(__cdecl*)(lua_State*, int32_t, const char*)>(0x000000 /* TODO_TBC */)(L, idx, k);
}

void FrameScript::PushCFunction(lua_State* L, int32_t (*fn)(lua_State*))
{
    // TODO_TBC: Find lua_pushcclosure address (with nup=0 for plain C functions)
    // WotLK 3.3.5 address was: 0x84E460
    // How to find: Search for lua_pushcclosure calls in FrameScript registration
    reinterpret_cast<void(__cdecl*)(lua_State*, int32_t(*)(lua_State*), int32_t)>(0x000000 /* TODO_TBC */)(L, fn, 0);
}

int32_t FrameScript::LuaType(lua_State* L, int32_t idx)
{
    // TODO_TBC: Find lua_type address
    // WotLK 3.3.5 address was: 0x84DEC0
    // How to find: Search for the function that returns an integer type code (0-8)
    return reinterpret_cast<int32_t(__cdecl*)(lua_State*, int32_t)>(0x000000 /* TODO_TBC */)(L, idx);
}

void FrameScript::GetMetaTable(lua_State* L, const char* tname)
{
    // Equivalent to luaL_getmetatable(L, tname):
    //   lua_getfield(L, LUA_REGISTRYINDEX, tname)
    // LUA_REGISTRYINDEX is -10000 in the standard Lua 5.1 build embedded
    // in TBC 2.4.3. Update this value if the WoW client uses a modified Lua.
    // TODO_TBC: Once lua_getfield address is filled in, this will work automatically.
    GetField(L, -10000 /* LUA_REGISTRYINDEX */, tname);
}
