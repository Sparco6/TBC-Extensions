# MasterWoW DevTools Native Bridge

## Runtime verification (2026-09-02)

The user confirmed the address inspector against `0x007059B0` in the custom
build-8606 client. DevTools reported `WOW_STATIC_ADDRESS`, readable `yes`,
executable `yes`, module `Wow.exe`, RVA `0x003059B0`, Ghidra
`FUN_007059b0`, friendly name `FrameScriptRegisterFunction`, status `VERIFIED`,
and confidence `CONFIRMED`. The inspector classified the address without
calling it.

MasterWoW DevTools remains fully usable without TBCExtensions. The first native
integration milestone will detect `type(TBCExt_GetVersion) == "function"` and
display the returned version/build only after the minimal DLL registration has
been confirmed in-game.

The Research / Export tab does not call native addresses. Opening it performs
no runtime scan. Scanning and export are explicit, incremental, cancelable, and
bounded; visible output is limited to 75 results per page.

Native trace and address-inspection UI are deliberately deferred until the
minimal bridge passes the runtime checklist.
# Client profile display

The Native / DLL tab supports API version 2 status fields while remaining compatible with the previously tested API version 1 DLL. It reports the Lua state explicitly as `LUA_RUNTIME_POINTER; Stable: NO` and displays client profile, core Lua signature, callback-validator, trace, and model-hook states. Trace remains disabled.

In Research / Export, entering a hexadecimal address in the notebook address field and pressing Enter performs a lookup in the generated native catalog. Adding the note copies the catalog's Ghidra name, friendly name, category, size, status, and confidence into SavedVariables. This is a table lookup only; the address is never called.
# API 3 research panels

Native Functions provides explicit incremental search and 50-record paging. Friend IDC labels remain separate CANDIDATE/LOW proposals, including labeled non-function addresses. Open Notebook links exact catalog starts to Research notes. DBC Inspector shows bounded offline WDBC samples without inferred schemas. Client Internals uses only whitelisted bridge statuses; unverified map/object-manager reads remain disabled. See API3_RUNTIME_TEST.md.
