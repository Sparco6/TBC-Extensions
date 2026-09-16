#include "BLPReader.hpp"
#include "BLP2Decoder.hpp"
#include <cstring>
#include <sstream>

namespace Blp {
namespace {
uint32_t U32(const uint8_t* p) {
    return uint32_t(p[0]) | uint32_t(p[1]) << 8 | uint32_t(p[2]) << 16 | uint32_t(p[3]) << 24;
}
}
Result Read(const uint8_t* data, size_t size) {
    if (!data || size < 4) return {false, {}, "File is shorter than a BLP magic"};
    if (!std::memcmp(data, "BLP2", 4)) return DecodeBLP2(data, size);
    if (!std::memcmp(data, "BLP1", 4)) {
        if (size < 156) return {false, {}, "Truncated BLP1 header"};
        const uint32_t compression=U32(data+4), flags=U32(data+8), width=U32(data+12), height=U32(data+16);
        if (!width || !height || width>8192 || height>8192)
            return {false, {}, "Invalid BLP1 dimensions"};
        if (compression != 0 && compression != 1)
            return {false, {}, "Unsupported BLP1 compression value"};
        DecodedTexture t; t.version=Version::BLP1; t.sourceFormat=Format::NativeBLP1;
        t.width=width;t.height=height;t.alphaDepth=uint8_t(flags & 0xff);
        t.note=compression==0 ? "BLP1 JPEG: preserve stock TBC path" : "BLP1 palette: preserve stock TBC path";
        for (uint32_t i=0;i<16;++i) {
            const uint32_t off=U32(data+28+i*4), bytes=U32(data+92+i*4);
            if ((!off) != (!bytes)) return {false, {}, "BLP1 mip has only one of offset/size"};
            if (off && (off>size || bytes>size-off)) return {false, {}, "BLP1 mip outside file"};
        }
        Result r;r.ok=true;r.texture=std::move(t);return r;
    }
    return {false, {}, "Not BLP1 or BLP2"};
}
std::string Describe(const Result& r) {
    if (!r.ok) return "Parser: ERROR\nReason: "+r.error;
    const auto& t=r.texture;size_t bytes=0;
    for(const auto& m:t.mipLevels)bytes+=m.rgba.size();
    std::ostringstream s;s<<Name(t.version)<<"\nDimensions: "<<t.width<<"x"<<t.height
        <<"\nSource format: "<<Name(t.sourceFormat)<<"\nEncoding: "<<unsigned(t.encoding)
        <<"\nAlpha depth: "<<unsigned(t.alphaDepth)<<"\nAlpha encoding: "<<unsigned(t.alphaEncoding)
        <<"\nMip levels: "<<t.mipLevels.size()<<"\nDecoded RGBA bytes: "<<bytes<<"\nParser: OK";
    if(!t.note.empty())s<<"\n"<<t.note;
    for(size_t i=0;i<t.mipLevels.size();++i)
        s<<"\nMip "<<i<<": "<<t.mipLevels[i].width<<"x"<<t.mipLevels[i].height
         <<", "<<t.mipLevels[i].rgba.size()<<" bytes";
    return s.str();
}
}
