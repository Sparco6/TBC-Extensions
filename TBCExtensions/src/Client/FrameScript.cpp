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

    // TBC 2.4.3: luaL_error — takes (L, fmt, ...), calls lua_pushfstring then lua_error
    reinterpret_cast<void(_cdecl*)(lua_State*, char*, ...)>(0x0072F5C0)(L, "%s", buffer);
}

bool FrameScript::GetBoolean(lua_State* L, int32_t index)
{
    // TBC 2.4.3: lua_toboolean
    return reinterpret_cast<bool (__cdecl*)(lua_State*, int32_t)>(0x0072DFC0)(L, index);
}

double FrameScript::GetNumber(lua_State* L, int32_t index)
{
    // TBC 2.4.3: lua_tonumber
    return reinterpret_cast<double(__cdecl*)(lua_State*, int32_t)>(0x0072DF40)(L, index);
}

bool FrameScript::GetParam(lua_State* L, int32_t index, bool p)
{
    // TODO_TBC: Find FrameScript__GetParam address
    // WotLK 3.3.5 address was: 0x815500
    return reinterpret_cast<bool(__cdecl*)(lua_State*, int32_t, bool)>(0x000000 /* TODO_TBC */)(L, index, p);
}

char* FrameScript::GetString(lua_State* L, int32_t index, bool p)
{
    // TBC 2.4.3: lua_tolstring
    return reinterpret_cast<char* (__cdecl*)(lua_State*, int32_t, bool)>(0x0072DFF0)(L, index, p);
}

char* FrameScript::GetText(const char* str, int32_t a2, int32_t a3)
{
    // TODO_TBC: Find FrameScript__GetText (localization lookup) address
    // WotLK 3.3.5 address was: 0x819D40
    return reinterpret_cast<char* (__cdecl*)(const char*, int32_t, int32_t)>(0x000000 /* TODO_TBC */)(str, a2, a3);
}

int32_t FrameScript::GetTop(lua_State* L, int32_t p)
{
    // TBC 2.4.3: lua_gettop
    return reinterpret_cast <int32_t(__cdecl*)(lua_State*, int32_t)>(0x0072DAE0)(L, p);
}

int32_t FrameScript::IsNumber(lua_State* L, int32_t index)
{
    // TBC 2.4.3: lua_isnumber
    return reinterpret_cast<int32_t(__cdecl*)(lua_State*, int32_t)>(0x0072DE30)(L, index);
}

char* FrameScript::IsString(lua_State* L, int32_t index)
{
    // TBC 2.4.3: lua_isstring
    return reinterpret_cast<char* (__cdecl*)(lua_State*, int32_t)>(0x0072DE70)(L, index);
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
    // TBC 2.4.3: lua_pushboolean
    return reinterpret_cast<int32_t(__cdecl*)(lua_State*, bool)>(0x0072E3B0)(L, state);
}

int32_t FrameScript::PushNil(lua_State* L)
{
    // TBC 2.4.3: lua_pushnil
    return reinterpret_cast<int32_t(__cdecl*)(lua_State*)>(0x0072E180)(L);
}

int32_t FrameScript::PushNumber(lua_State* L, double value)
{
    // TBC 2.4.3: lua_pushnumber
    return reinterpret_cast<int32_t(__cdecl*)(lua_State*, double)>(0x0072E1A0)(L, value);
}

int32_t FrameScript::PushString(lua_State* L, const char* str)
{
    // TBC 2.4.3: lua_pushstring
    return reinterpret_cast<int32_t(__cdecl*)(lua_State*, const char*)>(0x0072E250)(L, str);
}

int32_t FrameScript::RegisterFunction(const char* name, void* ptr)
{
    // TBC 2.4.3: FrameScript__RegisterFunction
    return reinterpret_cast<int32_t(__cdecl*)(const char*, void*)>(0x007059B0)(name, ptr);
}

int32_t FrameScript::SetTop(lua_State* L, int32_t p)
{
    // TBC 2.4.3: lua_settop
    return reinterpret_cast<int32_t(__cdecl*)(lua_State * L, int32_t)>(0x0072DB00)(L, p);
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
    // TBC 2.4.3: global lua_State* at 0xE1DB84 (confirmed via FrameScript__GetState at 0x7057C0)
    return *reinterpret_cast<lua_State**>(0x00E1DB84);
}

void FrameScript::GetField(lua_State* L, int32_t idx, const char* k)
{
    // TBC 2.4.3: lua_getfield
    reinterpret_cast<void(__cdecl*)(lua_State*, int32_t, const char*)>(0x0072E550)(L, idx, k);
}

void FrameScript::SetField(lua_State* L, int32_t idx, const char* k)
{
    // TBC 2.4.3: lua_setfield
    reinterpret_cast<void(__cdecl*)(lua_State*, int32_t, const char*)>(0x0072E7E0)(L, idx, k);
}

void FrameScript::PushCFunction(lua_State* L, int32_t (*fn)(lua_State*))
{
    // TBC 2.4.3: lua_pushcclosure (nup=0 for plain C function)
    reinterpret_cast<void(__cdecl*)(lua_State*, int32_t(*)(lua_State*), int32_t)>(0x0072E2F0)(L, fn, 0);
}

int32_t FrameScript::LuaType(lua_State* L, int32_t idx)
{
    // TBC 2.4.3: lua_type
    return reinterpret_cast<int32_t(__cdecl*)(lua_State*, int32_t)>(0x0072DDC0)(L, idx);
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
