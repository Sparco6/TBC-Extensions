# API 2 test update — 2026-09-02

Prior milestone: user confirmed DLL callbacks, addon status, and safe inspection of 0x007059B0 in-game. This update is statically checked and built, but is NOT yet runtime-confirmed.

## Install

1. Fully exit WoW. Keep a copy of your currently working TBCExtensions.dll and MasterWoWDevTools addon for rollback.
2. Extract the test archive. Copy TBCExtensions.dll alongside your existing custom Wow.exe. Copy MasterWoWDevTools into Interface/AddOns, replacing that addon only.
3. Do NOT run the patcher again. Do NOT replace Wow.exe. The existing working loader is unchanged.

## Test

1. Launch normally and log into a character. Expected: no startup/login crash.
2. Run `/run print(TBCExt_GetVersion())`. Expected: `TBCExtensions 0.1.1-research`.
3. Run `/run print(TBCExt_GetNativeApiVersion())`. Expected: `2`.
4. Open `/mdev`, Native / DLL, Refresh DLL Status. Expected: profile `MASTERWOW_CUSTOM_8606`, both startup signature statuses VALID, trace DISABLED, model hooks NOT CONFIGURED.
5. Inspect `0x007059B0`. Expected: readable/executable, Wow.exe, RVA 0x003059B0. Inspect `0xFFFFFFFF`: must not be labeled Wow.exe. This never invokes either address.
6. Research / Export: type `0x007059B0` into notebook address and press Enter. Expected: name/category filled from catalog. Edit your friendly name/observation, Add Note, then Export Notes. Expected: catalog metadata retained and your edited name preserved.
7. Attempt Add Note with `0x1E272A38`. Expected: rejected unless it is an exact native catalog record (it is not in the current catalog). Existing saved notes are not deleted.
8. Reload UI and repeat version/status checks. Send the status screenshot and any new log/error output.

If startup fingerprint validation fails, the DLL logs the mismatch and skips Lua registration and bridge hooks. The addon remains standalone. Restore only the previously working DLL/addon if rollback is needed; leave Wow.exe unchanged.

## Scope and limitations

- Snapshot is taken BEFORE bridge hooks modify the registration/validator prologues. Status describes that startup snapshot, not ongoing tamper detection or hook health.
- Checks PE machine x86, actual base 0x00400000, timestamp 1215715495, SizeOfImage 0xAC8000, and exact prefixes of registration, lua_pushstring, lua_pushnumber, lua_pushboolean, lua_tonumber, and callback validator.
- `MASTERWOW_CUSTOM_8606` is a compatible critical-region profile, not proof of the full custom patch set. Other clients with matching critical regions may match too. Whole-file SHA is not required.
- The six additional status callbacks are individually whitelisted; no blanket external callback exemption or generic native call API is introduced.
- A mismatch disables the dependent bridge as a unit. Trace/model features remain unimplemented and disabled. Per-feature checks for those will be required before enabling them.
- Notebook lookup uses compact catalog metadata; long signatures are not present in that catalog and remain empty. Catalog status is inherited evidence, not a claim that a new observation verifies a function.

## Static verification

- VS2022 x86 rebuild succeeded.
- DLL PE machine 0x014C and optional header 0x010B confirmed.
- All 25 addon Lua files parsed successfully in Lua 5.1 mode.
- DLL SHA-256: `34F1353E2BC5ECC568621839DAA20666DA1AE8C24A650CD174B590BCA441CAB9`.
- No client executable modified during this update.

## Files changed

- TBCExtensions/src/Client/CustomLua.cpp
- TBCExtensions/src/Client/CustomLua.hpp
- TBCExtensions/src/Main.cpp
- ../MasterWoWDevTools/Modules/Native.lua
- ../MasterWoWDevTools/Modules/Research.lua
- Docs/TBC_NATIVE_API.md
- Docs/DEVTOOLS_NATIVE_BRIDGE.md
- Docs/API2_RUNTIME_TEST.md

Build artifact: build-x86-nmake/bin/TBCExtensions.dll.
