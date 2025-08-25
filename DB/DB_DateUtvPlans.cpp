#include "unDefDBconnect.h"
#include "DB_DateUtvPlans.h"

void TDB_DateUtvPlans::GetAllGodPokazd(TQueryUni *qIn) {
    qIn->SQL->Clear();
 
    qIn->SQL->Add("SELECT DISTINCT god, pokaz_id, pokaz_name");
    qIn->SQL->Add("FROM DateUtvPlans, Pokaztl");
    qIn->SQL->Add("WHERE cast_id = pokaz_id");
    qIn->SQL->Add("ORDER BY god");
    qIn->Open();
}

void TDB_DateUtvPlans::GetAllGodPokazd2(TQueryUni *qIn, int god) {
    qIn->SQL->Clear();
 
    qIn->SQL->Add("SELECT DISTINCT god, pokaz_id, pokaz_name");
    qIn->SQL->Add("FROM DateUtvPlans, Pokaztl");
    qIn->SQL->Add("WHERE cast_id = pokaz_id");
    qIn->SQL->Add("AND god = :god");
    qIn->ParamByName("god")->AsInteger() = god;
    qIn->Open();
}

bool TDB_DateUtvPlans::GetGodPlanCast(TQueryUni *qIn, int plan_id, int cast_id, int god) {
    qIn->SQL->Clear();
 
    qIn->SQL->Add("SELECT *");
    qIn->SQL->Add("FROM DateUtvPlans");
    qIn->SQL->Add("WHERE plan_id = :plan_id");
    qIn->SQL->Add("AND cast_id = :cast_id");
    qIn->SQL->Add("AND god = :god");
    qIn->ParamByName("plan_id")->AsInteger() = plan_id;
    qIn->ParamByName("cast_id")->AsInteger() = cast_id;
    qIn->ParamByName("god")->AsInteger() = god;
    qIn->Open();
    return !qIn->EoF();
}

void TDB_DateUtvPlans::GetAllItems(TQueryUni *qIn, int God, int pokaz_id) {
    qIn->SQL->Clear();
 
    qIn->SQL->Add("SELECT Plans.plan_id, plan_name, date_utv, plan_order, 1 as isEst");
    qIn->SQL->Add("FROM DateUtvPlans, Plans");
    qIn->SQL->Add("WHERE god = :god");
    qIn->SQL->Add("AND cast_id = :pokaz_id");
    qIn->SQL->Add("AND DateUtvPlans.plan_id = Plans.plan_id");
    qIn->SQL->Add("UNION");
    qIn->SQL->Add("SELECT plan_id, plan_name, :dt as date_utv, plan_order, 0 as isEst");
    qIn->SQL->Add("FROM Plans p");
    qIn->SQL->Add("WHERE plan_id > 1");
    qIn->SQL->Add("AND plan_is_rajon > 0");
    qIn->SQL->Add("AND NOT EXISTS");
    qIn->SQL->Add("(SELECT *");
    qIn->SQL->Add("FROM DateUtvPlans du");
    qIn->SQL->Add("WHERE god = :god1");
    qIn->SQL->Add("AND cast_id = :pokaz_id1");
    qIn->SQL->Add("AND du.plan_id = p.plan_id)");
    qIn->SQL->Add("ORDER BY plan_order");
    qIn->ParamByName("god")->AsInteger() = God;
    qIn->ParamByName("god1")->AsInteger() = God;
    qIn->ParamByName("pokaz_id")->AsInteger() = pokaz_id;
    qIn->ParamByName("pokaz_id1")->AsInteger() = pokaz_id;
    qIn->ParamByName("dt")->AsDateTime() = Date();
    qIn->Open();
}

void TDB_DateUtvPlans::AddOrUpdItem(int plan_id, int cast_id, int god, TDateTime date_utv, TDateTime date_copy) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    int dup_id = TDB_DateUtvPlans::FindItem(plan_id, cast_id, god);
    if (dup_id == 0)
        TDB_DateUtvPlans::AddItem(plan_id, cast_id, god, date_utv, date_copy);
    else {
        q->SQL->Clear();
        q->SQL->Add("UPDATE DateUtvPlans");
        q->SQL->Add("SET date_copy = :date_copy, date_utv = :date_utv");
        q->SQL->Add("WHERE dup_id = :dup_id");
        q->ParamByName("dup_id")->AsInteger() = dup_id;
        q->ParamByName("date_copy")->AsDateTime() = date_copy;
        q->ParamByName("date_utv")->AsDateTime() = date_utv;
        q->ExecSQL();
    }
   //MAK q->Free();
delete q;
}

void TDB_DateUtvPlans::AddItem(int plan_id, int cast_id, int god, TDateTime date_utv, TDateTime date_copy) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    q->SQL->Clear();
    q->SQL->Add("INSERT INTO DateUtvPlans");
    q->SQL->Add("(plan_id, cast_id, god, date_utv, date_copy)");
    q->SQL->Add("VALUES (:plan_id, :cast_id, :god,:date_utv, :date_copy)");
    q->ParamByName("plan_id")->AsInteger() = plan_id;
    q->ParamByName("cast_id")->AsInteger() = cast_id;
    q->ParamByName("god")->AsInteger() = god;
    q->ParamByName("date_utv")->AsDateTime() = date_utv;
    q->ParamByName("date_copy")->AsDateTime() = date_copy;
    q->ExecSQL();
   //MAK q->Free();
delete q;
}

void TDB_DateUtvPlans::AddItem2(int plan_id, int cast_id, int god, TDateTime date_utv, int month, std::string podpisFIO, std::string dlgnost) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    q->SQL->Clear();
    q->SQL->Add("INSERT INTO DateUtvPlans");
    q->SQL->Add("(plan_id, cast_id, god, date_utv, month, podpisFIO, dlgnost)");
    q->SQL->Add("VALUES (:plan_id, :cast_id, :god, :date_utv, :month, :podpisFIO, :dlgnost)");
    q->ParamByName("plan_id")->AsInteger() = plan_id;
    q->ParamByName("cast_id")->AsInteger() = cast_id;
    q->ParamByName("god")->AsInteger() = god;
    q->ParamByName("date_utv")->AsDateTime() = date_utv;
    q->ParamByName("month")->AsInteger() = month;
    q->ParamByName("podpisFIO")->AsString() = podpisFIO.c_str();
    q->ParamByName("dlgnost")->AsString() = dlgnost.c_str();
    q->ExecSQL();
   //MAK q->Free();
delete q;
}

void TDB_DateUtvPlans::UpdItem(int plan_id, int cast_id, int god, TDateTime date_utv) {
    int dup_id = TDB_DateUtvPlans::FindItem(plan_id, cast_id, god);
    if (dup_id == 0) return;

    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    q->SQL->Clear();
    q->SQL->Add("UPDATE DateUtvPlans");
    q->SQL->Add("SET date_utv = :date_utv");
    q->SQL->Add("WHERE dup_id = :dup_id");
    q->ParamByName("dup_id")->AsInteger() = dup_id;
    q->ParamByName("date_utv")->AsDateTime() = date_utv;
    q->ExecSQL();
   //MAK q->Free();
delete q;
}

int TDB_DateUtvPlans::FindItem(int plan_id, int cast_id, int god) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    int result = 0;
    q->SQL->Clear();
    q->SQL->Add("SELECT *");
    q->SQL->Add("FROM DateUtvPlans");
    q->SQL->Add("WHERE plan_id = :plan_id");
    q->SQL->Add("AND cast_id = :cast_id");
    q->SQL->Add("AND god = :god");
    q->ParamByName("plan_id")->AsInteger() = plan_id;
    q->ParamByName("cast_id")->AsInteger() = cast_id;
    q->ParamByName("god")->AsInteger() = god;
    q->Open();
    if (!q->EoF()) {
        result = q->FieldByName("dup_id")->AsInteger();
    }
   //MAK q->Free();
delete q;
    return result;
}

int TDB_DateUtvPlans::FindItem1(int plan_id, int cast_id, int god, int month, TDateTime &date_utv) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    int result = 0;
    q->SQL->Clear();
    q->SQL->Add("SELECT *");
    q->SQL->Add("FROM DateUtvPlans");
    q->SQL->Add("WHERE plan_id = :plan_id");
    q->SQL->Add("AND cast_id = :cast_id");
    q->SQL->Add("AND god = :god");
    q->SQL->Add("AND month = :month");
    q->ParamByName("plan_id")->AsInteger() = plan_id;
    q->ParamByName("cast_id")->AsInteger() = cast_id;
    q->ParamByName("god")->AsInteger() = god;
    q->ParamByName("month")->AsInteger() = month;
    q->Open();
    if (!q->EoF()) {
        date_utv = q->FieldByName("date_utv")->AsDateTime();
        result = q->FieldByName("dup_id")->AsInteger();
    }
   //MAK q->Free();
delete q;
    return result;
}

void TDB_DateUtvPlans::DelItem(int god, int cast_id) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    q->SQL->Clear();
    q->SQL->Add("DELETE FROM DateUtvPlans");
    q->SQL->Add("WHERE god = :god");
    q->SQL->Add("AND cast_id = :cast_id");
    q->ParamByName("god")->AsInteger() = god;
    q->ParamByName("cast_id")->AsInteger() = cast_id;
    q->ExecSQL();
   //MAK q->Free();
delete q;
}

void TDB_DateUtvPlans::DelItem1(int plan_id, int cast_id, int god) {
    int dup_id = TDB_DateUtvPlans::FindItem(plan_id, cast_id, god);
    if (dup_id == 0) return;

    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    q->SQL->Clear();
    q->SQL->Add("DELETE FROM DateUtvPlans");
    q->SQL->Add("WHERE dup_id = :dup_id");
    q->ParamByName("dup_id")->AsInteger() = dup_id;
    q->ExecSQL();
   //MAK q->Free();
delete q;
}
