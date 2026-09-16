# AoREngine to TBC Port Matrix

| Module | Reuse class | TBC dependencies | Catalog link/status | Difficulty |
|---|---|---|---|---|
| Bootstrap/module registry | Architecture reusable | x86 lifecycle and safe feature gates | DLL loader awaiting runtime test | Low |
| Logging | Direct architecture reuse | writable path only | no native address | Low |
| Lua bridge | Adapted logic | RegisterFunction `0x7059B0`, Lua API set | 17 confirmed catalog records | Medium |
| Hook wrapper/MinHook | Architecture reusable | x86 executable-section validation | deferred until Lua bridge confirmation | Medium |
| Offset registry | Direct architecture reuse | build/profile-specific values | `ClientOffsets_8606.hpp` | Low |
| DBC reader | Logic reusable after adaptation | TBC schemas, DB globals, GetRow `0x4047C0` | partial | High |
| Model/display extensions | Requires TBC RE | model manager/renderer structures | unresolved | Very high |
| Camera | Requires TBC RE | camera object and methods | unresolved | High |
| Object manager | Requires TBC RE | globals/layouts; handler symbol `0x46E060` | candidate dependency | Very high |
| Custom packets | Requires TBC RE | net client and CDataStore methods | partial symbols, unsafe | Very high |
| Retail DB2/WDC | Not appropriate | TBC uses DBC | excluded | N/A |

No WotLK address is accepted as a TBC candidate without independent evidence.
