#include <Windows.h>
#include <chrono>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <vector>
namespace fs=std::filesystem;
struct P{const char*n;size_t o;std::vector<unsigned char>a,b;};
static const std::vector<P> ps={
{"loader_entry",0x244A17,{0xB9,0xCC,0x51,0xD7,0},{0xE9,0x65,0x88,0x0C,0}},
{"loader_cave_1",0x30D281,std::vector<unsigned char>(10,0xCC),{0x68,0,0xD3,0x88,0,0xE9,0xF6,0x3A,0,0}},
{"loader_cave_2",0x310D81,std::vector<unsigned char>(11,0xCC),{0xFF,0x15,0x74,0xA1,0x88,0,0xE9,0x55,4,0,0}},
{"loader_cave_3",0x3111E1,std::vector<unsigned char>(10,0xCC),{0xB9,0xCC,0x51,0xD7,0,0xE9,0x31,0x38,0xF3,0xFF}},
{"dll_name",0x48B900,{0x70,0x6C,0x61,0x79,0x65,0x72,0x20,0x68,0x69,0x64,0x64,0x65,0x6E,0,0,0,0x70,0x6C},{0x54,0x42,0x43,0x45,0x78,0x74,0x65,0x6E,0x73,0x69,0x6F,0x6E,0x73,0x2E,0x64,0x6C,0x6C,0}},
{"visible_ref",0x5DD0,{0xB8,0x10,0xD3,0x88,0},{0xB8,0x31,0x1F,0x71,0}},
{"hidden_ref",0x5DD7,{0xB8,0,0xD3,0x88,0},{0xB8,1,0x1E,0x71,0}},
{"hidden_copy",0x311201,std::vector<unsigned char>(14,0xCC),{0x70,0x6C,0x61,0x79,0x65,0x72,0x20,0x68,0x69,0x64,0x64,0x65,0x6E,0}},
{"visible_copy",0x311331,std::vector<unsigned char>(15,0xCC),{0x70,0x6C,0x61,0x79,0x65,0x72,0x20,0x76,0x69,0x73,0x69,0x62,0x6C,0x65,0}}
};
static std::vector<unsigned char>read(const fs::path&p){std::ifstream f(p,std::ios::binary);return{std::istreambuf_iterator<char>(f),{}};}
static std::string st(const std::vector<unsigned char>&d,const P&p){if(p.o+p.a.size()>d.size())return"UNKNOWN";std::vector<unsigned char>x(d.begin()+p.o,d.begin()+p.o+p.a.size());return x==p.a?"NOT_APPLIED":x==p.b?"ALREADY_APPLIED":"CONFLICT";}
static std::string hexbytes(const std::vector<unsigned char>&v){std::ostringstream s;s<<std::hex<<std::setfill('0');for(auto b:v)s<<std::setw(2)<<(unsigned)b;return s.str();}
static std::string jsonstr(std::string v){std::string r;for(char c:v){if(c=='\\'||c=='\"')r+='\\';r+=c;}return r;}
static bool build(const fs::path&p){DWORD z=0,n=GetFileVersionInfoSizeW(p.c_str(),&z);std::vector<unsigned char>b(n);VS_FIXEDFILEINFO*i=0;UINT l=0;return n&&GetFileVersionInfoW(p.c_str(),0,n,b.data())&&VerQueryValueW(b.data(),L"\\",(void**)&i,&l)&&HIWORD(i->dwFileVersionMS)==2&&LOWORD(i->dwFileVersionMS)==4&&HIWORD(i->dwFileVersionLS)==3&&LOWORD(i->dwFileVersionLS)==8606;}
static bool x86(const std::vector<unsigned char>&d){if(d.size()<256)return false;auto o=*(uint32_t*)&d[0x3c];return o+6<d.size()&&*(uint32_t*)&d[o]==0x4550&&*(uint16_t*)&d[o+4]==0x14c;}
static void report(const fs::path&p,const std::vector<unsigned char>&d,bool b,bool x){std::ofstream j("patch_report.json"),t("patch_report.txt");j<<"{\n\"file\":\""<<jsonstr(p.string())<<"\",\"build_8606\":"<<(b?"true":"false")<<",\"x86\":"<<(x?"true":"false")<<",\"supported_states\":[\"NOT_APPLIED\",\"ALREADY_APPLIED\",\"COMPATIBLE\",\"CONFLICT\",\"UNKNOWN\"],\"patches\":[\n";t<<"Target: "<<p<<"\nBuild 8606: "<<b<<"\nPE x86: "<<x<<"\n";for(size_t i=0;i<ps.size();i++){auto s=st(d,ps[i]);j<<"{\"name\":\""<<ps[i].n<<"\",\"purpose\":\"Load TBCExtensions.dll without replacing other custom patches\",\"file_offset\":\"0x"<<std::hex<<ps[i].o<<"\",\"expected_bytes\":\""<<hexbytes(ps[i].a)<<"\",\"replacement_bytes\":\""<<hexbytes(ps[i].b)<<"\",\"rollback_bytes\":\""<<hexbytes(ps[i].a)<<"\",\"state\":\""<<s<<"\",\"confidence\":\"HIGH\",\"source\":\"legacy TBC loader; byte-checked against build 8606\"}"<<(i+1<ps.size()?",":"")<<"\n";t<<ps[i].n<<": "<<s<<"\n  expected/rollback: "<<hexbytes(ps[i].a)<<"\n  replacement:      "<<hexbytes(ps[i].b)<<"\n";}j<<"]}\n";}
static std::string stamp(){auto q=std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());tm m{};localtime_s(&m,&q);std::ostringstream s;s<<std::put_time(&m,"%Y%m%d_%H%M%S");return s.str();}
int main(int c,char**v){if(c<3){std::cerr<<"TBCPatcher --analyze|--apply Wow.exe | --restore backup [target]\n";return 2;}fs::path p=v[2];std::string mode=v[1];if(mode=="--restore"){auto marker=p.string().find(".backup_");if(c<=3&&marker==std::string::npos){std::cerr<<"Backup name has no .backup_ marker; provide an explicit target.\n";return 2;}auto q=c>3?fs::path(v[3]):fs::path(p.string().substr(0,marker));if(fs::absolute(p).lexically_normal()==fs::absolute(q).lexically_normal()){std::cerr<<"Backup and restore target must differ.\n";return 2;}fs::copy_file(p,q,fs::copy_options::overwrite_existing);std::cout<<"Restored "<<q<<"\n";return 0;}auto d=read(p);bool b=build(p),x=x86(d);report(p,d,b,x);size_t pending=0,applied=0,conflicts=0,unknown=0;for(auto&a:ps){auto s=st(d,a);pending+=s=="NOT_APPLIED";applied+=s=="ALREADY_APPLIED";conflicts+=s=="CONFLICT";unknown+=s=="UNKNOWN";}std::cout<<"Build 8606: "<<(b?"yes":"no")<<"; PE x86: "<<(x?"yes":"no")<<"\nNOT_APPLIED: "<<pending<<"; ALREADY_APPLIED: "<<applied<<"; CONFLICT: "<<conflicts<<"; UNKNOWN: "<<unknown<<"\n";if(mode=="--analyze"){std::cout<<"Dry run complete. Zero changes. See patch_report.txt/json.\n";return b&&x?0:1;}if(mode!="--apply"||!b||!x)return 1;if(conflicts||unknown){std::cerr<<"Refusing because at least one patch location is unsafe.\n";return 1;}if(pending==0){std::cout<<"Loader is already fully applied; no file changed and no new backup was created.\n";return 0;}auto backup=fs::path(p.string()+".backup_"+stamp());fs::copy_file(p,backup);for(auto&a:ps)if(st(d,a)=="NOT_APPLIED")std::copy(a.b.begin(),a.b.end(),d.begin()+a.o);std::ofstream f(p,std::ios::binary|std::ios::trunc);f.write((char*)d.data(),d.size());f.close();report(p,d,b,x);std::cout<<"Applied "<<pending<<" patch regions. Backup: "<<backup<<"\n";}
