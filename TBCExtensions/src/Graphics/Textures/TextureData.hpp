#pragma once
#include "BLPFormat.hpp"
#include <cstdint>
#include <string>
#include <vector>

namespace Blp {
struct MipLevel {
    uint32_t width = 0;
    uint32_t height = 0;
    std::vector<uint8_t> rgba; // top-left row-major RGBA8
};
struct DecodedTexture {
    Version version = Version::BLP2;
    Format sourceFormat = Format::PaletteRGBA8;
    uint8_t encoding = 0;
    uint8_t alphaDepth = 0;
    uint8_t alphaEncoding = 0;
    bool hasMipFlag = false;
    uint32_t width = 0;
    uint32_t height = 0;
    std::vector<MipLevel> mipLevels;
    std::string note;
};
}
