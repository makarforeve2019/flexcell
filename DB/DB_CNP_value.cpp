#include "DB_CNP_value.h"
#include "unDefDBconnect.h"
#include "TT_SummaBySubjects.h"

void TDB_CNP_value::DelItemsForPokaz(int pokaz_id) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    q->SQL->Clear();
    q->SQL->Add("DELETE FROM CNP_value");
    q->SQL->Add("WHERE pokaz_id = :pokaz_id");
    q->ParamByName("pokaz_id")->AsInteger() = pokaz_id;
    q->ExecSQL();
   //MAK q->Free();
delete q;
}

void TDB_CNP_value::GetDataForObject(TQueryUni *qIn, int pokaz_id, int plan_id) {
    qIn->SQL->Clear();
    qIn->SQL->Add("SELECT * FROM CNP_value");
    qIn->SQL->Add("WHERE pokaz_id = :pokaz_id");
    qIn->SQL->Add("AND plan_id = :plan_id");
    qIn->ParamByName("pokaz_id")->AsInteger() = pokaz_id;
    qIn->ParamByName("plan_id")->AsInteger() = plan_id;
    qIn->Open();
}

bool TDB_CNP_value::getDocumId(int pokaz_id, int comnp_id, int vid_period, int plan_id,
                               int vid_otc, short god, int kvrtl, int mesjc, short den, std::string &DocumId) {
    TQueryUni *qW = new TQueryUni(DefDBconnect.Conn);
    bool Result = false;
    qW->SQL->Add("SELECT docum_id");
    qW->SQL->Add("FROM CNP_value");
    qW->SQL->Add("WHERE pokaz_id = :pokaz_id");
    qW->SQL->Add("AND comnp_id = :comnp_id");
    qW->SQL->Add("AND vid_period = :vid_period");
    qW->SQL->Add("AND plan_id = :plan_id");
    qW->SQL->Add("AND vid_otc = :vid_otc");
    qW->SQL->Add("AND god = :god");
    qW->SQL->Add("AND kvrtl = :kvrtl");
    qW->SQL->Add("AND mesjc = :mesjc");
    qW->SQL->Add("AND den = :den");
    qW->ParamByName("pokaz_id")->AsInteger() = pokaz_id;
    qW->ParamByName("comnp_id")->AsInteger() = comnp_id;
    qW->ParamByName("vid_period")->AsInteger() = vid_period;
    qW->ParamByName("plan_id")->AsInteger() = plan_id;
    qW->ParamByName("vid_otc")->AsInteger() = vid_otc;
    qW->ParamByName("god")->AsInteger() = god;
    qW->ParamByName("kvrtl")->AsInteger() = kvrtl;
    qW->ParamByName("mesjc")->AsInteger() = mesjc;
    qW->ParamByName("den")->AsInteger() = den;
    qW->Open();
    if (!qW->EoF()) {
        DocumId = qW->FieldByName("docum_id")->AsString();
        Result = true;
    }
    qW->Free();
    return Result;
}

bool TDB_CNP_value::getNumberItem(int pokaz_id, int comnp_id, int vid_period, int plan_id,
                                  int vid_otc, short god, int kvrtl, int mesjc, short den, double &value) {
    TracerManager::Instance().StartFunctionTrace("TDB_CNP_value::getNumberItem");
    TQueryUni *qW = new TQueryUni(DefDBconnect.Conn);
    bool Result = false;
    qW->SQL->Add("SELECT value");
    qW->SQL->Add("FROM CNP_value");
    qW->SQL->Add("WHERE pokaz_id = :pokaz_id");
    qW->SQL->Add("AND comnp_id = :comnp_id");
    qW->SQL->Add("AND vid_period = :vid_period");
    qW->SQL->Add("AND plan_id = :plan_id");
    qW->SQL->Add("AND vid_otc = :vid_otc");
    qW->SQL->Add("AND god = :god");
    qW->SQL->Add("AND kvrtl = :kvrtl");
    qW->SQL->Add("AND mesjc = :mesjc");
    qW->SQL->Add("AND den = :den");
    qW->ParamByName("pokaz_id")->AsInteger() = pokaz_id;
    qW->ParamByName("comnp_id")->AsInteger() = comnp_id;
    qW->ParamByName("vid_period")->AsInteger() = vid_period;
    qW->ParamByName("plan_id")->AsInteger() = plan_id;
    qW->ParamByName("vid_otc")->AsInteger() = vid_otc;
    qW->ParamByName("god")->AsInteger() = god;
    qW->ParamByName("kvrtl")->AsInteger() = kvrtl;
    qW->ParamByName("mesjc")->AsInteger() = mesjc;
    qW->ParamByName("den")->AsInteger() = den;
    qW->Open();
    if (!qW->EoF()) {
        value = qW->FieldByName("value")->AsFloat();
        Result = true;
    }
    qW->Free();
    TracerManager::Instance().EndFunctionTrace();
    return Result;
}

void TDB_CNP_value::addNumberItem(int pokaz_id, int comnp_id, int vid_period, int plan_id,
                                  int vid_otc, short god, int kvrtl, int mesjc, short den, double value) {
    TracerManager::Instance().StartFunctionTrace("TDB_CNP_value::addNumberItem");
    double wvalue;
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    if (getNumberItem(pokaz_id, comnp_id, vid_period, plan_id, vid_otc, god, kvrtl, mesjc, den, wvalue)) {
        q->SQL->Clear();
        q->SQL->Add("UPDATE CNP_value");
        q->SQL->Add("SET value=:value");
        q->SQL->Add("WHERE pokaz_id = :pokaz_id");
        q->SQL->Add("AND comnp_id = :comnp_id");
        q->SQL->Add("AND vid_period = :vid_period");
        q->SQL->Add("AND plan_id = :plan_id");
        q->SQL->Add("AND vid_otc = :vid_otc");
        q->SQL->Add("AND god = :god");
        q->SQL->Add("AND kvrtl = :kvrtl");
        q->SQL->Add("AND mesjc = :mesjc");
        q->SQL->Add("AND den = :den");
        q->ParamByName("pokaz_id")->AsInteger() = pokaz_id;
        q->ParamByName("comnp_id")->AsInteger() = comnp_id;
        q->ParamByName("vid_period")->AsInteger() = vid_period;
        q->ParamByName("plan_id")->AsInteger() = plan_id;
        q->ParamByName("vid_otc")->AsInteger() = vid_otc;
        q->ParamByName("god")->AsInteger() = god;
        q->ParamByName("kvrtl")->AsInteger() = kvrtl;
        q->ParamByName("mesjc")->AsInteger() = mesjc;
        q->ParamByName("den")->AsInteger() = den;
        q->ParamByName("value")->AsFloat() = value;
        q->ExecSQL();
    } else {
        q->SQL->Clear();
        q->SQL->Add("INSERT INTO CNP_value");
        q->SQL->Add("(pokaz_id,comnp_id,vid_period,plan_id,vid_otc,god,kvrtl,mesjc,den,value)");
        q->SQL->Add("VALUES (:pokaz_id,:comnp_id,:vid_period,:plan_id,:vid_otc,:god,:kvrtl,:mesjc,:den,:value)");
        q->ParamByName("pokaz_id")->AsInteger() = pokaz_id;
        q->ParamByName("comnp_id")->AsInteger() = comnp_id;
        q->ParamByName("vid_period")->AsInteger() = vid_period;
        q->ParamByName("plan_id")->AsInteger() = plan_id;
        q->ParamByName("vid_otc")->AsInteger() = vid_otc;
        q->ParamByName("god")->AsInteger() = god;
        q->ParamByName("kvrtl")->AsInteger() = kvrtl;
        q->ParamByName("mesjc")->AsInteger() = mesjc;
        q->ParamByName("den")->AsInteger() = den;
        q->ParamByName("value")->AsFloat() = value;
        q->ExecSQL();
    }
    delete q;
    TracerManager::Instance().EndFunctionTrace();
}

bool TDB_CNP_value::getNumberItemallPlans(int pokaz_id, int comnp_id, int vid_period, int plan_id,
                                          int vid_otc, short god, int kvrtl, int mesjc, short den, double &value) {
    TQueryUni *qW = new TQueryUni(DefDBconnect.Conn);
    bool Result = false;
    std::string strPlans;
    qW->SQL->Add("SELECT SUM(value) as value");
    qW->SQL->Add("FROM CNP_value");
    qW->SQL->Add("WHERE pokaz_id = :pokaz_id");
    qW->SQL->Add("AND comnp_id = :comnp_id");
    qW->SQL->Add("AND vid_period = :vid_period");

    strPlans = TSummaBySubjects::getPlanIds(god);
    strPlans = "AND plan_id IN (" + strPlans + ")";
    qW->SQL->Add(strPlans);

    qW->SQL->Add("AND vid_otc = :vid_otc");
    qW->SQL->Add("AND god = :god");
    qW->SQL->Add("AND kvrtl = :kvrtl");
    qW->SQL->Add("AND mesjc = :mesjc");
    qW->SQL->Add("AND den = :den");
    qW->ParamByName("pokaz_id")->AsInteger() = pokaz_id;
    qW->ParamByName("comnp_id")->AsInteger() = comnp_id;
    qW->ParamByName("vid_period")->AsInteger() = vid_period;

    qW->ParamByName("vid_otc")->AsInteger() = vid_otc;
    qW->ParamByName("god")->AsInteger() = god;
    qW->ParamByName("kvrtl")->AsInteger() = kvrtl;
    qW->ParamByName("mesjc")->AsInteger() = mesjc;
    qW->ParamByName("den")->AsInteger() = den;
    qW->Open();
    if (!qW->EoF()) {
        value = qW->FieldByName("value")->AsFloat();
        Result = true;
    }
    qW->Free();
    return Result;
}

void TDB_CNP_value::addStringItem(int pokaz_id, int comnp_id, int vid_period, int plan_id,
                                  int vid_otc, short god, int kvrtl, int mesjc, short den, const std::string &str_value) {
    std::string wstr_value;
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    if (TDB_CNP_value::getStringItem(pokaz_id, comnp_id, vid_period, plan_id, vid_otc, god, kvrtl, mesjc, den, wstr_value)) {
        q->SQL->Clear();
        q->SQL->Add("UPDATE CNP_value");
        q->SQL->Add("SET str_value=:str_value");
        q->SQL->Add("WHERE pokaz_id = :pokaz_id");
        q->SQL->Add("AND comnp_id = :comnp_id");
        q->SQL->Add("AND vid_period = :vid_period");
        q->SQL->Add("AND plan_id = :plan_id");
        q->SQL->Add("AND vid_otc = :vid_otc");
        q->SQL->Add("AND god = :god");
        q->SQL->Add("AND kvrtl = :kvrtl");
        q->SQL->Add("AND mesjc = :mesjc");
        q->SQL->Add("AND den = :den");
        q->ParamByName("pokaz_id")->AsInteger() = pokaz_id;
        q->ParamByName("comnp_id")->AsInteger() = comnp_id;
        q->ParamByName("vid_period")->AsInteger() = vid_period;
        q->ParamByName("plan_id")->AsInteger() = plan_id;
        q->ParamByName("vid_otc")->AsInteger() = vid_otc;
        q->ParamByName("god")->AsInteger() = god;
        q->ParamByName("kvrtl")->AsInteger() = kvrtl;
        q->ParamByName("mesjc")->AsInteger() = mesjc;
        q->ParamByName("den")->AsInteger() = den;
        q->ParamByName("str_value")->AsString() = str_value;
        q->ExecSQL();
    } else {
        q->SQL->Clear();
        q->SQL->Add("INSERT INTO CNP_value");
        q->SQL->Add("(pokaz_id, comnp_id, vid_period, plan_id, vid_otc, god, kvrtl, mesjc, den, str_value)");
        q->SQL->Add("VALUES (:pokaz_id, :comnp_id, :vid_period, :plan_id, :vid_otc, :god, :kvrtl, :mesjc, :den, :str_value)");
        q->ParamByName("pokaz_id")->AsInteger() = pokaz_id;
        q->ParamByName("comnp_id")->AsInteger() = comnp_id;
        q->ParamByName("vid_period")->AsInteger() = vid_period;
        q->ParamByName("plan_id")->AsInteger() = plan_id;
        q->ParamByName("vid_otc")->AsInteger() = vid_otc;
        q->ParamByName("god")->AsInteger() = god;
        q->ParamByName("kvrtl")->AsInteger() = kvrtl;
        q->ParamByName("mesjc")->AsInteger() = mesjc;
        q->ParamByName("den")->AsInteger() = den;
        q->ParamByName("str_value")->AsString() = str_value;
        q->ExecSQL();
    }
   //MAK q->Free();
delete q;
}

bool TDB_CNP_value::getStringItem(int pokaz_id, int comnp_id, int vid_period, int plan_id,
                                  int vid_otc, short god, int kvrtl, int mesjc, short den, std::string &str_value) {
    TQueryUni *qW = new TQueryUni(DefDBconnect.Conn);
    bool result = false;
    qW->SQL->Add("SELECT str_value");
    qW->SQL->Add("FROM CNP_value");
    qW->SQL->Add("WHERE pokaz_id = :pokaz_id");
    qW->SQL->Add("AND comnp_id = :comnp_id");
    qW->SQL->Add("AND vid_period = :vid_period");
    qW->SQL->Add("AND plan_id = :plan_id");
    qW->SQL->Add("AND vid_otc = :vid_otc");
    qW->SQL->Add("AND god = :god");
    qW->SQL->Add("AND kvrtl = :kvrtl");
    qW->SQL->Add("AND mesjc = :mesjc");
    qW->SQL->Add("AND den = :den");
    qW->ParamByName("pokaz_id")->AsInteger() = pokaz_id;
    qW->ParamByName("comnp_id")->AsInteger() = comnp_id;
    qW->ParamByName("vid_period")->AsInteger() = vid_period;
    qW->ParamByName("plan_id")->AsInteger() = plan_id;
    qW->ParamByName("vid_otc")->AsInteger() = vid_otc;
    qW->ParamByName("god")->AsInteger() = god;
    qW->ParamByName("kvrtl")->AsInteger() = kvrtl;
    qW->ParamByName("mesjc")->AsInteger() = mesjc;
    qW->ParamByName("den")->AsInteger() = den;
    qW->Open();
    if (!qW->EoF()) {
        str_value = qW->FieldByName("str_value")->AsString();
        result = true;
    }
    qW->Free();
    return result;
}

void TDB_CNP_value::DelOnlyValue(int pokaz_id, int comnp_id, int vid_period, int plan_id,
                                 int vid_otc, short god, int kvrtl, int mesjc, short den) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    q->SQL->Clear();
    q->SQL->Add("DELETE FROM CNP_value");
    q->SQL->Add("WHERE pokaz_id = :pokaz_id");
    q->SQL->Add("AND comnp_id = :comnp_id");
    q->SQL->Add("AND vid_period = :vid_period");
    q->SQL->Add("AND plan_id = :plan_id");
    q->SQL->Add("AND vid_otc = :vid_otc");
    q->SQL->Add("AND god = :god");
    q->SQL->Add("AND kvrtl = :kvrtl");
    q->SQL->Add("AND mesjc = :mesjc");
    q->SQL->Add("AND den = :den");
    q->ParamByName("pokaz_id")->AsInteger() = pokaz_id;
    q->ParamByName("comnp_id")->AsInteger() = comnp_id;
    q->ParamByName("vid_period")->AsInteger() = vid_period;
    q->ParamByName("plan_id")->AsInteger() = plan_id;
    q->ParamByName("vid_otc")->AsInteger() = vid_otc;
    q->ParamByName("god")->AsInteger() = god;
    q->ParamByName("kvrtl")->AsInteger() = kvrtl;
    q->ParamByName("mesjc")->AsInteger() = mesjc;
    q->ParamByName("den")->AsInteger() = den;
    q->ExecSQL();
   //MAK q->Free();
delete q;
}

void TDB_CNP_value::addNote(const std::string &Note, int pokaz_id, int comnp_id, int vid_period, int plan_id,
                            int vid_otc, short god, int kvrtl, int mesjc, short den) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    q->SQL->Clear();
    q->SQL->Add("UPDATE CNP_value");
    q->SQL->Add("SET Note = :Note");
    q->SQL->Add("WHERE pokaz_id = :pokaz_id");
    q->SQL->Add("AND comnp_id = :comnp_id");
    q->SQL->Add("AND vid_period = :vid_period");
    q->SQL->Add("AND plan_id = :plan_id");
    q->SQL->Add("AND vid_otc = :vid_otc");
    q->SQL->Add("AND god = :god");
    q->SQL->Add("AND kvrtl = :kvrtl");
    q->SQL->Add("AND mesjc = :mesjc");
    q->SQL->Add("AND den = :den");
    q->ParamByName("pokaz_id")->AsInteger() = pokaz_id;
    q->ParamByName("comnp_id")->AsInteger() = comnp_id;
    q->ParamByName("vid_period")->AsInteger() = vid_period;
    q->ParamByName("plan_id")->AsInteger() = plan_id;
    q->ParamByName("vid_otc")->AsInteger() = vid_otc;
    q->ParamByName("god")->AsInteger() = god;
    q->ParamByName("kvrtl")->AsInteger() = kvrtl;
    q->ParamByName("mesjc")->AsInteger() = mesjc;
    q->ParamByName("den")->AsInteger() = den;
    q->ParamByName("Note")->AsString() = Note;
    q->ExecSQL();
   //MAK q->Free();
delete q;
}

std::string TDB_CNP_value::GetNote(int pokaz_id, int comnp_id, int vid_period, int plan_id,
                                   int vid_otc, short god, int kvrtl, int mesjc, short den) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    std::string Result = "";
    q->SQL->Add("SELECT note");
    q->SQL->Add("FROM CNP_value");
    q->SQL->Add("WHERE pokaz_id = :pokaz_id");
    q->SQL->Add("AND comnp_id = :comnp_id");
    q->SQL->Add("AND vid_period = :vid_period");
    q->SQL->Add("AND plan_id = :plan_id");
    q->SQL->Add("AND vid_otc = :vid_otc");
    q->SQL->Add("AND god = :god");
    q->SQL->Add("AND kvrtl = :kvrtl");
    q->SQL->Add("AND mesjc = :mesjc");
    q->SQL->Add("AND den = :den");
    q->ParamByName("pokaz_id")->AsInteger() = pokaz_id;
    q->ParamByName("comnp_id")->AsInteger() = comnp_id;
    q->ParamByName("vid_period")->AsInteger() = vid_period;
    q->ParamByName("plan_id")->AsInteger() = plan_id;
    q->ParamByName("vid_otc")->AsInteger() = vid_otc;
    q->ParamByName("god")->AsInteger() = god;
    q->ParamByName("kvrtl")->AsInteger() = kvrtl;
    q->ParamByName("mesjc")->AsInteger() = mesjc;
    q->ParamByName("den")->AsInteger() = den;
    q->Open();
    if (!q->EoF()) {
        Result = q->FieldByName("note")->AsString();
    }
   //MAK q->Free();
delete q;
    return Result;
}
