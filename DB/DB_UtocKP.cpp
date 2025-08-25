#include "DB_UtocKP.h" 
#include "unDefDBconnect.h"
#include "SystFuncsLogic.h"
#include "FormDopInp.h"

int TDB_UtocKP::GetIdGrnlUtocKP() {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    int id = 0;
    try {
        q->SQL->Clear();
        switch (GetDBType()) {
            case TDBType::MSSQL:
                q->SQL->Add("SELECT @@IDENTITY as id");
                break;
            case TDBType::Postgre:
                q->SQL->Add("SELECT currval(pg_get_serial_sequence('GrnlUtocKP', 'grnlutkp_id')) as id");
                break;
        }
        q->Open();
        id = q->FieldByName("id")->AsInteger();
    }     catch (std::exception& e) {
        // Обработка исключения
        //std::cerr << "Exception caught: " << e.what() << std::endl;
    }
   //MAK q->Free();
delete q;
    return id;
}

int TDB_UtocKP::GetIdUtocKP() {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    int id = 0;
    try {
        q->SQL->Clear();
        switch (GetDBType()) {
            case TDBType::MSSQL:
                q->SQL->Add("SELECT @@IDENTITY as id");
                break;
            case TDBType::Postgre:
                q->SQL->Add("SELECT currval(pg_get_serial_sequence('UtocKP', 'utockp_id')) as id");
                break;
        }
        q->Open();
        id = q->FieldByName("id")->AsInteger();
    }     catch (std::exception& e) {
        // Обработка исключения
        //std::cerr << "Exception caught: " << e.what() << std::endl;
    }
   //MAK q->Free();
delete q;
    return id;
}

int TDB_UtocKP::addZagUtocKP(int god, int cast_id, std::string nomerDok, TDateTime date_dok) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    int grnlutkp_id = 0;
    try {
        q->SQL->Clear();
        q->SQL->Add("INSERT INTO GrnlUtocKP");
        q->SQL->Add("(god, cast_id, nomerDok, sost, date_dok)");
        q->SQL->Add("values (:god, :cast_id, :nomerDok, 1, :date_dok)");
        q->ParamByName("god")->AsInteger() = god;
        q->ParamByName("cast_id")->AsInteger() = cast_id;
        q->ParamByName("nomerDok")->AsString() = nomerDok;
        q->ParamByName("date_dok")->AsDateTime() = date_dok;
        q->ExecSQL();
        grnlutkp_id = GetIdGrnlUtocKP();
    }     catch (std::exception& e) {
        // Обработка исключения
        //std::cerr << "Exception caught: " << e.what() << std::endl;
    }
   //MAK q->Free();
delete q;
    return grnlutkp_id;
}

void TDB_UtocKP::updZagUtocKP(int grnlutkp_id, std::string nomerDok, TDateTime date_dok, std::string obosnov, TDateTime fromData, TDateTime byData, int prdlg_id) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    try {
        q->SQL->Clear();
        q->SQL->Add("UPDATE GrnlUtocKP");
        q->SQL->Add("SET nomerDok=:nomerDok, date_dok=:date_dok, obosnov = :obosnov, fromData = :fromData, byData = :byData, prdlg_id = :prdlg_id");
        q->SQL->Add("WHERE grnlutkp_id = :grnlutkp_id");
        q->ParamByName("grnlutkp_id")->AsInteger() = grnlutkp_id;
        q->ParamByName("nomerDok")->AsString() = nomerDok;
        q->ParamByName("date_dok")->AsDateTime() = date_dok;
        q->ParamByName("obosnov")->AsString() = obosnov;
        q->ParamByName("fromData")->AsDateTime() = fromData;
        q->ParamByName("byData")->AsDateTime() = byData;
        q->ParamByName("prdlg_id")->AsInteger() = prdlg_id;
        q->ExecSQL();
    }     catch (std::exception& e) {
        // Обработка исключения
        //std::cerr << "Exception caught: " << e.what() << std::endl;
    }
   //MAK q->Free();
delete q;
}

void TDB_UtocKP::UpdDateUpdDB(int grnlutkp_id, TDateTime date_UpdDB) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    q->SQL->Clear();
    q->SQL->Add("UPDATE GrnlUtocKP");
    q->SQL->Add("SET date_UpdDB=:date_UpdDB");
    q->SQL->Add("WHERE grnlutkp_id = :grnlutkp_id");
    q->ParamByName("grnlutkp_id")->AsInteger() = grnlutkp_id;
    q->ParamByName("date_UpdDB")->AsDateTime() = date_UpdDB;
    q->ExecSQL();
   //MAK q->Free();
delete q;
}

void TDB_UtocKP::UpdObosnov(int grnlutkp_id, std::string obosnov) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    q->SQL->Clear();
    q->SQL->Add("UPDATE GrnlUtocKP");
    q->SQL->Add("SET obosnov=:obosnov");
    q->SQL->Add("WHERE grnlutkp_id = :grnlutkp_id");
    q->ParamByName("grnlutkp_id")->AsInteger() = grnlutkp_id;
    q->ParamByName("obosnov")->AsString() = obosnov;
    q->ExecSQL();
   //MAK q->Free();
delete q;
}

void TDB_UtocKP::UpdId_FromPref(int grnlutkp_id, int id_FromPref) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    q->SQL->Clear();
    q->SQL->Add("UPDATE GrnlUtocKP");
    q->SQL->Add("SET id_FromPref = :id_FromPref");
    q->SQL->Add("WHERE grnlutkp_id = :grnlutkp_id");
    q->ParamByName("grnlutkp_id")->AsInteger() = grnlutkp_id;
    q->ParamByName("id_FromPref")->AsInteger() = id_FromPref;
    q->ExecSQL();
   //MAK q->Free();
delete q;
}

int TDB_UtocKP::GetId_FromPref(int grnlutkp_id) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    TDB_UtocKP::GetZagUtocbyID(q, grnlutkp_id);
    int result = q->FieldByName("id_FromPref")->AsInteger();
   //MAK q->Free();
delete q;
    return result;
}

void TDB_UtocKP::GetZagsUtvUtocKP(TQueryUni *qIn, int god) {
 
    qIn->SQL->Clear();
    qIn->SQL->Add("SELECT * FROM GrnlUtocKP WHERE god = :god AND sost = 3");
    qIn->ParamByName("god")->AsInteger() = god;
    qIn->Open();
}

void TDB_UtocKP::GetAllUtocKP(TQueryUni *qIn, int god) {
 
    qIn->SQL->Clear();
    qIn->SQL->Add("SELECT * FROM GrnlUtocKP WHERE god = :god AND sost < 100 ORDER BY date_dok");
    qIn->ParamByName("god")->AsInteger() = god;
    qIn->Open();
}

void TDB_UtocKP::GetAllUtocKPDesc(TQueryUni *qIn, int god) {
 
    qIn->SQL->Clear();
    qIn->SQL->Add("SELECT * FROM GrnlUtocKP WHERE god = :god AND sost < 100 ORDER BY date_dok DESC");
    qIn->ParamByName("god")->AsInteger() = god;
    qIn->Open();
}

void TDB_UtocKP::GetAllUtocKPDescSPO(TQueryUni *qIn, TDateTime s, TDateTime po) {
 
    qIn->SQL->Clear();
    qIn->SQL->Add("SELECT * FROM GrnlUtocKP WHERE date_dok BETWEEN :s AND :po AND sost < 100 ORDER BY date_dok DESC");
    qIn->ParamByName("s")->AsDateTime() = ConvertToDateBgn(s);
    qIn->ParamByName("po")->AsDateTime() = ConvertToDateEnd(po);
    qIn->Open();
}

void TDB_UtocKP::DelUtocKP(int grnlutkp_id) {
    TDB_UtocKP::DelAllDetStr(grnlutkp_id);
    TDB_UtocKP::DelZagUtocKP(grnlutkp_id);
}

void TDB_UtocKP::DelUtocKpPOF(int prdlg_id) {
    int grnlutkp_id = TDB_UtocKP::getGrnlutkpIdForPOF(prdlg_id);
    if (grnlutkp_id == 0) return;
    TDB_UtocKP::DelUtocKP(grnlutkp_id);
}

void TDB_UtocKP::DelZagUtocKP(int grnlutkp_id) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    q->SQL->Clear();
    q->SQL->Add("DELETE FROM GrnlUtocKP WHERE grnlutkp_id = :grnlutkp_id");
    q->ParamByName("grnlutkp_id")->AsInteger() = grnlutkp_id;
    q->ExecSQL();
   //MAK q->Free();
delete q;
}

void TDB_UtocKP::DelAllDetStr(int grnlutkp_id) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    q->SQL->Clear();
    q->SQL->Add("DELETE FROM UtocKP WHERE grnlutkp_id = :grnlutkp_id");
    q->ParamByName("grnlutkp_id")->AsInteger() = grnlutkp_id;
    q->ExecSQL();
   //MAK q->Free();
delete q;
}

int TDB_UtocKP::getGrnlutkpIdForPOF(int prdlg_id) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    int result = 0;
    q->SQL->Clear();
    q->SQL->Add("SELECT grnlutkp_id FROM GrnlUtocKP WHERE prdlg_idRoot = :prdlg_id");
    q->ParamByName("prdlg_id")->AsInteger() = prdlg_id;
    q->Open();
    if (!q->EoF()) result = q->FieldByName("grnlutkp_id")->AsInteger();
   //MAK q->Free();
delete q;
    return result;
}

void TDB_UtocKP::GetZagUtocbyID(TQueryUni *qIn, int grnlutkp_id) {
 
    qIn->SQL->Clear();
    qIn->SQL->Add("SELECT * FROM GrnlUtocKP WHERE grnlutkp_id = :grnlutkp_id");
    qIn->ParamByName("grnlutkp_id")->AsInteger() = grnlutkp_id;
    qIn->Open();
}

void TDB_UtocKP::GetNDokDateDok(int grnlutkp_id, std::string &nomerDok, TDateTime &date_dok) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    TDB_UtocKP::GetZagUtocbyID(q, grnlutkp_id);
    nomerDok = q->FieldByName("nomerDok")->AsString();
    date_dok = q->FieldByName("date_dok")->AsDateTime();
   //MAK q->Free();
delete q;
}

std::string TDB_UtocKP::GetObosnov(int grnlutkp_id) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    TDB_UtocKP::GetZagUtocbyID(q, grnlutkp_id);
    std::string result = q->FieldByName("obosnov")->AsString();
   //MAK q->Free();
delete q;
    return result;
}

void TDB_UtocKP::GetAllDetStr(TQueryUni *qIn, int grnlutkp_id) {
 
    qIn->SQL->Clear();
    qIn->SQL->Add("SELECT * FROM UtocKP WHERE grnlutkp_id = :grnlutkp_id");
    qIn->ParamByName("grnlutkp_id")->AsInteger() = grnlutkp_id;
    qIn->Open();
}

void TDB_UtocKP::CopyAllDetStr(int grnlutkp_id, int grnlutkp_idNew) {
    TQueryUni *qW = new TQueryUni(DefDBconnect.Conn);
    qW->SQL->Add("SELECT * FROM UtocKP WHERE grnlutkp_id = :grnlutkp_id");
    qW->ParamByName("grnlutkp_id")->AsInteger() = grnlutkp_id;
    qW->Open();
    while (!qW->EoF()) {
        addUtocKP(grnlutkp_idNew,
                  qW->FieldByName("plan_id")->AsInteger(),
                  qW->FieldByName("god")->AsInteger(),
                  qW->FieldByName("kv")->AsInteger(),
                  qW->FieldByName("pokaz_id")->AsInteger(),
                  qW->FieldByName("cell_id")->AsInteger(),
                  qW->FieldByName("vid_otc")->AsInteger(),
                  qW->FieldByName("summa")->AsFloat());
        qW->Next();
    }
    delete qW;
}

int TDB_UtocKP::GetUtocIDbyNomDok(int god, std::string nomerDok) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    int result = 0;
    q->SQL->Add("SELECT grnlutkp_id FROM GrnlUtocKP WHERE god = :god AND nomerDok = :nomerDok");
    q->ParamByName("god")->AsInteger() = god;
    q->ParamByName("nomerDok")->AsString() = nomerDok;
    q->Open();
    if (!q->EoF()) result = q->FieldByName("grnlutkp_id")->AsInteger();
   //MAK q->Free();
delete q;
    return result;
}

void TDB_UtocKP::getUtocKpByPrdlgId(TQueryUni *qIn, int prdlg_id) {
    qIn->SQL->Clear();
    qIn->SQL->Add("SELECT u.utockp_id, u.grnlutkp_id, u.plan_id, u.god, u.kv, u.pokaz_id, u.cell_id, u.summa, u.vid_otc FROM prdlgup as p");
    qIn->SQL->Add("INNER JOIN grnlutockp as g ON g.prdlg_idroot = p.prdlg_id");
    qIn->SQL->Add("INNER JOIN utockp as u ON u.grnlutkp_id = g.grnlutkp_id");
    qIn->SQL->Add("WHERE p.prdlg_id = :prdlg_id");
    qIn->ParamByName("prdlg_id")->AsInteger() = prdlg_id;
    qIn->Open();
}

int TDB_UtocKP::GetSostUtoc(int grnlutkp_id) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    GetZagUtocbyID(q, grnlutkp_id);
    int result = q->FieldByName("sost")->AsInteger();
   //MAK q->Free();
delete q;
    return result;
}

int TDB_UtocKP::GetPrdlgId_IsMetodSosdanij(int grnlutkp_id, int& isMetodSosdanij, TDateTime& fromData, TDateTime& byData) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    GetZagUtocbyID(q, grnlutkp_id);
    isMetodSosdanij = q->FieldByName("isMetodSosdanij")->AsInteger();
    fromData = q->FieldByName("fromData")->AsDateTime();
    byData = q->FieldByName("byData")->AsDateTime();
    int result = q->FieldByName("prdlg_id")->AsInteger();
   //MAK q->Free();
delete q;
    return result;
}

void TDB_UtocKP::SetSostUtoc(int grnlutkp_id, int sost, TDateTime DateSost) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    q->SQL->Clear();
    q->SQL->Add("UPDATE GrnlUtocKP SET sost=:sost, DateSost=:DateSost WHERE grnlutkp_id = :grnlutkp_id");
    q->ParamByName("grnlutkp_id")->AsInteger() = grnlutkp_id;
    q->ParamByName("sost")->AsInteger() = sost;
    q->ParamByName("DateSost")->AsDateTime() = DateSost;
    q->ExecSQL();
   //MAK q->Free();
delete q;
}

void TDB_UtocKP::UpduserName(int grnlutkp_id, std::string userName) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    q->SQL->Clear();
    q->SQL->Add("UPDATE GrnlUtocKP SET userName=:userName WHERE grnlutkp_id = :grnlutkp_id");
    q->ParamByName("grnlutkp_id")->AsInteger() = grnlutkp_id;
    q->ParamByName("userName")->AsString() = userName;
    q->ExecSQL();
   //MAK q->Free();
delete q;
}

double TDB_UtocKP::GetUtKassPl_pdg(PCellValueAtr vd) {
    int kv;
    int Year, Month, Day;
    double summa = 0;
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    DecodeDate(vd->ValueDate, Year, Month, Day);
    kv = GetNKv(Month);
    q->SQL->Clear();
    q->SQL->Add("SELECT summa FROM GrnlUtocKP a, UtocKP b WHERE a.god = :god AND a.sost = 1 AND b.grnlutkp_id = a.grnlutkp_id AND b.plan_id = :plan_id AND b.god = :god AND b.kv = :kv AND b.Pokaz_id = :pokaz_id AND b.cell_id = :cell_id AND b.vid_otc = :vid_otc");
    q->ParamByName("god")->AsInteger() = Year;
    q->ParamByName("plan_id")->AsInteger() = vd->PlanId;
    q->ParamByName("kv")->AsInteger() = kv;
    q->ParamByName("pokaz_id")->AsInteger() = vd->PokazId;
    q->ParamByName("cell_id")->AsInteger() = vd->CellId;
    q->ParamByName("vid_otc")->AsInteger() = vd->VidOtch;
    q->Open();
    if (!q->EoF()) summa = q->FieldByName("summa")->AsFloat();
   //MAK q->Free();
delete q;
    return summa;
}

double TDB_UtocKP::GetUtKassPl_utv(TFormLogicF1* FormLogicF1, PCellValueAtr vd) {
    int kv, wgrnlutkp_id;
    int Year, Month, Day;
    double summa = 0;
    TwwStringList* p = TFormDopInp::GetTww(FormLogicF1);
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    DecodeDate(vd->ValueDate, Year, Month, Day);
    kv = GetNKv(Month);
    wgrnlutkp_id = p->grnlutkp_id;
    q->SQL->Clear();
    q->SQL->Add("SELECT summa FROM GrnlUtocKP a, UtocKP b WHERE a.grnlutkp_id = :grnlutkp_id AND b.grnlutkp_id = a.grnlutkp_id AND b.plan_id = :plan_id AND b.god = :god AND b.kv = :kv AND b.Pokaz_id = :pokaz_id AND b.cell_id = :cell_id AND b.vid_otc = :vid_otc");
    q->ParamByName("grnlutkp_id")->AsInteger() = wgrnlutkp_id;
    q->ParamByName("plan_id")->AsInteger() = vd->PlanId;
    q->ParamByName("god")->AsInteger() = Year;
    q->ParamByName("kv")->AsInteger() = kv;
    q->ParamByName("pokaz_id")->AsInteger() = vd->PokazId;
    q->ParamByName("cell_id")->AsInteger() = vd->CellId;
    q->ParamByName("vid_otc")->AsInteger() = vd->VidOtch;
    q->Open();
    if (!q->EoF()) summa = q->FieldByName("summa")->AsFloat();
   //MAK q->Free();
delete q;
    return summa;
}

int TDB_UtocKP::GetId_pdg(int god) {
    std::string username = GetUserLogin();
    int result = 0;
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    q->SQL->Clear();
    q->SQL->Add("SELECT grnlutkp_id FROM GrnlUtocKP a WHERE a.god = :god AND a.sost = 1 AND a.UserName = :username");
    q->ParamByName("god")->AsInteger() = god;
    q->ParamByName("username")->AsString() = username;
    q->Open();
    if (!q->EoF()) result = q->FieldByName("grnlutkp_id")->AsInteger();
   //MAK q->Free();
delete q;
    return result;
}

int TDB_UtocKP::FindByPrdlgId(int prdlg_id, int& sost, std::string& nomerDok, TDateTime& date_dok) {
    int result = 0;
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    q->SQL->Clear();
    q->SQL->Add("SELECT * FROM GrnlUtocKP a WHERE a.prdlg_id = :prdlg_id");
    q->ParamByName("prdlg_id")->AsInteger() = prdlg_id;
    q->Open();
    if (!q->EoF()) {
        nomerDok = q->FieldByName("nomerDok")->AsString();
        sost = q->FieldByName("sost")->AsInteger();      
        date_dok = q->FieldByName("date_dok")->AsDateTime();
        result = q->FieldByName("grnlutkp_id")->AsInteger();
    }
   //MAK q->Free();
delete q;
    return result;
}

void TDB_UtocKP::updIsMetodSosdanij(int grnlutkp_id, int isMetodSosdanij) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    q->SQL->Clear();
    q->SQL->Add("UPDATE GrnlUtocKP SET isMetodSosdanij = :isMetodSosdanij WHERE grnlutkp_id = :grnlutkp_id");
    q->ParamByName("grnlutkp_id")->AsInteger() = grnlutkp_id;
    q->ParamByName("isMetodSosdanij")->AsInteger() = isMetodSosdanij;
    q->ExecSQL();
   //MAK q->Free();
delete q;
}

int TDB_UtocKP::addUtocKP(int grnlutkp_id, int plan_id, int god, int kv, int pokaz_id, int cell_id, int vid_otc, double summa) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    q->SQL->Clear();
    q->SQL->Add("INSERT INTO UtocKP (grnlutkp_id, plan_id, god, kv, pokaz_id, cell_id, vid_otc, summa) values (:grnlutkp_id, :plan_id, :god, :kv, :pokaz_id, :cell_id, :vid_otc, :summa)");
    q->ParamByName("grnlutkp_id")->AsInteger() = grnlutkp_id;
    q->ParamByName("plan_id")->AsInteger() = plan_id;
    q->ParamByName("god")->AsInteger() = god;
    q->ParamByName("kv")->AsInteger() = kv;
    q->ParamByName("pokaz_id")->AsInteger() = pokaz_id;
    q->ParamByName("cell_id")->AsInteger() = cell_id;
    q->ParamByName("vid_otc")->AsInteger() = vid_otc;
    q->ParamByName("summa")->AsFloat() = summa;
    q->ExecSQL();
    int result = GetIdUtocKP();
   //MAK q->Free();
delete q;
    return result;
}

void TDB_UtocKP::updGrnlutkpId_InUtoc(int grnlutkp_idFrom, int grnlutkp_idTo) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    q->SQL->Clear();
    q->SQL->Add("UPDATE UtocKP SET grnlutkp_id = :grnlutkp_idTo WHERE grnlutkp_id = :grnlutkp_idFrom");
    q->ParamByName("grnlutkp_idFrom")->AsInteger() = grnlutkp_idFrom;
    q->ParamByName("grnlutkp_idTo")->AsInteger() = grnlutkp_idTo;
    q->ExecSQL();
   //MAK q->Free();
delete q;
}

void TDB_UtocKP::updUtocKP_bysumma(int utockp_id, double summa) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    q->SQL->Clear();
    q->SQL->Add("UPDATE UtocKP SET summa = :summa WHERE utockp_id = :utockp_id");
    q->ParamByName("utockp_id")->AsInteger() = utockp_id;
    q->ParamByName("summa")->AsFloat() = summa;
    q->ExecSQL();
   //MAK q->Free();
delete q;
}

void TDB_UtocKP::saveUtValue(PCellValueAtr vd, int grnlutkp_id, double value) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    int kv, utockp_id;
    utockp_id = 0;
    int Year, Month, Day;
    DecodeDate(vd->ValueDate, Year, Month, Day);
    kv = GetNKv(Month);
    q->SQL->Clear();
    q->SQL->Add("SELECT utockp_id FROM UtocKP WHERE grnlutkp_id = :grnlutkp_id AND plan_id = :plan_id AND god = :god AND kv = :kv AND Pokaz_id = :pokaz_id AND cell_id = :cell_id AND vid_otc = :vid_otc");
    q->ParamByName("grnlutkp_id")->AsInteger() = grnlutkp_id;
    q->ParamByName("plan_id")->AsInteger() = vd->PlanId;
    q->ParamByName("god")->AsInteger() = Year;
    q->ParamByName("kv")->AsInteger() = kv;
    q->ParamByName("pokaz_id")->AsInteger() = vd->PokazId;
    q->ParamByName("cell_id")->AsInteger() = vd->CellId;
    q->ParamByName("vid_otc")->AsInteger() = vd->VidOtch;
    q->Open();
    if (!q->EoF()) utockp_id = q->FieldByName("utockp_id")->AsInteger();
   //MAK q->Free();
delete q;
    if (utockp_id == 0) 
        addUtocKP(grnlutkp_id, vd->PlanId, Year, kv, vd->PokazId, vd->CellId, vd->VidOtch, value);
    else 
        updUtocKP_bysumma(utockp_id, value);
}

bool TDB_UtocKP::UnikNomDok(int grnlutkp_id, string nomerDok) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    GetZagUtocbyID(q, grnlutkp_id);
    int god = q->FieldByName("god")->AsInteger();
    bool result = true;
    q->SQL->Clear();
    q->SQL->Add("SELECT userName FROM GrnlUtocKP WHERE god = :god AND nomerDok = :nomerDok AND grnlutkp_id <> :grnlutkp_id");
    q->ParamByName("god")->AsInteger() = god;
    q->ParamByName("nomerDok")->AsString() = nomerDok;
    q->ParamByName("grnlutkp_id")->AsInteger() = grnlutkp_id;
    q->Open();
    if (!q->EoF()) result = false;
   //MAK q->Free();
delete q;
    return result;
}

int TDB_UtocKP::addShablonFormUKP(int god, int Form_id_shablon) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    q->SQL->Clear();
    q->SQL->Add("INSERT INTO GrnlUtocKP (god, Form_id_shablon, sost) values (:god, :Form_id_shablon, 100)");
    q->ParamByName("god")->AsInteger() = god;
    q->ParamByName("Form_id_shablon")->AsInteger() = Form_id_shablon;
    q->ExecSQL();
    int result = GetIdGrnlUtocKP();
   //MAK q->Free();
delete q;
    return result;
}

void TDB_UtocKP::updShablonFormUKP(int grnlutkp_id, int Form_id_shablon) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    q->SQL->Clear();
    q->SQL->Add("UPDATE GrnlUtocKP SET Form_id_shablon = :Form_id_shablon WHERE grnlutkp_id = :grnlutkp_id");
    q->ParamByName("grnlutkp_id")->AsInteger() = grnlutkp_id;
    q->ParamByName("Form_id_shablon")->AsInteger() = Form_id_shablon;
    q->ExecSQL();
   //MAK q->Free();
delete q;
}

int TDB_UtocKP::selShablonFormUKP(int god, int &grnlutkp_id) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    int result = 0;
    q->SQL->Clear();
    q->SQL->Add("SELECT Form_id_shablon, grnlutkp_id FROM GrnlUtocKP WHERE god = :god AND sost = 100");
    q->ParamByName("god")->AsInteger() = god;
    q->Open();
    if (!q->EoF()) {
        grnlutkp_id = q->FieldByName("grnlutkp_id")->AsInteger();
        result = q->FieldByName("Form_id_shablon")->AsInteger();
    }
   //MAK q->Free();
delete q;
    return result;
}

const uint32_t C1BIT = 0x00000001;

void TDB_UtocKP::Set_1_Bit(int grnlutkp_id) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    q->SQL->Clear();
    q->SQL->Add("SELECT isMetodSosdanij FROM GrnlUtocKP WHERE grnlutkp_id = :grnlutkp_id");
    q->ParamByName("grnlutkp_id")->AsInteger() = grnlutkp_id;
    q->Open();
    int isMetodSosdanij = q->FieldByName("isMetodSosdanij")->AsInteger();
    isMetodSosdanij |= C1BIT;

    q->SQL->Clear();
    q->SQL->Add("UPDATE GrnlUtocKP SET isMetodSosdanij = :isMetodSosdanij WHERE grnlutkp_id = :grnlutkp_id");
    q->ParamByName("grnlutkp_id")->AsInteger() = grnlutkp_id;
    q->ParamByName("isMetodSosdanij")->AsInteger() = isMetodSosdanij;
    q->ExecSQL();
   //MAK q->Free();
    delete q;
}

void TDB_UtocKP::ReSet_1_Bit(int grnlutkp_id) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    q->SQL->Clear();
    q->SQL->Add("SELECT isMetodSosdanij FROM GrnlUtocKP WHERE grnlutkp_id = :grnlutkp_id");
    q->ParamByName("grnlutkp_id")->AsInteger() = grnlutkp_id;
    q->Open();
    int isMetodSosdanij = q->FieldByName("isMetodSosdanij")->AsInteger();

    isMetodSosdanij &= ~C1BIT;

    q->SQL->Clear();
    q->SQL->Add("UPDATE GrnlUtocKP SET isMetodSosdanij = :isMetodSosdanij WHERE grnlutkp_id = :grnlutkp_id");
    q->ParamByName("grnlutkp_id")->AsInteger() = grnlutkp_id;
    q->ParamByName("isMetodSosdanij")->AsInteger() = isMetodSosdanij;
    q->ExecSQL();
    delete q;
}

bool TDB_UtocKP::Get_1_Bit(int grnlutkp_id) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    q->SQL->Clear();
    q->SQL->Add("SELECT isMetodSosdanij FROM GrnlUtocKP WHERE grnlutkp_id = :grnlutkp_id");
    q->ParamByName("grnlutkp_id")->AsInteger() = grnlutkp_id;
    q->Open();
    int isMetodSosdanij = q->FieldByName("isMetodSosdanij")->AsInteger();
   //MAK q->Free();
delete q;
    return (isMetodSosdanij & C1BIT) != 0;
}
