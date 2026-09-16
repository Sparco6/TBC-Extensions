#include "BLP2Decoder.hpp"
#include <algorithm>
#include <array>
#include <cstring>
#include <limits>
#include <stdexcept>

namespace Blp {
namespace {
constexpr size_t Header=148, PaletteEnd=Header+1024;
constexpr uint64_t MaxDecoded=256ull*1024*1024;
uint32_t U32(const uint8_t* p){return uint32_t(p[0])|uint32_t(p[1])<<8|uint32_t(p[2])<<16|uint32_t(p[3])<<24;}
uint16_t U16(const uint8_t* p){return uint16_t(p[0])|uint16_t(p[1])<<8;}
uint8_t Clamp(int v){return uint8_t((std::max)(0,(std::min)(255,v)));}
void Colors(const uint8_t* p,bool transparent,uint8_t c[4][4]) {
    uint16_t a=U16(p),b=U16(p+2);uint16_t v[2]={a,b};
    for(int i=0;i<2;++i){c[i][0]=uint8_t(((v[i]>>11)&31)*255/31);c[i][1]=uint8_t(((v[i]>>5)&63)*255/63);c[i][2]=uint8_t((v[i]&31)*255/31);c[i][3]=255;}
    if(!transparent||a>b){for(int k=0;k<3;++k){c[2][k]=Clamp((2*c[0][k]+c[1][k])/3);c[3][k]=Clamp((c[0][k]+2*c[1][k])/3);}c[2][3]=c[3][3]=255;}
    else {for(int k=0;k<3;++k)c[2][k]=uint8_t((c[0][k]+c[1][k])/2);c[2][3]=255;c[3][0]=c[3][1]=c[3][2]=c[3][3]=0;}
}
size_t AlphaBytes(uint64_t pixels,uint8_t depth){return size_t((pixels*depth+7)/8);}
uint8_t PaletteAlpha(const uint8_t* a,size_t pixel,uint8_t depth) {
    if(depth==0)return 255;
    if(depth==1)return (a[pixel/8]>>(pixel%8)&1)?255:0;
    if(depth==4)return uint8_t(((a[pixel/2]>>((pixel%2)*4))&15)*17);
    return a[pixel];
}
MipLevel PaletteMip(const uint8_t* file,size_t size,uint32_t off,uint32_t bytes,uint32_t w,uint32_t h,uint8_t depth){
    uint64_t pixels=uint64_t(w)*h;size_t alpha=AlphaBytes(pixels,depth);
    if(pixels+alpha!=bytes)throw std::runtime_error("Palette mip size is not indices plus packed alpha");
    if(off<PaletteEnd||off>size||bytes>size-off)throw std::runtime_error("Palette mip outside file/palette header");
    MipLevel m;m.width=w;m.height=h;m.rgba.resize(size_t(pixels)*4);const uint8_t* pal=file+Header;const uint8_t* idx=file+off;const uint8_t* al=idx+pixels;
    for(size_t i=0;i<pixels;++i){const uint8_t* p=pal+size_t(idx[i])*4;m.rgba[i*4]=p[2];m.rgba[i*4+1]=p[1];m.rgba[i*4+2]=p[0];m.rgba[i*4+3]=PaletteAlpha(al,i,depth);}
    return m;
}
MipLevel BgraMip(const uint8_t* file,size_t size,uint32_t off,uint32_t bytes,uint32_t w,uint32_t h){
    uint64_t pixels=uint64_t(w)*h;if(pixels*4!=bytes)throw std::runtime_error("BGRA mip size mismatch");
    if(off<Header||off>size||bytes>size-off)throw std::runtime_error("BGRA mip outside file");
    MipLevel m;m.width=w;m.height=h;m.rgba.resize(size_t(pixels)*4);
    for(size_t i=0;i<pixels;++i){auto p=file+off+i*4;m.rgba[i*4]=p[2];m.rgba[i*4+1]=p[1];m.rgba[i*4+2]=p[0];m.rgba[i*4+3]=p[3];}return m;
}
MipLevel DxtMip(const uint8_t* file,size_t size,uint32_t off,uint32_t bytes,uint32_t w,uint32_t h,Format format,uint8_t alphaDepth){
    const uint32_t blockBytes=format==Format::DXT1?8:16,bx=(w+3)/4,by=(h+3)/4;
    const uint64_t need=uint64_t(bx)*by*blockBytes;if(need!=bytes)throw std::runtime_error("DXT mip block size mismatch");
    if(off<Header||off>size||bytes>size-off)throw std::runtime_error("DXT mip outside file");
    MipLevel m;m.width=w;m.height=h;m.rgba.assign(size_t(w)*h*4,0);
    for(uint32_t yb=0;yb<by;++yb)for(uint32_t xb=0;xb<bx;++xb){const uint8_t* block=file+off+(size_t(yb)*bx+xb)*blockBytes;uint8_t alpha[16];std::fill(alpha,alpha+16,255);const uint8_t* color=block;
        if(format==Format::DXT3){for(int i=0;i<16;++i)alpha[i]=uint8_t(((block[i/2]>>((i&1)*4))&15)*17);color=block+8;}
        else if(format==Format::DXT5){uint8_t at[8]={block[0],block[1]};if(at[0]>at[1])for(int i=1;i<7;++i)at[i+1]=uint8_t(((7-i)*at[0]+i*at[1])/7);else{for(int i=1;i<5;++i)at[i+1]=uint8_t(((5-i)*at[0]+i*at[1])/5);at[6]=0;at[7]=255;}uint64_t bits=0;for(int i=0;i<6;++i)bits|=uint64_t(block[2+i])<<(i*8);for(int i=0;i<16;++i)alpha[i]=at[(bits>>(i*3))&7];color=block+8;}
        uint8_t c[4][4];Colors(color,format==Format::DXT1&&alphaDepth!=0,c);uint32_t ci=U32(color+4);
        for(uint32_t py=0;py<4;++py)for(uint32_t px=0;px<4;++px){uint32_t x=xb*4+px,y=yb*4+py;if(x>=w||y>=h)continue;uint32_t i=py*4+px,k=(ci>>(i*2))&3;uint8_t* d=m.rgba.data()+(size_t(y)*w+x)*4;d[0]=c[k][0];d[1]=c[k][1];d[2]=c[k][2];d[3]=(format==Format::DXT1?c[k][3]:alpha[i]);}}
    return m;
}
}
Result DecodeBLP2(const uint8_t* d,size_t n) {
    try {
        if(!d||n<Header||std::memcmp(d,"BLP2",4))throw std::runtime_error("Truncated or invalid BLP2 header");
        if(U32(d+4)!=1)throw std::runtime_error("Unsupported BLP2 content type");
        uint8_t enc=d[8],depth=d[9],alpha=d[10],mips=d[11];uint32_t w=U32(d+12),h=U32(d+16);
        if(!w||!h||w>8192||h>8192||uint64_t(w)*h>67108864)throw std::runtime_error("Impossible or over-limit BLP2 dimensions");
        if(depth!=0&&depth!=1&&depth!=4&&depth!=8)throw std::runtime_error("Unsupported alpha depth");
        Format fmt;
        if(enc==1){fmt=Format::PaletteRGBA8;if(alpha!=0&&alpha!=1&&alpha!=8)throw std::runtime_error("Unsupported palette alpha encoding");}
        else if(enc==2){if(alpha==0&&(depth==0||depth==1))fmt=Format::DXT1;else if(alpha==1&&(depth==4||depth==8))fmt=Format::DXT3;else if(alpha==7&&depth==8)fmt=Format::DXT5;else throw std::runtime_error("Unsupported DXT alpha combination");}
        else if(enc==3){fmt=Format::BGRA8;if(depth!=8)throw std::runtime_error("BGRA8 requires 8-bit alpha");}
        else throw std::runtime_error("Unsupported BLP2 encoding");
        DecodedTexture t;t.version=Version::BLP2;t.sourceFormat=fmt;t.encoding=enc;t.alphaDepth=depth;t.alphaEncoding=alpha;t.hasMipFlag=mips!=0;t.width=w;t.height=h;
        bool ended=false;uint64_t decoded=0;uint32_t mw=w,mh=h;std::array<std::pair<uint64_t,uint64_t>,16> ranges{};uint32_t rangeCount=0;
        for(uint32_t i=0;i<16;++i){uint32_t off=U32(d+20+i*4),bytes=U32(d+84+i*4);if((off==0)!=(bytes==0))throw std::runtime_error("Mip has only one of offset/size");if(!off){ended=true;continue;}if(ended)throw std::runtime_error("Sparse mip table");if(i>0&&!mips)throw std::runtime_error("Mip data present while hasMip flag is zero");
            uint32_t maxLevels=1;for(uint32_t tw=w,th=h;tw>1||th>1;tw=(std::max)(1u,tw/2),th=(std::max)(1u,th/2))++maxLevels;
            if(i>=maxLevels)throw std::runtime_error("Mip chain continues past 1x1");
            uint64_t begin=off,end=begin+bytes;if(end<begin||end>n)throw std::runtime_error("Mip range outside file");
            for(uint32_t q=0;q<rangeCount;++q)if(begin<ranges[q].second&&ranges[q].first<end)throw std::runtime_error("Overlapping mip ranges");ranges[rangeCount++]={begin,end};
            uint64_t outBytes=uint64_t(mw)*mh*4;if(outBytes>MaxDecoded-decoded)throw std::runtime_error("Decoded texture exceeds 256 MiB");decoded+=outBytes;
            if(enc==1)t.mipLevels.push_back(PaletteMip(d,n,off,bytes,mw,mh,depth));else if(enc==2)t.mipLevels.push_back(DxtMip(d,n,off,bytes,mw,mh,fmt,depth));else t.mipLevels.push_back(BgraMip(d,n,off,bytes,mw,mh));mw=(std::max)(1u,mw/2);mh=(std::max)(1u,mh/2);}
        if(t.mipLevels.empty())throw std::runtime_error("BLP2 has no mip data");
        Result r;r.ok=true;r.texture=std::move(t);return r;
    } catch(const std::exception& e){Result r;r.error=e.what();return r;} catch(...){Result r;r.error="Unknown BLP2 decoding failure";return r;}
}
}
