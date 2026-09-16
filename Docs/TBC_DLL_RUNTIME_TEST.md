# TBCExtensions DLL Runtime Test

The project loads the DLL through a startup code stub in `Wow.exe` which calls
the client's existing `LoadLibraryA` import. It is not a proxy DLL or remote
thread injector.

1. Keep the working custom `Wow.exe`; never replace it with the reference file.
2. Run `TBCPatcher.exe --analyze "path\to\Wow.exe"` and inspect both reports.
3. Only if every loader record is `NOT_APPLIED` or `ALREADY_APPLIED`, run
   `TBCPatcher.exe --apply "path\to\Wow.exe"`. This creates a timestamped backup.
4. Copy the x86 `TBCExtensions.dll` beside that same `Wow.exe`.
5. Launch normally and run the three `/run` commands in the test checklist.

The loader patch, world-state lifecycle registration, and six-function safe
bridge were runtime-confirmed by the user on 2026-09-02. Static inspection found
no rejection in `FrameScript__RegisterFunction` -> `lua_pushcclosure`; rejection
occurs later at `0x0074A160`, immediately before client callback dispatch. The
bridge byte-validates that function and bypasses it only for the six exact safe
TBCExt callback addresses. Restore with
`TBCPatcher.exe --restore "Wow.exe.backup_TIMESTAMP"`.
