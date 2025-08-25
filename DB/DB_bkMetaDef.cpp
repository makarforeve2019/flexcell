#include "DB_bkMetaDef.h"
#include "unDefDBconnect.h"
#include "../SystFuncs/SystFuncsLogic.h"

std::string TDB_bkMetaDef::GetNameVidById(int metadef_id) {
    std::string result;
    if (metadef_id >= 1 && metadef_id <= 999999)
        result = "Раздел/Подраздел";
    else if (metadef_id >= 1000000 && metadef_id <= 1999999)
        result = "Целевая статья";
    else if (metadef_id >= 2000000 && metadef_id <= 2999999)
        result = "Вид расходов";
    else if (metadef_id >= 3000000 && metadef_id <= 3999999)
        result = "КОСГУ";
    return result;
}

int TDB_bkMetaDef::GetVidById(int metadef_id) {
    int result = 0;
    if (metadef_id >= 1 && metadef_id <= 999999)
        result = 0;
    else if (metadef_id >= 1000000 && metadef_id <= 1999999)
        result = 1;
    else if (metadef_id >= 2000000 && metadef_id <= 2999999)
        result = 2;
    else if (metadef_id >= 3000000 && metadef_id <= 3999999)
        result = 3;
    return result;
}

std::string TDB_bkMetaDef::GetSubKBKByVid(std::string fullKBK, int vid, int God) {
    int nacalo_vKBK, dlina_vKBK;
    // Assuming GetVidInfo function is available in C++
    TDB_bkMetaDef::GetVidInfo(vid, nacalo_vKBK, dlina_vKBK, God);
    std::string result = "";
    if (fullKBK.length() > nacalo_vKBK)
        result = fullKBK.substr(nacalo_vKBK, dlina_vKBK);
    return result;
}

std::string TDB_bkMetaDef::GetRasdel(std::string fullKBK, int God) {
    return GetSubKBKByVid(fullKBK, 0, God);
}

std::string TDB_bkMetaDef::GetCelSt(std::string fullKBK, int God) {
    return GetSubKBKByVid(fullKBK, 1, God);
}

std::string TDB_bkMetaDef::GetVidRasx(std::string fullKBK, int God) {
    return GetSubKBKByVid(fullKBK, 2, God);
}

std::string TDB_bkMetaDef::GetKOSGU(std::string fullKBK, int God) {
    return GetSubKBKByVid(fullKBK, 3, God);
}

void TDB_bkMetaDef::GetDefAll(TQueryUni *qIn) {
    qIn->SQL->Clear();
    qIn->SQL->Add("SELECT *");
    qIn->SQL->Add("FROM bkMetaDef");
    qIn->Open();
}

void TDB_bkMetaDef::AddDef(int vid, int nacalo_vKBK, int dlina_vKBK, int beg_God, int end_God) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    int metadef_id = Getmetadef_id(vid);
    q->SQL->Clear();
    q->SQL->Add("INSERT INTO bkMetaDef");
    q->SQL->Add("(metadef_id, nacalo_vKBK, dlina_vKBK, beg_date, end_date, create_date)");
    q->SQL->Add("VALUES (:metadef_id, :nacalo_vKBK, :dlina_vKBK, :beg_date, :end_date, :create_date)");
    q->ParamByName("metadef_id")->AsInteger() = metadef_id;
    q->ParamByName("nacalo_vKBK")->AsInteger() = nacalo_vKBK;
    q->ParamByName("dlina_vKBK")->AsInteger() = dlina_vKBK;
    q->ParamByName("beg_date")->AsDateTime() = EncodeDate(beg_God, 1, 1);
    q->ParamByName("end_date")->AsDateTime() = EncodeDate(end_God, 12, 31);
    q->ParamByName("create_date")->AsDateTime() = Date();
    q->ExecSQL();
   //MAK q->Free();
delete q;
}

void TDB_bkMetaDef::ModDef(int metadef_id, int nacalo_vKBK, int dlina_vKBK, int beg_God, int end_God) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    q->SQL->Clear();
    q->SQL->Add("UPDATE bkMetaDef");
    q->SQL->Add("SET nacalo_vKBK=:nacalo_vKBK, dlina_vKBK=:dlina_vKBK, beg_date=:beg_date, end_date=:end_date");
    q->SQL->Add("WHERE metadef_id = :metadef_id");
    q->ParamByName("metadef_id")->AsInteger() = metadef_id;
    q->ParamByName("nacalo_vKBK")->AsInteger() = nacalo_vKBK;
    q->ParamByName("dlina_vKBK")->AsInteger() = dlina_vKBK;
    q->ParamByName("beg_date")->AsDateTime() = EncodeDate(beg_God, 1, 1);
    q->ParamByName("end_date")->AsDateTime() = EncodeDate(end_God, 12, 31);
    q->ExecSQL();
   //MAK q->Free();
delete q;
}

void TDB_bkMetaDef::DelDef(int metadef_id) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    q->SQL->Clear();
    q->SQL->Add("DELETE FROM bkMetaDef");
    q->SQL->Add("WHERE metadef_id = :metadef_id");
    q->ParamByName("metadef_id")->AsInteger() = metadef_id;
    q->ExecSQL();
   //MAK q->Free();
delete q;
}

int TDB_bkMetaDef::Get0(int vid) {
    int result = 0;
    switch (vid) {
        case 0:
            result = 1;
            break;
        case 1:
            result = 1000000;
            break;
        case 2:
            result = 2000000;
            break;
        case 3:
            result = 3000000;
            break;
    }
    return result;
}

int TDB_bkMetaDef::Getmetadef_id(int vid) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    int result = TDB_bkMetaDef::Get0(vid);
    try {
        q->SQL->Clear();
        q->SQL->Add("SELECT MAX(metadef_id) as wmetadef_id");
        q->SQL->Add("FROM bkMetaDef");
        switch (vid) {
            case 0:
                q->SQL->Add("WHERE metadef_id >= 1");
                q->SQL->Add("AND metadef_id <= 999999");
                break;
            case 1:
                q->SQL->Add("WHERE metadef_id >= 1000000");
                q->SQL->Add("AND metadef_id <= 1999999");
                break;
            case 2:
                q->SQL->Add("WHERE metadef_id >= 2000000");
                q->SQL->Add("AND metadef_id <= 2999999");
                break;
            case 3:
                q->SQL->Add("WHERE metadef_id >= 3000000");
                q->SQL->Add("AND metadef_id <= 3999999");
                break;
        }
        q->Open();
        if (!q->EoF()) {
            result = q->FieldByName("wmetadef_id")->AsInteger();
            if (result > 0) result += 1;
            else result = TDB_bkMetaDef::Get0(vid);
        }
    }
    catch (exception& e) {
        // Handle exception
    }
    q->Close();
   //MAK q->Free();
delete q;
    return result;
}

void TDB_bkMetaDef::GetDefInfo(TQueryUni* qIn, int metadef_id) {
    qIn->SQL->Clear();
    qIn->SQL->Add("SELECT *");
    qIn->SQL->Add("FROM bkMetaDef");
    qIn->SQL->Add("WHERE metadef_id = :metadef_id");
    qIn->ParamByName("metadef_id")->AsInteger() = metadef_id;
    qIn->Open();
}

std::array<TVidInfo, 100> arVidInfo;

bool TDB_bkMetaDef::GetVidInfo(int vid, int& nacalo_vKBK, int& dlina_vKBK, int curGod) {
    int Year, Month, Day;
    int i;
    
    if (!isLoad) {
        TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
        Nkol = 0;
        
        while (!q->EoF()) {
            Nkol++;
            if (Nkol > 100) break;
            arVidInfo[Nkol].vid = GetVidById(q->FieldByName("metadef_id")->AsInteger());
            arVidInfo[Nkol].nacalo_vKBK = q->FieldByName("nacalo_vKBK")->AsInteger();
            arVidInfo[Nkol].dlina_vKBK = q->FieldByName("dlina_vKBK")->AsInteger();
            DecodeDate(q->FieldByName("beg_date")->AsDateTime(), Year, Month, Day);
            arVidInfo[Nkol].beg_God = Year;
            DecodeDate(q->FieldByName("end_date")->AsDateTime(), Year, Month, Day);
            arVidInfo[Nkol].end_God = Year;
            q->Next();
        }
        isLoad = true;
    }
    
    // Set default values
    switch (vid) {
        case 0:
            nacalo_vKBK = 1;
            dlina_vKBK = 4;
            break;
        case 1:
            nacalo_vKBK = 5;
            dlina_vKBK = 7;
            break;
        case 2:
            nacalo_vKBK = 12;
            dlina_vKBK = 3;
            break;
        case 3:
            nacalo_vKBK = 15;
            dlina_vKBK = 3;
            break;
    }
    
    for (i = 0; i < Nkol; i++) {
        if (arVidInfo[i].vid == vid && arVidInfo[i].beg_God <= curGod && curGod <= arVidInfo[i].end_God) {
            nacalo_vKBK = arVidInfo[i].nacalo_vKBK;
            dlina_vKBK = arVidInfo[i].dlina_vKBK;
            break;
        }
    }
    
    return true;
}

int TDB_bkMetaDef::GetDl_R_CS(int curGod) {
    int nacalo_vKBK;
    int dlina_vKBK;
    
    TDB_bkMetaDef::GetVidInfo(2, nacalo_vKBK, dlina_vKBK, curGod);
    
    return nacalo_vKBK - 1;
}

int TDB_bkMetaDef::GetDl_CS(int curGod) {
    int nacalo_vKBK;
    int dlina_vKBK;

    TDB_bkMetaDef::GetVidInfo(1, nacalo_vKBK, dlina_vKBK, curGod);
    
    return dlina_vKBK;
}

int TDB_bkMetaDef::GetDl_All(int curGod) {
    int nacalo_vKBK;
    int dlina_vKBK;

    TDB_bkMetaDef::GetVidInfo(3, nacalo_vKBK, dlina_vKBK, curGod);
    
    return nacalo_vKBK - 1 + dlina_vKBK;
}

int TDB_bkMetaDef::GetDl_R_CS_VR(int curGod) {
    int nacalo_vKBK;
    int dlina_vKBK;

    TDB_bkMetaDef::GetVidInfo(3, nacalo_vKBK, dlina_vKBK, curGod);
    
    return nacalo_vKBK - 1;
}


