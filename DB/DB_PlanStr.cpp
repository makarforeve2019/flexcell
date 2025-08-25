#include "DB_PlanStr.h"
#include "unDefDBconnect.h"
#include "DB_Pokaztl.h"
#include "DB_Plan.h"
#include "unDBType.h"
#include "SystFuncsLogic.h"


void TDB_PlanStr::AddElem(bool isItogo, int plan_id, int plan_str_owner_id, int pokaz_id) {
    int plan_str_order;
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    if (isItogo) {
        plan_str_order = 999;
    } else {
        plan_str_order = TDB_PlanStr::GetMaxOrder(plan_id, plan_str_owner_id);
        if (plan_str_order == 999) {
            plan_str_order = plan_str_order + 1; // 16 февр 2010
        }
    }
    q->SQL->Clear();
    q->SQL->Add("INSERT INTO Plan_str");
    q->SQL->Add("(plan_id, plan_str_owner_id, plan_str_order, pokaz_id)");
    q->SQL->Add("VALUES (:plan_id, :plan_str_owner_id, :plan_str_order, :pokaz_id)");
    q->ParamByName("plan_id")->AsInteger() = plan_id;
    q->ParamByName("pokaz_id")->AsInteger() = pokaz_id;
    q->ParamByName("plan_str_owner_id")->AsInteger() = plan_str_owner_id;
    q->ParamByName("plan_str_order")->AsInteger() = plan_str_order + 1;
    q->ExecSQL();
    //MAK q->Free();
    delete q;
}

void TDB_PlanStr::AddElemByNum(int plan_id, int plan_str_owner_id, int plan_str_order, int pokaz_id) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    q->SQL->Clear();
    q->SQL->Add("INSERT INTO Plan_str");
    q->SQL->Add("(plan_id, plan_str_owner_id, plan_str_order, pokaz_id)");
    q->SQL->Add("VALUES (:plan_id, :plan_str_owner_id, :plan_str_order, :pokaz_id)");
    q->ParamByName("plan_id")->AsInteger() = plan_id;
    q->ParamByName("pokaz_id")->AsInteger() = pokaz_id;
    q->ParamByName("plan_str_owner_id")->AsInteger() = plan_str_owner_id;
    q->ParamByName("plan_str_order")->AsInteger() = plan_str_order;
    q->ExecSQL();
    //MAK q->Free();
    delete q;
}

void TDB_PlanStr::InsertElemByNum(int plan_id, int plan_str_owner_id, int plan_str_order, int pokaz_id, bool isNew) {
    int InsertOrder = 1;
    TQueryUni *qWrk = new TQueryUni(DefDBconnect.Conn);
    TDB_PlanStr::GetAllSubItemOnlyPlanStr(qWrk, plan_id, plan_str_owner_id);
    if (!qWrk->EoF()) {
        qWrk->Last();
        while (!qWrk->BoF()) {
            if (qWrk->FieldByName("plan_str_order")->AsInteger() == 1000) // Итого по разделу
            {
                qWrk->Prior();
                continue;
            }
            if (qWrk->FieldByName("plan_str_order")->AsInteger() < plan_str_order) {
                if (qWrk->FieldByName("plan_str_order")->AsInteger() == 999) {
                    InsertOrder = qWrk->FieldByName("plan_str_order")->AsInteger() + 2;
                } else {
                    InsertOrder = qWrk->FieldByName("plan_str_order")->AsInteger() + 1;
                }
                break;
            }
            if (qWrk->FieldByName("plan_str_order")->AsInteger() == 999) {
                int wplan_str_order = qWrk->FieldByName("plan_str_order")->AsInteger() + 2;
                TDB_PlanStr::ChangePlan_str_order(plan_id, qWrk->FieldByName("plan_str_owner_id")->AsInteger(),
                                                  qWrk->FieldByName("plan_str_order")->AsInteger(), wplan_str_order);
            } else {
                TDB_PlanStr::ChangePlan_str_order(plan_id, qWrk->FieldByName("plan_str_owner_id")->AsInteger(),
                                                  qWrk->FieldByName("plan_str_order")->AsInteger(),
                                                  qWrk->FieldByName("plan_str_order")->AsInteger() + 1);
            }
            qWrk->Prior();
        }
    }
    delete qWrk;
    if (isNew) {
        TDB_PlanStr::AddElemByNum(plan_id, plan_str_owner_id, InsertOrder, pokaz_id);
    } else {
        TDB_PlanStr::ChangePlan_str_order2(plan_id, plan_str_owner_id, pokaz_id, plan_str_order);
    }
}

void TDB_PlanStr::InsertElemByNum2(int plan_id, int plan_str_owner_id, int plan_str_order, int pokaz_id, bool isNew) {
//vernut    TStoredProcUni *sp = TStoredProcFactory::Create(NULL);
//    sp->Connection = DefDBconnect->Conn;
//    sp->StoredProcName = "InsertElemByNum";
//
//    switch (GetDBType())
//    {
//    case TDBType::MSSQL:
//        // sp->Params->CreateParam(ftInteger, "plan_id_", ptInput);
//        // sp->Params->CreateParam(ftInteger, "plan_str_owner_id_", ptInput);
//        // sp->Params->CreateParam(ftInteger, "plan_str_order_", ptInput);
//        // sp->Params->CreateParam(ftInteger, "pokaz_id_", ptInput);
//        // sp->Params->CreateParam(ftBoolean, "isNew_", ptInput);
//
//        // sp->Params->ParamByName("plan_id_")->AsInteger() = plan_id;
//        // sp->Params->ParamByName("plan_str_owner_id_")->AsInteger() = plan_str_owner_id;
//        // sp->Params->ParamByName("plan_str_order_")->AsInteger() = plan_str_order;
//        // sp->Params->ParamByName("pokaz_id_")->AsInteger() = pokaz_id;
//        // sp->Params->ParamByName("isNew_")->AsBoolean() = isNew;
//
//        // sp->ExecProc();
//        break;
//    case TDBType::Postgre:
//        sp->Params->CreateParam(ftInteger, "plan_id_", ptInput);
//        sp->Params->CreateParam(ftInteger, "plan_str_owner_id_", ptInput);
//        sp->Params->CreateParam(ftInteger, "plan_str_order_", ptInput);
//        sp->Params->CreateParam(ftInteger, "pokaz_id_", ptInput);
//        sp->Params->CreateParam(ftBoolean, "isNew_", ptInput);
//
//        sp->Params->ParamByName("plan_id_")->AsInteger() = plan_id;
//        sp->Params->ParamByName("plan_str_owner_id_")->AsInteger() = plan_str_owner_id;
//        sp->Params->ParamByName("plan_str_order_")->AsInteger() = plan_str_order;
//        sp->Params->ParamByName("pokaz_id_")->AsInteger() = pokaz_id;
//        sp->Params->ParamByName("isNew_")->AsBoolean() = isNew;
//
//        sp->ExecProc();
//        break;
//    }
//
//    delete sp;
}

int TDB_PlanStr::GetMaxOrder(int plan_id, int plan_str_owner_id) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);

    int result = 0;

    q->SQL->Clear();
    q->SQL->Add("SELECT MAX (plan_str_order) FROM Plan_str ");
    q->SQL->Add("WHERE plan_id = :plan_id");
    q->SQL->Add("AND plan_str_owner_id = :plan_str_owner_id");
    ////q->SQL->Add("AND plan_str_order < 1000");
    q->SQL->Add("AND plan_str_order <> 1000"); // 16 февр 2010
    q->ParamByName("plan_id")->AsInteger() = plan_id;
    q->ParamByName("plan_str_owner_id")->AsInteger() = plan_str_owner_id;
    q->Open();
    if (!q->EoF()) {
        try {
            result = q->Fields[0].AsInteger();
        } catch (...) {
            result = 0;
        }
    }


    //MAK q->Free();
    delete q;
    return result;
}

bool TDB_PlanStr::GetPlanStr(TQueryUni *qIn, int plan_id, int pokaz_id) {
    bool result = true;

    qIn->SQL->Clear();
    qIn->SQL->Add("SELECT *");
    qIn->SQL->Add("FROM Plan_str");
    qIn->SQL->Add("WHERE plan_id = :plan_id");
    qIn->SQL->Add("AND pokaz_id = :pokaz_id");
    qIn->ParamByName("plan_id")->AsInteger() = plan_id;
    qIn->ParamByName("pokaz_id")->AsInteger() = pokaz_id;
    qIn->Open();
    if (qIn->EoF())
        result = false;

    return result;
}

bool TDB_PlanStr::GetPlanStr1(TQueryUni *qIn, int plan_id, int pokaz_id) {
    bool result = true;

    qIn->SQL->Clear();
    qIn->SQL->Add("SELECT *");
    qIn->SQL->Add("FROM Plan_str, Pokaztl");
    qIn->SQL->Add("WHERE Pokaztl.pokaz_id = :pokaz_id");
    qIn->SQL->Add("AND (Plan_str.plan_id = 1 OR Plan_str.plan_id = :plan_id)");
    qIn->SQL->Add("AND Plan_str.pokaz_id = Pokaztl.pokaz_id");
    qIn->ParamByName("plan_id")->AsInteger();
    qIn->ParamByName("pokaz_id")->AsInteger();
    qIn->Open();
    if (qIn->EoF())
        result = false;

    return result;
}

bool TDB_PlanStr::InPlanStrAn(int pokaz_id, int &plan_id) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);

    bool result = true;

    q->SQL->Clear();
    q->SQL->Add("SELECT *");
    q->SQL->Add("FROM Plan_str");
    q->SQL->Add("WHERE pokaz_id = :pokaz_id");
    q->ParamByName("pokaz_id")->AsInteger() = pokaz_id;
    q->Open();
    if (!q->EoF()) {
        plan_id = q->FieldByName("plan_id")->AsInteger();
    }
    if (q->EoF()) {
        plan_id = 0;
        result = false;
    }

    //MAK q->Free();
    delete q;
    return result;
}

std::string TDB_PlanStr::getPlanNameByPokazId(int pokaz_id, int &plan_id) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);

    std::string result = "???";
    plan_id = 0;

    q->SQL->Clear();
    q->SQL->Add("SELECT plan_name, Plans.plan_id");
    q->SQL->Add("FROM Plan_str, Plans");
    q->SQL->Add("WHERE pokaz_id = :pokaz_id");
    q->SQL->Add("AND Plan_str.plan_id = Plans.plan_id");
    q->ParamByName("pokaz_id")->AsInteger() = pokaz_id;
    q->Open();
    if (!q->EoF()) {
        plan_id = q->FieldByName("plan_id")->AsInteger();
        result = q->FieldByName("plan_name")->AsString();
    }

    //MAK q->Free();
    delete q;
    return result;
}

int TDB_PlanStr::getOrderByPokazId(int plan_id, int pokaz_id) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    TDB_PlanStr::GetPlanStr(q, plan_id, pokaz_id);
    int result = q->FieldByName("plan_str_order")->AsInteger();
    //MAK q->Free();
    delete q;
    return result;
}

int TDB_PlanStr::getPlanStrOwnerId(int plan_id, int pokaz_id) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    TDB_PlanStr::GetPlanStr(q, plan_id, pokaz_id);
    int result = q->FieldByName("plan_str_owner_id")->AsInteger();
    //MAK q->Free();
    delete q;
    return result;
}

int TDB_PlanStr::getPredPokazId(int plan_str_owner_id, int plan_id, int pokaz_id) {
    int plan_str_order;
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    int result = 0;
    {
        q->SQL->Clear();
        q->SQL->Add("SELECT plan_str_order");
        q->SQL->Add("FROM Plan_str a");
        q->SQL->Add("WHERE plan_str_owner_id = :plan_str_owner_id");
        q->SQL->Add("AND (plan_id = 1 OR plan_id = :plan_id)");
        if (pokaz_id > 0)
            q->SQL->Add("AND pokaz_id = :pokaz_id");
        else
            q->SQL->Add("ORDER BY plan_str_order");
        q->ParamByName("plan_str_owner_id")->AsInteger() = plan_str_owner_id;
        q->ParamByName("plan_id")->AsInteger() = plan_id;
        q->ParamByName("pokaz_id")->AsInteger() = pokaz_id;
        q->Open();
        if (q->EoF())
            return result;
        q->Last();
        plan_str_order = q->FieldByName("plan_str_order")->AsInteger();
    }
    q->Close();
    {
        q->SQL->Clear();
        q->SQL->Add("SELECT plan_str_order, pokaz_id");
        q->SQL->Add("FROM Plan_str a");
        q->SQL->Add("WHERE plan_str_owner_id = :plan_str_owner_id");
        q->SQL->Add("AND plan_id = :plan_id");
        q->SQL->Add("AND plan_str_order < :plan_str_order");
        q->SQL->Add("ORDER BY plan_str_order DESC");
        q->ParamByName("plan_str_owner_id")->AsInteger() = plan_str_owner_id;
        q->ParamByName("plan_id")->AsInteger() = plan_id;
        q->ParamByName("plan_str_order")->AsInteger() = plan_str_order;
        q->Open();
        if (q->EoF())
            return result;
        result = q->FieldByName("pokaz_id")->AsInteger();
    }
    //MAK q->Free();
    delete q;
    return result;
}

void TDB_PlanStr::ChangeOrder(int plan_id, int pokaz_id1, int pokaz_id2) {
    int plan_str_order1, plan_str_order2;
    bool bInTransaction = false;
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    TDB_PlanStr::GetPlanStr(q, plan_id, pokaz_id1);
    plan_str_order1 = q->FieldByName("plan_str_order")->AsInteger();
    TDB_PlanStr::GetPlanStr(q, plan_id, pokaz_id2);
    plan_str_order2 = q->FieldByName("plan_str_order")->AsInteger();
    q->SQL->Clear();
    q->SQL->Add("UPDATE Plan_str");
    q->SQL->Add("SET plan_str_order = 10000");
    q->SQL->Add("WHERE plan_id = :plan_id");
    q->SQL->Add("AND pokaz_id = :pokaz_id1");
    q->ParamByName("plan_id")->AsInteger() = plan_id;
    q->ParamByName("pokaz_id1")->AsInteger() = pokaz_id1;
    q->ExecSQL();
    q->SQL->Clear();
    q->SQL->Add("UPDATE Plan_str");
    q->SQL->Add("SET plan_str_order = :plan_str_order1");
    q->SQL->Add("WHERE plan_id = :plan_id");
    q->SQL->Add("AND pokaz_id = :pokaz_id2");
    q->ParamByName("plan_id")->AsInteger() = plan_id;
    q->ParamByName("pokaz_id2")->AsInteger() = pokaz_id2;
    q->ParamByName("plan_str_order1")->AsInteger() = plan_str_order1;
    q->ExecSQL();
    q->SQL->Clear();
    q->SQL->Add("UPDATE Plan_str");
    q->SQL->Add("SET plan_str_order = :plan_str_order2");
    q->SQL->Add("WHERE plan_id = :plan_id");
    q->SQL->Add("AND pokaz_id = :pokaz_id1");
    q->ParamByName("plan_id")->AsInteger() = plan_id;
    q->ParamByName("pokaz_id1")->AsInteger() = pokaz_id1;
    q->ParamByName("plan_str_order2")->AsInteger() = plan_str_order2;
    q->ExecSQL();
    //MAK q->Free();
    delete q;
}

void TDB_PlanStr::ChangePlan_str_order(int plan_id, int plan_str_owner_id, int plan_str_order, int plan_str_orderNew) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    q->SQL->Clear();
    q->SQL->Add("UPDATE Plan_str");
    q->SQL->Add("SET plan_str_order = :plan_str_orderNew");
    q->SQL->Add("WHERE plan_id = :plan_id");
    q->SQL->Add("AND plan_str_owner_id = :plan_str_owner_id");
    q->SQL->Add("AND plan_str_order = :plan_str_order");
    q->ParamByName("plan_str_orderNew")->AsInteger() = plan_str_orderNew;
    q->ParamByName("plan_id")->AsInteger() = plan_id;
    q->ParamByName("plan_str_owner_id")->AsInteger() = plan_str_owner_id;
    q->ParamByName("plan_str_order")->AsInteger() = plan_str_order;
    q->ExecSQL();
    //MAK q->Free();
    delete q;
}

void TDB_PlanStr::ChangePlan_str_order2(int plan_id, int plan_str_owner_id, int pokaz_id, int plan_str_orderNew) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    q->SQL->Clear();
    q->SQL->Add("UPDATE Plan_str");
    q->SQL->Add("SET plan_str_order = :plan_str_orderNew");
    q->SQL->Add("WHERE plan_id = :plan_id");
    q->SQL->Add("AND plan_str_owner_id = :plan_str_owner_id");
    q->SQL->Add("AND pokaz_id = :pokaz_id");
    q->ParamByName("plan_str_orderNew")->AsInteger() = plan_str_orderNew;
    q->ParamByName("plan_id")->AsInteger() = plan_id;
    q->ParamByName("plan_str_owner_id")->AsInteger() = plan_str_owner_id;
    q->ParamByName("pokaz_id")->AsInteger() = pokaz_id;
    q->ExecSQL();
    //MAK q->Free();
    delete q;
}

int TDB_PlanStr::ChangePlan_str_order3(int plan_id, int pokaz_id, int plan_str_orderNew) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    q->SQL->Clear();
    q->SQL->Add("UPDATE Plan_str");
    q->SQL->Add("SET plan_str_order = :plan_str_orderNew");
    q->SQL->Add("WHERE plan_id = :plan_id");
    q->SQL->Add("AND pokaz_id = :pokaz_id");
    q->ParamByName("plan_str_orderNew")->AsInteger() = plan_str_orderNew;
    q->ParamByName("plan_id")->AsInteger() = plan_id;
    q->ParamByName("pokaz_id")->AsInteger() = pokaz_id;
    q->ExecSQL();
    delete q;
    return 0;
}

void TDB_PlanStr::ChangePlan_str4(int plan_id, int pokaz_id, int plan_str_owner_id, int plan_str_order, int Vedomstvo) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    q->SQL->Clear();
    q->SQL->Add("UPDATE Plan_str");
    q->SQL->Add("SET plan_str_owner_id = :plan_str_owner_id, plan_str_order = :plan_str_order, Vedomstvo = :Vedomstvo");
    q->SQL->Add("WHERE plan_id = :plan_id");
    q->SQL->Add("AND pokaz_id = :pokaz_id");
    q->ParamByName("plan_str_owner_id")->AsInteger() = plan_str_owner_id;
    q->ParamByName("plan_str_order")->AsInteger() = plan_str_order;
    q->ParamByName("Vedomstvo")->AsInteger() = Vedomstvo;
    q->ParamByName("plan_id")->AsInteger() = plan_id;
    q->ParamByName("pokaz_id")->AsInteger() = pokaz_id;
    q->ExecSQL();
    //MAK q->Free();
    delete q;
}

void TDB_PlanStr::ChangeOwnerId(int plan_id, int pokaz_id, int plan_str_owner_id) {
    int plan_str_order;
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    plan_str_order = TDB_PlanStr::GetMaxOrder(plan_id, plan_str_owner_id) + 1;
    if (plan_str_order == 1000) {
        plan_str_order++;
    }
    q->SQL->Clear();
    q->SQL->Add("UPDATE Pokaztl");
    //--q->SQL->Add("SET pokaz_owner_id = :plan_str_owner_id, pokaz_vert = :pokaz_vert"); // Это НЕПРАВИЛЬНО
    q->SQL->Add("SET pokaz_owner_id = :plan_str_owner_id");
    q->SQL->Add("WHERE pokaz_id = :pokaz_id");
    q->ParamByName("plan_str_owner_id")->AsInteger() = plan_str_owner_id;
    q->ParamByName("pokaz_id")->AsInteger() = pokaz_id;
    //--if (plan_str_owner_id == 0) q->ParamByName("pokaz_vert")->AsInteger = 0;
    //--else  q->ParamByName("pokaz_vert")->AsInteger = 1;
    q->ExecSQL();
    q->SQL->Clear();
    q->SQL->Add("UPDATE Plan_str");
    q->SQL->Add("SET plan_str_owner_id = :plan_str_owner_id, plan_str_order = :plan_str_order");
    q->SQL->Add("WHERE pokaz_id = :pokaz_id");
    q->ParamByName("plan_str_owner_id")->AsInteger() = plan_str_owner_id;
    q->ParamByName("plan_str_order")->AsInteger() = plan_str_order;
    q->ParamByName("pokaz_id")->AsInteger() = pokaz_id;
    q->ExecSQL();
    //MAK q->Free();
    delete q;
}

void TDB_PlanStr::ChangePokazId(int plan_id, int pokaz_id, int plan_str_owner_id, int NewPokaz_id) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    q->SQL->Clear();
    q->SQL->Add("UPDATE Plan_str");
    q->SQL->Add("SET pokaz_id = :NewPokaz_id");
    q->SQL->Add("WHERE plan_id = :plan_id");
    q->SQL->Add("AND plan_str_owner_id = :plan_str_owner_id");
    q->SQL->Add("AND pokaz_id = :pokaz_id");
    q->ParamByName("plan_id")->AsInteger() = plan_id;
    q->ParamByName("plan_str_owner_id")->AsInteger() = plan_str_owner_id;
    q->ParamByName("pokaz_id")->AsInteger() = pokaz_id;
    q->ParamByName("NewPokaz_id")->AsInteger() = NewPokaz_id;
    q->ExecSQL();
    //MAK q->Free();
    delete q;
}

void TDB_PlanStr::ChangeOwnerId2(int plan_id, int pokaz_id, int plan_str_owner_id) {
    int plan_str_order;
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    plan_str_order = TDB_PlanStr::GetMaxOrder(plan_id, plan_str_owner_id) + 1;
    if (plan_str_order == 1000) {
        plan_str_order++;
    }
    q->SQL->Clear();
    q->SQL->Add("UPDATE Pokaztl");
    q->SQL->Add("SET pokaz_owner_id = :plan_str_owner_id");
    q->SQL->Add("WHERE pokaz_id = :pokaz_id");
    q->ParamByName("plan_str_owner_id")->AsInteger() = plan_str_owner_id;
    q->ParamByName("pokaz_id")->AsInteger() = pokaz_id;
    q->ExecSQL();

    q->SQL->Clear();
    q->SQL->Add("UPDATE Plan_str");
    q->SQL->Add("SET plan_str_owner_id = :plan_str_owner_id, plan_str_order = :plan_str_order");
    q->SQL->Add("WHERE pokaz_id = :pokaz_id");
    q->ParamByName("plan_str_owner_id")->AsInteger() = plan_str_owner_id;
    q->ParamByName("plan_str_order")->AsInteger() = plan_str_order;
    q->ParamByName("pokaz_id")->AsInteger() = pokaz_id;
    q->ExecSQL();

    //MAK q->Free();
    delete q;
}

void TDB_PlanStr::GetAllSubItemAndOwner(TQueryUni *qIn, int plan_id, int pokaz_owner_id, TDateTime curDate) {
    qIn->SQL->Clear();
    qIn->SQL->Add("SELECT *");
    qIn->SQL->Add("FROM Plan_str a, Pokaztl b");
    qIn->SQL->Add("WHERE a.pokaz_id = b.pokaz_id");
    qIn->SQL->Add("AND (a.plan_id = 1 OR a.plan_id = :plan_id)");
    qIn->SQL->Add("AND b.pokaz_id = :pokaz_owner_id");
    qIn->SQL->Add("AND :curDate BETWEEN pokaz_beg_date AND pokaz_end_date");
    qIn->SQL->Add("ORDER BY plan_str_order");
    qIn->ParamByName("plan_id")->AsInteger() = plan_id;
    qIn->ParamByName("pokaz_owner_id")->AsInteger() = pokaz_owner_id;
    qIn->ParamByName("curDate")->AsDateTime() = curDate;
    qIn->Open();
}

void TDB_PlanStr::GetAllSubItem(TQueryUni *qIn, int plan_id, int pokaz_owner_id, TDateTime curDate) {
    qIn->SQL->Clear();
    qIn->SQL->Add("SELECT *");
    qIn->SQL->Add("FROM Plan_str a, Pokaztl b");
    qIn->SQL->Add("WHERE a.pokaz_id = b.pokaz_id");
    qIn->SQL->Add("AND (a.plan_id = 1 OR a.plan_id = :plan_id)");
    qIn->SQL->Add("AND b.pokaz_owner_id = :pokaz_owner_id");
    qIn->SQL->Add("AND :curDate BETWEEN pokaz_beg_date AND pokaz_end_date");
    qIn->SQL->Add(
            "ORDER BY CASE WHEN plan_id > 1 THEN plan_str_order+400 WHEN plan_id = 1 and plan_str_order = 1000 THEN plan_str_order+10000 ELSE plan_str_order END");
    qIn->ParamByName("plan_id")->AsInteger() = plan_id;
    qIn->ParamByName("pokaz_owner_id")->AsInteger() = pokaz_owner_id;
    qIn->ParamByName("curDate")->AsDateTime() = curDate;
    qIn->Open();
}

void TDB_PlanStr::GetAllSubItem2(TQueryUni *qIn, int plan_id, TDateTime curDate) {
    qIn->SQL->Clear();
    qIn->SQL->Add("SELECT *");
    qIn->SQL->Add("FROM Plan_str a, Pokaztl b");
    qIn->SQL->Add("WHERE a.pokaz_id = b.pokaz_id");
    qIn->SQL->Add("AND a.plan_id = :plan_id");
    qIn->SQL->Add("AND :curDate BETWEEN pokaz_beg_date AND pokaz_end_date");
    qIn->ParamByName("plan_id")->AsInteger() = plan_id;
    qIn->ParamByName("curDate")->AsDateTime() = curDate;
    qIn->Open();
}

void TDB_PlanStr::GetAllSubItem0(TQueryUni *qIn, int pokaz_owner_id, TDateTime Year) {
    qIn->SQL->Clear();
    qIn->SQL->Add("SELECT *");
    qIn->SQL->Add("FROM Plan_str a, Pokaztl b");
    qIn->SQL->Add("WHERE a.pokaz_id = b.pokaz_id");
    //!!qIn->SQL->Add("AND (a.plan_id = 1 OR a.plan_id = :plan_id)"); для всех планов
    qIn->SQL->Add("AND b.pokaz_owner_id = :pokaz_owner_id");
    qIn->SQL->Add("AND :Year BETWEEN pokaz_beg_date AND pokaz_end_date");
    //??qIn->SQL->Add("ORDER BY plan_str_owner_id , plan_str_order");
    qIn->SQL->Add("ORDER BY CASE WHEN plan_id > 1 THEN plan_str_order+400 ELSE plan_str_order END");
    //!!qIn->ParamByName("plan_id")->AsInteger = plan_id; для всех планов
    qIn->ParamByName("pokaz_owner_id")->AsInteger() = pokaz_owner_id;
    qIn->ParamByName("Year")->AsDateTime() = Year;
    qIn->Open();
}

void TDB_PlanStr::GetAllSubItemOnlyPlanStr(TQueryUni *qIn, int plan_id, int plan_str_owner_id) {
    qIn->SQL->Clear();
    qIn->SQL->Add("SELECT *");
    qIn->SQL->Add("FROM Plan_str");
    qIn->SQL->Add("WHERE (plan_id = 1 OR plan_id = :plan_id)");
    qIn->SQL->Add("AND plan_str_owner_id = :plan_str_owner_id");
    qIn->SQL->Add("ORDER BY plan_str_order");
    qIn->ParamByName("plan_id")->AsInteger() = plan_id;
    qIn->ParamByName("plan_str_owner_id")->AsInteger() = plan_str_owner_id;
    qIn->Open();
}

void TDB_PlanStr::GetAllSubItemAn(TQueryUni *qIn, int pokaz_owner_id, TDateTime CurDate) {
    qIn->SQL->Clear();
    qIn->SQL->Add("SELECT *");
    qIn->SQL->Add("FROM Pokaztl");
    qIn->SQL->Add("WHERE pokaz_owner_id = :pokaz_owner_id");
    qIn->SQL->Add("AND :CurDate BETWEEN pokaz_beg_date AND pokaz_end_date");
    qIn->ParamByName("pokaz_owner_id")->AsInteger() = pokaz_owner_id;
    qIn->ParamByName("CurDate")->AsDateTime() = CurDate;
    qIn->Open();
}

void TDB_PlanStr::GetAllSubClassItem(TQueryUni *qIn, int class_id, TDateTime CurDate) {
    int parent_class_id;
    std::string s;
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    if (class_id == 0) return;
    s = std::to_string(class_id) + ")";
    TDB_Pokaztl::GetPokaz(q, class_id);
    parent_class_id = q->FieldByName("pokaz_base_id")->AsInteger();
    while (parent_class_id != 0) {
        s = std::to_string(parent_class_id) + "," + s;
        q->Close();
        TDB_Pokaztl::GetPokaz(q, parent_class_id);
        parent_class_id = q->FieldByName("pokaz_base_id")->AsInteger();
    }
    s = "(" + s;
    qIn->SQL->Clear();
    qIn->SQL->Add("SELECT *");
    qIn->SQL->Add("FROM Plan_str a, Pokaztl b");
    qIn->SQL->Add("WHERE a.plan_id = :plan_id ");
    qIn->SQL->Add("AND a.plan_str_owner_id IN " + s);
    qIn->SQL->Add("AND a.pokaz_id = b.pokaz_id");
    qIn->SQL->Add("AND :CurDate BETWEEN pokaz_beg_date AND pokaz_end_date");
    qIn->SQL->Add("ORDER BY plan_str_owner_id , plan_str_order");
    qIn->ParamByName("CurDate")->AsDateTime() = CurDate;
    qIn->ParamByName("plan_id")->AsInteger() = 1;
    qIn->Open();
}

std::string TDB_PlanStr::GetVedomstvo(int plan_id, int pokaz_id) {
    std::string s;
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    q->SQL->Clear();
    q->SQL->Add("SELECT Vedomstvo");
    q->SQL->Add("FROM Plan_str");
    q->SQL->Add("WHERE Plan_str.plan_id = :plan_id");
    q->SQL->Add("AND Plan_str.pokaz_id = :pokaz_id");
    q->ParamByName("plan_id")->AsInteger() = plan_id;
    q->ParamByName("pokaz_id")->AsInteger() = pokaz_id;
    q->Open();
    if (!q->EoF()) {
        s = q->FieldByName("Vedomstvo")->AsString();
        if (s.empty()) s = TDB_Plan::GetPPPOkrug();
        return s;
    }
    return "";
}

void TDB_PlanStr::SetVedomstvo(int plan_id, int pokaz_id, std::string Vedomstvo) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    std::string vedomstvo = Vedomstvo;
    if (vedomstvo == TDB_Plan::GetPPPOkrug()) {
        vedomstvo.clear();
    }
    q->SQL->Clear();
    q->SQL->Add("UPDATE Plan_str");
    q->SQL->Add("SET Vedomstvo = :Vedomstvo");
    q->SQL->Add("WHERE Plan_str.plan_id = :plan_id");
    q->SQL->Add("AND Plan_str.pokaz_id = :pokaz_id");
    q->ParamByName("plan_id")->AsInteger() = plan_id;
    q->ParamByName("pokaz_id")->AsInteger() = pokaz_id;
    q->ParamByName("Vedomstvo")->AsString() = vedomstvo;
    q->ExecSQL();
}

bool TDB_PlanStr::EstLiPokazBK(int plan_str_owner_id, int kbk_id) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    bool result = false;
    q->SQL->Clear();
    q->SQL->Add("SELECT Plans.plan_id, plan_name");
    q->SQL->Add("FROM Plan_str, Plans, Pokaztl");
    q->SQL->Add("WHERE plan_str_owner_id = :plan_str_owner_id");
    q->SQL->Add("AND Plan_str.plan_id = Plans.plan_id");
    q->SQL->Add("AND Plan_str.pokaz_id = Pokaztl.pokaz_id");
    q->SQL->Add("AND Pokaztl.enum_id = :kbk_id");
    q->ParamByName("plan_str_owner_id")->AsInteger() = plan_str_owner_id;
    q->ParamByName("kbk_id")->AsInteger() = kbk_id;
    q->Open();
    if (!q->EoF()) {
        //--SystFuncs.MessageBoxExclamation(PChar('Такая часть КБК у этого владельца уже включена для субъекта <'+FieldByName('plan_name').AsString+'>'), MsgVnim);
        result = true;
    }
    return result;
}

bool TDB_PlanStr::EstLiItem(int plan_id, int plan_str_owner_id, int pokaz_id, bool isShowMsg) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    bool result = false;
    q->SQL->Clear();
    q->SQL->Add("SELECT pokaz_name");
    q->SQL->Add("FROM Plan_str, Pokaztl");
    q->SQL->Add("WHERE plan_id = :plan_id");
    q->SQL->Add("AND plan_str_owner_id = :plan_str_owner_id");
    q->SQL->Add("AND Plan_str.pokaz_id = :pokaz_id");
    q->SQL->Add("AND Pokaztl.pokaz_id = Plan_str.pokaz_id");
    q->ParamByName("plan_id")->AsInteger() = plan_id;
    q->ParamByName("plan_str_owner_id")->AsInteger() = plan_str_owner_id;
    q->ParamByName("pokaz_id")->AsInteger() = pokaz_id;
    q->Open();
    if (!q->EoF()) {
        //--if (isShowMsg) {
        //--SystFuncs.MessageBoxExclamation(PChar("Для субъекта показатель <"+FieldByName("pokaz_name").AsString+"> уже описан"), MsgVnim);
        //--}
        result = true;
    }
    return result;
}

bool TDB_PlanStr::EstLiItemForSubject(int plan_id, int pokaz_id) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    bool result = false;
    q->SQL->Clear();
    q->SQL->Add("SELECT pokaz_id");
    q->SQL->Add("FROM Plan_str");
    q->SQL->Add("WHERE ((plan_id = :plan_id) or (plan_id = 1))");
    q->SQL->Add("AND pokaz_id = :pokaz_id");
    q->ParamByName("plan_id")->AsInteger() = plan_id;
    q->ParamByName("pokaz_id")->AsInteger() = pokaz_id;
    q->Open();
    if (!q->EoF()) {
        result = true;
    }
    return result;
}

int TDB_PlanStr::getPlanidByPokazid(int pokaz_id) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    int result = 0;

    q->SQL->Clear();
    q->SQL->Add("SELECT plan_id, pokaz_id");
    q->SQL->Add("FROM Plan_str");
    q->SQL->Add("WHERE pokaz_id = :pokaz_id");
    q->ParamByName("pokaz_id")->AsInteger() = pokaz_id;
    q->Open();

    if (!q->EoF()) {
        result = q->FieldByName("plan_id")->AsInteger();
    }

    //MAK q->Free();
    delete q;
    return result;
}

int TDB_PlanStr::getPokazid(int plan_id, int plan_str_owner_id, int plan_str_order) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    int result = 0;

    q->SQL->Clear();
    q->SQL->Add("SELECT pokaz_id");
    q->SQL->Add("FROM Plan_str");
    q->SQL->Add("WHERE plan_id = :plan_id");
    q->SQL->Add("AND plan_str_owner_id = :plan_str_owner_id");
    q->SQL->Add("AND plan_str_order = :plan_str_order");
    q->ParamByName("plan_id")->AsInteger() = plan_id;
    q->ParamByName("plan_str_owner_id")->AsInteger() = plan_str_owner_id;
    q->ParamByName("plan_str_order")->AsInteger() = plan_str_order;
    q->Open();

    if (!q->EoF()) {
        result = q->FieldByName("pokaz_id")->AsInteger();
    }

    //MAK q->Free();
    delete q;
    return result;
}

std::string TDB_PlanStr::GetAllSubRazd(int plan_id, int owner_id, int year) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    std::string result = "(";

    q->SQL->Clear();
    q->SQL->Add("SELECT a.pokaz_id FROM Plan_str a, Pokaztl b");
    q->SQL->Add("WHERE plan_id IN (1, " + std::to_string(plan_id) + ")");
    q->SQL->Add("AND plan_str_owner_id = " + std::to_string(owner_id));
    switch (GetDBType()) {
        case TDBType::MSSQL:
            q->SQL->Add("AND " + std::to_string(year) + " BETWEEN YEAR(b.pokaz_beg_date) AND YEAR(b.pokaz_end_date)");
            break;
        case TDBType::Postgre:
            q->SQL->Add("AND " + std::to_string(year) +
                        " BETWEEN date_part('year', b.pokaz_beg_date) AND date_part('year', b.pokaz_end_date)");
            break;
    }
    q->SQL->Add("AND pokaz_is_razd = 1");
    q->Open();

    if (q->EoF()) {
        result += std::to_string(owner_id) + ")";
        //MAK q->Free();
        delete q;
        return result;
    }

    while (!q->EoF()) {
        if (result != "(") result += ",";
        result += q->FieldByName("pokaz_id")->AsString();
        q->Next();
    }

    result += ")";
    //MAK q->Free();
    delete q;
    return result;
}
