#pragma once
#include <cstdint>

namespace Blp {
enum class Version { BLP1, BLP2 };
enum class Format { PaletteRGBA8, DXT1, DXT3, DXT5, BGRA8, NativeBLP1 };

inline const char* Name(Version v) { return v == Version::BLP1 ? "BLP1" : "BLP2"; }
inline const char* Name(Format f) {
    switch (f) {
    case Format::PaletteRGBA8: return "Palette/RGBA8";
    case Format::DXT1: return "DXT1";
    case Format::DXT3: return "DXT3";
    case Format::DXT5: return "DXT5";
    case Format::BGRA8: return "Uncompressed BGRA8";
    case Format::NativeBLP1: return "Native BLP1 (not decoded by TBCExt)";
    }
    return "Unknown";
}
}
