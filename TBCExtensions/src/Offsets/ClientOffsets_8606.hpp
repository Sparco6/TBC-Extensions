#pragma once

#include <cstdint>

// Central address registry for the 32-bit enUS TBC 2.4.3 build 8606 client.
// Source: Ghidra analysis merged in commit 641d799a5e355b76eb2f694c711a76cf69b6eacc.
// Confidence: CONFIRMED in that research commit; runtime use still requires the
// exact build and remains subject to the user's in-game verification.
namespace Offsets8606
{
constexpr std::uintptr_t LuaLError                    = 0x0072F5C0;
constexpr std::uintptr_t LuaToBoolean                 = 0x0072DFC0;
constexpr std::uintptr_t LuaToNumber                  = 0x0072DF40;
constexpr std::uintptr_t LuaToLString                 = 0x0072DFF0;
constexpr std::uintptr_t LuaGetTop                    = 0x0072DAE0;
constexpr std::uintptr_t LuaSetTop                    = 0x0072DB00;
constexpr std::uintptr_t LuaIsNumber                  = 0x0072DE30;
constexpr std::uintptr_t LuaIsString                  = 0x0072DE70;
constexpr std::uintptr_t LuaType                      = 0x0072DDC0;
constexpr std::uintptr_t LuaPushNil                   = 0x0072E180;
constexpr std::uintptr_t LuaPushNumber                = 0x0072E1A0;
constexpr std::uintptr_t LuaPushString                = 0x0072E250;
constexpr std::uintptr_t LuaPushBoolean               = 0x0072E3B0;
constexpr std::uintptr_t LuaPushCClosure              = 0x0072E2F0;
constexpr std::uintptr_t LuaGetField                  = 0x0072E550;
constexpr std::uintptr_t LuaSetField                  = 0x0072E7E0;
constexpr std::uintptr_t FrameScriptRegisterFunction = 0x007059B0;
constexpr std::uintptr_t GlobalLuaStatePointer        = 0x00E1DB84;
// Calls the crash reporter with "Invalid function pointer: %p" when a
// callback lies outside the Wow.exe .text bounds cached at E1F830-E1F834.
// Source: build-8606 disassembly, unique string xref. Confidence: HIGH.
constexpr std::uintptr_t ValidateFunctionPointer      = 0x0074A160;
}
