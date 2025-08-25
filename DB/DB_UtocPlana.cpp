#include "DB_UtocPlana.h" 
#include "unDefDBconnect.h"
#include "DB_TmpPokaztl.h"
#include "DB_Pokaztl.h"
#include "DB_GurnProtokol.h"
#include "../SystFuncs/SystFuncsLogic.h"
#include "../calc/UpdDBbyValue.h"
#include "UpdDBbyValue.h"

void TDB_UtocPlana::GetDetStr(TQueryUni *qIn, int utoc_plana_id) {
 
    qIn->SQL->Clear();
    qIn->SQL->Add("SELECT *");
    qIn->SQL->Add("FROM Utoc_plana");
    qIn->SQL->Add("WHERE utoc_plana_id = :utoc_plana_id");
    qIn->ParamByName("utoc_plana_id")->AsInteger() = utoc_plana_id;
    qIn->Open();
}

void TDB_UtocPlana::SetOwnerId(int utoc_plana_id, int owner_id) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    q->SQL->Clear();
    q->SQL->Add("UPDATE Utoc_plana");
    q->SQL->Add("SET owner_id = :owner_id");
    q->SQL->Add("WHERE utoc_plana_id = :utoc_plana_id");
    q->ParamByName("utoc_plana_id")->AsInteger() = utoc_plana_id;
    q->ParamByName("owner_id")->AsInteger() = owner_id;
    q->ExecSQL();
    delete q;
}

double TDB_UtocPlana::GetSumUtocPosleKvartala(PCellValueAtr vd) {
    int kv, Year, Month, Day, PlanId, cellId;
    TDateTime PokazDate;
    PlanId = vd->PlanId;
    PokazDate = vd->ValueDate;
    cellId = vd->CellId;
    
    if (vd->EvalType == 1 && vd->ObjectId == 0) {
        return TDB_UtocPlana::GetSumUtocPosleKvartalaRAZD(vd);
    }
    
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    DecodeDate(PokazDate, Year, Month, Day);
    kv = GetNKv(Month);
    
    q->SQL->Clear();
    q->SQL->Add("SELECT -SUM(summa_otkuda) as sumval FROM Utoc_plana");
    q->SQL->Add("WHERE plan_id = :PlanId");
    q->SQL->Add("AND god = :God");
    q->SQL->Add("AND forOldBudget = 0"); // 13 янв 2015
    q->SQL->Add("AND kv_otkuda = :kv");
    q->SQL->Add("AND cell_id_otkuda = :cellId");
    q->SQL->Add("AND utoc_date > :PokazDate");
    
    q->SQL->Add("UNION");
    q->SQL->Add("SELECT SUM(summa_kuda) as sumval FROM Utoc_plana");
    q->SQL->Add("WHERE plan_id = :PlanId1");
    q->SQL->Add("AND god = :God1");
    q->SQL->Add("AND forOldBudget = 0"); // 13 янв 2015
    q->SQL->Add("AND kv_kuda = :kv1");
    q->SQL->Add("AND cell_id_kuda = :cellId1");
    q->SQL->Add("AND utoc_date > :PokazDate1");
    
    q->ParamByName("PlanId")->AsInteger() = PlanId;
    q->ParamByName("PlanId1")->AsInteger() = PlanId;
    q->ParamByName("God")->AsInteger() = Year;
    q->ParamByName("God1")->AsInteger() = Year;
    q->ParamByName("kv")->AsInteger() = kv;
    q->ParamByName("kv1")->AsInteger() = kv;
    q->ParamByName("cellId")->AsInteger() = cellId;
    q->ParamByName("cellId1")->AsInteger() = cellId;
    q->ParamByName("PokazDate")->AsDateTime() = PokazDate;
    q->ParamByName("PokazDate1")->AsDateTime() = PokazDate;
    q->Open();
    
    double Result = 0;
    while (!q->EoF()) {
        Result += q->FieldByName("sumval")->AsFloat();
        q->Next();
    }
    q->Close();
    delete q;
    
    return Result;
}

double TDB_UtocPlana::GetSumUtocPosleKvartalaRAZD(PCellValueAtr vd) {
    int kv, Year, Month, Day, PlanId, cellId, root_Pokaz_id;
    TDateTime PokazDate;
    PlanId = vd->PlanId;
    PokazDate = vd->ValueDate;
    cellId = vd->CellId;
    DecodeDate(PokazDate, Year, Month, Day);
    kv = GetNKv(Month);
    root_Pokaz_id = TDB_Pokaztl::GetPokazOwnerId(vd->PokazId);
    TDB_TmpPokaztl::CreateTmpPokaztl(Year, root_Pokaz_id, PlanId);
    
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    
    q->SQL->Clear();
    q->SQL->Add("SELECT -SUM(summa_otkuda) as sumval");
    q->SQL->Add("FROM Utoc_plana a, #TmpPokaztl b");
    q->SQL->Add("WHERE plan_id = :PlanId");
    q->SQL->Add("AND god = :God");
    q->SQL->Add("AND forOldBudget = 0"); // 13 янв 2015
    q->SQL->Add("AND kv_otkuda = :kv");
    q->SQL->Add("AND utoc_date > :PokazDate");
    q->SQL->Add("AND a.pokaz_id_otkuda = b.pokaz_id");
    
    q->SQL->Add("UNION");
    q->SQL->Add("SELECT SUM(summa_kuda) as sumval");
    q->SQL->Add("FROM Utoc_plana a, #TmpPokaztl b");
    q->SQL->Add("WHERE plan_id = :PlanId1");
    q->SQL->Add("AND god = :God1");
    q->SQL->Add("AND forOldBudget = 0"); // 13 янв 2015
    q->SQL->Add("AND kv_kuda = :kv1");
    q->SQL->Add("AND utoc_date > :PokazDate1");
    q->SQL->Add("AND a.pokaz_id_kuda = b.pokaz_id");
    
    q->ParamByName("PlanId")->AsInteger() = PlanId;
    q->ParamByName("PlanId1")->AsInteger() = PlanId;
    q->ParamByName("God")->AsInteger() = Year;
    q->ParamByName("God1")->AsInteger() = Year;
    q->ParamByName("kv")->AsInteger() = kv;
    q->ParamByName("kv1")->AsInteger() = kv;
    q->ParamByName("PokazDate")->AsDateTime() = PokazDate;
    q->ParamByName("PokazDate1")->AsDateTime() = PokazDate;
    q->Open();
    
    double Result = 0;
    while (!q->EoF()) {
        Result += q->FieldByName("sumval")->AsFloat();
        q->Next();
    }
    q->Close();
    delete q;
    
    TDB_TmpPokaztl::DropTmpPokaztl();
    return Result;
}

bool TDB_UtocPlana::IsUrovenGU_IS_Raj(int prdlg_id, int &parent_id) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    
    q->SQL->Clear();
    q->SQL->Add("SELECT utoc_plana_id, b.parent_id");
    q->SQL->Add("FROM Utoc_plana a, Plans b, Plans c");
    q->SQL->Add("WHERE a.prdlg_id = :prdlg_id");
    q->SQL->Add("AND a.plan_id = b.plan_id");
    q->SQL->Add("AND b.plan_is_rajon = 3");
    q->SQL->Add("AND b.parent_id > 0");
    q->SQL->Add("AND c.plan_id = b.parent_id");
    q->SQL->Add("AND c.plan_is_rajon = 1");
    
    q->ParamByName("prdlg_id")->AsInteger() = prdlg_id;
    q->Open();
    
    bool Result = false;
    parent_id = 0;
    if (!q->EoF()) {
        Result = true;
        parent_id = q->FieldByName("parent_id")->AsInteger();
    }
    q->Close();
    delete q;
    return Result;
}

int TDB_UtocPlana::IsUrovenGU_IS_Raj2(int prdlg_id, int plan_idRaj) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    
    q->SQL->Clear();
    q->SQL->Add("SELECT 1 as vidUved");
    q->SQL->Add("FROM Utoc_plana a");
    q->SQL->Add("WHERE a.prdlg_id = :prdlg_id");
    q->SQL->Add("AND a.plan_id = :plan_idRaj");
    
    q->SQL->Add("UNION");
    q->SQL->Add("SELECT 2 as vidUved");
    q->SQL->Add("FROM Utoc_plana a, Plans b");
    q->SQL->Add("WHERE a.prdlg_id = :prdlg_id1");
    q->SQL->Add("AND a.plan_id = b.plan_id");
    q->SQL->Add("AND b.plan_is_rajon = 3");
    q->SQL->Add("AND b.parent_id > 0");
    q->SQL->Add("AND b.parent_id = :plan_idRaj1");
    
    q->ParamByName("prdlg_id")->AsInteger() = prdlg_id;
    q->ParamByName("prdlg_id1")->AsInteger() = prdlg_id;
    q->ParamByName("plan_idRaj")->AsInteger() = plan_idRaj;
    q->ParamByName("plan_idRaj1")->AsInteger() = plan_idRaj;
    q->Open();
    
    int Result = 0;
    while (!q->EoF()) {
        if (q->FieldByName("vidUved")->AsInteger() > Result)
            Result = q->FieldByName("vidUved")->AsInteger();
        q->Next();
    }
    q->Close();
    delete q;
    return Result;
}

void TDB_UtocPlana::UpdPrdlg2_id(int utoc_plana_id, int prdlg2_id) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);

    q->SQL->Clear();
    q->SQL->Add("UPDATE Utoc_plana");
    q->SQL->Add("SET prdlg2_id = :prdlg2_id");
    q->SQL->Add("WHERE utoc_plana_id = :utoc_plana_id");

    q->ParamByName("utoc_plana_id")->AsInteger() = utoc_plana_id;
    q->ParamByName("prdlg2_id")->AsInteger() = prdlg2_id;
    q->ExecSQL();

    delete q;
}

int TDB_UtocPlana::AddToGurnal(PCellValueAtr aVd, pUtoc_plana up, int owner_id) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    q->SQL->Add("INSERT INTO Utoc_plana");
    q->SQL->Add("(plan_id, god, utoc_date, n_protocol, kv_otkuda, pokaz_id_otkuda, cell_id_otkuda,");
    q->SQL->Add("kv_kuda, pokaz_id_kuda, cell_id_kuda, budget_poluc_id, summa_kuda, summa_otkuda, budget_poluc_id_kuda,");
    q->SQL->Add("Primec, owner_id, prdlg2_id)");
    q->SQL->Add("values (:plan_id, :god, :utoc_date, :n_protocol, :kv_otkuda, :pokaz_id_otkuda, :cell_id_otkuda,");
    q->SQL->Add(":kv_kuda, :pokaz_id_kuda, :cell_id_kuda, :budget_poluc_id, :summa_kuda, :summa_otkuda, :budget_poluc_id_kuda,");
    q->SQL->Add(":Primec, :owner_id, :prdlg2_id)");

    q->ParamByName("plan_id")->AsInteger() = up->plan_id;
    q->ParamByName("god")->AsInteger() = up->god;
    q->ParamByName("utoc_date")->AsDateTime() = up->utoc_date;
    q->ParamByName("n_protocol")->AsString() = up->n_protocol;
    q->ParamByName("kv_otkuda")->AsInteger() = up->kv_otkuda;
    q->ParamByName("pokaz_id_otkuda")->AsInteger() = up->pokaz_id_otkuda;
    q->ParamByName("cell_id_otkuda")->AsInteger() = up->cell_id_otkuda;
    q->ParamByName("kv_kuda")->AsInteger() = up->kv_kuda;
    q->ParamByName("pokaz_id_kuda")->AsInteger() = up->pokaz_id_kuda;
    q->ParamByName("cell_id_kuda")->AsInteger() = up->cell_id_kuda;
    q->ParamByName("budget_poluc_id")->AsInteger() = up->budget_poluc_id_otkuda;
    q->ParamByName("budget_poluc_id_kuda")->AsInteger() = up->budget_poluc_id_kuda;
    q->ParamByName("summa_kuda")->AsFloat() = PreobrValue(up->summa_kuda, aVd->DataType, aVd->coef_Forma, aVd->coef_Pokaz);
    q->ParamByName("summa_otkuda")->AsFloat() = PreobrValue(up->summa_otkuda, aVd->DataType, aVd->coef_Forma, aVd->coef_Pokaz);
    q->ParamByName("Primec")->AsString() = "Введено " + DateTimeToStr(Now()) + " (" + GetUserLogin() + ")";
    q->ParamByName("owner_id")->AsInteger() = owner_id;
    q->ParamByName("prdlg2_id")->AsInteger() = up->prdlg2_id;
    q->ExecSQL();

    q->SQL->Clear();
    switch (GetDBType()) {
        case TDBType::MSSQL:
            q->SQL->Add("SELECT @@IDENTITY as utoc_plana_id");
            break;
        case TDBType::Postgre:
            q->SQL->Add("SELECT currval(pg_get_serial_sequence('Utoc_plana', 'utoc_plana_id')) as utoc_plana_id");
            break;
    }
    q->Open();
    int utoc_plana_id = q->FieldByName("utoc_plana_id")->AsInteger();
    int result = q->FieldByName("utoc_plana_id")->AsInteger();
    delete q;

    // вывод в протокол действий пользователя
    string s = "plan_id=" + IntToStr(up->plan_id) + ";";
    if (up->summa_otkuda != 0) {
        s += "ОТКУДА:";
        s += "kv=" + IntToStr(up->kv_otkuda) + ";pokaz_id=" + IntToStr(up->pokaz_id_otkuda) + ";summa=" +
             FloatToStr(PreobrValue(up->summa_otkuda, aVd->DataType, aVd->coef_Forma, aVd->coef_Pokaz)) + ";";
    }
    if (up->summa_kuda != 0) {
        s += "КУДА:";
        s += "kv=" + IntToStr(up->kv_kuda) + ";pokaz_id=" + IntToStr(up->pokaz_id_kuda) + ";summa=" +
             FloatToStr(PreobrValue(up->summa_kuda, aVd->DataType, aVd->coef_Forma, aVd->coef_Pokaz)) + ";";
    }
    TDB_GurnProtokol::Protokol_AddItem("Изм-я уточн. плана", GetUserLogin(), GetUserId(), utoc_plana_id, s);

    return result;
}

bool TDB_UtocPlana::doUpdDBbyValue1(PCellValueAtr vd, double val) {
    bool Result = false;
    bool aIsMsg = false;
    try {
        val = PreobrValue(val, vd->DataType, vd->coef_Forma, vd->coef_Pokaz);
        if (TUpdDBbyValue::UpdDBbyValue1(vd->CellId, vd->PlanId, vd->VidOtch, vd->ValueDate, val, vd->ObjectId, aIsMsg)) {
            //--fmForm.FormShowF1.NeedUpdate := True
            Result = true;
        }
        else {
            //{Cancel := 1};
        }
    }
    catch (...) {
        //--SystFuncs.MessageBoxError('Неверное значение', MsgVnim);
        Result = false;
        //{Cancel := 1;}
    }
    return Result;
}

std::string TDB_UtocPlana::getNDocSmeta(int pokaz000, int god, int planId) {
    std::string ws = TDB_Pokaztl::GetCodeBK(pokaz000);
    ws = getKBK_KOSGU(ws, god);
    if (ws != "000") {
        return "";  // Если код BK не равен '000', возвращаем пустую строку
    }

    TQueryUni *wq = new TQueryUni(DefDBconnect.Conn);
    wq->SQL->Add("SELECT nomerDok, NDoc_otveta FROM PrdlgUP PD, Utoc_plana UP");
    wq->SQL->Add("WHERE PD.god = :god AND PD.sost_predlog = 3 AND PD.prdlg_id = UP.prdlg_id");
    wq->SQL->Add("AND UP.plan_id = :plan_id AND ((UP.pokaz_id_kuda = :pokaz_id) OR (UP.pokaz_id_kuda = :pokaz_id_otkuda))");
    wq->SQL->Add("ORDER BY DateSost DESC");

    wq->ParamByName("god")->AsInteger() = god;
    wq->ParamByName("plan_id")->AsInteger() = planId;
    wq->ParamByName("pokaz_id")->AsInteger() = pokaz000;
    wq->ParamByName("pokaz_id_otkuda")->AsInteger() = pokaz000;

    wq->Open();

    std::string result = "";
    if (!wq->EoF()) {
        result = wq->FieldByName("NDoc_otveta")->AsString();
    }

    delete wq;
    return result;
}



int TDB_UtocPlana::AddToGurnalSubSmeta(PCellValueAtr aVd, double delta, double valuePokaztl) {
    int god = YearOf(aVd->ValueDate);
    int pokaz999 = TDB_Pokaztl::GetPokazOwnerId(aVd->PokazId);
    int pokaz000 = TDB_Pokaztl::GetPokazOwnerId(pokaz999);

    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);

    q->SQL->Add("INSERT INTO Utoc_plana");
    q->SQL->Add("(plan_id, god, utoc_date, n_protocol, kv_otkuda, pokaz_id_otkuda, cell_id_otkuda,");
    q->SQL->Add("kv_kuda, pokaz_id_kuda, cell_id_kuda, budget_poluc_id, summa_kuda, summa_otkuda, budget_poluc_id_kuda,");
    q->SQL->Add("Primec, owner_id, prdlg2_id, value)");
    q->SQL->Add("VALUES (:plan_id, :god, :utoc_date, :n_protocol, :kv_otkuda, :pokaz_id_otkuda, :cell_id_otkuda,");
    q->SQL->Add(":kv_kuda, :pokaz_id_kuda, :cell_id_kuda, :budget_poluc_id, :summa_kuda, :summa_otkuda, :budget_poluc_id_kuda,");
    q->SQL->Add(":Primec, :owner_id, :prdlg2_id, :value)");
    q->ParamByName("plan_id")->AsInteger() = aVd->PlanId;
    q->ParamByName("god")->AsInteger() = god;
    q->ParamByName("utoc_date")->AsDateTime() = Now();
    q->ParamByName("n_protocol")->AsString() = getNDocSmeta(pokaz000, god, aVd->PokazId);

    if (delta > 0) {
        q->ParamByName("kv_kuda")->AsInteger() = 1;
        q->ParamByName("pokaz_id_kuda")->AsInteger() = aVd->PokazId;
        q->ParamByName("cell_id_kuda")->AsInteger() = aVd->CellId;
        q->ParamByName("budget_poluc_id_kuda")->AsInteger() = 0;
        q->ParamByName("summa_kuda")->AsFloat() = delta;

        q->ParamByName("kv_otkuda")->AsInteger() = 0;
        q->ParamByName("pokaz_id_otkuda")->AsInteger() = 0;
        q->ParamByName("cell_id_otkuda")->AsInteger() = 0;
        q->ParamByName("budget_poluc_id")->AsInteger() = 0;
        q->ParamByName("summa_otkuda")->AsFloat() = 0;
    } else {
        q->ParamByName("kv_kuda")->AsInteger() = 0;
        q->ParamByName("pokaz_id_kuda")->AsInteger() = 0;
        q->ParamByName("cell_id_kuda")->AsInteger() = 0;
        q->ParamByName("budget_poluc_id_kuda")->AsInteger() = 0;
        q->ParamByName("summa_kuda")->AsFloat() = 0;

        q->ParamByName("kv_otkuda")->AsInteger() = 1;
        q->ParamByName("pokaz_id_otkuda")->AsInteger() = aVd->PokazId;
        q->ParamByName("cell_id_otkuda")->AsInteger() = aVd->CellId;
        q->ParamByName("budget_poluc_id")->AsInteger() = 0;
        q->ParamByName("summa_otkuda")->AsFloat() = -delta; // save positive
    }
    q->ParamByName("Primec")->AsString() = "$$$" + ("Введено " + DateTimeToStr(Now()) + " (" + GetUserLogin() + ")");
    q->ParamByName("owner_id")->AsInteger() = pokaz999;
    q->ParamByName("prdlg2_id")->AsInteger() = 0;
    q->ParamByName("value")->AsFloat() = valuePokaztl;

    q->ExecSQL();

    q->SQL->Clear();
    switch (GetDBType()) {
        case TDBType::MSSQL:
            q->SQL->Add("SELECT @@IDENTITY as utoc_plana_id");
            break;
        case TDBType::Postgre:
            q->SQL->Add("SELECT currval(pg_get_serial_sequence('Utoc_plana', 'utoc_plana_id')) as utoc_plana_id");
            break;
    }
    q->Open();
    int Result = q->FieldByName("utoc_plana_id")->AsInteger();

    delete q;
    return Result;
}

void TDB_UtocPlana::SetDocumInfo(int utoc_plana_id, std::string DocumNum, TDateTime DocumDate) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    
    q->SQL->Clear();
    q->SQL->Add("UPDATE Utoc_plana");
    q->SQL->Add("SET documNum = :documNum, documDate = :documDate");
    q->SQL->Add("WHERE utoc_plana_id = :utoc_plana_id");
    q->ParamByName("utoc_plana_id")->AsInteger() = utoc_plana_id;
    q->ParamByName("documNum")->AsString() = DocumNum;
    q->ParamByName("documDate")->AsDateTime() = DocumDate;
    q->ExecSQL();

    delete q;
}

int TDB_UtocPlana::getDocumInfo(std::string DocumNum, TDateTime DocumDate) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    
    q->SQL->Clear();
    q->SQL->Add("SELECT utoc_plana_id");
    q->SQL->Add("FROM Utoc_plana");
    q->SQL->Add("WHERE documNum = :documNum");
    q->SQL->Add("AND documDate = :documDate");
    q->ParamByName("documNum")->AsString() = DocumNum;
    q->ParamByName("documDate")->AsDateTime() = DocumDate;
    q->Open();
    int Result = 0;
    if (!q->EoF())
        Result = q->FieldByName("utoc_plana_id")->AsInteger();

    delete q;
    return Result;
}

void TDB_UtocPlana::updPrimec(int utoc_plana_id, std::string substr) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    
    std::string Primec;
    q->SQL->Clear();
    q->SQL->Add("SELECT Primec");
    q->SQL->Add("FROM Utoc_plana");
    q->SQL->Add("WHERE utoc_plana_id = :utoc_plana_id");
    q->ParamByName("utoc_plana_id")->AsInteger() = utoc_plana_id;
    q->Open();
    Primec = q->FieldByName("Primec")->AsString();

    q->SQL->Clear();
    q->SQL->Add("UPDATE Utoc_plana");
    q->SQL->Add("SET Primec = :Primec");
    q->SQL->Add("WHERE utoc_plana_id = :utoc_plana_id");
    q->ParamByName("utoc_plana_id")->AsInteger() = utoc_plana_id;
    q->ParamByName("Primec")->AsString() = Primec + substr;
    q->ExecSQL();

    delete q;
}