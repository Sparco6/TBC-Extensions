#include "NativeDBC.hpp"
#include "WdbcTable.hpp"
#include <Windows.h>
#include <memory>
#include <mutex>
#include <sstream>
#include <algorithm>

namespace NativeDBC {
namespace {
std::mutex gate;
std::unique_ptr<Table> active;
std::wstring source;
struct File {
    HANDLE h = INVALID_HANDLE_VALUE;
    ~File() { if (h != INVALID_HANDLE_VALUE) CloseHandle(h); }
};
std::wstring Folder() {
    wchar_t exe[32768] = {};
    DWORD n = GetModuleFileNameW(nullptr,exe,32768);
    if (!n || n >= 32768) throw std::runtime_error("Cannot locate client folder");
    std::wstring path(exe,n);
    return path.substr(0,path.find_last_of(L"\\/")+1)+L"TBCExtensions\\DBFilesClient\\";
}
std::string Utf8(const std::wstring& s) {
    int n=WideCharToMultiByte(CP_UTF8,0,s.data(),int(s.size()),nullptr,0,nullptr,nullptr);
    std::string out(n,'\0');
    if (n) WideCharToMultiByte(CP_UTF8,0,s.data(),int(s.size()),&out[0],n,nullptr,nullptr);
    return out;
}
bool ValidName(const std::string& s) {
    if (s.size()<5 || s.size()>96 || s.substr(s.size()-4)!=".dbc") return false;
    if (s.rfind("AoR_",0)!=0 && s.rfind("TBCExt_",0)!=0) return false;
    for (size_t i=0;i<s.size()-4;++i) {
        char c=s[i];
        if (!(c>='a'&&c<='z') && !(c>='A'&&c<='Z') && !(c>='0'&&c<='9') && c!='_') return false;
    }
    return true;
}
std::string Info() {
    if (!active) return "No native custom DBC loaded. Folder: "+Utf8(Folder());
    return "DLL-owned WDBC: "+active->name+"\nSource: "+Utf8(source)+
        "\nRows: "+std::to_string(active->rows)+"; fields: "+std::to_string(active->fields)+
        "; bytes: "+std::to_string(active->data.size())+
        "\nFirst field is the ID. Columns start at 1. Loaded on demand; survives UI reload.\nStock DBCs unchanged. Retail rendering/morph integration NOT enabled.";
}
}
std::string Request(const std::string& action, const std::string& filename,
    uint32_t id, uint32_t column, const std::string& type) {
    std::lock_guard<std::mutex> lock(gate);
    if (action=="info") return Info();
    if (action=="unload") { active.reset(); source.clear(); return "Custom DBC unloaded; stock DBCs unchanged"; }
    if (action=="load") {
        if (!ValidName(filename)) throw std::runtime_error("Use an AoR_ or TBCExt_ basename ending .dbc; paths are not accepted");
        const std::wstring path=Folder()+std::wstring(filename.begin(),filename.end());
        File file;
        file.h=CreateFileW(path.c_str(),GENERIC_READ,FILE_SHARE_READ,nullptr,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,nullptr);
        if (file.h==INVALID_HANDLE_VALUE) throw std::runtime_error("Cannot open custom DBC (Windows error "+std::to_string(GetLastError())+"): "+Utf8(path));
        LARGE_INTEGER size{};
        if (!GetFileSizeEx(file.h,&size) || size.QuadPart<20 || size.QuadPart>Table::MaxBytes)
            throw std::runtime_error("File must be 20 bytes to 64 MiB");
        std::vector<unsigned char> bytes(size_t(size.QuadPart)); DWORD read=0;
        if (!ReadFile(file.h,bytes.data(),DWORD(bytes.size()),&read,nullptr) || read!=bytes.size())
            throw std::runtime_error("Incomplete DBC read");
        auto next=std::make_unique<Table>(); next->name=filename; next->Parse(std::move(bytes));
        // Commit only after complete validation. Failed loads preserve the active table.
        active=std::move(next); source=path;
        return Info();
    }
    if (!active) throw std::runtime_error("Load a custom DBC first");
    if (action=="field") {
        if (type=="string") return active->String(id,column);
        if (type=="float") { std::ostringstream s; s<<active->Float(id,column); return s.str(); }
        if (type=="uint32") return std::to_string(active->Cell(id,column));
        throw std::runtime_error("Field type must be uint32, float or string");
    }
    if (action=="row") {
        std::ostringstream out; out<<active->name<<" | ID "<<id<<"\n";
        for (uint32_t c=1;c<=(std::min)(active->fields,32u);++c)
            out<<"f"<<c<<"="<<active->Cell(id,c)<<" ";
        if (active->fields>32) out<<"[first 32 fields only]";
        if (active->name=="AoR_CreatureModelData.dbc")
            out<<"\nAoR schema: ModelPath="<<active->String(id,2)<<"\nMountHeight="<<active->Float(id,3);
        if (active->name=="AoR_CreatureDisplayInfo.dbc") {
            out<<"\nAoR schema: ModelID="<<active->Cell(id,2)<<"\nModelPath="<<active->String(id,3);
            for (uint32_t c=4;c<=7;++c) out<<"\nTexture"<<c-3<<"="<<active->String(id,c);
        }
        return out.str();
    }
    throw std::runtime_error("Action must be load, info, row, field or unload");
}
}
