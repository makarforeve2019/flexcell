
#include "SystFuncsLogic.h"
#include "unDefDBconnect.h"
#include "DB_Pokaztl.h"
#include "DB_Cell.h"
#include "FormDopInp.h"
#include "DB_DenegnPotok.h"
#include "unDBType.h"

#include "TT_ValForPeriod.h"

TStringList* TValForPeriod::sl = nullptr;

// создает объект класса TValForPeriod
void TValForPeriod::CreateObjValForPeriod() {
    GlobalVars->ValForPeriod = TValForPeriod();
    GlobalVars->ValForPeriod.sl = new TStringList();
    //GlobalVars->ValForPeriod.sl->Sorted = true;
}

 // освобождает объект класса TValForPeriod
void TValForPeriod::FreeObjValForPeriod() {
    int to = GlobalVars->ValForPeriod.sl->Count();
    for (int i = 0; i < to; ++i) {
        TrcrdFM *rcrdFM = std::get<TrcrdFM*>(GlobalVars->ValForPeriod.sl->Objects[i]);
        delete rcrdFM;
    }

    delete GlobalVars->ValForPeriod.sl;
 }

std::string TValForPeriod::CreateKey(int plan_id, int objectid, int pokaz_id, int vid_otc, int god, int nMesjc, int day, int ist_fin_id) {
    return std::to_string(plan_id) + "_" + std::to_string(objectid) + "_" + std::to_string(pokaz_id) + "_" +
           std::to_string(vid_otc) + "_" + std::to_string(god) + "_" + std::to_string(nMesjc) + "_" +
           std::to_string(day) + "_" + std::to_string(ist_fin_id);
}

// добавить запись
void TValForPeriod::Addrcrd(int plan_id, int objectid, int pokaz_id, int vid_otc, int god, int nMesjc, int day,
                            int ist_fin_id, int cell_id, double value) {
    std::string key = std::to_string(plan_id) + "_" + std::to_string(objectid) + "_" +
                      std::to_string(pokaz_id) + "_" + std::to_string(vid_otc) + "_" +
                      std::to_string(god) + "_" + std::to_string(nMesjc) + "_" +
                      std::to_string(day) + "_" + std::to_string(ist_fin_id);
    int Index;
    if (sl->Find(key, Index)) {
        TrcrdFM* rcrdFM = std::get<PrcrdFM>(sl->Objects[Index]);
        if (vid_otc == 4 && ist_fin_id == 2) rcrdFM->valueUtCB = value;
        else if (vid_otc == 4 && ist_fin_id == 3) rcrdFM->valueUtTB = value;
        else if (vid_otc == 4 && ist_fin_id == 4) rcrdFM->valueUtCS = value;
        else if (vid_otc == 5 && ist_fin_id == 2) rcrdFM->valueFcCB = value;
        else if (vid_otc == 5 && ist_fin_id == 3) rcrdFM->valueFcTB = value;
        else if (vid_otc == 5 && ist_fin_id == 4) rcrdFM->valueFcCS = value;
    } else {
        TrcrdFM* rcrdFM = new TrcrdFM();
        rcrdFM->plan_id = plan_id;
        rcrdFM->objectid = objectid;
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
double TValForPeriod::Getrcrd(int plan_id, int objectid, int pokaz_id, int vid_otc, int god, int nMesjc, int day,
                              int ist_fin_id, int cell_id, int pokaz_is_razd, int pokaz_vert) {
    if (pokaz_vert == 3)
        pokaz_vert = 0;

    if (pokaz_is_razd == 13)
        pokaz_is_razd = 2;

    std::string key = CreateKey(plan_id, objectid, pokaz_id, vid_otc, god, nMesjc, day, ist_fin_id);
    int Index;
    if (sl->Find(key, Index)) {
        TrcrdFM* rcrdFM = std::get<PrcrdFM>(sl->Objects[Index]);
        if (vid_otc == 4 && ist_fin_id == 2) return rcrdFM->valueUtCB;
        else if (vid_otc == 4 && ist_fin_id == 3) return rcrdFM->valueUtTB;
        else if (vid_otc == 4 && ist_fin_id == 4) return rcrdFM->valueUtCS;
        else if (vid_otc == 5 && ist_fin_id == 2) return rcrdFM->valueFcCB;
        else if (vid_otc == 5 && ist_fin_id == 3) return rcrdFM->valueFcTB;
        else if (vid_otc == 5 && ist_fin_id == 4) return rcrdFM->valueFcCS;
    }

    if (pokaz_is_razd == 2 && pokaz_vert == 0) {
        switch (vid_otc) {
            case 4: return Val_Addrcrd_Det4(plan_id, objectid, pokaz_id, vid_otc, god, nMesjc, day, ist_fin_id, cell_id);
            case 5: return Val_Addrcrd_Det5(plan_id, objectid, pokaz_id, vid_otc, god, nMesjc, day, ist_fin_id, cell_id);
        }
    }

    if (pokaz_is_razd == 2 && pokaz_vert == 2) {
        switch (vid_otc) {
            case 4: return Val_Addrcrd_Det4(plan_id, objectid, pokaz_id, vid_otc, god, nMesjc, day, ist_fin_id, cell_id);
            case 5: return Val_Addrcrd_Det5(plan_id, objectid, pokaz_id, vid_otc, god, nMesjc, day, ist_fin_id, cell_id);
        }
    }

    if (pokaz_is_razd == 2 && pokaz_vert == 1) {
        return Val_Addrcrd_VtomCisle(plan_id, objectid, pokaz_id, vid_otc, god, nMesjc, day, ist_fin_id, cell_id);
    }

    if (pokaz_is_razd == 3) {
        return Val_Addrcrd_Itogo(plan_id, objectid, pokaz_id, vid_otc, god, nMesjc, day, ist_fin_id, cell_id);
    }

    if (pokaz_is_razd == 4 && pokaz_vert == 0) {
        switch (vid_otc) {
            case 4: return Val_Addrcrd_Det4(plan_id, objectid, pokaz_id, vid_otc, god, nMesjc, day, ist_fin_id, cell_id);
            case 5: return Val_Addrcrd_Det5(plan_id, objectid, pokaz_id, vid_otc, god, nMesjc, day, ist_fin_id, cell_id);
        }
    }

    if (pokaz_is_razd == 4 && pokaz_vert == 2) {
        switch (vid_otc) {
            case 4: return Val_Addrcrd_Det4(plan_id, objectid, pokaz_id, vid_otc, god, nMesjc, day, ist_fin_id, cell_id);
            case 5: return Val_Addrcrd_Det5(plan_id, objectid, pokaz_id, vid_otc, god, nMesjc, day, ist_fin_id, cell_id);
        }
    }

    if (pokaz_is_razd == 4 && pokaz_vert == 1) {
        return Val_Addrcrd_VtomCisle(plan_id, objectid, pokaz_id, vid_otc, god, nMesjc, day, ist_fin_id, cell_id);
    }

    if (pokaz_is_razd == 4) {
        return Val_Addrcrd_Itogo(plan_id, objectid, pokaz_id, vid_otc, god, nMesjc, day, ist_fin_id, cell_id);
    }

    return 0;
}

// вычислить, добавить запись для детальной строки и вернуть значение
double TValForPeriod::Val_Addrcrd_Det5(int plan_id, int objectid, int pokaz_id, int vid_otc, int god, int nMesjc,
                                       int day, int ist_fin_id, int cell_id) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    double result = 0;
    TDateTime BegDate = EncodeDate(god, 1, 1);
    TDateTime EndDate = EncodeDate(god, nMesjc, day);
    q->SQL->Clear();
    q->SQL->Add("SELECT sum(value) as value FROM Cell, Denegn_potok");
    q->SQL->Add("WHERE Cell.cell_id = :cell_id");
    q->SQL->Add("AND Denegn_potok.cell_id = Cell.cell_id");
    q->SQL->Add("AND Denegn_potok.pokaz_id = Cell.pokaz_id");
    q->SQL->Add("AND plan_id = :plan_id");
    q->SQL->Add("AND vid_otc = :vid_otc");
    q->SQL->Add("AND value_date BETWEEN :BegDate AND :EndDate");
    q->SQL->Add("AND Denegn_potok.objectid = :objectid");
    q->ParamByName("plan_id")->AsInteger() = plan_id;
    q->ParamByName("objectid")->AsInteger() = objectid;
    q->ParamByName("cell_id")->AsInteger() = cell_id;
    q->ParamByName("vid_otc")->AsInteger() = vid_otc;
    q->ParamByName("BegDate")->AsDateTime() = BegDate;
    q->ParamByName("EndDate")->AsDateTime() = EndDate;
    q->Open();
    if (!q->EoF()) {
        result = q->FieldByName("value")->AsFloat();
    }
    Addrcrd(plan_id, objectid, pokaz_id, vid_otc, god, nMesjc, day, ist_fin_id, cell_id, result);
    delete q;
    return result;
}

// вычислить, добавить запись для детальной строки и вернуть значение - уточненный план
double TValForPeriod::Val_Addrcrd_Det4(int plan_id, int objectid, int pokaz_id, int vid_otc, int god, int nMesjc, 
                                       int day, int ist_fin_id, int cell_id_Mes) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    double sumvalue = 0;
    int cell_id_God = cell_id_Mes - 2;
    q->SQL->Clear();
    q->SQL->Add("SELECT value");
    q->SQL->Add("FROM Cell_value");
    q->SQL->Add("WHERE objectid = 0");
    q->SQL->Add("AND plan_id = :plan_id");
    q->SQL->Add("AND cell_id = :cell_id_God");
    q->SQL->Add("AND vid_otc = 4");
    switch (GetDBType()) {
        case TDBType::MSSQL:
            q->SQL->Add("AND DATEPART(YEAR, value_date) = :God");
            break;
        case TDBType::Postgre:
            q->SQL->Add("AND date_part('year', value_date) = :God");
            break;
    }
    q->ParamByName("plan_id")->AsInteger() = plan_id;
    q->ParamByName("cell_id_God")->AsInteger() = cell_id_God;
    q->ParamByName("God")->AsInteger() = god;
    q->Open();
    if (!q->EoF()) {
        sumvalue = q->FieldByName("value")->AsFloat();
    }
    TDateTime dt = EncodeDate(god, nMesjc, day);
    q->SQL->Clear();
    q->SQL->Add("SELECT -summa_otkuda as summa");
    q->SQL->Add("FROM PrdlgUP a, Utoc_plana b");
    q->SQL->Add("WHERE b.god = :god");
    q->SQL->Add("AND b.forOldBudget = 0");
    q->SQL->Add("AND a.sost_predlog = 3");
    q->SQL->Add("AND a.date_otveta IS NOT NULL");
    switch (GetDBType()) {
        case TDBType::MSSQL:
            q->SQL->Add("AND CAST(FLOOR(CAST(a.date_otveta AS FLOAT)) AS DATETIME) > :dt");
            break;
        case TDBType::Postgre:
            q->SQL->Add("AND CAST(a.date_otveta AS DATE) > :dt");
            break;
    }
    q->SQL->Add("AND b.prdlg_id = a.prdlg_id");
    q->SQL->Add("AND b.summa_otkuda > 0");
    q->SQL->Add("AND b.plan_id = :plan_id");
    q->SQL->Add("AND b.pokaz_id_otkuda = :pokaz_id");
    q->ParamByName("god")->AsInteger() = god;
    q->ParamByName("plan_id")->AsInteger() = plan_id;
    q->ParamByName("pokaz_id")->AsInteger() = pokaz_id;
    q->ParamByName("dt")->AsDateTime() = dt;
    q->Open();
    while (!q->EoF()) {
        sumvalue += (-q->FieldByName("summa")->AsFloat());
        q->Next();
    }
    q->SQL->Clear();
    q->SQL->Add("SELECT summa_kuda as summa");
    q->SQL->Add("FROM PrdlgUP a, Utoc_plana b");
    q->SQL->Add("WHERE b.god = :god");
    q->SQL->Add("AND b.forOldBudget = 0");
    q->SQL->Add("AND a.sost_predlog = 3");
    q->SQL->Add("AND a.date_otveta IS NOT NULL");
    switch (GetDBType()) {
        case TDBType::MSSQL:
            q->SQL->Add("AND CAST(FLOOR(CAST(a.date_otveta AS FLOAT)) AS DATETIME) > :dt");
            break;
        case TDBType::Postgre:
            q->SQL->Add("AND CAST(a.date_otveta AS DATE) > :dt");
            break;
    }
    q->SQL->Add("AND b.prdlg_id = a.prdlg_id");
    q->SQL->Add("AND b.summa_kuda > 0");
    q->SQL->Add("AND b.plan_id = :plan_id");
    q->SQL->Add("AND b.pokaz_id_kuda = :pokaz_id");
    q->ParamByName("god")->AsInteger() = god;
    q->ParamByName("plan_id")->AsInteger() = plan_id;
    q->ParamByName("pokaz_id")->AsInteger() = pokaz_id;
    q->ParamByName("dt")->AsDateTime() = dt;
    q->Open();
    while (!q->EoF()) {
        sumvalue += (-q->FieldByName("summa")->AsFloat());
        q->Next();
    }
    delete q;
    Addrcrd(plan_id, objectid, pokaz_id, vid_otc, god, nMesjc, day, ist_fin_id, cell_id_Mes, sumvalue);
    return sumvalue;
}

// вычислить, добавить запись для групповой строки "в том числе" и вернуть значение
double TValForPeriod::Val_Addrcrd_VtomCisle(int plan_id, int objectid, int pokaz_id, int vid_otc, int god, int nMesjc, 
                                             int day, int ist_fin_id, int cell_id) {
    double result = Val_forOwner(plan_id, objectid, pokaz_id, vid_otc, god, nMesjc, day, ist_fin_id);
    Addrcrd(plan_id, objectid, pokaz_id, vid_otc, god, nMesjc, day, ist_fin_id, cell_id, result);
    return result;
}

// вычислить, добавить запись для строки "Итого по разделу" и вернуть значение
double TValForPeriod::Val_Addrcrd_Itogo(int plan_id, int objectid, int pokaz_id, int vid_otc, int god, int nMesjc,
                                        int day, int ist_fin_id, int cell_id) {
    int pokaz_owner_id = TDB_Pokaztl::GetPokazOwnerId(pokaz_id);
    double result = Val_forOwner(plan_id, objectid, pokaz_owner_id, vid_otc, god, nMesjc, day, ist_fin_id);
    Addrcrd(plan_id, objectid, pokaz_id, vid_otc, god, nMesjc, day, ist_fin_id, cell_id, result);
    return result;
}

// организовать вычисления подчиненных и вернуть значение
double TValForPeriod::Val_forOwner(int plan_id, int objectid, int pokaz_owner_id, int vid_otc, int god, int nMesjc,
                                   int day, int ist_fin_id) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    double result = 0;
    int w_pokaz_id, w_cell_id;
    q->SQL->Clear();
    q->SQL->Add("SELECT b.pokaz_id, b.pokaz_type, b.pokaz_is_razd, b.pokaz_vert, c.cell_id");
    q->SQL->Add("FROM Plan_str a, Pokaztl b, Cell c");
    q->SQL->Add("WHERE a.plan_str_owner_id = :pokaz_owner_id ");
    q->SQL->Add("AND (a.plan_id = 1 OR a.plan_id = :plan_id)");
    q->SQL->Add("AND b.pokaz_id = a.pokaz_id");
    q->SQL->Add("AND b.pokaz_is_razd = 2");
    if (GetDBType() == TDBType::MSSQL) {
        q->SQL->Add("AND :god BETWEEN YEAR(pokaz_beg_date) AND YEAR(pokaz_end_date)");
    } else if (GetDBType() == TDBType::Postgre) {
        q->SQL->Add("AND :god BETWEEN date_part('year', pokaz_beg_date) AND date_part('year', pokaz_end_date)");
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
    if (GetDBType() == TDBType::MSSQL) {
        q->SQL->Add("AND :god1 BETWEEN YEAR(pokaz_beg_date) AND YEAR(pokaz_end_date)");
    } else if (GetDBType() == TDBType::Postgre) {
        q->SQL->Add("AND :god1 BETWEEN date_part('year', pokaz_beg_date) AND date_part('year', pokaz_end_date)");
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
            w_pokaz_id = TDB_Pokaztl::GetIdItogoRazd(q->FieldByName("pokaz_id")->AsInteger());
            if (w_pokaz_id > 0) {
                w_cell_id = TDB_Cell::GetCellId(w_pokaz_id, 3, ist_fin_id);
                result += GlobalVars->ValForPeriod.Getrcrd(plan_id, objectid, w_pokaz_id, vid_otc, god, nMesjc, day,
                                                           ist_fin_id, w_cell_id, 3, 0);
            }
        } else if (q->FieldByName("pokaz_is_razd")->AsInteger() == 2 &&
                   q->FieldByName("pokaz_type")->AsInteger() == 1) {
            if (q->FieldByName("pokaz_vert")->AsInteger() == 0 ||
                q->FieldByName("pokaz_vert")->AsInteger() == 3) { // включает
                result += GlobalVars->ValForPeriod.Getrcrd(plan_id, objectid, q->FieldByName("pokaz_id")->AsInteger(),
                                                           vid_otc, god, nMesjc, day, ist_fin_id,
                                                           q->FieldByName("cell_id")->AsInteger(), 2, 0);
            } else if (q->FieldByName("pokaz_vert")->AsInteger() == 1) {
                result += GlobalVars->ValForPeriod.Getrcrd(plan_id, objectid, q->FieldByName("pokaz_id")->AsInteger(),
                                                           vid_otc, god, nMesjc, day, ist_fin_id,
                                                           q->FieldByName("cell_id")->AsInteger(), 2, 1);
            }
        }
        q->Next();
    }
    delete q;
    return result;
}

// Специально для графика финансирования - рассчет факта к началу периода
double TValForPeriod::valFactToBegPeriod(int plan_id, int pokaz_id, int god, int nMesjc, int day) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    double result = 0;
    string KBK = TDB_Pokaztl::GetCodeBK(pokaz_id);
    TDateTime BegDate = EncodeDate(god, 1, 1);
    TDateTime EndDate = EncodeDate(god, nMesjc, day);
    EndDate = IncDay(EndDate, -1); // 20.09.2023
    q->SQL->Clear();
    q->SQL->Add("SELECT den_potok_id, value, reg_nomer, nom_plategki, attrib3");
    q->SQL->Add("FROM Denegn_potok a, Pokaztl b");
    q->SQL->Add("WHERE a.plan_id = :plan_id");
    q->SQL->Add("AND a.value_date BETWEEN :BegDate AND :EndDate");
    q->SQL->Add("AND a.vid_otc = 5");
    q->SQL->Add("AND a.objectid = 0");
    q->SQL->Add("AND a.Pokaz_id = b.Pokaz_id");
    q->SQL->Add("AND b.pokaz_bk_code LIKE :wStr");
    q->SQL->Add("UNION");
    q->SQL->Add("SELECT den_potok_id, value, reg_nomer, nom_plategki, attrib3");
    q->SQL->Add("FROM Denegn_potok a, Pokaztl b");
    q->SQL->Add("WHERE a.plan_id = :plan_id");
    q->SQL->Add("AND a.value_date BETWEEN :BegDate AND :EndDate");
    q->SQL->Add("AND a.vid_otc = 5");
    q->SQL->Add("AND a.objectid > 0");
    q->SQL->Add("AND a.objectid = b.Pokaz_id");
    q->SQL->Add("AND b.pokaz_bk_code LIKE :wStr");
    q->ParamByName("plan_id")->AsInteger() = plan_id;
    q->ParamByName("BegDate")->AsDateTime() = BegDate;
    q->ParamByName("EndDate")->AsDateTime() = EndDate;
    q->ParamByName("wStr")->AsString() = KBK + "%";
    q->Open();
    while (!q->EoF()) {
        if (!((q->FieldByName("reg_nomer")->AsString() == "") && (q->FieldByName("nom_plategki")->AsString() != ""))) {
            if (q->FieldByName("attrib3")->AsString() != PIV_ASU_GF) {
                if (!TDB_DenegnPotok::isFromAisUBP(q->FieldByName("reg_nomer")->AsString())) {
                    goto LNext;
                }
            }
        }
        result += q->FieldByName("value")->AsFloat();
    LNext:
        q->Next();
    }
    delete q;
    return result;
}

// Специально для графика финансирования - рассчет факта за квартал на дату
double TValForPeriod::valFactKvNaDatu(int plan_id, int pokaz_id, int god, int nMesjc, int day, int kv) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    string KBK = TDB_Pokaztl::GetCodeBK(pokaz_id);
    int m = (kv - 1) * 3 + 1;
    TDateTime BegDate = EncodeDate(god, m, 1);
    TDateTime EndDate = EncodeDate(god, nMesjc, day);
    double Result = 0;
    q->SQL->Clear();
    q->SQL->Add("SELECT den_potok_id, value, reg_nomer, nom_plategki, attrib3");
    q->SQL->Add("FROM Denegn_potok a, Pokaztl b");
    q->SQL->Add("WHERE a.plan_id = :plan_id");
    q->SQL->Add("AND a.value_date BETWEEN :BegDate AND :EndDate");
    q->SQL->Add("AND a.vid_otc = 5");
    q->SQL->Add("AND (((a.objectid = 0) AND (a.Pokaz_id = b.Pokaz_id) and (b.pokaz_bk_code LIKE :wStr))");
    q->SQL->Add(" OR ((a.objectid > 0) AND (a.objectid = b.Pokaz_id) and (b.pokaz_bk_code LIKE :wStr)))");
    q->ParamByName("plan_id")->AsInteger() = plan_id;
    q->ParamByName("BegDate")->AsDateTime() = BegDate;
    q->ParamByName("EndDate")->AsDateTime() = EndDate;
    q->ParamByName("wStr")->AsString() = KBK + "%";
    q->Open();
    while (!q->EoF()) {
        if (!((q->FieldByName("reg_nomer")->AsString() == "") && (q->FieldByName("nom_plategki")->AsString() != ""))) {
            if (q->FieldByName("attrib3")->AsString() != PIV_ASU_GF) {
                if (!TDB_DenegnPotok::isFromAisUBP(q->FieldByName("reg_nomer")->AsString())) {
                    goto LNext;
                }
            }
        }
        Result += q->FieldByName("value")->AsFloat();
    LNext:
        q->Next();
    }
    delete q;
    return Result;
}
