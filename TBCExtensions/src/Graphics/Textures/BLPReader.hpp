#pragma once
#include "TextureData.hpp"
#include <cstddef>
#include <cstdint>
#include <string>

namespace Blp {
struct Result {
    bool ok = false;
    DecodedTexture texture;
    std::string error;
};
Result Read(const uint8_t* data, size_t size);
std::string Describe(const Result& result);
}
