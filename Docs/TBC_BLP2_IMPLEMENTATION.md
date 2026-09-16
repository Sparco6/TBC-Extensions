# TBC BLP2 implementation and test plan

## Implemented

The independent subsystem under `TBCExtensions/src/Graphics/Textures` provides:

- BLP1 header inspection, without decoding or changing the stock path;
- bounded BLP2 parsing;
- palette decoding with 0/1/4/8-bit alpha;
- DXT1, DXT3, and DXT5 decoding to RGBA8;
- uncompressed BGRA8 decoding to RGBA8;
- validated mip chains and explicit error messages;
- an offline loose-file inspector exposed through API version 5.

Runtime texture fallback and D3D9 upload are deliberately disabled. `TBCExt_GetTextureLoaderStatus()` reports that state. `TBCExt_InspectBLP("TBCExt_BLP2Test.blp")` reads only from `TBCExtensions\Textures` next to the client.

Phase 2 adds stock-path DXT1, DXT3, and DXT5 controls. It still installs no texture hook and performs no runtime transformation. This is a mandatory safety gate before selecting DXT5 as the transform target.

## Offline tests

`Tools/TextureResearch/test_blp.cpp` builds synthetic 4x4 palette, DXT1, DXT3, DXT5, BGRA8, and BLP1 inputs. It also rejects truncated, invalid, sparse, overlapping, and size-mismatched inputs and runs 5,000 deterministic byte mutations under the same output budget.

`Tools/TextureResearch/make_blp_samples.js` produces two isolated 64x64 assets:

- `TBCExt_BLP2Test.blp`: BLP2 encoding 3 BGRA8 candidate (also installed as `Interface\MasterWoW\BLP2Test.blp`);
- `TBCExt_BLP2Palette.blp`: visually related encoding 1 paletted control (also installed as `Interface\MasterWoW\BLP2Palette.blp`).

## First controlled in-game test

1. Back up the current DLL and addon.
2. Put `TBCExt_BLP2Test.blp` beside the DLL under `TBCExtensions\Textures` for offline inspection.
3. Put both samples under `Interface\MasterWoW\`.
4. Start build 8606 and open Developer Toolkit > Textures.
5. Press **Inspect BLP2**. Expected: parsed metadata and `OK`; no texture is loaded by this action.
6. Press **Palette Control**. Expected: a colored 64x64 checker through stock `Texture:SetTexture()`.
7. Press **BLP2 Candidate**. Record whether it renders the same class of image, renders white/blank, rejects cleanly, or crashes.
8. Reopen ordinary icons and play normally. They must remain unchanged.

Do not use a crash-prone weapon, map, model, or production asset for this test. The candidate is isolated and the DLL does not hook texture loading.

## Decision after the result

- If both render, TBC already supports this encoding and no fallback is needed for it.
- If palette renders and BGRA8 fails cleanly, implement a storage/file-read transform that converts only validated encoding-3 `TBCExt_` test assets to a stock-supported BLP2 encoding. Resolve and validate that boundary before patching.
- If either asset crashes, stop. Capture the client crash report and stack before adding a hook.
- Direct D3D9 RGBA upload is a later option only if a byte transform cannot reuse the stock texture cache. It requires verified device/thread ownership, format creation, mip upload, cache lifetime, reset handling, and destruction semantics.

## Current boundary

This revision is parser/decoder plus an explicit stock-path experiment. It does not claim end-to-end fallback, rendering, or live safety. M2/SKIN and all unrelated expansion features remain out of scope.
