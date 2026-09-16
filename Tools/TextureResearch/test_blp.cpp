#include "Graphics/Textures/BLPReader.hpp"
#include <algorithm>
#include <cstring>
#include <fstream>
#include <iostream>
#include <random>
#include <vector>
using namespace Blp;
void w32(std::vector<uint8_t>& b,size_t p,uint32_t v){for(int i=0;i<4;++i)b[p+i]=uint8_t(v>>(8*i));}
void need(bool v,const char* why){if(!v)throw std::runtime_error(why);}
void bad(const std::vector<uint8_t>& b){need(!Read(b.data(),b.size()).ok,"malformed accepted");}
std::vector<uint8_t> base(uint8_t enc,uint8_t depth,uint8_t alpha,uint32_t bytes){
 const size_t start=enc==1?1172:148;std::vector<uint8_t>b(start+bytes);std::memcpy(b.data(),"BLP2",4);
 w32(b,4,1);b[8]=enc;b[9]=depth;b[10]=alpha;w32(b,12,4);w32(b,16,4);w32(b,20,uint32_t(start));w32(b,84,bytes);return b;
}
std::vector<uint8_t> file(const char* path){std::ifstream f(path,std::ios::binary);need(bool(f),"sample open");return {std::istreambuf_iterator<char>(f),{}};}
int main(int argc,char** argv){
 auto d1=base(2,0,0,8);d1[149]=0xf8;d1[151]=0xf8;
 auto r=Read(d1.data(),d1.size());need(r.ok&&r.texture.sourceFormat==Format::DXT1&&r.texture.mipLevels[0].rgba.size()==64,"DXT1");
 auto d3=base(2,4,1,16);std::fill(d3.begin()+148,d3.begin()+156,0xff);d3[156]=0xe0;d3[157]=7;d3[158]=0xe0;d3[159]=7;
 r=Read(d3.data(),d3.size());need(r.ok&&r.texture.sourceFormat==Format::DXT3&&r.texture.mipLevels[0].rgba[3]==255,"DXT3");
 auto d5=base(2,8,7,16);d5[148]=255;d5[156]=31;d5[158]=31;
 r=Read(d5.data(),d5.size());need(r.ok&&r.texture.sourceFormat==Format::DXT5&&r.texture.mipLevels[0].rgba[3]==255,"DXT5");
 auto pal=base(1,8,8,32);pal[150]=255;std::fill(pal.begin()+1188,pal.end(),127);
 r=Read(pal.data(),pal.size());need(r.ok&&r.texture.sourceFormat==Format::PaletteRGBA8&&r.texture.mipLevels[0].rgba[0]==255&&r.texture.mipLevels[0].rgba[3]==127,"palette");
 auto bgra=base(3,8,0,64);for(size_t i=148;i<bgra.size();i+=4){bgra[i]=3;bgra[i+1]=2;bgra[i+2]=1;bgra[i+3]=4;}
 r=Read(bgra.data(),bgra.size());need(r.ok&&r.texture.sourceFormat==Format::BGRA8&&r.texture.mipLevels[0].rgba[0]==1&&r.texture.mipLevels[0].rgba[3]==4,"BGRA");
 std::vector<uint8_t>b1(157);std::memcpy(b1.data(),"BLP1",4);w32(b1,4,1);w32(b1,12,4);w32(b1,16,4);w32(b1,28,156);w32(b1,92,1);
 r=Read(b1.data(),b1.size());need(r.ok&&r.texture.version==Version::BLP1&&r.texture.mipLevels.empty(),"BLP1 inspect");
 {auto b=d1;b[0]='X';bad(b);}{auto b=d1;b.resize(147);bad(b);}{auto b=d1;w32(b,12,0);bad(b);}{auto b=d1;b[8]=9;bad(b);}
 {auto b=d1;b[9]=8;bad(b);}{auto b=d1;w32(b,84,7);bad(b);}{auto b=d1;w32(b,20,0xffffffff);bad(b);}
 {auto b=d1;b[11]=1;w32(b,24,148);w32(b,88,8);bad(b);}{auto b=d1;w32(b,24,156);w32(b,88,8);bad(b);}
 std::mt19937 rng(8606);for(int i=0;i<5000;++i){auto b=d5;for(int k=0;k<1+int(rng()%5);++k)b[rng()%b.size()]=uint8_t(rng());auto x=Read(b.data(),b.size());if(x.ok)need(!x.texture.mipLevels.empty()&&x.texture.mipLevels[0].rgba.size()<=256ull*1024*1024,"fuzz invariant");}
 if(argc==6){auto a=file(argv[1]),b=file(argv[2]),c=file(argv[3]),d=file(argv[4]),e=file(argv[5]);auto ra=Read(a.data(),a.size()),rb=Read(b.data(),b.size()),rc=Read(c.data(),c.size()),rd=Read(d.data(),d.size()),re=Read(e.data(),e.size());need(ra.ok&&ra.texture.sourceFormat==Format::BGRA8,"generated BGRA sample");need(rb.ok&&rb.texture.sourceFormat==Format::PaletteRGBA8,"generated palette sample");need(rc.ok&&rc.texture.sourceFormat==Format::DXT1,"generated DXT1 sample");need(rd.ok&&rd.texture.sourceFormat==Format::DXT3,"generated DXT3 sample");need(re.ok&&re.texture.sourceFormat==Format::DXT5,"generated DXT5 sample");}
 std::cout<<"BLP parser: BLP1 inspect; BLP2 palette/DXT1/DXT3/DXT5/BGRA; malformed and 5000 mutation cases passed\n";
}
