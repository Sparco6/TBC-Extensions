# Canonical important address registry — custom TBC 8606

Status applies to the stated role, not merely the mapped address. `Runtime observed?` means this role in the relevant client path, not Toolkit inspection. All absolute code/data addresses below are for the fingerprinted custom x86 image; never transplant 3.3.5 addresses. Primary source: `TBCExtensions/src/Offsets/ClientOffsets_8606.hpp`, `Client/CustomLua.cpp`, `Main.cpp`, `Graphics/Textures/NativeBLPCorrelation.cpp`, `Docs/TBC_BLP2_RUNTIME_TRANSFORM.md`.

| Address | Proposed name | Subsystem | Evidence status | Runtime observed? | Hooked? | Production/research | Source/reference and notes |
|---|---|---|---|---|---|---|---|
| `0x0072F5C0` | LuaLError | Lua | STATIC VERIFIED | Bridge subset reported, individual unknown | No | Safe bridge | Offsets header |
| `0x0072DFC0` | LuaToBoolean | Lua | STATIC VERIFIED | Individual unknown | No | Safe bridge | Offsets header |
| `0x0072DF40` | LuaToNumber | Lua | STATIC VERIFIED | Individual unknown | No | Safe bridge | Critical signature checked |
| `0x0072DFF0` | LuaToLString | Lua | STATIC VERIFIED | Individual unknown | No | Safe bridge | Critical signature checked |
| `0x0072DAE0` | LuaGetTop | Lua | STATIC VERIFIED | Individual unknown | No | Safe bridge | Critical signature checked |
| `0x0072DB00` | LuaSetTop | Lua | STATIC VERIFIED | Individual unknown | No | Safe bridge | Critical signature checked |
| `0x0072DE30` | LuaIsNumber | Lua | STATIC VERIFIED | Individual unknown | No | Safe bridge | Offsets header |
| `0x0072DE70` | LuaIsString | Lua | STATIC VERIFIED | Individual unknown | No | Safe bridge | Offsets header |
| `0x0072DDC0` | LuaType | Lua | STATIC VERIFIED | Individual unknown | No | Safe bridge | Critical signature checked |
| `0x0072E180` | LuaPushNil | Lua | STATIC VERIFIED | Individual unknown | No | Safe bridge | Offsets header |
| `0x0072E1A0` | LuaPushNumber | Lua | STATIC VERIFIED | Individual unknown | No | Safe bridge | Critical signature checked |
| `0x0072E250` | LuaPushString | Lua | STATIC VERIFIED | Individual unknown | No | Safe bridge | Critical signature checked |
| `0x0072E3B0` | LuaPushBoolean | Lua | STATIC VERIFIED | Individual unknown | No | Safe bridge | Critical signature checked |
| `0x0072E2F0` | LuaPushCClosure | Lua | STATIC VERIFIED | Individual unknown | No | Safe bridge | Offsets header |
| `0x0072E550` | LuaGetField | Lua | STATIC VERIFIED | Individual unknown | No | Safe bridge | Critical signature checked |
| `0x0072E7E0` | LuaSetField | Lua | STATIC VERIFIED | Individual unknown | No | Safe bridge | Offsets header |
| `0x007059B0` | FrameScriptRegisterFunction | Lua | RUNTIME VERIFIED (bridge lifecycle) | Owner reported | Yes | Safe bridge | Exact prologue in Main; lifecycle detour |
| `0x00E1DB84` | GlobalLuaStatePointer | Lua | STATIC VERIFIED | State observed via API | No | Safe bridge | Pointer, not callable function |
| `0x0074A160` | ValidateFunctionPointer | Lua | RUNTIME VERIFIED (callback bridge) | Owner reported | Yes | Exact callback allowlist | Exact prologue in Main |
| `0x00890F50` | SetTexture method literal | UI | STATIC VERIFIED | No | No | RE | Method-table xref; data literal |
| `0x009110B0` | Texture method-table entry | UI | STATIC VERIFIED | No | No | RE | Pairs literal and `0x00434BE0` |
| `0x00434BE0` | native Texture:SetTexture | UI | STATIC VERIFIED | Indirect controlled request, not directly hooked | No | RE | Method-table and argument-reader anchors |
| `0x0042F920` | Texture-object setter | UI | STATIC VERIFIED | No direct hit | No | RE | ECX object, path arg1, ret 0x10; at `0x0042FA15` stores returned resource at +0xC4 after releasing previous via `0x004271A0` |
| `0x00457FC0` | resource request | Texture | **RUNTIME VERIFIED** for both controlled requests | Yes, matched 1 each | Yes | EXPERIMENTAL observation | V2 raw result address `0x27E8AC08` for both sequential tests; not a proven unique resource key. Printed `return 1` is non-null boolean |
| `0x004576E0` | loader branch | Texture | STATIC VERIFIED / CANDIDATE for controlled path | No | No | RE | Inspected branch constructs refcounted 0x164-byte resource, path at +0x0C; controlled branch/cache choice UNKNOWN |
| `0x00455EF0` | source/parser caller | Texture | STATIC VERIFIED, NOT runtime-correlated | Global calls yes; controlled unknown | Yes | EXPERIMENTAL observation | ECX resource, EDX source; totals rose, correlation zero |
| `0x005B1BA0` | native BLP2 parser | BLP | STATIC VERIFIED generally; controlled path UNKNOWN | Global calls yes; controlled unknown | Yes | EXPERIMENTAL observation | ECX parser, source arg; retains source +0x498 |
| `0x005B3170` | alternate BLP caller | BLP | STATIC VERIFIED; Interface role UNKNOWN | Filtered NO HIT in 0.1.6 | Yes | EXPERIMENTAL older trace | Separate global-buffer route; not primary path |
| `0x0091309C` | candidate raw-source base | Texture | STATIC VERIFIED instruction use | Controlled unknown | No | RE | Wow.exe `.data`; capacity/ownership UNKNOWN |
| `0x00D43118` | alternate global-buffer object | BLP | STATIC VERIFIED | Controlled unknown | No | RE | Separate 0x005B3170 route |
| `0x00D43120` | alternate global data pointer field | BLP | STATIC VERIFIED | Controlled unknown | No | RE | Reusable/reallocatable buffer |
| `0x00657590` | source acquisition | File | STATIC VERIFIED call role | Controlled unknown | No | RE | Exact archive/cache semantics UNKNOWN |
| `0x00654E00` | read/copy dispatch | File | STATIC VERIFIED | Controlled unknown | No | RE | Multiple backends |
| `0x00655290` | acquisition-handle cleanup | File | STATIC VERIFIED | Controlled unknown | No | RE | Not global-byte-buffer release |
| `0x005B2C50` | BLP mip accessor | BLP | STATIC VERIFIED | Controlled unknown | No | RE | Adds mip offset to parser +0x498 |
| `0x005B2F70` | BLP mip accessor branch | BLP | STATIC VERIFIED | Controlled unknown | No | RE | Same source-base calculation |
| `0x005AF040` | mip size/offset-table writer | BLP | STATIC VERIFIED | Controlled unknown | No | RE | Not texture allocator |
| `0x005B3327` | pointer relocation loop | BLP | STATIC VERIFIED | Controlled unknown | No | RE | Later clears +0x498 on one path; release UNKNOWN |
| `0x006755A0` | SFile OpenFileEx | File | CANDIDATE (symbol-derived cast) | No | No | Legacy only | `Client/SFile.cpp`; contract not independently qualified |
| `0x0067FF90` | SFile ReadFile | File | CANDIDATE (symbol-derived cast) | No | No | Legacy only | `Client/SFile.cpp`; CloseFile unresolved |
| `0x00573C90` | stock DBC load-all | DBC | CANDIDATE for legacy patch | No | No | UNSAFE_LEGACY_PORT OFF | `CDBCMgr.cpp`, not DLL-owned WDBC route |

Method-table structural neighbors: `0x00436330` SetTexCoord and `0x004344C0` SetBlendMode are **STATIC VERIFIED anchors only**, not patched. Older `Docs/ADDRESS_STATUS.md` reports all Lua addresses TODO; **DEPRECATED / SUPERSEDED** for this safe offset registry. Symbol labels alone do not establish semantics. For loader **file offsets/bytes**, use `Patcher/SafeMain.cpp`, not this memory-address table.
