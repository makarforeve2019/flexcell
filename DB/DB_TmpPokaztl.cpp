#include "DB_TmpPokaztl.h"
#include "unDefDBconnect.h" 
#include "DB_Pokaztl.h" 
#include "DB_bkKBK.h"
#include "../GlobalVars/dmGlobalVars.h"
#include "../SystFuncs/SystFuncsLogic.h"

void TDB_TmpPokaztl::CreateTmpPokaztl(int CurrYear, int root_Pokaz_id, int plan_id) {
    TDateTime ToDate;
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    q->SQL->Clear();
    
    switch (GetDBType()) {
        case TDBType::MSSQL:
            q->SQL->Add("CREATE TABLE #TmpPokaztl (pokaz_id int, pokaz_is_razd tinyint NULL, pokaz_owner_id int NULL, ");
            q->SQL->Add(" pokaz_vert tinyint NULL, pokaz_type tinyint NULL, is_obrab tinyint NULL)");
            q->ExecSQL();
            q->Close();
            q->SQL->Clear();
            q->SQL->Add("CREATE INDEX IP1 ON #TmpPokaztl (pokaz_id)");
            q->ExecSQL();
            q->Close();
            q->SQL->Clear();
            break;
        case TDBType::Postgre:
            q->SQL->Add("CREATE TEMP TABLE TmpPokaztl (pokaz_id integer, pokaz_is_razd smallint NULL, pokaz_owner_id integer NULL, ");
            q->SQL->Add(" pokaz_vert smallint NULL, pokaz_type smallint NULL, is_obrab smallint NULL);");
            q->ExecSQL();
            q->Close();
            q->SQL->Clear();
            q->SQL->Add("CREATE INDEX IP1 ON TmpPokaztl (pokaz_id)");
            q->ExecSQL();
            q->Close();
            q->SQL->Clear();
            break;
    }
    
   //MAK q->Free();
delete q;
    
    ToDate = EncodeDate(CurrYear, 12, 30);
    TDB_TmpPokaztl::CreateTmpTblPokaztl(root_Pokaz_id, plan_id, ToDate);
}

void TDB_TmpPokaztl::DropTmpPokaztl() {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    q->SQL->Clear();
    
    switch (GetDBType()) {
        case TDBType::MSSQL:
            q->SQL->Add("if object_id('tempdb..#TmpPokaztl') is not null");
            q->SQL->Add("DROP TABLE #TmpPokaztl");
            break;
        case TDBType::Postgre:
            q->SQL->Add("DROP TABLE IF EXISTS TmpPokaztl");
            break;
    }
    q->ExecSQL();
   //MAK q->Free();
delete q;
}

void TDB_TmpPokaztl::CreateTmpTblPokaztl(int aPokaz_id, int aPlan_id, TDateTime aToDate) {
    TFDStoredProc *sp = new TFDStoredProc(DefDBconnect.Conn);
    sp->storedProcName = "CreateTmpTblPokaztl";

    switch (GetDBType()) {
        case TDBType::MSSQL: {
            sp->Params->CreateParam(ftInteger, "@aPokaz_id", ptInput);
            sp->Params->CreateParam(ftInteger, "@aPlan_id", ptInput);
            sp->Params->CreateParam(ftDateTime, "@aToDate", ptInput);
            break;
        }
        case TDBType::Postgre: {
            sp->Params->CreateParam(ftInteger, "aPokaz_id", ptInput);
            sp->Params->CreateParam(ftInteger, "aPlan_id", ptInput);
            sp->Params->CreateParam(ftDateTime, "aToDate", ptInput);
            break;
        }
    }

    sp->ParamByName("@aPokaz_id")->AsInteger() = aPokaz_id;
    sp->ParamByName("@aPlan_id")->AsInteger() = aPlan_id;
    sp->ParamByName("@aToDate")->AsDateTime() = aToDate;

    sp->ExecProc();
    sp->Free();
}

void TDB_TmpPokaztl::CreateTmpByKOSGU(int isFilter, std::string aTblName, int CurrYear, int root_Pokaz_id) {
    TDateTime ToDate;
    int Index;

    //nameCreateTmpByKOSGU.sorted = true;

    if (nameCreateTmpByKOSGU.IndexOfName(aTblName) != -1) return;
    nameCreateTmpByKOSGU.Add(aTblName);

    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    q->SQL->Clear();

    switch (GetDBType()) {
        case TDBType::MSSQL: {
            q->SQL->Add("CREATE TABLE #PTB (pokaz_id int, pokaz_is_razd tinyint NULL, pokaz_owner_id int NULL,");
            q->SQL->Add(" pokaz_vert tinyint NULL, pokaz_type tinyint NULL, pokaz_bk_code varchar(30) NULL, ");
            q->SQL->Add(" rd varchar(10) NULL, cs varchar(10) NULL, vr varchar(10) NULL, ku varchar(10) NULL, ");
            q->SQL->Add(" is_obrab tinyint NULL, kusub varchar(10) NULL)");
            break;
        }
        case TDBType::Postgre: {
            q->SQL->Add("CREATE TEMP TABLE PTB (pokaz_id integer, pokaz_is_razd smallint NULL, pokaz_owner_id integer NULL,");
            q->SQL->Add(" pokaz_vert smallint NULL, pokaz_type smallint NULL, pokaz_bk_code character varying(30) NULL, ");
            q->SQL->Add(" rd character varying(10) NULL, cs character varying(10) NULL, vr character varying(10) NULL, ");
            q->SQL->Add(" ku character varying(10) NULL, is_obrab smallint NULL, kusub character varying(10) NULL);");
            break;
        }
    }
    q->ExecSQL();
    q->SQL->Clear();
    ToDate = EncodeDate(CurrYear, 12, 30);

    switch (isFilter) {
        case 1: 
            TDB_TmpPokaztl::CreateTmpPTB(root_Pokaz_id, ToDate); 
            break;
        case 14: 
            TDB_TmpPokaztl::CreateTmpPTBany(root_Pokaz_id, ToDate); 
            break;
    }

    q->SQL->Add("CREATE TABLE " + aTblName + " (pokaz_id int, pokaz_is_razd tinyint NULL, pokaz_owner_id int NULL,");
    q->SQL->Add(" pokaz_vert tinyint NULL, pokaz_type tinyint NULL, pokaz_bk_code varchar(30) NULL, ");
    q->SQL->Add(" rd varchar(10) NULL, cs varchar(10) NULL, vr varchar(10) NULL, ku varchar(10) NULL, ");
    q->SQL->Add(" is_obrab tinyint NULL, kusub varchar(10) NULL)");

    q->ExecSQL();
    q->SQL->Clear();
    q->SQL->Add("INSERT INTO " + aTblName);
    switch (GetDBType()) {
        case TDBType::MSSQL:
            q->SQL->Add("SELECT * FROM #PTB");
            break;
        case TDBType::Postgre:
            q->SQL->Add("SELECT * FROM PTB");
            break;
    }
    q->ExecSQL();

    q->SQL->Clear();
    q->SQL->Add("DELETE FROM " + aTblName);
    q->SQL->Add("WHERE ku = :ku0");
    q->ParamByName("ku0")->AsString() = "";
    q->ExecSQL();

    q->SQL->Clear();
    q->SQL->Add("UPDATE " + aTblName);
    q->SQL->Add("SET kusub = :kusub");
    q->SQL->Add("WHERE EXISTS");
    switch (GetDBType()) {
        case TDBType::MSSQL:
            q->SQL->Add("(SELECT * FROM #PTB WHERE #PTB.pokaz_owner_id = " + aTblName + ".pokaz_id)");
            break;
        case TDBType::Postgre:
            q->SQL->Add("(SELECT * FROM PTB WHERE PTB.pokaz_owner_id = " + aTblName + ".pokaz_id)");
            break;
    }
    q->ParamByName("kusub")->AsString() = "001$";
    q->ExecSQL();

    q->SQL->Clear();
    q->SQL->Add("DELETE FROM " + aTblName);
    q->SQL->Add("WHERE kusub = :kusub");
    q->ParamByName("kusub")->AsString() = "001$";
    q->ExecSQL();

    q->SQL->Clear();
    switch (GetDBType()) {
        case TDBType::MSSQL:
            q->SQL->Add("DROP TABLE #PTB");
            break;
        case TDBType::Postgre:
            q->SQL->Add("DROP TABLE IF EXISTS PTB");
            break;
    }
    q->ExecSQL();

    q->SQL->Clear();
    q->SQL->Add("CREATE INDEX IP1_" + aTblName + " ON " + aTblName + " (rd, cs, vr, ku)");
    q->ExecSQL();
   //MAK q->Free();
delete q;
}

void TDB_TmpPokaztl::addPrgrm(std::string aTblName, std::string shablon, int god, int isOpen) {
    std::string codePrgrm, codeKBK, wshablon;
    int kbk_id;
    TDB_TmpPokaztl::addCelStatj(aTblName, shablon, god, 18);
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    if (god < 2016)
        codePrgrm = shablon + "00000";
    else
        codePrgrm = shablon + "00000000";
    kbk_id = TDB_bkKBK::GetKBKidByCodeKBK(codePrgrm, god);
    TDB_bkKBK::GetSubAllByGod(q, kbk_id, god);
    while (!q->EoF()) {
        codeKBK = q->FieldByName("codeKBK")->AsString();
        wshablon = codeKBK.substr(0, 3);
        TDB_TmpPokaztl::addSubPrgrm(aTblName, wshablon, god, isOpen);
        q->Next();
    }
}

void TDB_TmpPokaztl::addSubPrgrm(std::string aTblName, std::string shablon, int god, int isOpen) {
    std::string codeSubPrgrm, codeBlok, codeKBK, wshablon;
    int kbk_id;
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    TQueryUni *q1 = new TQueryUni(DefDBconnect.Conn);
    switch (isOpen) {
        case 1: {
            TDB_TmpPokaztl::addCelStatj(aTblName, shablon, god, 19); 
            if (god < 2016)
                codeSubPrgrm = shablon + "0000";
            else
                codeSubPrgrm = shablon + "0000000";
            kbk_id = TDB_bkKBK::GetKBKidByCodeKBK(codeSubPrgrm, god);
            TDB_bkKBK::GetSubAllByGod(q, kbk_id, god);
            while (!q->EoF()) {
                codeKBK = q->FieldByName("codeKBK")->AsString();
                if (codeKBK.substr(5, 2) == "00") {
                    TDB_TmpPokaztl::addCelStatj(aTblName, codeKBK, god, 20);
                    kbk_id = TDB_bkKBK::GetKBKidByCodeKBK(codeKBK, god);
                    TDB_bkKBK::GetSubAllByGod(q1, kbk_id, god);
                    while (!q1->EoF()) {
                        TDB_TmpPokaztl::addCelStatj(aTblName, q1->FieldByName("codeKBK")->AsString(), god, 21);
                        q1->Next();
                    }
                } else {
                    TDB_TmpPokaztl::addCelStatj(aTblName, codeKBK, god, 21);
                }
                q->Next();
            }
            break;
        }
        default: {
            TDB_TmpPokaztl::addCelStatj(aTblName, shablon, god, 19);
            break;
        }
    }
}

void TDB_TmpPokaztl::addCelStatj(std::string aTblName, std::string shablon, int god, int pokaz_is_razd) {
    std::string codePrgrm;
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);


    switch (pokaz_is_razd) {
        case 18:
            codePrgrm = (god < 2016) ? shablon + "00000" : shablon + "00000000";
            break;
        case 19:
            codePrgrm = (god < 2016) ? shablon + "0000" : shablon + "0000000";
            break;
        case 20:
        case 21:
            codePrgrm = shablon;
            break;
    }

    q->SQL->Clear();
    q->SQL->Add("INSERT INTO " + aTblName + " (rd, cs, vr, ku, pokaz_bk_code, pokaz_is_razd, pokaz_id) ");
    q->SQL->Add("VALUES (:rd, :cs, :vr, :ku, :pokaz_bk_code, :pokaz_is_razd, :pokaz_id)");
    q->ParamByName("rd")->AsString() = "";
    q->ParamByName("cs")->AsString() = codePrgrm;
    q->ParamByName("vr")->AsString() = "";
    q->ParamByName("ku")->AsString() = "";
    q->ParamByName("pokaz_bk_code")->AsString() = codePrgrm;
    q->ParamByName("pokaz_is_razd")->AsInteger() = pokaz_is_razd;
    q->ParamByName("pokaz_id")->AsInteger() = TDB_bkKBK::GetKBKidByCodeKBK(codePrgrm, god);
    q->ExecSQL();

   //MAK q->Free();
delete q;
}

void TDB_TmpPokaztl::CreateTmpOrderCP(std::string aTblName, int CurrYear, int root_Pokaz_id) {
    TDateTime ToDate;
        TQueryUni *q = new TQueryUni(DefDBconnect.Conn);

    if (GlobalVars->slCreateTmpOrderCPNames->IndexOf(aTblName) >= 0)
        return;

    GlobalVars->isCreateTmpOrderCP = true;
    GlobalVars->slCreateTmpOrderCPNames->Add(aTblName);

    q->SQL->Clear();
    switch (GetDBType()) {
        case TDBType::MSSQL:
            q->SQL->Add("CREATE TABLE #PTB (pokaz_id int, pokaz_is_razd tinyint NULL, pokaz_owner_id int NULL, ");
            q->SQL->Add("pokaz_vert tinyint NULL, pokaz_type tinyint NULL, pokaz_bk_code varchar(30) NULL, ");
            q->SQL->Add("rd varchar(10) NULL, cs varchar(10) NULL, vr varchar(10) NULL, ku varchar(10) NULL, is_obrab tinyint NULL)");
            break;
        case TDBType::Postgre:
            q->SQL->Add("CREATE TEMP TABLE PTB (pokaz_id integer, pokaz_is_razd smallint NULL, pokaz_owner_id integer NULL, ");
            q->SQL->Add("pokaz_vert smallint NULL, pokaz_type smallint NULL, pokaz_bk_code character varying(30) NULL, ");
            q->SQL->Add("rd character varying(10) NULL, cs character varying(10) NULL, vr character varying(10) NULL, ku character varying(10) NULL, is_obrab smallint NULL);");
            break;
    }
    q->ExecSQL();
    q->SQL->Clear();

    ToDate = EncodeDate(CurrYear, 12, 30);
    TDB_TmpPokaztl::CreateTmpPTB(root_Pokaz_id, ToDate);

    q->SQL->Add("CREATE TABLE " + aTblName + " (pokaz_id int, pokaz_is_razd tinyint NULL, pokaz_owner_id int NULL, ");
    q->SQL->Add("pokaz_vert tinyint NULL, pokaz_type tinyint NULL, pokaz_bk_code varchar(30) NULL, ");
    q->SQL->Add("rd varchar(10) NULL, cs varchar(10) NULL, vr varchar(10) NULL, ku varchar(10) NULL, is_obrab tinyint NULL)");
    q->ExecSQL();
    q->SQL->Clear();

    q->SQL->Add("INSERT INTO " + aTblName);
    switch (GetDBType()) {
        case TDBType::MSSQL:
            q->SQL->Add("SELECT * FROM #PTB");
            break;
        case TDBType::Postgre:
            q->SQL->Add("SELECT * FROM PTB");
            break;
    }
    q->ExecSQL();
    q->SQL->Clear();

    q->SQL->Add("DELETE FROM " + aTblName);
    q->SQL->Add("WHERE vr = :vr0");
    q->SQL->Add("AND ku = :ku0");
    q->ParamByName("vr0")->AsString() = "";
    q->ParamByName("ku0")->AsString() = "";
    q->ExecSQL();
    q->SQL->Clear();

    q->SQL->Add("UPDATE " + aTblName);
    q->SQL->Add("SET ku = :ku");
    q->SQL->Add("WHERE ku = :ku0");
    q->SQL->Add("AND EXISTS");
    switch (GetDBType()) {
        case TDBType::MSSQL:
            q->SQL->Add("(SELECT * FROM #PTB");
            q->SQL->Add("WHERE #PTB.pokaz_owner_id = " + aTblName + ".pokaz_id");
            q->SQL->Add("AND #PTB.ku <> :ku0)");
            break;
        case TDBType::Postgre:
            q->SQL->Add("(SELECT * FROM PTB");
            q->SQL->Add("WHERE PTB.pokaz_owner_id = " + aTblName + ".pokaz_id");
            q->SQL->Add("AND PTB.ku <> :ku0)");
            break;
    }
    q->ParamByName("ku")->AsString() = "$$$"; // "000"
    q->ParamByName("ku0")->AsString() = "";
    q->ExecSQL();
    q->SQL->Clear();

    q->SQL->Add("DELETE FROM " + aTblName);
    q->SQL->Add("WHERE ku = :ku0");
    q->ParamByName("ku0")->AsString() = "";
    q->ExecSQL();

    switch (GetDBType()) {
        case TDBType::MSSQL:
            q->SQL->Add("DROP TABLE #PTB");
            break;
        case TDBType::Postgre:
            q->SQL->Add("DROP TABLE IF EXISTS PTB;");
            break;
    }
    q->ExecSQL();
    q->SQL->Clear();

    q->SQL->Add("CREATE INDEX IP1_" + aTblName + " ON " + aTblName + " (cs, rd, vr, ku)");
    q->ExecSQL();

   //MAK q->Free();
delete q;
}

void TDB_TmpPokaztl::CreateTmpPTB(int aPokaz_id, TDateTime aToDate) {
    TFDStoredProc *sp = new TFDStoredProc(DefDBconnect.Conn); 
    sp->storedProcName = "CreateTmpPTB";

    switch (GetDBType()) {
        case TDBType::MSSQL: {
            sp->Params->CreateParam(ftInteger, "@aPokaz_id", ptInput);
            sp->Params->CreateParam(ftDateTime, "@aToDate", ptInput);
            sp->Params->ParamByName("@aPokaz_id")->AsInteger() = aPokaz_id;
            sp->Params->ParamByName("@aToDate")->AsDateTime() = aToDate;
            break;
        }
        case TDBType::Postgre: {
            sp->Params->CreateParam(ftInteger, "aPokaz_id", ptInput);
            sp->Params->CreateParam(ftDateTime, "aToDate", ptInput);
            sp->Params->ParamByName("aPokaz_id")->AsInteger() = aPokaz_id;
            sp->Params->ParamByName("aToDate")->AsDateTime() = aToDate;
            break;
        }
    }

    sp->ExecProc();
    delete sp;
}

void TDB_TmpPokaztl::CreateTmpPTBanypg(int aPokaz_id, TDateTime aToDate) {
    TFDStoredProc *sp = new TFDStoredProc(DefDBconnect.Conn); 
    sp->storedProcName = "CreateTmpPTBanypg";
    
    sp->Params->CreateParam(ftInteger, "aPokaz_id", ptInput);
    sp->Params->CreateParam(ftDateTime, "aToDate", ptInput);
    sp->Params->ParamByName("aPokaz_id")->AsInteger() = aPokaz_id;
    sp->Params->ParamByName("aToDate")->AsDateTime() = aToDate;
    
    sp->ExecProc();
    delete sp;
}

void TDB_TmpPokaztl::CreateTmpPTBany(int aPokaz_id, TDateTime aToDate) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    q->SQL->Clear();

    switch (GetDBType()) {
        case TDBType::MSSQL: {
            q->SQL->Add("declare ");
            q->SQL->Add("@aPokaz_id  integer, ");
            q->SQL->Add("@aToDate  DateTime, ");

            q->SQL->Add("@pokaz_id integer, ");
            q->SQL->Add("@pokaz_is_razd integer, ");
            q->SQL->Add("@pokaz_vert integer, ");
            q->SQL->Add("@pokaz_type integer, ");
            q->SQL->Add("@dlCS integer, ");
            q->SQL->Add("@plVR integer, ");
            q->SQL->Add("@plKOSGU integer ");

            q->SQL->Add("set @aPokaz_id = :aPokaz_id ");
            q->SQL->Add("set @aToDate = :aToDate ");

            q->SQL->Add("if YEAR (@aToDate) < 2016 begin ");
                q->SQL->Add("set @dlCS = 7 ");
                q->SQL->Add("set @plVR = 12 ");
                q->SQL->Add("set @plKOSGU = 15 ");
            q->SQL->Add("end ");
            q->SQL->Add("else begin ");
                q->SQL->Add("set @dlCS = 10 ");
                q->SQL->Add("set @plVR = 15 ");
                q->SQL->Add("set @plKOSGU = 18 ");
            q->SQL->Add("end ");

            q->SQL->Add("INSERT INTO #PTB (pokaz_id, pokaz_is_razd, pokaz_owner_id, pokaz_vert, pokaz_type, pokaz_bk_code, ");
            q->SQL->Add("rd, cs, vr, ku, is_obrab) ");
            q->SQL->Add("SELECT pokaz_id, pokaz_is_razd, pokaz_owner_id, pokaz_vert, pokaz_type, pokaz_bk_code, ");
            q->SQL->Add("substring (pokaz_bk_code,1,4), ");
            q->SQL->Add("RTrim(substring (pokaz_bk_code,5,@dlCS)), ");
            q->SQL->Add("RTrim(substring (pokaz_bk_code,@plVR,3)), ");
            q->SQL->Add("RTrim(substring (pokaz_bk_code,@plKOSGU,3)), 0 ");
            q->SQL->Add("FROM Pokaztl ");
            q->SQL->Add("WHERE pokaz_id = @aPokaz_id ");
            q->SQL->Add("and @aToDate BETWEEN pokaz_beg_date AND pokaz_end_date ");

            q->SQL->Add("declare Cursor1 INSENSITIVE CURSOR ");
            q->SQL->Add("FOR select pokaz_id, pokaz_is_razd, pokaz_vert, pokaz_type ");
            q->SQL->Add("from #PTB ");
            q->SQL->Add("where is_obrab = 0 ");

            q->SQL->Add("while 1 = 1 begin ");
                q->SQL->Add("open Cursor1 ");
                q->SQL->Add("fetch from Cursor1 into @pokaz_id, @pokaz_is_razd, @pokaz_vert, @pokaz_type ");
                q->SQL->Add("if @@fetch_status <> 0 break ");

                q->SQL->Add("UPDATE #PTB ");
                q->SQL->Add("SET is_obrab = 1 ");

                q->SQL->Add("while @@fetch_status = 0 begin ");
                    q->SQL->Add("if (@pokaz_is_razd = 0) or (@pokaz_is_razd = 1) begin ");
                        q->SQL->Add("INSERT INTO #PTB (pokaz_id, pokaz_is_razd, pokaz_owner_id, pokaz_vert, pokaz_type, pokaz_bk_code, ");
                        q->SQL->Add("rd, cs, vr, ku, is_obrab) ");
                        q->SQL->Add("SELECT pokaz_id, pokaz_is_razd, pokaz_owner_id, pokaz_vert, pokaz_type, pokaz_bk_code, ");
                        q->SQL->Add("substring (pokaz_bk_code,1,4), ");
                        q->SQL->Add("RTrim(substring (pokaz_bk_code,5,@dlCS)), ");
                        q->SQL->Add("RTrim(substring (pokaz_bk_code,@plVR,3)), ");
                        q->SQL->Add("RTrim(substring (pokaz_bk_code,@plKOSGU,3)), 0 ");
                        q->SQL->Add("FROM Pokaztl ");
                        q->SQL->Add("WHERE pokaz_owner_id = @Pokaz_id ");
                        q->SQL->Add("and @aToDate BETWEEN pokaz_beg_date AND pokaz_end_date ");
                    q->SQL->Add("end ");
                    q->SQL->Add("fetch next from Cursor1 into @pokaz_id, @pokaz_is_razd, @pokaz_vert, @pokaz_type ");
                q->SQL->Add("end ");
                q->SQL->Add("close Cursor1 ");
            q->SQL->Add("end ");

            q->SQL->Add("deallocate Cursor1 ");
            break;
        }
        case TDBType::Postgre: {
            // Handle PostgreSQL case separately
            // Call function for PostgreSQL
            CreateTmpPTBanypg(aPokaz_id, aToDate);
            break;
        }
    }

    q->ParamByName("aPokaz_id")->AsInteger() = aPokaz_id;
    q->ParamByName("aToDate")->AsDateTime() = aToDate;

    q->ExecSQL();
   //MAK q->Free();
delete q;
}

bool TDB_TmpPokaztl::DropAllTmpOrderCP() {
    bool result = GlobalVars->isCreateTmpOrderCP;
    if (!GlobalVars->isCreateTmpOrderCP)
        return false;
    for (int i = 0; i < GlobalVars->slCreateTmpOrderCPNames->Count(); ++i)
        DropTmpOrderCP((*GlobalVars->slCreateTmpOrderCPNames)[i]);

    GlobalVars->slCreateTmpOrderCPNames->Clear();
    GlobalVars->isCreateTmpOrderCP = false;
    return result;
}

void TDB_TmpPokaztl::DropTmpOrderCP(string aTblName) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);

    try {
        q->SQL->Clear();

        switch (GetDBType()) {
            case TDBType::MSSQL:
                q->SQL->Add("DROP TABLE " + aTblName);
                break;
            case TDBType::Postgre:
                q->SQL->Add("DROP TABLE IF EXISTS " + aTblName);
                break;
        }

        q->ExecSQL();
    } catch (...) {
        // Handle exceptions if necessary
    }

   //MAK q->Free();
delete q;
}

void TDB_TmpPokaztl::DropTmpByKOSGUall() {
    for (int i = 0; i < nameCreateTmpByKOSGU.Count(); ++i)
        DropTmpByKOSGU(nameCreateTmpByKOSGU.Strings[i]);

    nameCreateTmpByKOSGU.Clear();
}

void TDB_TmpPokaztl::DropTmpByKOSGU(string aTblName) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);


    q->SQL->Clear();

    switch (GetDBType()) {
        case TDBType::MSSQL:
            q->SQL->Add("if object_id('tempdb.." + aTblName + "') is not null DROP TABLE " + aTblName);
            break;
        case TDBType::Postgre:
            q->SQL->Add("DROP TABLE IF EXISTS " + aTblName);
            break;
    }

    q->ExecSQL();

   //MAK q->Free();
delete q;

}

