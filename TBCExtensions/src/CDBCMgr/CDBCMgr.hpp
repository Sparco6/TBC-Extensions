#pragma once

class CDBCMgr
{
public:
    static void Load();
    static void PatchAddress();

private:
    CDBCMgr() = delete;
    ~CDBCMgr() = delete;
};
