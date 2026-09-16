#include "BLPInspector.hpp"
#include "BLPReader.hpp"
#include <Windows.h>
#include <vector>
#include <stdexcept>

namespace Blp {
namespace {
struct File { HANDLE h=INVALID_HANDLE_VALUE;~File(){if(h!=INVALID_HANDLE_VALUE)CloseHandle(h);} };
bool Valid(const std::string& s){
    if(s.size()<12||s.size()>96||_stricmp(s.c_str()+s.size()-4,".blp"))return false;
    if(_strnicmp(s.c_str(),"TBCExt_",7))return false;
    for(size_t i=0;i<s.size()-4;++i){char c=s[i];if(!(c>='A'&&c<='Z')&&!(c>='a'&&c<='z')&&!(c>='0'&&c<='9')&&c!='_')return false;}return true;
}
std::wstring Folder(){wchar_t exe[32768]={};DWORD n=GetModuleFileNameW(nullptr,exe,32768);if(!n||n>=32768)throw std::runtime_error("Cannot locate client folder");std::wstring p(exe,n);return p.substr(0,p.find_last_of(L"\\/")+1)+L"TBCExtensions\\Textures\\";}
}
std::string InspectLooseFile(const std::string& basename){
    if(!Valid(basename))throw std::runtime_error("Use a TBCExt_ basename ending .blp; paths are not accepted");
    std::wstring path=Folder()+std::wstring(basename.begin(),basename.end());File f;f.h=CreateFileW(path.c_str(),GENERIC_READ,FILE_SHARE_READ,nullptr,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,nullptr);
    if(f.h==INVALID_HANDLE_VALUE)throw std::runtime_error("Cannot open TBCExtensions\\Textures file (Windows error "+std::to_string(GetLastError())+")");
    LARGE_INTEGER n{};if(!GetFileSizeEx(f.h,&n)||n.QuadPart<4||n.QuadPart>128ll*1024*1024)throw std::runtime_error("BLP must be 4 bytes to 128 MiB");
    std::vector<uint8_t>b(static_cast<size_t>(n.QuadPart));DWORD read=0;if(!ReadFile(f.h,b.data(),static_cast<DWORD>(b.size()),&read,nullptr)||read!=b.size())throw std::runtime_error("Incomplete BLP read");
    return Describe(Read(b.data(),b.size()))+"\nStock TBC result: not determined by offline inspection\nRuntime fallback: DISABLED";
}
}
