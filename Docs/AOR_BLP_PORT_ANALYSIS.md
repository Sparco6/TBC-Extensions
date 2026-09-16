# AoR and WXL BLP port analysis

## Reusable concepts

- Parse and validate the complete header before decoding.
- Preserve all valid mip levels and rebase offsets when producing replacement bytes.
- Route by encoding: palette, DXT1, DXT3, DXT5, and later BGRA8.
- Treat character-component textures separately from ordinary GPU textures because legacy clients may CPU-compose them.
- Transform bytes at a file/storage boundary when possible, allowing the stock texture cache and renderer to retain ownership.

## AoR evidence

`AoREngine-main/tools/aor-blp-corpo` is an offline converter. It demonstrates DXT1 (`alphaEncoding=0`), DXT3 (`1`), and DXT5 (`7`) decoding and conversion to a paletted legacy shape. It is valuable format evidence, but it is not a TBC runtime hook or proof of TBC addresses.

AoR's runtime retail-assets code relies on WXL storage serving converted BLP bytes before the client texture cache loads them. Other AoR texture-cache addresses are for its 3.3.5 target and must not be copied into build 8606.

## WXL evidence

`wxl-core` contains three relevant transforms:

- uncompressed BGRA encoding 3 to DXT5;
- DXT character components to paletted BLP2;
- top-mip removal with offset rebasing.

Its public `wxl.modern-blp` interface is wired to a storage transform hook. This validates the architecture for WXL's client, not the hook address, ABI, cache lifetime, or exact capability set for TBC.

| Reference | Classification | Use here |
|---|---|---|
| `aor-blp-corpo/main.cpp` header and mip parsing | PARTIALLY_PORTABLE | Format guidance only; bounds policy was rewritten. |
| `aor-blp-corpo/main.cpp` DXT1/DXT3/DXT5 decode | PORTABLE concept | Used as behavioral reference; independent decoder implementation. |
| `aor-blp-corpo/main.cpp` paletted output | PARTIALLY_PORTABLE | Useful if the TBC stock path later needs transcoded bytes. Not enabled. |
| WXL `BlpTranscode::TranscodeBlp` | INCOMPLETE for TBC | Demonstrates BGRA-to-DXT5 policy, but is not validated against build 8606. |
| WXL `TextureComponentToPaletted` | WOTLK_SPECIFIC integration | Component policy belongs to the later model/character pipeline and is out of scope. |
| WXL `CapBlpMips` | PORTABLE concept | Offset rebasing is reusable; no runtime cap is enabled here. |
| WXL `ModernBlpApi` and storage transform hook | UNSAFE_TO_PORT | Client hook, ABI, and lifetime are 3.3.5/WXL-specific. |
| AoR `EquipPainter` texture-cache addresses | UNSAFE_TO_PORT | WotLK absolute addresses and object layouts. |
| AoR retail-assets M2 texture-name hooks | WOTLK_SPECIFIC | Model pipeline; explicitly outside this task. |
| D3D9 upload path | INCOMPLETE | No independently verified TBC device/cache ownership path was found, so none was implemented. |

## Not ported

- No WXL/AoR hook or address.
- No storage layer, async callback, cache object, D3D device, or model code.
- No GPL source copied into the independent TBCExtensions decoder. The local implementation was written against the documented BLP2 layout and synthetic fixtures.
- No retail M2/SKIN, animation, particle, ribbon, character-component, PNG, or DDS work.

## Safety differences in this implementation

TBCExtensions validates dimensions, decoded-byte budget, offset/size pairs, sparse and overlapping mip ranges, exact payload size per encoding, alpha combinations, and chain termination. Its loose-file inspector accepts only `TBCExt_*.blp` basenames from `TBCExtensions\Textures`; arbitrary paths are rejected.
