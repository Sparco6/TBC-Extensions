#pragma once
#include <cstdint>
#include <string>
namespace NativeDBC {
// One independently owned table. No writes to stock DBCs or executable memory.
std::string Request(const std::string& action, const std::string& filename,
    uint32_t id, uint32_t column, const std::string& type);
}
