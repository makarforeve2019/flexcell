#include "TT_ValForMesjc.h"
#include "SystFuncsLogic.h"
#include "unDefDBconnect.h"
#include "dmGlobalVars.h"
#include "FormDopInp.h"
#include "DB_Pokaztl.h"
#include "DB_Cell.h"
#include "DB_EtDog.h"
#include "DB_Dgvr.h"
#include "DB_DenegnPotok.h" 

 // создает объект класса TValForMesjc
void TValForMesjc::CreateObjValForMesjc() {
     GlobalVars->ValForMesjc = TValForMesjc();
     GlobalVars->ValForMesjc.sl = new TStringList();
    //GlobalVars->ValForMesjc.sl->Sorted = true;
}

 // освобождает объект класса TValForMesjc
void TValForMesjc::FreeObjValForMesjc() {
    for (int i = 0; i < GlobalVars->ValForMesjc.sl->Count(); ++i) {
        PrcrdFM rcrdFM = std::get<PrcrdFM>(GlobalVars->ValForMesjc.sl->Objects[i]);
        delete rcrdFM;
    }
    delete GlobalVars->ValForMesjc.sl;
}

std::string TValForMesjc::CreateKey(int plan_id, int objectid, int pokaz_id, int vid_otc, int god, int nMesjc, int ist_fin_id) {
    return std::to_string(plan_id) + "_" + std::to_string(objectid) + "_" + std::to_string(pokaz_id) + "_" + std::to_string(vid_otc) + "_" + std::to_string(god) + "_" + std::to_string(nMesjc) + "_" + std::to_string(ist_fin_id);
}

// добавить запись
void TValForMesjc::Addrcrd(int plan_id, int objectid, int pokaz_id, int vid_otc, int god, int nMesjc, int ist_fin_id, int cell_id, double value) {
    std::string key = std::to_string(plan_id) + "_" + std::to_string(objectid) + "_" + std::to_string(pokaz_id) + "_" + std::to_string(vid_otc) + "_" + std::to_string(god) + "_" + std::to_string(nMesjc) + "_" + std::to_string(ist_fin_id);
    int Index;
    if (sl->Find(key, Index)) {
        TrcrdFM* rcrdFM=std::get<PrcrdFM>(sl->Objects[Index]);
        if (vid_otc == 4) {
            if (ist_fin_id == 2)
                 rcrdFM->valueUtCB = value;
            else if (ist_fin_id == 3) rcrdFM->valueUtTB = value;
            else if (ist_fin_id == 4) rcrdFM->valueUtCS = value;
        } else if (vid_otc == 2 || vid_otc == 5) {
            if (ist_fin_id == 2) rcrdFM->valueFcCB = value;
            else if (ist_fin_id == 3) rcrdFM->valueFcTB = value;
            else if (ist_fin_id == 4) rcrdFM->valueFcCS = value;
        }
    } else {
        TrcrdFM* rcrdFM = new TrcrdFM();
        rcrdFM->plan_id = plan_id;
        rcrdFM->objectid = objectid;
        rcrdFM->pokaz_id = pokaz_id;
        rcrdFM->vid_otc = vid_otc;
        rcrdFM->god = god;
        rcrdFM->nMesjc = nMesjc;
        rcrdFM->ist_fin_id = ist_fin_id;
        rcrdFM->cell_id = cell_id;
        rcrdFM->valueUtCB = 0;
        rcrdFM->valueUtTB = 0;
        rcrdFM->valueUtCS = 0;
        rcrdFM->valueFcCB = 0;
        rcrdFM->valueFcTB = 0;
        rcrdFM->valueFcCS = 0;
        if (vid_otc == 4) {
            if (ist_fin_id == 2) rcrdFM->valueUtCB = value;
            else if (ist_fin_id == 3) rcrdFM->valueUtTB = value;
            else if (ist_fin_id == 4) rcrdFM->valueUtCS = value;
        } else if (vid_otc == 2 || vid_otc == 5) {
            if (ist_fin_id == 2) rcrdFM->valueFcCB = value;
            else if (ist_fin_id == 3) rcrdFM->valueFcTB = value;
            else if (ist_fin_id == 4) rcrdFM->valueFcCS = value;
        }
        sl->AddObject(key, rcrdFM);
    }
}

// вернуть числового значение записи
double TValForMesjc::Getrcrd(int plan_id, int objectid, int pokaz_id, int vid_otc, int god, int nMesjc, int ist_fin_id, int cell_id, int pokaz_is_razd, int pokaz_vert) {
    std::string key = CreateKey(plan_id, objectid, pokaz_id, vid_otc, god, nMesjc, ist_fin_id);
 int Index;
    if (sl->Find(key, Index)) {
        TrcrdFM* rcrdFM = std::get<PrcrdFM>(sl->Objects[Index]);
        if (vid_otc == 4) {
            if (ist_fin_id == 2) return rcrdFM->valueUtCB;
            else if (ist_fin_id == 3) return rcrdFM->valueUtTB;
            else if (ist_fin_id == 4) return rcrdFM->valueUtCS;
        } else if (vid_otc == 2 || vid_otc == 5) {
            if (ist_fin_id == 2) return rcrdFM->valueFcCB;
            else if (ist_fin_id == 3) return rcrdFM->valueFcTB;
            else if (ist_fin_id == 4) return rcrdFM->valueFcCS;
        }
    }

    if (pokaz_is_razd == 2 && pokaz_vert == 0) return Val_Addrcrd_Det(plan_id, objectid, pokaz_id, vid_otc, god, nMesjc, ist_fin_id, cell_id);
    if (pokaz_is_razd == 2 && pokaz_vert == 2) return Val_Addrcrd_Det(plan_id, objectid, pokaz_id, vid_otc, god, nMesjc, ist_fin_id, cell_id);
    if (pokaz_is_razd == 2 && pokaz_vert == 1) return Val_Addrcrd_VtomCisle(plan_id, objectid, pokaz_id, vid_otc, god, nMesjc, ist_fin_id, cell_id);
    if (pokaz_is_razd == 3) return Val_Addrcrd_Itogo(plan_id, objectid, pokaz_id, vid_otc, god, nMesjc, ist_fin_id, cell_id);
    if (pokaz_is_razd == 4 && pokaz_vert == 0) return Val_Addrcrd_Det(plan_id, objectid, pokaz_id, vid_otc, god, nMesjc, ist_fin_id, cell_id);
    if (pokaz_is_razd == 4 && pokaz_vert == 2) return Val_Addrcrd_Det(plan_id, objectid, pokaz_id, vid_otc, god, nMesjc, ist_fin_id, cell_id);
    if (pokaz_is_razd == 4 && pokaz_vert == 1) return Val_Addrcrd_VtomCisle(plan_id, objectid, pokaz_id, vid_otc, god, nMesjc, ist_fin_id, cell_id);
    if (pokaz_is_razd == 4) return Val_Addrcrd_Itogo(plan_id, objectid, pokaz_id, vid_otc, god, nMesjc, ist_fin_id, cell_id);

    return 0.0; // Return default value if none of the conditions match
}

// вычислить, добавить запись для детальной строки и вернуть значение
double TValForMesjc::Val_Addrcrd_Det(int plan_id, int objectid, int pokaz_id, int vid_otc, int god, int nMesjc, int ist_fin_id, int cell_id) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    double result = 0;

    q->SQL->Clear();
    q->SQL->Add("SELECT sum(value) as value FROM Cell, Denegn_potok");
    q->SQL->Add("WHERE Cell.cell_id = :cell_id");
    q->SQL->Add("AND Denegn_potok.cell_id = Cell.cell_id");
    q->SQL->Add("AND Denegn_potok.pokaz_id = Cell.pokaz_id");
    q->SQL->Add("AND plan_id = :plan_id");
    q->SQL->Add("AND vid_otc = :vid_otc");

    switch (GetDBType()) {
        case TDBType::MSSQL:
            q->SQL->Add("AND MONTH(value_date) = :nMesjc");
            q->SQL->Add("AND YEAR(value_date) = :god");
            break;
        case TDBType::Postgre:
            q->SQL->Add("AND date_part('month', value_date) = :nMesjc");
            q->SQL->Add("AND date_part('year', value_date) = :god");
            break;
    }

    q->SQL->Add("AND Denegn_potok.objectid = :objectid");
    q->ParamByName("plan_id")->AsInteger() = plan_id;
    q->ParamByName("objectid")->AsInteger() = objectid;
    q->ParamByName("cell_id")->AsInteger() = cell_id;
    q->ParamByName("vid_otc")->AsInteger() = vid_otc;
    q->ParamByName("nMesjc")->AsInteger() = nMesjc;
    q->ParamByName("god")->AsInteger() = god;
    q->Open();

    if (!q->EoF()) {
        result = q->FieldByName("value")->AsFloat();
    }

    Addrcrd(plan_id, objectid, pokaz_id, vid_otc, god, nMesjc, ist_fin_id, cell_id, result);

    delete q;
    return result;
}

// вычислить, добавить запись для групповой строки "в том числе" и вернуть значение
double TValForMesjc::Val_Addrcrd_VtomCisle(int plan_id, int objectid, int pokaz_id, int vid_otc, int god, int nMesjc, int ist_fin_id, int cell_id) {
    double result = Val_forOwner(plan_id, objectid, pokaz_id, vid_otc, god, nMesjc, ist_fin_id);
    Addrcrd(plan_id, objectid, pokaz_id, vid_otc, god, nMesjc, ist_fin_id, cell_id, result);
    return result;
}

// вычислить, добавить запись для строки "Итого по разделу" и вернуть значение
double TValForMesjc::Val_Addrcrd_Itogo(int plan_id, int objectid, int pokaz_id, int vid_otc, int god, int nMesjc, int ist_fin_id, int cell_id) {
    int pokaz_owner_id = TDB_Pokaztl::GetPokazOwnerId(pokaz_id);
    double result = Val_forOwner(plan_id, objectid, pokaz_owner_id, vid_otc, god, nMesjc, ist_fin_id);
    Addrcrd(plan_id, objectid, pokaz_id, vid_otc, god, nMesjc, ist_fin_id, cell_id, result);
    return result;
}

// организовать вычисления подчиненных и вернуть значение
double TValForMesjc::Val_forOwner(int plan_id, int objectid, int pokaz_owner_id, int vid_otc, int god, int nMesjc, int ist_fin_id) {
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
        switch (GetDBType()) {
            case TDBType::MSSQL:
                q->SQL->Add("AND :Year BETWEEN YEAR(pokaz_beg_date) AND YEAR(pokaz_end_date)");
                break;
            case TDBType::Postgre:
                q->SQL->Add("AND date_part('year', pokaz_beg_date) BETWEEN :Year AND :Year");
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
                q->SQL->Add("AND :Year1 BETWEEN YEAR(pokaz_beg_date) AND YEAR(pokaz_end_date)");
                break;
            case TDBType::Postgre:
                q->SQL->Add("AND date_part('year', pokaz_beg_date) BETWEEN :Year1 AND :Year1");
                break;
        }
        q->ParamByName("plan_id")->AsInteger() = plan_id;
        q->ParamByName("plan_id1")->AsInteger() = plan_id;
        q->ParamByName("pokaz_owner_id")->AsInteger() = pokaz_owner_id;
        q->ParamByName("pokaz_owner_id1")->AsInteger() = pokaz_owner_id;
        q->ParamByName("ist_fin_id")->AsInteger() = ist_fin_id;
        q->ParamByName("Year")->AsInteger() = god;
        q->ParamByName("Year1")->AsInteger() = god;
        q->Open();
        while (!q->EoF()) {
            if (q->FieldByName("pokaz_is_razd")->AsInteger() == 1) {
                w_pokaz_id = TDB_Pokaztl::GetIdItogoRazd(q->FieldByName("pokaz_id")->AsInteger());
                if (w_pokaz_id > 0) {
                    w_cell_id = TDB_Cell::GetCellId(w_pokaz_id, 3, ist_fin_id);
                    result += GlobalVars->ValForMesjc.Getrcrd(plan_id, objectid, w_pokaz_id, vid_otc, god, nMesjc, ist_fin_id, w_cell_id, 3, 0);
                }
            } else if (q->FieldByName("pokaz_is_razd")->AsInteger() == 2 && q->FieldByName("pokaz_type")->AsInteger() == 1) {
                if (q->FieldByName("pokaz_vert")->AsInteger() == 0) {
                    result += GlobalVars->ValForMesjc.Getrcrd(plan_id, objectid, q->FieldByName("pokaz_id")->AsInteger(), vid_otc, god, nMesjc, ist_fin_id, q->FieldByName("cell_id")->AsInteger(), 2, 0);
                } else if (q->FieldByName("pokaz_vert")->AsInteger() == 1) {
                    result += GlobalVars->ValForMesjc.Getrcrd(plan_id, objectid, q->FieldByName("pokaz_id")->AsInteger(), vid_otc, god, nMesjc, ist_fin_id, q->FieldByName("cell_id")->AsInteger(), 2, 1);
                }
            }
            q->Next();
        }

    delete q;
    return result;
}

double TValForMesjc::GetZaPeriod(TFormLogicF1* FormLogicF1, PCellValueAtr vd) {
    int EtDog_nacalo_id, col_nacalo_id;
    int EtDog_okoncanie_id, col_okoncanie_id;
    TDateTime dt_nacalo, dt_okoncanie;
    int field_Finansir_id, cell_id_FinansirMes;
    int dgvr_id;

    auto GetColForFieldId = [&](int field_id, int wRow) -> int {
        int result = 0;
        for (int i = 1; i <= FormLogicF1->f1Forms->LastCol; ++i) {
            PCellValueAtr wvd = TFormDopInp::GetCellValueAtrByRC(FormLogicF1, wRow, i);
            if (!wvd) continue;
            if (wvd->PokazId == field_id) {
                result = i;
                break;
            }
        }
        return result;
    };

    auto GetDataFromCell = [&](TDateTime& wdt, int wCol) -> bool {
        bool result = true;
        try {
            wdt = StrToDateTime(char16ToUTF8(FormLogicF1->f1Forms->TextRC(vd->Row, wCol)));
        } catch (...) {
            wdt = 0;
            result = false;
            #ifndef WEBSERVICE
            //-=-SystFuncsLogic.MessageBoxExclamation('Факт за период: Неправильная дата в ячейке', MsgVnim);
            #else
            throw Exception("Факт за период: Неправильная дата в ячейке");
            #endif
        }
        return result;
    };

    int Result = 0;
    if (TFormDopInp::GetListRowsItem(FormLogicF1, vd->Row)->CurRowVid != 4) {
        #ifndef WEBSERVICE
        //-=-SystFuncsLogic.MessageBoxExclamation('Факт за период: Текущая строка не соответствует объекту', MsgVnim);
        //throw Exception("Описание формы неверно!");
        #else
        throw Exception("Факт за период: Текущая строка не соответствует объекту");
        #endif
    }
    if (TFormDopInp::GetListRowsItem(FormLogicF1, vd->Row)->objekt_class_id != TDB_EtDog::GetIdClassEtDog()) {
        #ifndef WEBSERVICE
        //-=-SystFuncsLogic.MessageBoxExclamation('Факт за период: Объект не класса Этап договора', MsgVnim);
        //throw Exception("Описание формы неверно!");
        #else
        throw Exception("Факт за период: Объект не класса Этап договора");
        #endif
    }
    EtDog_nacalo_id = TDB_EtDog::GetEtDogNacaloId();
    if (EtDog_nacalo_id == 0) return Result;
    EtDog_okoncanie_id = TDB_EtDog::GetEtDogOkoncanieId();
    if (EtDog_okoncanie_id == 0) return Result;
    col_nacalo_id = GetColForFieldId(EtDog_nacalo_id, vd->Row);
    if (col_nacalo_id == 0) {
        #ifndef WEBSERVICE
        //-=-SystFuncsLogic.MessageBoxExclamation('Факт за период: В форме отсутствует столбец для Начала этапа', MsgVnim);
        //throw Exception("Описание формы неверно!");
        #else
        throw Exception("Факт за период: В форме отсутствует столбец для Начала этапа");
        #endif
    }
    col_okoncanie_id = GetColForFieldId(EtDog_okoncanie_id, vd->Row);
    if (col_okoncanie_id == 0) {
        #ifndef WEBSERVICE
        //-=-SystFuncsLogic.MessageBoxExclamation('Факт за период: В форме отсутствует столбец для Окончания этапа', MsgVnim);
        //throw Exception("Описание формы неверно!");
        #else
        throw Exception("Факт за период: В форме отсутствует столбец для Окончания этапа");
        #endif
    }
    if (!GetDataFromCell(dt_nacalo, col_nacalo_id)) return Result;
    if (!GetDataFromCell(dt_okoncanie, col_okoncanie_id)) return Result;
    field_Finansir_id = TDB_Dgvr::GetIdFieldObjemFin();
    cell_id_FinansirMes = TDB_Cell::GetCellId(field_Finansir_id, 3, vd->ist_fin_id);
    dgvr_id = TFormDopInp::GetListRowsItem(FormLogicF1, vd->Row)->OwnerId;

    // Calculation for the period value for the contract
    Result = TDB_DenegnPotok::GetSumZaPeriodforP_M_D(vd->PlanId, field_Finansir_id, cell_id_FinansirMes, dgvr_id, dt_nacalo, dt_okoncanie);
    return Result;
}

double TValForMesjc::GetPlGrFinZaPeriod(TFormLogicF1* FormLogicF1, int plan_id, int objectid, int pokaz_id, int vid_otc, int god,
                   int nMesjc, int ist_fin_id, int cell_id, bool isGod) {
    int kv;
    TDateTime BegDate, EndDate;
    double Result = 0;
    kv = GetNKv(nMesjc);
    BegDate = EncodeDate(god, 1, 01);
    if (!isGod)
        BegDate = IncMonth(BegDate, kv * 3 - 3);
    EndDate = EncodeDate(god, nMesjc, 1);
    EndDate = IncDay(EndDate, -1);
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);


        q->SQL->Clear();
        q->SQL->Add("SELECT SUM(value) as value");
        q->SQL->Add("FROM Denegn_potok");
        q->SQL->Add("WHERE plan_id = :plan_id");
        q->SQL->Add("AND vid_otc = 4");
        q->SQL->Add("AND objectid = :objectid");
        q->SQL->Add("AND pokaz_id = :pokaz_id");
        q->SQL->Add("AND value_date BETWEEN :BegDate AND :EndDate");
        q->ParamByName("plan_id")->AsInteger() = plan_id;
        q->ParamByName("pokaz_id")->AsInteger() = pokaz_id;
        q->ParamByName("objectid")->AsInteger() = objectid;
        q->ParamByName("BegDate")->AsDateTime() = BegDate;
        q->ParamByName("EndDate")->AsDateTime() = EndDate;
        q->Open();
        if (!q->EoF())
            Result = q->FieldByName("value")->AsFloat();

    delete q;
    return Result;
}

double TValForMesjc::GetPlGrFinZaKvrtl(TFormLogicF1* FormLogicF1, int plan_id, int objectid, int pokaz_id, int vid_otc, int god, int nMesjc, int ist_fin_id, int cell_id) {
    TDateTime DateLastGrFinForMonth[12];
    double Result = 0;
    int kv, n;
    kv = GetNKv(FormLogicF1->CurrMonth);
    for (n = 0; n < 12; n++) {
        DateLastGrFinForMonth[n] = 0;
    }
    for (n = kv * 3 - 2; n <= FormLogicF1->CurrMonth; n++) {
        //mak ??? TDateTime dt = TDB_MWSModuls::GetDateLastGrFinForMonth(3, FormLogicF1->CurrYear, n, id_FromPref); // дополнения
//mak ???        if (dt == 0)
//mak ???            dt = TDB_MWSModuls::GetDateLastGrFinForMonth(2, FormLogicF1->CurrYear, n, id_FromPref); // основной график
//mak ???        DateLastGrFinForMonth[n - 1] = dt;
//mak ???        if (dt == 0)
//mak ???            break;
    }
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    TDateTime BegDate = EncodeDate(FormLogicF1->CurrYear, kv * 3 - 2, 1);
    TDateTime EndDate = EncodeDate(FormLogicF1->CurrYear, FormLogicF1->CurrMonth, DaysInAMonth(FormLogicF1->CurrYear, FormLogicF1->CurrMonth));
    double d = 0;

        q->SQL->Clear();
        q->SQL->Add("SELECT *");
        q->SQL->Add("FROM Denegn_potok");
        q->SQL->Add("WHERE plan_id = :plan_id");
        q->SQL->Add("AND vid_otc = 4");
        q->SQL->Add("AND pokaz_id = :pokaz_id");
        q->SQL->Add("AND objectid = :objectid");
        q->SQL->Add("AND value_date BETWEEN :BegDate AND :EndDate");
        q->ParamByName("plan_id")->AsInteger() = plan_id;
        q->ParamByName("pokaz_id")->AsInteger() = pokaz_id;
        q->ParamByName("objectid")->AsInteger() = objectid;
        q->ParamByName("BegDate")->AsDateTime() = BegDate;
        q->ParamByName("EndDate")->AsDateTime() = EndDate;
        q->Open();
        while (!q->EoF()) {
            std::string attrib3 = q->FieldByName("attrib3")->AsString();
            if (Pos("ГрФин: ", attrib3) != 1) {
                q->Next();
                continue;
            }
            std::string s = Copy(attrib3, 8, 19);
            TDateTime dt = StrToDateTime(s);
            if (dt > DateLastGrFinForMonth[MonthOf(q->FieldByName("value_date")->AsDateTime()) - 1]){
                q->Next();
                continue;
            }
            d += q->FieldByName("value")->AsFloat();


        }
        Result = d;

        delete q;

    return Result;
}

double TValForMesjc::GetPlGrFinZaMesjc(TFormLogicF1* FormLogicF1, int plan_id, int objectid, int pokaz_id, int vid_otc, int god, int nMesjc, int ist_fin_id, int cell_id) {
    TDateTime DateLastGrFinForMonth[12];
    double Result = 0;
    int n;
    for (n = 0; n < 12; n++) {
        DateLastGrFinForMonth[n] = 0;
    }
    for (n = nMesjc; n <= nMesjc; n++) {
//mak ???         TDateTime dt = TDB_MWSModuls::GetDateLastGrFinForMonth(3, god, n, id_FromPref);
//mak ???        if (dt == 0)
//mak ???            dt = TDB_MWSModuls::GetDateLastGrFinForMonth(2, god, n, id_FromPref);
//mak ???        DateLastGrFinForMonth[n - 1] = dt;
//mak ???        if (dt == 0)
//mak ???            break;
    }
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    TDateTime BegDate = EncodeDate(god, nMesjc, 1);
    TDateTime EndDate = EncodeDate(god, nMesjc, DaysInAMonth(god, nMesjc));
    double d = 0;

        q->SQL->Clear();
        q->SQL->Add("SELECT " );
    q->SQL->Add("den_potok_id,value_date,value,coef,attrib1,attrib2,attrib3,attrib4,attrib5");
        q->SQL->Add("FROM Denegn_potok");
        q->SQL->Add("WHERE plan_id = :plan_id");
        q->SQL->Add("AND vid_otc = 4");
        q->SQL->Add("AND id_FromPref > 0");
        q->SQL->Add("AND pokaz_id = :pokaz_id");
        q->SQL->Add("AND objectid = :objectid");
        q->SQL->Add("AND value_date BETWEEN :BegDate AND :EndDate");
        q->ParamByName("plan_id")->AsInteger() = plan_id;
        q->ParamByName("pokaz_id")->AsInteger() = pokaz_id;
        q->ParamByName("objectid")->AsInteger() = objectid;
        q->ParamByName("BegDate")->AsDateTime() = BegDate;
        q->ParamByName("EndDate")->AsDateTime() = EndDate;
        q->Open();
        while (!q->EoF()) {
            std::string attrib3 = q->FieldByName("attrib3")->AsString();
            if (Pos("ГрФин: ", attrib3) != 1){
                q->Next();
                continue;
            }
            std::string s = Copy(attrib3, 8, 19);
            TDateTime dt = StrToDateTime(s);
            if (dt > DateLastGrFinForMonth[MonthOf(q->FieldByName("value_date")->AsDateTime()) - 1]){
                q->Next();
                continue;
            }
            d += q->FieldByName("value")->AsFloat();


        }
        Result = d;

        delete q;

    return Result;
}


