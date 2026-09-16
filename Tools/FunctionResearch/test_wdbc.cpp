#include "CDBCMgr/WdbcTable.hpp"
#include <iostream>
#include <functional>
#include "CDBCMgr/NativeDBC.hpp"
using NativeDBC::Table;
void put(std::vector<unsigned char>& b,size_t p,uint32_t n) { for(int i=0;i<4;++i)b[p+i]=static_cast<unsigned char>(n>>(8*i)); }
std::vector<unsigned char> good() {
    std::vector<unsigned char>b(50,0);std::memcpy(b.data(),"WDBC",4);
    put(b,4,2);put(b,8,3);put(b,12,12);put(b,16,6);
    put(b,20,1);put(b,24,1);put(b,28,0x3fe00000);
    put(b,32,2);put(b,36,1);put(b,40,0x40200000);
    std::memcpy(b.data()+44,"\0test\0",6);return b;
}
void require(bool ok) { if(!ok)throw std::runtime_error("Test failed"); }
void rejects(const std::function<void()>& fn) { bool caught=false;try{fn();}catch(const std::exception&){caught=true;}require(caught); }
int main() {
    Table t;t.name="AoR_CreatureModelData.dbc";t.Parse(good());
    require(t.rows==2 && t.Cell(2,1)==2 && t.String(1,2)=="test" && t.Float(1,3)==1.75f);
    rejects([&]{t.Cell(99,1);});rejects([&]{t.Cell(1,0);});rejects([&]{t.Cell(1,4);});
    for(int mutation=0;mutation<11;++mutation) {
        auto b=good();
        switch(mutation) {
        case 0:b[0]='X';break;
        case 1:b.pop_back();break;
        case 2:b.push_back(0);break;
        case 3:put(b,12,16);break;
        case 4:put(b,4,0xffffffff);break;
        case 5:put(b,32,1);break;
        case 6:put(b,24,6);break;
        case 7:put(b,28,0x7f800000);break;
        case 8:b.back()='x';break;
        case 9:b[44]='x';break;
        case 10:put(b,8,257);break;
        }
        rejects([&]{Table x;x.name="AoR_CreatureModelData.dbc";x.Parse(b);});
    }
    rejects([&]{Table x;x.name="AoR_CreatureDisplayInfo.dbc";x.Parse(good());});
    std::cout<<"WDBC parser: valid typed lookup + 15 negative cases passed\n";
    // The demo fixture is installed beside this test executable by the test command.
    NativeDBC::Request("load","TBCExt_Demo.dbc",0,0,"");
    require(NativeDBC::Request("field","",1,2,"string")=="Native DBC loaded from disk");
    require(NativeDBC::Request("field","",1,3,"float")=="1.75");
    rejects([]{NativeDBC::Request("load","../TBCExt_Demo.dbc",0,0,"");});
    rejects([]{NativeDBC::Request("load","TBCExt_Missing.dbc",0,0,"");});
    rejects([]{NativeDBC::Request("load","TBCExt_Bad.dbc",0,0,"");});
    require(NativeDBC::Request("row","",1,0,"").find("TBCExt_Demo.dbc")!=std::string::npos);
    rejects([]{NativeDBC::Request("field","",1,3,"pointer");});
    NativeDBC::Request("unload","",0,0,"");
    rejects([]{NativeDBC::Request("row","",1,0,"");});
    std::cout<<"Native file service: load/typed queries, failed-load preservation, path rejection and unload passed\n";
}
