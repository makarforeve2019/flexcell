#include "SystFuncsLogic.h"
#include "unDefDBconnect.h"
#include "DB_Pokaztl.h"
#include "DB_Cell.h"
#include "FormDopInp.h"
#include "DB_Plan.h"
#include "dmGlobalVars.h"
#include "TT_PostuplForPeriod.h"

// создает объект класса TPostuplForPeriod
void TPostuplForPeriod::CreateObjValForPeriod() {
    GlobalVars->PostuplForPeriod = TPostuplForPeriod();
    GlobalVars->PostuplForPeriod.sl = new TStringList;
    //GlobalVars->PostuplForPeriod.sl->Sorted = true;
}

// освобождает объект класса TPostuplForPeriod
void TPostuplForPeriod::FreeObjValForPeriod() {
    for (int i = 0; i < GlobalVars->PostuplForPeriod.sl->Count(); ++i) {
        PrcrdFM rcrdFM = std::get<PrcrdFM>(GlobalVars->PostuplForPeriod.sl->Objects[i]);
        delete rcrdFM;
    }
    delete GlobalVars->PostuplForPeriod.sl;

}

std::string TPostuplForPeriod::CreateKey(int plan_id, int pokaz_id, int vid_otc, int god, int nMesjc, int day, int ist_fin_id) {
    return std::to_string(plan_id) + '_' + std::to_string(pokaz_id) + '_' + std::to_string(vid_otc) + '_' +
           std::to_string(god) + '_' + std::to_string(nMesjc) + '_' + std::to_string(day) + '_' +
           std::to_string(ist_fin_id);
}

// добавить запись
void TPostuplForPeriod::Addrcrd(int plan_id, int pokaz_id, int vid_otc, int god, int nMesjc, int day, int ist_fin_id, int cell_id, double value)
{
    std::string key = CreateKey(plan_id, pokaz_id, vid_otc, god, nMesjc, day, ist_fin_id);

    int index = sl->IndexOf(key);
    if (index != -1) {
        PrcrdFM rcrdFM = std::get<PrcrdFM>(sl->Objects[index]);
        if (vid_otc == 4 && ist_fin_id == 2) rcrdFM->valueUtCB = value;
        else if (vid_otc == 4 && ist_fin_id == 3) rcrdFM->valueUtTB = value;
        else if (vid_otc == 4 && ist_fin_id == 4) rcrdFM->valueUtCS = value;
        else if (vid_otc == 5 && ist_fin_id == 2) rcrdFM->valueFcCB = value;
        else if (vid_otc == 5 && ist_fin_id == 3) rcrdFM->valueFcTB = value;
        else if (vid_otc == 5 && ist_fin_id == 4) rcrdFM->valueFcCS = value;
    } else {
        PrcrdFM rcrdFM;
        rcrdFM->plan_id = plan_id;
        rcrdFM->pokaz_id = pokaz_id;
        rcrdFM->vid_otc = vid_otc;
        rcrdFM->god = god;
        rcrdFM->nMesjc = nMesjc;
        rcrdFM->day = day;
        rcrdFM->ist_fin_id = ist_fin_id;
        rcrdFM->cell_id = cell_id;
        rcrdFM->valueUtCB = 0;
        rcrdFM->valueUtTB = 0;
        rcrdFM->valueUtCS = 0;
        rcrdFM->valueFcCB = 0;
        rcrdFM->valueFcTB = 0;
        rcrdFM->valueFcCS = 0;

        if (vid_otc == 4 && ist_fin_id == 2) rcrdFM->valueUtCB = value;
        else if (vid_otc == 4 && ist_fin_id == 3) rcrdFM->valueUtTB = value;
        else if (vid_otc == 4 && ist_fin_id == 4) rcrdFM->valueUtCS = value;
        else if (vid_otc == 5 && ist_fin_id == 2) rcrdFM->valueFcCB = value;
        else if (vid_otc == 5 && ist_fin_id == 3) rcrdFM->valueFcTB = value;
        else if (vid_otc == 5 && ist_fin_id == 4) rcrdFM->valueFcCS = value;

        sl->AddObject(key, rcrdFM);
    }
}

// вернуть числового значение записи
double TPostuplForPeriod::Getrcrd(int plan_id, int pokaz_id, int vid_otc, TDateTime BegDate, int god, int nMesjc, int day,
                                  int ist_fin_id, int cell_id, int pokaz_is_razd, int pokaz_vert) {
    std::string key = CreateKey(plan_id, pokaz_id, vid_otc, god, nMesjc, day, ist_fin_id);
    int index = sl->IndexOf(key);

    if (pokaz_is_razd == 13) // кортеж
        pokaz_is_razd = 2; // для вычислений это показатель

    if (index != -1) {
        PrcrdFM rcrdFM = std::get<PrcrdFM>(sl->Objects[index]);
        if (vid_otc == 4 && ist_fin_id == 2) return rcrdFM->valueUtCB;
        else if (vid_otc == 4 && ist_fin_id == 3) return rcrdFM->valueUtTB;
        else if (vid_otc == 4 && ist_fin_id == 4) return rcrdFM->valueUtCS;
        else if (vid_otc == 5 && ist_fin_id == 2) return rcrdFM->valueFcCB;
        else if (vid_otc == 5 && ist_fin_id == 3) return rcrdFM->valueFcTB;
        else if (vid_otc == 5 && ist_fin_id == 4) return rcrdFM->valueFcCS;
    }

    if (pokaz_is_razd == 2 && pokaz_vert == 0) {
        switch (vid_otc) {
            case 5:
                return Val_Addrcrd_Det5(plan_id, pokaz_id, vid_otc, BegDate, god, nMesjc, day, ist_fin_id, cell_id);
        }
    }

    if (pokaz_is_razd == 2 && pokaz_vert == 2) {
        switch (vid_otc) {
            case 5:
                return Val_Addrcrd_Det5(plan_id, pokaz_id, vid_otc, BegDate, god, nMesjc, day, ist_fin_id, cell_id);
        }
    }

    if (pokaz_is_razd == 2 && pokaz_vert == 1) {
        return Val_Addrcrd_VtomCisle(plan_id, pokaz_id, vid_otc, BegDate, god, nMesjc, day, ist_fin_id, cell_id);
    }

    if (pokaz_is_razd == 3)
        return Val_Addrcrd_Itogo(plan_id, pokaz_id, vid_otc, BegDate, god, nMesjc, day, ist_fin_id, cell_id);

    if (pokaz_is_razd == 4 && pokaz_vert == 0) {
        switch (vid_otc) {
            case 5:
                return Val_Addrcrd_Det5(plan_id, pokaz_id, vid_otc, BegDate, god, nMesjc, day, ist_fin_id, cell_id);
        }
    }

    if (pokaz_is_razd == 4 && pokaz_vert == 2) {
        switch (vid_otc) {
            case 5:
                return Val_Addrcrd_Det5(plan_id, pokaz_id, vid_otc, BegDate, god, nMesjc, day, ist_fin_id, cell_id);
        }
    }

    if (pokaz_is_razd == 4 && pokaz_vert == 1) {
        return Val_Addrcrd_VtomCisle(plan_id, pokaz_id, vid_otc, BegDate, god, nMesjc, day, ist_fin_id, cell_id);
    }

    if (pokaz_is_razd == 4)
        return Val_Addrcrd_Itogo(plan_id, pokaz_id, vid_otc, BegDate, god, nMesjc, day, ist_fin_id, cell_id);

    return 0.0; // Return default value if no condition matches
}

// вычислить, добавить запись для детальной строки и вернуть значение
double TPostuplForPeriod::Val_Addrcrd_Det5(int plan_id, int pokaz_id, int vid_otc, TDateTime BegDate, int god, int nMesjc, int day,
                                           int ist_fin_id, int cell_id) {
    TQueryUni *q = new TQueryUni(DefDBconnect.MWSModulsConn);
    double Result = 0;
    TDateTime EndDate = EncodeDate(god, nMesjc, day);
    std::string unc;
    q->SQL->Clear();
    q->SQL->Add("SELECT sum(value) as value FROM PostuplPP");
    q->SQL->Add("WHERE PostuplPP.cell_id = :cell_id");
    q->SQL->Add("AND PostuplPP.pokaz_id = :pokaz_id");
    q->SQL->Add("AND PostuplPP.unc = :unc");
    q->SQL->Add("AND vid_otc = :vid_otc");
    q->SQL->Add("AND value_date BETWEEN :BegDate AND :EndDate");
    q->ParamByName("cell_id")->AsInteger() = cell_id;
    q->ParamByName("pokaz_id")->AsInteger() = pokaz_id;
    unc = TDB_Plan::getUNK(plan_id);
    q->ParamByName("unc")->AsString() = unc;
    q->ParamByName("vid_otc")->AsInteger() = vid_otc;
    q->ParamByName("BegDate")->AsDateTime() = BegDate;
    q->ParamByName("EndDate")->AsDateTime() = EndDate;
    q->Open();
    if (!q->EoF()) {
        Result = q->FieldByName("value")->AsFloat();
    }
    Addrcrd(plan_id, pokaz_id, vid_otc, god, nMesjc, day, ist_fin_id, cell_id, Result);
    delete q;
//    TDB_MWSModuls::CloseLinkMWSModuls();
    return Result;
}

// вычислить, добавить запись для групповой строки "в том числе" и вернуть значение
double TPostuplForPeriod::Val_Addrcrd_VtomCisle(int plan_id, int pokaz_id, int vid_otc, TDateTime BegDate, int god, int nMesjc,
                                                  int day, int ist_fin_id, int cell_id) {
    double Result = Val_forOwner(plan_id, pokaz_id, vid_otc, BegDate, god, nMesjc, day, ist_fin_id);
    Addrcrd(plan_id, pokaz_id, vid_otc, god, nMesjc, day, ist_fin_id, cell_id, Result);
    return Result;
}

// вычислить, добавить запись для строки "Итого по разделу" и вернуть значение
double TPostuplForPeriod::Val_Addrcrd_Itogo(int plan_id, int pokaz_id, int vid_otc, TDateTime BegDate, int god, int nMesjc,
                                              int day, int ist_fin_id, int cell_id) {
    int pokaz_owner_id = TDB_Pokaztl::GetPokazOwnerId(pokaz_id);
    double Result = Val_forOwner(plan_id, pokaz_owner_id, vid_otc, BegDate, god, nMesjc, day, ist_fin_id);
    Addrcrd(plan_id, pokaz_id, vid_otc, god, nMesjc, day, ist_fin_id, cell_id, Result);
    return Result;
}

// организовать вычисления подчиненных и вернуть значение
double TPostuplForPeriod::Val_forOwner(int plan_id, int pokaz_owner_id, int vid_otc, TDateTime BegDate, int god, int nMesjc,
                                        int day, int ist_fin_id) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    double Result = 0;

    q->SQL->Clear();
    q->SQL->Add("SELECT b.pokaz_id, b.pokaz_type, b.pokaz_is_razd, b.pokaz_vert, c.cell_id");
    q->SQL->Add("FROM Plan_str a, Pokaztl b, Cell c");
    q->SQL->Add("WHERE a.plan_str_owner_id = :pokaz_owner_id ");
    q->SQL->Add("AND (a.plan_id = 1 OR a.plan_id = :plan_id)");
    q->SQL->Add("AND b.pokaz_id = a.pokaz_id");
    q->SQL->Add("AND b.pokaz_is_razd = 2");
    switch (GetDBType()) {
        case TDBType::MSSQL:
            q->SQL->Add("AND :god BETWEEN YEAR(pokaz_beg_date) AND YEAR(pokaz_end_date)");
            break;
        case TDBType::Postgre:
            q->SQL->Add("AND :god BETWEEN date_part('year', pokaz_beg_date) AND date_part('year', pokaz_end_date)");
            break;
    }
    q->SQL->Add("AND c.pokaz_id = a.pokaz_id");
    q->SQL->Add("AND c.cell_period = 3");
    q->SQL->Add("AND c.ist_fin_id = :ist_fin_id");
    q->SQL->Add("UNION");
    q->SQL->Add("SELECT b.pokaz_id, b.pokaz_type, b.pokaz_is_razd, b.pokaz_vert, 0 as cell_id");
    q->SQL->Add("FROM Plan_str a, Pokaztl b");
    q->SQL->Add("WHERE a.plan_str_owner_id = :pokaz_owner_id1 ");
    q->SQL->Add("AND (a.plan_id = 1 OR a.plan_id = :plan_id1)");
    q->SQL->Add("AND b.pokaz_id = a.pokaz_id");
    q->SQL->Add("AND b.pokaz_is_razd = 1");
    switch (GetDBType()) {
        case TDBType::MSSQL:
            q->SQL->Add("AND :god1 BETWEEN YEAR(pokaz_beg_date) AND YEAR(pokaz_end_date)");
            break;
        case TDBType::Postgre:
            q->SQL->Add("AND :god1 BETWEEN date_part('year', pokaz_beg_date) AND date_part('year', pokaz_end_date)");
            break;
    }
    q->ParamByName("plan_id")->AsInteger() = plan_id;
    q->ParamByName("plan_id1")->AsInteger() = plan_id;
    q->ParamByName("pokaz_owner_id")->AsInteger() = pokaz_owner_id;
    q->ParamByName("pokaz_owner_id1")->AsInteger() = pokaz_owner_id;
    q->ParamByName("ist_fin_id")->AsInteger() = ist_fin_id;
    q->ParamByName("god")->AsInteger() = god;
    q->ParamByName("god1")->AsInteger() = god;
    q->Open();

    while (!q->EoF()) {
        if (q->FieldByName("pokaz_is_razd")->AsInteger() == 1) {
            int w_pokaz_id = TDB_Pokaztl::GetIdItogoRazd(q->FieldByName("pokaz_id")->AsInteger());
            if (w_pokaz_id > 0) {
                int w_cell_id = TDB_Cell::GetCellId(w_pokaz_id, 3, ist_fin_id);
                Result += GlobalVars->PostuplForPeriod.Getrcrd(plan_id, w_pokaz_id, vid_otc, BegDate, god, nMesjc, day, ist_fin_id, w_cell_id, 3, 0);
            }
        } else if (q->FieldByName("pokaz_is_razd")->AsInteger() == 2 && q->FieldByName("pokaz_type")->AsInteger() == 1) {
            if (q->FieldByName("pokaz_vert")->AsInteger() == 0) {
                Result += GlobalVars->PostuplForPeriod.Getrcrd(plan_id, q->FieldByName("pokaz_id")->AsInteger(), vid_otc, BegDate, god, nMesjc, day, ist_fin_id, q->FieldByName("cell_id")->AsInteger(), 2, 0);
            } else if (q->FieldByName("pokaz_vert")->AsInteger() == 1) {
                Result += GlobalVars->PostuplForPeriod.Getrcrd(plan_id, q->FieldByName("pokaz_id")->AsInteger(), vid_otc, BegDate, god, nMesjc, day, ist_fin_id, q->FieldByName("cell_id")->AsInteger(), 2, 1);
            }
        }
        q->Next();
    }

    delete q;
    return Result;
}


