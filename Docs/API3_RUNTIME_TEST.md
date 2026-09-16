# 0.1.2 / API 3 research package

## Installation

Exit WoW completely. Back up your working TBCExtensions.dll and MasterWoWDevTools folder. Extract the package, copy the DLL beside your existing custom Wow.exe, and copy MasterWoWDevTools into Interface/AddOns. Do not rerun the patcher or replace Wow.exe. No live client files were changed by this build.

## Tests in order

1. Start WoW and log into character A. Run `/run print(TBCExt_GetVersion(), TBCExt_GetNativeApiVersion())`. Expected: `TBCExtensions 0.1.2-research`, `3`.
2. Log out to character selection and enter character B WITHOUT exiting the game. Repeat the command and refresh Native / DLL. Expected: API remains available.
3. Run `/reload`, then repeat. Expected: API remains available. If either lifecycle test fails, stop native tests and send TBCExtensions.log plus the observed sequence. No full global export needed.
4. Native / DLL: inspect `0x007059B0`. Expected: Wow.exe, `.text`, RVA `0x003059B0`, readable/executable. Inspect `0xFFFFFFFF`: no false module/function claim. These addresses are classified, never called.
5. Native Functions: enter `0x0072E550`, filter ALL, Search. Expected: current catalog identity and separate proposed `wow_lua_tolstring` marked NOT VERIFIED. Test name/category searches, CANDIDATE filter, Stop and page buttons. Search is explicit and incremental (100 entries/frame); display is capped at 50 records/page.
6. Native Functions: notebook address `0x007059B0`, Open Notebook. Expected: Research tab opens and metadata is filled. Edit observation, Add Note, Show Notes, Export Notes. Non-function/data candidates are deliberately not accepted as native function notes.
7. DBC Inspector: browse SpellDuration.dbc and pages. Expected: 133 raw rows, four uint32 fields; 20 rows/page. This is a workspace sample, not claimed to be the currently installed client DBC. IDs, signed integers, floats and string-offset meanings are NOT inferred.
8. Client Internals: Refresh. Expected: bridge/profile/signature statuses; map/loading/object-manager reads NOT CONFIGURED. No friend address is dereferenced.
9. Standalone addon: test on a client session where the bridge is unavailable if convenient. Catalog and DBC browsing should still operate; status should say Unavailable. Do not alter your executable just to perform this optional test.

## Implementation and limits

The lifecycle hook now checks whether TBCExt_GetVersion exists as a function after native registration. If absent, it re-registers the bridge even when lua_State has the same address. The added gettop/getfield/type/settop calls preserve Lua stack height and their prefixes are checked at startup against the current custom 8606 binary. No worker-thread Lua polling was added. Recovery still relies on the existing FrameScript registration hook being reached after a global rebuild; in-game lifecycle validation is required.

API 3 adds TBCExt_GetAddressInfo(address), returning text from VirtualQuery and OS-identified module headers. It reports module, section, RVA and protection flags. It never reads the supplied address as an object or invokes it. Private memory is not automatically labeled a Lua table or heap object. PE section membership is not proof that an address is a function start. Status/signature values describe startup validation, not continuous hook-health monitoring.

All 205 friend labels are generated separately from the canonical catalog. 117 match native function starts; remaining labels are displayed as NOT a catalog function start. Nothing is promoted to VERIFIED and Functions.csv is unchanged. No AoR/WXL implementation was copied; the architecture is implemented locally.

DBC tooling supports validated WDBC headers with fixed 32-bit fields, exact file-length checks, at most 256 fields and 5000 included rows/table. It does not support DB2 or arbitrary layouts. UI displays at most 32 fields/row. To generate another sample explicitly:

`py -3 Tools/FunctionResearch/build_research_data.py --addon ../MasterWoWDevTools --audit Docs/FRIEND_LABELS_AUDIT.json --dbc "absolute path to extracted table.dbc"`

This replaces only the generated ProposedLabels.lua and DBCSamples.lua outputs, not source DBC files. Repeat --dbc to include multiple tables. Source paths and SHA-256 are included for provenance.

Native trace, modern-model hooks, camera/render hooks, custom packets and unverified native data reads remain disabled. A raw DBC viewer and status panel are not full native DBC/object-manager implementations.

## Static/local verification

- x86 build successful; DLL machine 0x014C, optional-header magic 0x010B.
- 30 addon Lua files passed Lua 5.1 syntax parsing.
- Mock runtime tests passed native search, proposal warnings, bounded paging, standalone internals and DBC page transitions.
- Existing mock export regression passed 42000 streaming rows and 12000 retained rows.
- These checks are NOT in-game validation.
- DLL SHA-256: 0A961196EAB43614416DCF3FC576864B84773029D6775D80D1B983256D4EBDDF.

## Files changed/added in this pass

- TBCExtensions/src/Main.cpp
- TBCExtensions/src/Client/CustomLua.cpp and CustomLua.hpp
- Tools/FunctionResearch/build_research_data.py
- ../MasterWoWDevTools/MasterWoWDevTools.toc
- ../MasterWoWDevTools/Modules/Native.lua and Research.lua
- ../MasterWoWDevTools/Modules/NativeCatalog.lua, ClientInternals.lua, DBCInspector.lua
- ../MasterWoWDevTools/Data/ProposedLabels.lua, DBCSamples.lua (generated)
- ../lua-validation/test_panels.js
- Docs/API3_RUNTIME_TEST.md, TBC_NATIVE_API.md, DEVTOOLS_NATIVE_BRIDGE.md

Build output: build-x86-nmake/bin/TBCExtensions.dll. Existing unrelated workspace changes were preserved.
