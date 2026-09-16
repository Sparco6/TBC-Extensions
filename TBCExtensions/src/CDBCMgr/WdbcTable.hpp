#pragma once
#include <cstdint>
#include <algorithm>
#include <cstring>
#include <cmath>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>

// Fixed-width WDBC only. No client pointers, native DB layouts or DB2 inference.
namespace NativeDBC {
class Table {
public:
    static constexpr size_t MaxBytes = 64u * 1024u * 1024u;
    uint32_t rows = 0, fields = 0, recordBytes = 0, stringBytes = 0;
    std::string name;
    std::vector<unsigned char> data;
    std::unordered_map<uint32_t, uint32_t> index;

    static uint32_t U32(const unsigned char* p) {
        return uint32_t(p[0]) | uint32_t(p[1]) << 8 | uint32_t(p[2]) << 16 | uint32_t(p[3]) << 24;
    }
    void Parse(std::vector<unsigned char> bytes) {
        if (bytes.size() < 20 || bytes.size() > MaxBytes || std::memcmp(bytes.data(), "WDBC", 4))
            throw std::runtime_error("Expected a WDBC file, 20 bytes to 64 MiB; DB2/WDC/CSV not supported");
        rows = U32(bytes.data()+4); fields = U32(bytes.data()+8);
        recordBytes = U32(bytes.data()+12); stringBytes = U32(bytes.data()+16);
        if (rows > 1000000 || fields < 1 || fields > 256 || recordBytes != fields * 4 || stringBytes < 1)
            throw std::runtime_error("Invalid or unsupported WDBC dimensions");
        const uint64_t total = 20ull + uint64_t(rows)*recordBytes + stringBytes;
        if (total != bytes.size()) throw std::runtime_error("WDBC length does not match header");
        const auto strings = 20ull + uint64_t(rows)*recordBytes;
        if (bytes[size_t(strings)] != 0 || bytes.back() != 0)
            throw std::runtime_error("String block must start and end with NUL");
        data = std::move(bytes); index.clear(); index.reserve(rows);
        for (uint32_t i=0; i<rows; ++i) {
            auto id = U32(data.data()+20+size_t(i)*recordBytes);
            if (!index.emplace(id,i).second) throw std::runtime_error("Duplicate ID in first field");
        }
        // Schema from local AoR retail-assets sources, not the stock TBC schema.
        if (name == "AoR_CreatureModelData.dbc" && fields != 3)
            throw std::runtime_error("AoR_CreatureModelData requires ID, ModelPath, MountHeight (3 fields)");
        if (name == "AoR_CreatureDisplayInfo.dbc" && fields != 7)
            throw std::runtime_error("AoR_CreatureDisplayInfo requires 7 fields");
        for (const auto& entry : index) {
            if (name == "AoR_CreatureModelData.dbc") {
                String(entry.first,2); Float(entry.first,3);
            } else if (name == "AoR_CreatureDisplayInfo.dbc") {
                for (uint32_t c=3; c<=7; ++c) String(entry.first,c);
            }
        }
    }
    uint32_t Cell(uint32_t id, uint32_t column) const {
        if (column < 1 || column > fields) throw std::runtime_error("Column outside table (columns start at 1)");
        const auto it = index.find(id);
        if (it == index.end()) throw std::runtime_error("ID not found");
        return U32(data.data()+20+size_t(it->second)*recordBytes+(column-1)*4);
    }
    std::string String(uint32_t id, uint32_t column) const {
        const auto offset = Cell(id,column);
        if (offset >= stringBytes) throw std::runtime_error("String offset outside string block");
        const char* p = reinterpret_cast<const char*>(data.data()+20+size_t(rows)*recordBytes+offset);
        const auto limit = (std::min)(size_t(stringBytes-offset),size_t(4097));
        const char* end = static_cast<const char*>(std::memchr(p,0,limit));
        if (!end) throw std::runtime_error("Unterminated string or string exceeds 4096 bytes");
        return std::string(p,end);
    }
    float Float(uint32_t id, uint32_t column) const {
        auto bits = Cell(id,column); float result; std::memcpy(&result,&bits,4);
        if (!std::isfinite(result)) throw std::runtime_error("Non-finite float");
        return result;
    }
};
}
