#include "DB_NMrpr.h"
#include "unDefDBconnect.h"
#include "DB_Pokaztl.h"
#include "../SystFuncs/SystFuncsLogic.h"

std::string TDB_NMrpr::GetCurDBNameFromDB() {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    std::string dbname;
    switch (GetDBType()) {
        case TDBType::MSSQL:
            q->SQL->Add("SELECT convert(char, DB_NAME()) as dbname");
            break;
        case TDBType::Postgre:
            q->SQL->Add("SELECT current_database() as dbname");
            break;
    }
    q->Open();
    dbname = q->FieldByName("dbname")->AsString();
    dbname = dbname.erase(dbname.find_last_not_of(" \n\r\t")+1);
    delete q;
    return dbname;
}

void TDB_NMrpr::SetCurDBName(std::string aCurDBName) {
    CurDBName = aCurDBName;
}

std::string TDB_NMrpr::GetCurDBName() {
    return CurDBName;
}

std::string TDB_NMrpr::PokazidToNMP(int pokaz_id, int God, int& NMP) {
    std::string CurDBName = GetCurDBName();
    NMP = 0;
    std::string Result = "";

    if (God < 2008)
        goto LL;

    // In C++, you can't use goto across variable initialization
    // So, we need to use a boolean flag or use a separate function
    if (TDB_Pokaztl::GetInCast(pokaz_id) == 10)
        return Result;

LL: // Label LL
    for (int i = 0; i < KolElemNMRPR; i++) {
        if (tblDBGodN0[i].DBName == " ")
            break;
        if (tblDBGodN0[i].DBName == CurDBName && tblDBGodN0[i].God == God) {
            NMP = pokaz_id - tblDBGodN0[i].N0;
            if (NMP > 0) {
                // Convert NMP to string
                Result = std::to_string(NMP);
            }
            return Result;
        }
    }

    return Result;
}

int TDB_NMrpr::NMPToPokazid(int NMP, int God) {
    std::string CurDBName = GetCurDBName();
    int Result = 0;

    for (int i = 0; i < KolElemNMRPR; i++) {
        if (tblDBGodN0[i].DBName == " ")
            break;
        if (tblDBGodN0[i].DBName == CurDBName && tblDBGodN0[i].God == God) {
            Result = NMP + tblDBGodN0[i].N0;
            break;
        }
    }

    return Result;
}

