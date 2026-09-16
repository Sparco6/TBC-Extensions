# Runtime confirmation and WXL research intake

## User runtime confirmation

Screenshots confirm TBCExtensions 0.1.1-research, API 2, profile MASTERWOW_CUSTOM_8606, startup Lua/validator signatures VALID, and working inspection of 0x007059B0. Trace remains DISABLED. Reload persistence and successful note saving are not newly established by these screenshots.

The 0x1E272A38 note rejection is intentional: it is not a catalog function start. Readable/executable describes page permissions, not object type, validity as a function, or safety to call. In the address-only inspector its classification remains UNKNOWN_POINTER; the earlier table display provides runtime-Lua provenance, not a stable native address.

## Export correction

The screenshot reports 1,506 rows / 239,826 characters. The previous exporter reached MAX_OUTPUT_CHARS=240000 and incorrectly called truncated output complete. The 42k scan count is globals visited, while only 12k matches are retained. Neither count is the number of built-in native APIs.

Updated controls:

- Export Current: the visible retained page.
- Export All: all retained results, delivered in bounded chunks.
- Export Notes: bounded notebook chunks.
- Next Export: after copying the current chunk, build the next one.
- Export _G: explicit live streaming of all globals, bypassing the 12k retained-result cap without retaining all results. No sort, recursion, native calls, or SavedVariables dump is performed. It ignores the search filter; use Scan Runtime for filtered exports.

Every chunk has a CSV header. Save chunks separately; when concatenating, retain only the first header. Keep the tab open until complete. Stop Scan cancels active work. Export _G is a live traversal, not an atomic snapshot: avoid loading/unloading addons while exporting. Detected invalid traversal keys stop with a restart message. Concurrent additions/removals may still make a live traversal incomplete.

Bounds remain 50 entries per frame, 5000 rows/chunk, 240000 characters/chunk, 75 visible results/page. Scanning still retains at most 12000 results. No full-export data is added to SavedVariables.

Static/local test: Lua 5.1 syntax parse; a mocked Lua runtime exported 42000 synthetic globals across 48 chunks and all 12000 retained rows within limits. Runtime behavior/layout requires the user's next test. DLL and Wow.exe were not changed in this pass.

## Third-party label intake

`export+search/friendfoundadresses.txt` is IDC source (IDA naming/data-definition script). It is not a Ghidra executable script. We read it as data; we did NOT execute it or its instructions to clear existing labels.

Read-only audit generated `FRIEND_LABELS_AUDIT.json`:

- 205 proposed address labels.
- 117 exact starts in the existing native function catalog.
- 76 explicit Data declarations.
- Function-start matches establish location consistency only, not semantics/signatures/calling convention.
- No supplied labels were merged into the verified registry or enabled in the DLL.

Important disputes to resolve using disassembly:

| Address | Friend label | Current registry |
| --- | --- | --- |
| 0x0072E550 | wow_lua_tolstring | LuaGetField |
| 0x0072EE70 | wow_lua_settop | LuaSetTop is 0x0072DB00 |
| 0x007059B0 | wow_lua_RegisterFunction | Agrees with FrameScriptRegisterFunction |

Data, vtables, strings, and interior code labels must not be imported as function starts. All incoming claims remain CANDIDATE/LOW/DO_NOT_CALL pending independent evidence. Original files are preserved.

## API list and UI Studio

`API.txt` contains 5525 unique function-name entries, matching its header. Addon examples include ACP and ACalc names. This is a useful current-session inventory, not proof of stock-client availability. Names alone do not identify native addresses, argument types, or calling conventions.

`MasterWoW.UIStudio/TbcApi/api-registry.json` currently holds seven curated designer API entries. Designer implementation/test status is separate from client runtime support and should not be promoted to native-address evidence. It can help organize frame/method capabilities, but it is not an exhaustive API list. No UI Studio files changed.

## WXL scope and priorities

[WarcraftXL](https://github.com/orgs/WarcraftXL/repositories) describes wxl-core as a 3.3.5a framework. Local wxl-core, wxl-modern-m2, wxl-modern-wmo and wxl-db2 README files explicitly specify build 12340. Treat their code as architecture reference, not TBC offsets.

| Reference | Useful direction | TBC prerequisite |
| --- | --- | --- |
| wxl-core | Module contracts, offset organization, diagnostics | Independently validated 8606 addresses/ABI |
| wxl-modern-m2 | Modern format parsing/normalization, model pipeline separation | TBC runtime model layouts, allocator ownership, loading/render call sites |
| wxl-modern-wmo | Separate parsing/loading/render layers | TBC WMO layouts and hook boundaries |
| wxl-db2 | Offline/schema decoding architecture | Data-version schemas and TBC integration contract |
| ShadowSpace.cpp | Investigate shadow draw/pose behavior | TBC callers and shadow pipeline evidence; no direct hook port |

The local M2 README describes direct normalization into client runtime structures, not merely addon texture loading. AoR and WXL approaches should be compared per subsystem; neither provides automatic TBC compatibility. WXL code is GPL-3.0-or-later per its README; review licensing before copying code into a distributed build.

Your custom Wow.exe contains ASCII `Textures\\ShadowBlob.blp`. This corroborates the friend's path recollection, but not the broader claim that every shadow is just that texture. Next safe research step: locate references to that exact string in Ghidra, record callers and their catalog entries, and inspect loading/render data flow. No arbitrary callback, model load, or trace hook was enabled.

## Next manual test

Install only the updated MasterWoWDevTools addon with WoW closed. Keep the working 0.1.1 DLL and custom Wow.exe unchanged.

1. Research: Export _G; wait for a chunk and copy/save it.
2. Click Next Export, save separately, repeat until complete. Row ranges should continue without gaps, beyond 1506 and beyond 12000.
3. Confirm Stop Scan stops a running export and the client stays responsive.
4. For native notes use 0x007059B0, press Enter, edit observation/name, Add Note, Show Notes, Export Notes. A rejected 0x1E272A38 remains expected.
