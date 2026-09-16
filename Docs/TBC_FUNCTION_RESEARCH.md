# TBC 2.4.3 Function Research

Target: Windows x86 WoW 2.4.3 build 8606. Addresses are build-specific.
Runtime behavior is not considered verified until it is tested in-game by the
workspace owner.

## Runtime-confirmed bridge findings (2026-09-02)

- `0x007059B0` — `FrameScriptRegisterFunction`: registration succeeds in Glue
  and world contexts on build 8606.
- `0x0074A160` — function-pointer validator: identified through the unique
  `Invalid function pointer: %p` reference. It checks callbacks against `.text`
  bounds cached at `0x00E1F830-0x00E1F834`. DLL callbacks crashed before exact
  whitelisting and executed after it. Status: VERIFIED; confidence: CONFIRMED
  for the tested custom client.
- `0x00E1DB84` — global `lua_State*`: valid; its pointed-to value is a
  `LUA_RUNTIME_POINTER` and is not stable between launches or contexts.

## Confirmed Lua/FrameScript set

The centralized list is `TBCExtensions/src/Offsets/ClientOffsets_8606.hpp`.
Its 16 callable functions and one global pointer originate from Ghidra work in
commit `641d799a5e355b76eb2f694c711a76cf69b6eacc`. The current static pass matched
all 16 callable addresses against `Functions.csv`.

`FrameScript__LoadFunctions`, `FrameScript__GetParam`, `FrameScript__GetText`,
`FrameScript__SignalEvent`, and the old out-of-bounds Lua function patch sites
remain unknown. They must not be called or patched.

## Record schema

Research records use: address, Ghidra name, friendly name, category, signature,
calling convention, size, status, confidence, danger level, description,
evidence, source, test notes, hit count, and last test.

Statuses: `UNKNOWN`, `CANDIDATE`, `OBSERVED`, `VERIFIED`, `REJECTED`.
Confidence: `LOW`, `MEDIUM`, `HIGH`, `CONFIRMED`.
Danger: `SAFE_READ`, `INSTRUMENT_ONLY`, `DANGEROUS_CALL`, `DO_NOT_CALL`.

The merge tool is under `Tools/FunctionResearch`. It never modifies the source
Ghidra or symbol files.
