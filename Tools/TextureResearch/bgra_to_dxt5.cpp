#include "Graphics/Textures/BLPReader.hpp"
#include <algorithm>
#include <array>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <random>
#include <stdexcept>
#include <vector>

namespace {
constexpr size_t kHeader = 148;
constexpr size_t kMaxFile = 128u * 1024u * 1024u;
void put32(std::vector<uint8_t>& out, size_t p, uint32_t v) {
    for (int i = 0; i < 4; ++i) out[p + i] = uint8_t(v >> (8 * i));
}
uint16_t rgb565(const std::array<uint8_t, 4>& p) {
    return uint16_t(((p[0] >> 3) << 11) | ((p[1] >> 2) << 5) | (p[2] >> 3));
}
std::array<uint8_t, 3> expand565(uint16_t c) {
    return {uint8_t(((c >> 11) & 31) * 255 / 31),
            uint8_t(((c >> 5) & 63) * 255 / 63),
            uint8_t((c & 31) * 255 / 31)};
}
std::array<std::array<uint8_t, 3>, 4> colors(uint16_t hi, uint16_t lo) {
    auto a = expand565(hi), b = expand565(lo);
    return {a, b,
            std::array<uint8_t, 3>{uint8_t((2 * a[0] + b[0]) / 3), uint8_t((2 * a[1] + b[1]) / 3), uint8_t((2 * a[2] + b[2]) / 3)},
            std::array<uint8_t, 3>{uint8_t((a[0] + 2 * b[0]) / 3), uint8_t((a[1] + 2 * b[1]) / 3), uint8_t((a[2] + 2 * b[2]) / 3)}};
}
void encodeBlock(const Blp::MipLevel& mip, uint32_t bx, uint32_t by, std::vector<uint8_t>& out) {
    std::array<std::array<uint8_t, 4>, 16> p{};
    uint8_t amin = 255, amax = 0;
    for (uint32_t y = 0; y < 4; ++y) for (uint32_t x = 0; x < 4; ++x) {
        const auto sx = std::min(bx * 4 + x, mip.width - 1);
        const auto sy = std::min(by * 4 + y, mip.height - 1);
        const size_t at = (size_t(sy) * mip.width + sx) * 4;
        auto& q = p[y * 4 + x];
        for (int c = 0; c < 4; ++c) q[c] = mip.rgba[at + c];
        amin = std::min(amin, q[3]); amax = std::max(amax, q[3]);
    }
    out.push_back(amax); out.push_back(amin);
    std::array<uint8_t, 8> ap{amax, amin};
    for (int i = 1; i <= 6; ++i) ap[i + 1] = uint8_t(((7 - i) * amax + i * amin) / 7);
    uint64_t abits = 0;
    for (int i = 0; i < 16; ++i) {
        unsigned best = 0, delta = 256;
        for (unsigned j = 0; j < 8; ++j) {
            unsigned d = unsigned(std::abs(int(p[i][3]) - int(ap[j])));
            if (d < delta) { delta = d; best = j; }
        }
        abits |= uint64_t(best) << (i * 3);
    }
    for (int i = 0; i < 6; ++i) out.push_back(uint8_t(abits >> (8 * i)));
    // Pick the farthest color pair in the block, then choose the nearest palette entry.
    int distance = -1; uint16_t c0 = 0, c1 = 0;
    for (int i = 0; i < 16; ++i) for (int j = i + 1; j < 16; ++j) {
        int d = 0; for (int k = 0; k < 3; ++k) { int v = int(p[i][k]) - p[j][k]; d += v * v; }
        if (d > distance) { distance = d; c0 = rgb565(p[i]); c1 = rgb565(p[j]); }
    }
    if (c0 < c1) std::swap(c0, c1);
    if (c0 == c1) { if (c0 == 0) ++c0; else --c1; }
    auto cp = colors(c0, c1);
    uint32_t cbits = 0;
    for (int i = 0; i < 16; ++i) {
        unsigned best = 0; int delta = INT32_MAX;
        for (unsigned j = 0; j < 4; ++j) {
            int d = 0; for (int k = 0; k < 3; ++k) { int v = int(p[i][k]) - cp[j][k]; d += v * v; }
            if (d < delta) { delta = d; best = j; }
        }
        cbits |= uint32_t(best) << (i * 2);
    }
    out.push_back(uint8_t(c0)); out.push_back(uint8_t(c0 >> 8));
    out.push_back(uint8_t(c1)); out.push_back(uint8_t(c1 >> 8));
    for (int i = 0; i < 4; ++i) out.push_back(uint8_t(cbits >> (8 * i)));
}
std::vector<uint8_t> transform(const std::vector<uint8_t>& source) {
    auto src = Blp::Read(source.data(), source.size());
    if (!src.ok || src.texture.version != Blp::Version::BLP2 || src.texture.sourceFormat != Blp::Format::BGRA8 ||
        src.texture.encoding != 3 || src.texture.alphaDepth != 8 || src.texture.alphaEncoding != 0 ||
        source.size() < kHeader || source[4] != 1 || source[5] || source[6] || source[7])
        throw std::runtime_error("source is not validated content-type-1 BLP2 encoding-3 BGRA8");
    const auto& t = src.texture;
    if (t.mipLevels.empty() || t.mipLevels.size() > 16) throw std::runtime_error("invalid mip chain");
    std::vector<uint8_t> out(kHeader, 0);
    out[0] = 'B'; out[1] = 'L'; out[2] = 'P'; out[3] = '2'; put32(out, 4, 1);
    out[8] = 2; out[9] = 8; out[10] = 7; out[11] = uint8_t(t.hasMipFlag);
    put32(out, 12, t.width); put32(out, 16, t.height);
    for (size_t n = 0; n < t.mipLevels.size(); ++n) {
        const auto& m = t.mipLevels[n];
        if (!m.width || !m.height || m.rgba.size() != size_t(m.width) * m.height * 4)
            throw std::runtime_error("decoded mip metadata mismatch");
        const uint64_t blocks = uint64_t((m.width + 3) / 4) * ((m.height + 3) / 4);
        if (blocks > (kMaxFile - out.size()) / 16) throw std::runtime_error("output budget exceeded");
        put32(out, 20 + n * 4, uint32_t(out.size()));
        put32(out, 84 + n * 4, uint32_t(blocks * 16));
        for (uint32_t y = 0; y < (m.height + 3) / 4; ++y)
            for (uint32_t x = 0; x < (m.width + 3) / 4; ++x) encodeBlock(m, x, y, out);
    }
    auto check = Blp::Read(out.data(), out.size());
    if (!check.ok || check.texture.sourceFormat != Blp::Format::DXT5 || check.texture.width != t.width ||
        check.texture.height != t.height || check.texture.mipLevels.size() != t.mipLevels.size())
        throw std::runtime_error("generated DXT5 failed independent validation");
    for (size_t i = 0; i < t.mipLevels.size(); ++i)
        if (check.texture.mipLevels[i].width != t.mipLevels[i].width ||
            check.texture.mipLevels[i].height != t.mipLevels[i].height)
            throw std::runtime_error("generated mip dimension mismatch");
    return out;
}
std::vector<uint8_t> readFile(const char* name) {
    std::ifstream f(name, std::ios::binary | std::ios::ate);
    if (!f || f.tellg() < 0 || uint64_t(f.tellg()) > kMaxFile) throw std::runtime_error("input open/budget failure");
    std::vector<uint8_t> bytes(size_t(f.tellg())); f.seekg(0);
    if (!f.read(reinterpret_cast<char*>(bytes.data()), bytes.size())) throw std::runtime_error("input read failure");
    return bytes;
}
void selfTest(const std::vector<uint8_t>& src) {
    auto original = Blp::Read(src.data(), src.size());
    if (!original.ok) throw std::runtime_error("self-test source parse failure");
    auto a = transform(src), b = transform(src);
    if (a != b) throw std::runtime_error("repeated transform differs");
    { auto multi = src; multi[11] = 1; put32(multi, 24, uint32_t(multi.size()));
      put32(multi, 88, 32 * 32 * 4); multi.insert(multi.end(), 32 * 32 * 4, 0);
      for (size_t p = multi.size() - 32 * 32 * 4; p < multi.size(); p += 4) {
          multi[p] = 20; multi[p + 1] = 100; multi[p + 2] = 240; multi[p + 3] = 128;
      }
      auto encoded = transform(multi); auto parsed = Blp::Read(encoded.data(), encoded.size());
      if (!parsed.ok || parsed.texture.mipLevels.size() != 2 ||
          parsed.texture.mipLevels[1].width != 32 || parsed.texture.mipLevels[1].height != 32)
          throw std::runtime_error("two-mip transform failure"); }
    auto decoded = Blp::Read(a.data(), a.size());
    for (size_t i = 0; i < original.texture.mipLevels.size(); ++i) {
        const auto& x = original.texture.mipLevels[i].rgba;
        const auto& y = decoded.texture.mipLevels[i].rgba;
        if (x.size() != y.size()) throw std::runtime_error("alpha test size mismatch");
        for (size_t p = 3; p < x.size(); p += 4)
            if (std::abs(int(x[p]) - int(y[p])) > 20)
                throw std::runtime_error("DXT5 alpha error exceeds 20/255");
    }
    auto reject = [](std::vector<uint8_t> v) {
        try { (void)transform(v); } catch (const std::exception&) { return; }
        throw std::runtime_error("malformed source accepted");
    };
    { auto v = src; v[9] = 4; reject(v); }
    { auto v = src; v[10] = 7; reject(v); }
    { auto v = src; v.resize(v.size() - 1); reject(v); }
    { auto v = src; put32(v, 20, UINT32_MAX); reject(v); }
    { auto v = src; v[11] = 1; put32(v, 24, 148); put32(v, 88, 16); reject(v); }
    { auto v = src; put32(v, 12, 8192); put32(v, 16, 8192); reject(v); }
    std::mt19937 rng(8606);
    for (int i = 0; i < 5000; ++i) {
        auto v = src;
        for (int j = 0, n = 1 + int(rng() % 5); j < n; ++j) v[rng() % v.size()] = uint8_t(rng());
        try {
            auto o = transform(v);
            if (o.size() > kMaxFile || !Blp::Read(o.data(), o.size()).ok)
                throw std::runtime_error("mutation generated invalid output");
        } catch (const std::runtime_error& e) {
            if (std::string(e.what()) == "mutation generated invalid output") throw;
        }
    }
    std::cout << "Offline transform self-test PASS: repeat, two mips, metadata, alpha <=20/255, explicit malformed cases, 5000 deterministic mutations\n";
}
}
int main(int argc, char** argv) {
    try {
        if (argc == 2) { selfTest(readFile(argv[1])); return 0; }
        if (argc != 3) throw std::runtime_error("usage: bgra_to_dxt5 source.blp [output.blp]");
        auto out = transform(readFile(argv[1]));
        std::ofstream f(argv[2], std::ios::binary | std::ios::trunc);
        if (!f.write(reinterpret_cast<const char*>(out.data()), out.size())) throw std::runtime_error("output write failure");
        std::cout << "Offline BGRA8->DXT5 PASS: " << out.size() << " bytes; independent reparse PASS\n";
        return 0;
    } catch (const std::exception& e) { std::cerr << "Offline transform FAIL: " << e.what() << '\n'; return 1; }
}
