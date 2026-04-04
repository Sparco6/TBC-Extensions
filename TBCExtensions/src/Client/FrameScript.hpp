#pragma once

#include <cstdint>

struct lua_State;

class FrameScript
{
public:
    static void    DisplayError(lua_State* L, char* fmt, ...);
    static bool    GetBoolean(lua_State* L, int32_t index);
    static double  GetNumber(lua_State* L, int32_t index);
    static bool    GetParam(lua_State* L, int32_t index, bool p);
    static char*   GetString(lua_State* L, int32_t index, bool p);
    static char*   GetText(const char* str, int32_t a2, int32_t a3);
    static int32_t GetTop(lua_State* L, int32_t p);
    static int32_t IsNumber(lua_State* L, int32_t index);
    static char*   IsString(lua_State* L, int32_t index);
    static int32_t LoadFunctions();
    static int32_t PushBoolean(lua_State* L, bool state);
    static int32_t PushNil(lua_State* L);
    static int32_t PushNumber(lua_State* L, double value);
    static int32_t PushString(lua_State* L, const char* str);
    static int32_t RegisterFunction(const char* name, void* ptr);
    static int32_t SetTop(lua_State* L, int32_t p);
    static int32_t SignalEvent(uint32_t event, char* fmt, ...);

    // ---------------------------------------------------------------
    // Extended Lua API helpers – used for Frame metatable injection
    // ---------------------------------------------------------------
    // Returns the global Lua state managed by the WoW client.
    static lua_State* GetState();
    // lua_getfield: pushes t[k] where t is the value at stack index idx.
    static void       GetField(lua_State* L, int32_t idx, const char* k);
    // lua_setfield: pops a value and stores it as t[k] where t is at idx.
    static void       SetField(lua_State* L, int32_t idx, const char* k);
    // lua_pushcclosure(L, fn, 0): pushes a C function onto the stack.
    static void       PushCFunction(lua_State* L, int32_t (*fn)(lua_State*));
    // lua_type: returns the type of the value at the given stack index.
    static int32_t    LuaType(lua_State* L, int32_t idx);
    // luaL_getmetatable: pushes the metatable registered under tname.
    static void       GetMetaTable(lua_State* L, const char* tname);

private:
    FrameScript() = delete;
    ~FrameScript() = delete;
};
