#include "TT_SummaBySubjects.h"
#include "unDefDBconnect.h"
#include "DB_Plan.h"
#include "SystFuncsLogic.h"
#include "dmGlobalVars.h"

// дать список субъектов на год
std::string TSummaBySubjects::getPlanIds(int god)
{
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    int plan_id_okrug;
    std::string ws;
    if (GlobalVars->glbGod == god)
    {
        return GlobalVars->glbStrPlans;
    }
    plan_id_okrug = TDB_Plan::GetIdOkrug(); 
    q->SQL->Add("SELECT * FROM Plans");
    q->SQL->Add("WHERE plan_id > 1");
    q->Open();
    ws = "";
    while (!q->EoF())
    {
        if (q->FieldByName("plan_is_rajon")->AsInteger() == 0)
        {
            goto PNext;
        }
        if (q->FieldByName("plan_is_rajon")->AsInteger() == 4)
        { // НеУБП
            goto PNext;
        }
        if (q->FieldByName("plan_id")->AsInteger() == plan_id_okrug)
        {
            goto PNext;
        }
        if (god < q->FieldByName("god_beg")->AsInteger())
        {
            goto PNext;
        }
        if (god > q->FieldByName("god_end")->AsInteger())
        {
            goto PNext;
        }
        if (ws == "")
        {
            ws = q->FieldByName("plan_id")->AsString();
        }
        else
        {
            ws = ws + "," + q->FieldByName("plan_id")->AsString();
        }
    PNext:
        q->Next();
    }
    delete q;
    GlobalVars->glbGod = god;
    GlobalVars->glbStrPlans = ws;
    return GlobalVars->glbStrPlans;
}

// создает объект класса TSummaBySubjects
void TSummaBySubjects::CreateObjSummaBySubjects()
{
    GlobalVars->SummaBySubjects = TSummaBySubjects();
    GlobalVars->SummaBySubjects.sl = new TStringList();
    //GlobalVars->SummaBySubjects.sl->Sorted = true;
}

// освобождает объект класса TSummaBySubjects
void TSummaBySubjects::FreeObjSummaBySubjects()
{
    for (int i = 0; i < GlobalVars->SummaBySubjects.sl->Count(); ++i)
    {
        TrcrdFM *rcrdFM = std::get<TrcrdFM*>(GlobalVars->SummaBySubjects.sl->Objects[i]);
        delete rcrdFM;
    }
    delete GlobalVars->SummaBySubjects.sl;
}

std::string TSummaBySubjects::CreateKey(int objectid, int pokaz_id, int vid_otc, int god, int nMesjc, int day)
{
    return std::to_string(objectid) + "_" + std::to_string(pokaz_id) + "_" + std::to_string(vid_otc) + "_" +
           std::to_string(god) + "_" + std::to_string(nMesjc) + "_" + std::to_string(day);
}

// вычислить годовую сумму по всем субъектам и вернуть
double TSummaBySubjects::Val_SumToGodKv(int cell_id, int vid_otc, TDateTime ValueDate)
{
    int god = YearOf(ValueDate);
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    std::string strPlans; 
    q->SQL->Clear();
    q->SQL->Add("SELECT SUM(value) as value");
    q->SQL->Add("FROM Cell_value");
    q->SQL->Add("WHERE objectid = 0");
    strPlans = TSummaBySubjects::getPlanIds(god);
    strPlans = "AND plan_id IN (" + strPlans + ")";
    q->SQL->Add(strPlans);
    q->SQL->Add("AND cell_id = :cell_id");
    q->SQL->Add("AND vid_otc = :vid_otc");
    q->SQL->Add("AND value_date = :value_date");
    q->ParamByName("cell_id")->AsInteger() = cell_id;
    q->ParamByName("vid_otc")->AsInteger() = vid_otc;
    q->ParamByName("value_date")->AsDateTime() = ValueDate;
    q->Open();
    double resultValue = q->EoF() ? 0 : q->FieldByName("value")->AsFloat();
    delete q;
    return resultValue;
}

// добавить запись
void TSummaBySubjects::Addrcrd(int objectid, int pokaz_id, int vid_otc, int god, int nMesjc, int day, int ist_fin_id, int cell_id, double value)
{
    std::string key = TSummaBySubjects::CreateKey(objectid, pokaz_id, vid_otc, god, nMesjc, day);
    int Index = sl->IndexOf(key.c_str());
    TrcrdFM* rcrdFM;
    if (Index != -1)
    {
//        rcrdFM = static_cast<PrcrdFM>(sl->Objects[Index]);
        rcrdFM = std::get<PrcrdFM>(sl->Objects[Index]);
        if (vid_otc == 4)
            rcrdFM->valueUtTB = value;
        else if (vid_otc == 5)
            rcrdFM->valueFcTB = value;
    }
    else
    {
        rcrdFM = new TrcrdFM();
        rcrdFM->objectid = objectid;
        rcrdFM->pokaz_id = pokaz_id;
        rcrdFM->vid_otc = vid_otc;
        rcrdFM->god = god;
        rcrdFM->nMesjc = nMesjc;
        rcrdFM->day = day;
        rcrdFM->ist_fin_id = ist_fin_id;
        rcrdFM->cell_id = cell_id;
        rcrdFM->valueUtTB = 0;
        rcrdFM->valueFcTB = 0;
        if (vid_otc == 4)
            rcrdFM->valueUtTB = value;
        else if (vid_otc == 5)
            rcrdFM->valueFcTB = value;
        sl->AddObject(key, rcrdFM);
    }
}

// вычислить, добавить запись для детальной строки и вернуть значение - уточненный план
double TSummaBySubjects::Val_Addrcrd_Det4(int objectid, int pokaz_id, int vid_otc, int god, int nMesjc, int day, int ist_fin_id, int cell_id_Mes)
{
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    int cell_id_God = cell_id_Mes - 2;
    double sumvalue = 0;
    TDateTime dt;
    std::string strPlans;

    q->SQL->Clear();
    q->SQL->Add("SELECT SUM(value) as value FROM Cell_value WHERE objectid = 0");
    strPlans = TSummaBySubjects::getPlanIds(god);
    strPlans = "AND plan_id IN (" + strPlans + ")";
    q->SQL->Add(strPlans);
    q->SQL->Add("AND cell_id = :cell_id_God");
    q->SQL->Add("AND vid_otc = 4");
    switch (GetDBType())
    {
    case TDBType::MSSQL:
        q->SQL->Add("AND DATEPART(YEAR, value_date) = :God");
        break;
    case TDBType::Postgre:
        q->SQL->Add("AND date_part('year', value_date) = :God");
        break;
    }
    q->ParamByName("cell_id_God")->AsInteger() = cell_id_God;
    q->ParamByName("God")->AsInteger() = god;
    q->Open();
    if (!q->EoF())
    {
        sumvalue = q->FieldByName("value")->AsFloat();
    }
    q->Close();

    dt = EncodeDate(god, nMesjc, day);
    q->SQL->Clear();
    q->SQL->Add("SELECT -summa_otkuda as summa FROM PrdlgUP a, Utoc_plana b WHERE b.god = :god AND b.forOldBudget = 0 AND a.sost_predlog = 3 AND a.date_otveta IS NOT NULL");
    switch (GetDBType())
    {
    case TDBType::MSSQL:
        q->SQL->Add("AND CAST(FLOOR(CAST(a.date_otveta AS FLOAT)) AS DATETIME) > :dt");
        break;
    case TDBType::Postgre:
        q->SQL->Add("AND CAST(a.date_otveta AS DATE) > :dt");
        break;
    }
    q->SQL->Add("AND b.prdlg_id = a.prdlg_id AND b.summa_otkuda > 0");
    strPlans = TSummaBySubjects::getPlanIds(god);
    strPlans = "AND b.plan_id IN (" + strPlans + ")";
    q->SQL->Add(strPlans);
    q->SQL->Add("AND b.pokaz_id_otkuda = :pokaz_id");
    q->ParamByName("god")->AsInteger() = god;
    q->ParamByName("pokaz_id")->AsInteger() = pokaz_id;
    q->ParamByName("dt")->AsDateTime() = dt;
    q->Open();
    while (!q->EoF())
    {
        sumvalue += (-q->FieldByName("summa")->AsFloat());
        q->Next();
    }
    q->Close();

    q->SQL->Clear();
    q->SQL->Add("SELECT summa_kuda as summa FROM PrdlgUP a, Utoc_plana b WHERE b.god = :god AND b.forOldBudget = 0 AND a.sost_predlog = 3 AND a.date_otveta IS NOT NULL");
    switch (GetDBType())
    {
    case TDBType::MSSQL:
        q->SQL->Add("AND CAST(FLOOR(CAST(a.date_otveta AS FLOAT)) AS DATETIME) > :dt");
        break;
    case TDBType::Postgre:
        q->SQL->Add("AND CAST(a.date_otveta AS DATE) > :dt");
        break;
    }
    q->SQL->Add("AND b.prdlg_id = a.prdlg_id AND b.summa_kuda > 0");
    q->SQL->Add(strPlans);
    q->SQL->Add("AND b.pokaz_id_kuda = :pokaz_id");
    q->ParamByName("god")->AsInteger() = god;
    q->ParamByName("pokaz_id")->AsInteger() = pokaz_id;
    q->ParamByName("dt")->AsDateTime() = dt;
    q->Open();
    while (!q->EoF())
    {
        sumvalue += (-q->FieldByName("summa")->AsFloat());
        q->Next();
    }
    q->Close();

    double Result = sumvalue;
    Addrcrd(objectid, pokaz_id, vid_otc, god, nMesjc, day, ist_fin_id, cell_id_Mes, Result);
    delete q;
    return Result;
}

// вычислить, добавить запись для детальной строки и вернуть значение - факт
double TSummaBySubjects::Val_Addrcrd_Det5(int objectid, int pokaz_id, int vid_otc, int god, int nMesjc, int day, int ist_fin_id, int cell_id)
{
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    double Result = 0;
    TDateTime BegDate = EncodeDate(god, 1, 1);
    TDateTime EndDate = EncodeDate(god, nMesjc, day);
    std::string strPlans;

    q->SQL->Clear();
    q->SQL->Add("SELECT sum(value) as value FROM Cell, Denegn_potok");
    q->SQL->Add("WHERE Cell.cell_id = :cell_id AND Denegn_potok.cell_id = Cell.cell_id AND Denegn_potok.pokaz_id = Cell.pokaz_id");
    strPlans = TSummaBySubjects::getPlanIds(god);
    strPlans = "AND plan_id IN (" + strPlans + ")";
    q->SQL->Add(strPlans);
    q->SQL->Add("AND vid_otc = :vid_otc AND value_date BETWEEN :BegDate AND :EndDate AND Denegn_potok.objectid = :objectid");
    q->ParamByName("objectid")->AsInteger() = objectid;
    q->ParamByName("cell_id")->AsInteger() = cell_id;
    q->ParamByName("vid_otc")->AsInteger() = vid_otc;
    q->ParamByName("BegDate")->AsDateTime() = BegDate;
    q->ParamByName("EndDate")->AsDateTime() = EndDate;
    q->Open();
    if (!q->EoF())
    {
        Result = q->FieldByName("value")->AsFloat();
        if (fabs(Result) < 1E-6)
        {
            Result = 0;
        }
    }
    q->Close();

    Addrcrd(objectid, pokaz_id, vid_otc, god, nMesjc, day, ist_fin_id, cell_id, Result);
    delete q;
    return Result;
}

// вычислить, добавить запись для групповой строки "в том числе" и вернуть значение
double TSummaBySubjects::Val_Addrcrd_VtomCisle(int objectid, int pokaz_id, int vid_otc, int god, int nMesjc, int day, int ist_fin_id, int cell_id)
{
    double Result = Val_forOwner(objectid, pokaz_id, vid_otc, god, nMesjc, day, ist_fin_id);
    Addrcrd(objectid, pokaz_id, vid_otc, god, nMesjc, day, ist_fin_id, cell_id, Result);
    return Result;
}

// организовать вычисления подчиненных и вернуть значение
double TSummaBySubjects::Val_forOwner(int objectid, int pokaz_owner_id, int vid_otc, int god, int nMesjc, int day, int ist_fin_id) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    double Result = 0;

    q->SQL->Clear();
    q->SQL->Add("SELECT b.pokaz_id, b.pokaz_type, b.pokaz_is_razd, b.pokaz_vert, c.cell_id");
    q->SQL->Add("FROM Plan_str a, Pokaztl b, Cell c");
    q->SQL->Add("WHERE a.plan_str_owner_id = :pokaz_owner_id ");
    q->SQL->Add("AND a.plan_id = 1");
    q->SQL->Add("AND b.pokaz_id = a.pokaz_id");
    q->SQL->Add("AND b.pokaz_is_razd = 2");
    switch (GetDBType()) {
        case TDBType::MSSQL:
            q->SQL->Add("AND :Year BETWEEN YEAR(pokaz_beg_date) AND YEAR(pokaz_end_date)");
            break;
        case TDBType::Postgre:
            q->SQL->Add("AND :Year BETWEEN date_part('year', pokaz_beg_date) AND date_part('year', pokaz_end_date)");
            break;
    }
    q->SQL->Add("AND c.pokaz_id = a.pokaz_id");
    q->SQL->Add("AND c.cell_period = 3");
    q->SQL->Add("AND c.ist_fin_id = :ist_fin_id");
    q->SQL->Add("UNION");
    q->SQL->Add("SELECT b.pokaz_id, b.pokaz_type, b.pokaz_is_razd, b.pokaz_vert, 0 as cell_id");
    q->SQL->Add("FROM Plan_str a, Pokaztl b");
    q->SQL->Add("WHERE a.plan_str_owner_id = :pokaz_owner_id1 ");
    q->SQL->Add("AND a.plan_id = 1");
    q->SQL->Add("AND b.pokaz_id = a.pokaz_id");
    q->SQL->Add("AND b.pokaz_is_razd = 1");
    switch (GetDBType()) {
        case TDBType::MSSQL:
            q->SQL->Add("AND :Year1 BETWEEN YEAR(pokaz_beg_date) AND YEAR(pokaz_end_date)");
            break;
        case TDBType::Postgre:
            q->SQL->Add("AND :Year1 BETWEEN date_part('year', pokaz_beg_date) AND date_part('year', pokaz_end_date)");
            break;
    }
    q->ParamByName("pokaz_owner_id")->AsInteger() = pokaz_owner_id;
    q->ParamByName("pokaz_owner_id1")->AsInteger() = pokaz_owner_id;
    q->ParamByName("ist_fin_id")->AsInteger() = ist_fin_id;
    q->ParamByName("Year")->AsInteger() = god;
    q->ParamByName("Year1")->AsInteger() = god;
    q->Open();
    while (!q->EoF()) {
        if ((q->FieldByName("pokaz_is_razd")->AsInteger() == 2) && (q->FieldByName("pokaz_type")->AsInteger() == 1)) {
            if ((q->FieldByName("pokaz_vert")->AsInteger() == 0) || (q->FieldByName("pokaz_vert")->AsInteger() == 3)) {
                Result += GlobalVars->SummaBySubjects.Getrcrd(objectid, q->FieldByName("pokaz_id")->AsInteger(), vid_otc, god, nMesjc, day, ist_fin_id, q->FieldByName("cell_id")->AsInteger(), 2, 0);
            }
            else if (q->FieldByName("pokaz_vert")->AsInteger() == 1) {
                Result += GlobalVars->SummaBySubjects.Getrcrd(objectid, q->FieldByName("pokaz_id")->AsInteger(), vid_otc, god, nMesjc, day, ist_fin_id, q->FieldByName("cell_id")->AsInteger(), 2, 1);
            }
        }
        q->Next();
    }
    delete q;
    return Result;
}

// вернуть числового значение записи
double TSummaBySubjects::Getrcrd(int objectid, int pokaz_id, int vid_otc, int god, int nMesjc, int day, int ist_fin_id, int cell_id, int pokaz_is_razd, int pokaz_vert) {
    std::string key;
    PrcrdFM rcrdFM;
    int Index;

    if (pokaz_is_razd == 13) {
        pokaz_is_razd = 2;
    }

    if (pokaz_vert == 3) {
        pokaz_vert = 0;
    }

    key = CreateKey(objectid, pokaz_id, vid_otc, god, nMesjc, day);

    if (sl->Find(key, Index)) {
        rcrdFM = std::get<PrcrdFM>(sl->Objects[Index]);
        if (vid_otc == 4) {
            return rcrdFM->valueUtTB;
        }
        else if (vid_otc == 5) {
            return rcrdFM->valueFcTB;
        }
    }

    if ((pokaz_is_razd == 2) && (pokaz_vert == 0)) {
        switch (vid_otc) {
            case 4:
                return Val_Addrcrd_Det4(objectid, pokaz_id, vid_otc, god, nMesjc, day, ist_fin_id, cell_id);
            case 5:
                return Val_Addrcrd_Det5(objectid, pokaz_id, vid_otc, god, nMesjc, day, ist_fin_id, cell_id);
        }
    }

    if ((pokaz_is_razd == 2) && (pokaz_vert == 2)) {
        switch (vid_otc) {
            case 4:
                return Val_Addrcrd_Det4(objectid, pokaz_id, vid_otc, god, nMesjc, day, ist_fin_id, cell_id);
            case 5:
                return Val_Addrcrd_Det5(objectid, pokaz_id, vid_otc, god, nMesjc, day, ist_fin_id, cell_id);
        }
    }

    if ((pokaz_is_razd == 2) && (pokaz_vert == 1)) {
        return Val_Addrcrd_VtomCisle(objectid, pokaz_id, vid_otc, god, nMesjc, day, ist_fin_id, cell_id);
    }
    return 0.0;
}


