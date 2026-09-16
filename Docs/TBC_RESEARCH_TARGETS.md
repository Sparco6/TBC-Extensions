# TBC Research Targets

| Priority | Goal | Known/candidate | Safe experiment | Required confidence |
|---|---|---|---|---|
| 1 | External Lua callback validation | RegisterFunction `0x7059B0`; range-check site unknown | Load minimal DLL, query version only | Confirmed runtime |
| 1 | DLL integration | Existing validated LoadLibrary stub offsets | Startup/no-crash/version tests | Confirmed runtime |
| 1 | DBC lookup | `StaticDBLoadAll 0x573C90`, `WowClientDB::GetRow 0x4047C0` | Instrument only after trace milestone | High |
| 1 | Model loading | `CMap::SafeOpen 0x6B8D10` is adjacent evidence, not a model loader | Explicit known-model preview | High |
| 1 | Texture loading | unresolved | UI preview of known paths; no native call | High |
| 2 | Camera | unresolved candidates from strings/xrefs | one camera action per trace window | High |
| 2 | Object manager | `ObjectMgrClient::RegisterHandlers 0x46E060` | instrument only | High |
| 2 | Item display/spells | `Script_GetItemInfo 0x49B6F0`; spell Lua symbols | existing Lua queries first | High |
| 3 | Renderer/network | network symbols exist; structures incomplete | no calls; controlled instrumentation later | Confirmed |

Unknown functions must enter as `CANDIDATE` or `OBSERVED`, never `VERIFIED`,
until static evidence and repeatable experiments justify promotion.
