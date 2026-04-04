#pragma once

class CDBCMgr
{
public:
    static void Load();
    static void PatchAddress();
    static int  RegisterDBCEx();

private:
    CDBCMgr() = delete;
    ~CDBCMgr() = delete;
};
