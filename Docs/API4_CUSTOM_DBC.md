# Native custom DBC loader — 0.1.3 / API 4

## Install and use

Close WoW completely. Back up the working DLL and addon. Extract the package into your game directory, preserving paths:

- `TBCExtensions.dll` beside the existing custom `Wow.exe`.
- `Interface/AddOns/MasterWoWDevTools/` for the addon.
- `TBCExtensions/DBFilesClient/TBCExt_Demo.dbc` for the binary demonstration table.

Do not run the patcher or replace Wow.exe. No game installation files were changed by this build.

In `/mdev` → **DBC Inspector**, click **Load via DLL** with `TBCExt_Demo.dbc` selected. With ID `1`, column `2`, type `string`, click **Read Field**. Expected text: `Native DBC loaded from disk`. Change column to `3` and type to `float`: expected `1.75`. These values are read from a binary file by the DLL, not embedded in the addon. The table is synthetic and is not retail content.

The Previous/Next Table/Page buttons still browse the separately labeled offline samples. They do not change the native active table. Native Info returns the actual file path and dimensions. Read ID displays a native row by its first-field ID, not its row number. Unload releases the active custom table. Load via DLL explicitly rereads a file; editing a disk file does not silently update the active snapshot.

## Load your AoR tables

Place extracted WDBC files in `TBCExtensions/DBFilesClient/` under the running executable's directory. Enter the basename, for example `AoR_CreatureModelData.dbc`, then Load via DLL. Use an ID actually present in that table; a creature display ID is not necessarily its model ID.

Known AoR layouts from the provided retail-assets sources:

| Table | One-based fields |
| --- | --- |
| AoR_CreatureModelData.dbc | 1 ID (uint32), 2 ModelPath (string offset), 3 MountHeight (float) |
| AoR_CreatureDisplayInfo.dbc | 1 ID, 2 ModelID, 3 ModelPath, 4–7 Texture1–4 (string offsets) |

These exact layouts receive validation and decoded Read ID output. Other AoR_ and TBCExt_ fixed-width WDBC tables can be loaded and queried with explicitly chosen types, but their schema is not inferred. Stock TBC CreatureModelData is a different layout.

Only one table is active at a time in this first implementation. Successful loads replace the active table; failed loads leave it unchanged. DLL-owned data survives addon reload/relog in the same process; a full game restart requires loading it again. Loading is synchronous and explicit; large files may briefly pause the UI.

## Lua contract

All calls return `text, status`, where status is `OK` or `ERROR`. Numeric field values are returned as text; use `tonumber(text)` only after checking status.

```lua
/run print(TBCExt_CustomDBC("load", "TBCExt_Demo.dbc"))
/run print(TBCExt_CustomDBC("field", 1, 2, "string"))
/run print(TBCExt_CustomDBC("field", 1, 3, "float"))
/run print(TBCExt_CustomDBC("row", 1))
/run print(TBCExt_CustomDBC("info"))
/run print(TBCExt_CustomDBC("unload"))
```

Arguments are strictly typed. IDs and columns must be unsigned integral values; columns are one-based. Types: `uint32`, `float`, `string`. Strings are bounded and embedded NUL input is rejected. This single callback is included in the existing exact callback whitelist and both registration paths.

## Deliberate limits

- Reads loose extracted files using Windows file I/O, not the game's MPQ/file functions.
- Basenames only, restricted to `AoR_` or `TBCExt_` and ASCII letters/digits/underscores with `.dbc` suffix. No caller-supplied paths.
- WDBC only, exact header length; 64 MiB/file, 1 million rows, 1–256 fixed 32-bit fields, valid string-block boundaries, unique first-field IDs.
- String reads validate offsets and termination and are limited to 4096 bytes. Float reads reject NaN/infinity. Unknown table schemas are not automatically interpreted or fully semantically validated.
- Row display is limited to 32 raw fields; individual fields remain queryable.
- No stock-table injection, automatic startup hook, morph support, mount-height override, rendering patches, MPQ access, or raw retail DB2/WDC decoding. Reading an AoR model path does not load that model.
- Loading data for retail rendering is now possible; connecting those records to TBC model/display consumers remains a separate implementation step.

## Supplied hook / symbols audit

The custom executable contains bytes `E8 62 EE FF FF` at VA `0x00574E29`, which decode to a call to `0x00573C90`. The friend's `0x574E2A` is its relative operand, and `0x574E2E` is the following instruction. This confirms that specific call-site proposal, not the entire calling convention, timing or safety of redirecting it. The loader does not patch this call and leaves the incomplete legacy CDBCMgr path disabled. No supplied DLL bootstrap patch was reapplied.

`export+search/label.sym` matches the existing `Docs/symbols/label.sym` exactly: SHA256 `793C8B5087C752E6239DF4586A16224B3F93BAA00365BC634FEBD38CAC7CB16A`. Symbols remain evidence, not authority to call functions.

## Local validation (not in-game confirmation)

- x86 DLL compiled successfully.
- C++ parser tests: typed valid records plus 15 negative cases.
- Native Windows file-service tests: binary load, typed fields, invalid/missing-file rejection, path rejection, failed-load preservation, and unload.
- All 30 addon Lua files parsed as Lua 5.1.
- Mocked addon tests exercised missing DLL, all native DBC controls, arguments and escaped display text, plus previous catalog/offline DBC behavior.
- Research export regression: 42,000 streamed records and 12,000 retained records passed.
- Added startup prefix check for `lua_tolstring` used by the new API; existing fingerprints and exact callback whitelist remain enabled.
- DLL SHA256: `EB28BDE579A272E15BA8FD315EAAB332A1A5567F8A24CDB53D3635D97EC14C17`.

Fixture generation: `node Tools/FunctionResearch/make_dbc_demo.js output-directory [message]`. It generates a real two-row WDBC; use a separate output directory and copy intentionally. Parser/service test source: `Tools/FunctionResearch/test_wdbc.cpp`. Call-site audit: `Tools/FunctionResearch/audit_dbc_hook.js path-to-Wow.exe` (read-only).
