# Custom Wow.exe Baseline

Authoritative comparison date: 2026-09-02. Full machine-readable detail is in
`Docs/custom_wow_diff.json`.

- Custom SHA-256: `C1ED7F0404DBC953EC40CF13D92DE33018AC8B197AD91ABAB9D2FA4D95136C44`
- Reference SHA-256: `9561DE227C3B1C05E56D542C7DD308EB5A00DCB6C6A0B64AC75C2725CAABF3AA`
- Both: PE32 x86, file version 2.4.3.8606, PE timestamp 1215715495, image size 11,304,960
- Custom size: 8,267,776; reference size: 8,272,528
- Differences: 5,065 bytes across 56 contiguous ranges

The differences include PE metadata, multiple code edits/NOP regions, floating
point constants, and a 4,752-byte tail difference. Semantics such as FoV,
FrameXML, GlueXML, items, and nameplate range are not assigned solely from byte
shape; those labels require prior patch records or disassembly evidence.

All nine loader patch locations currently match their expected unpatched build
8606 bytes. Applying only this validated loader manifest does not rewrite the
other 56 custom-difference regions.
