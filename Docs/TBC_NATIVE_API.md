# TBCExtensions Native API

## API 3 test revision

Version 0.1.2 adds `TBCExt_GetAddressInfo(address)` and missing-global detection on the existing registration hook. The new callback returns read-only OS module/section/RVA/protection metadata, not memory contents. Thirteen callbacks are now individually whitelisted. See API3_RUNTIME_TEST.md for build verification, scope and required in-game tests; lifecycle recovery is not yet runtime-confirmed.

## Milestone 1 API

> Runtime safety update (2026-09-02): registration succeeded in the Glue state,
> but the state was replaced on world entry. Invoking a DLL-resident callback
> triggered build 8606's function-pointer guard. Static disassembly identified
> the guard at `0x0074A160`; it accepts only the Wow.exe `.text` range cached at
> `0x00E1F830-0x00E1F834` and otherwise reports `Invalid function pointer: %p`.
> The bridge now byte-validates and hooks that guard, whitelisting only the six
> exact safe callback addresses while delegating every other pointer to the
> original validator. Runtime confirmation was supplied by the user on
> 2026-09-02: `type(TBCExt_GetVersion)` returned `function`, and DevTools
> successfully displayed version `0.1.0-research`, client `2.4.3.8606`, module
> base `0x00400000`, and the current Lua-state pointer without a client crash.

The default build exposes only the following Lua globals:

- `TBCExt_GetVersion()` -> string
- `TBCExt_GetClientBuild()` -> `"2.4.3.8606"`
- `TBCExt_GetModuleBase()` -> numeric process module base
- `TBCExt_GetLuaState()` -> numeric current Lua state pointer
- `TBCExt_IsAddressExecutable(address)` -> boolean
- `TBCExt_IsAddressReadable(address)` -> boolean

The address predicates use `VirtualQuery`; they do not dereference or invoke
the supplied address. There is intentionally no generic call, write, or raw
memory mutation API.

## Safety boundary

`UNSAFE_LEGACY_PORT` defaults to `OFF`. With it off, startup does not execute
the incomplete WotLK-derived patch modules containing `TODO_TBC` addresses.
Enabling that option is not supported until every reachable patch address has
been independently established for build 8606.

Registration uses `FrameScript__RegisterFunction` at `0x007059B0` after the
global Lua state pointer becomes non-null. This is statically compiled but is
not yet runtime-confirmed.

Static disassembly of build 8606 shows `FrameScript__RegisterFunction` passing
the callback to `lua_pushcclosure` at `0x0072E2F0`. The inspected
`lua_pushcclosure` path stores the callback pointer in the closure and contains
no check that it lies inside `Wow.exe`. No TBC callback-range patch is therefore
enabled or required by the current static evidence. Runtime registration still
needs the user's in-game confirmation before this is treated as proven.
# Client profile/status API (API version 2)

The next safe research API revision adds read-only status calls:

- `TBCExt_GetNativeApiVersion()` returns `2`.
- `TBCExt_GetClientProfile()` returns `MASTERWOW_CUSTOM_8606` only when the in-memory PE identity and the critical Lua registration, string-push, and callback-validator signatures match.
- `TBCExt_GetCoreLuaStatus()` returns `VALID` or `INVALID`.
- `TBCExt_GetCallbackValidatorStatus()` returns `VALID` or `INVALID`.
- `TBCExt_GetNativeTraceStatus()` currently returns `DISABLED`.
- `TBCExt_GetModelHooksStatus()` currently returns `NOT CONFIGURED`.

This fingerprint deliberately does not compare the entire executable. Existing unrelated custom patches remain acceptable. No status function writes memory, invokes an arbitrary address, or enables a feature.
