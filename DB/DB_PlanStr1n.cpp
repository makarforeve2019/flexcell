#include "DB_PlanStr1n.h"
#include "unDefDBconnect.h"
#include "../SystFuncs/SystFuncsLogic.h"
#include "DB_Pokaztl.h"
#include "DB_Plan.h"

void TDB_PlanStr1n::AddElem(int plan_id, int plan_str_owner_id, int pokaz_id, int class_id) {
    int plan_str_order;
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    
    plan_str_order = TDB_PlanStr1n::GetMaxOrder(plan_id, plan_str_owner_id, class_id);
    plan_str_order += 1;
    
    q->SQL->Clear();
    q->SQL->Add("INSERT INTO Plan_str1n");
    q->SQL->Add("(plan_id, plan_str_owner_id, plan_str_order, pokaz_id, class_id)");
    q->SQL->Add("VALUES (:plan_id, :plan_str_owner_id, :plan_str_order, :pokaz_id, :class_id)");
    q->ParamByName("plan_id")->AsInteger() = plan_id;
    q->ParamByName("pokaz_id")->AsInteger() = pokaz_id;
    q->ParamByName("plan_str_owner_id")->AsInteger() = plan_str_owner_id;
    q->ParamByName("plan_str_order")->AsInteger() = plan_str_order;
    q->ParamByName("class_id")->AsInteger() = class_id;
    q->ExecSQL();
    
    delete q;
}

int TDB_PlanStr1n::GetMaxOrder(int plan_id, int plan_str_owner_id, int class_id) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    
    int result = 0;
    q->SQL->Clear();
    q->SQL->Add("SELECT MAX(plan_str_order) FROM Plan_str1n ");
    q->SQL->Add("WHERE plan_id = :plan_id");
    q->SQL->Add("AND plan_str_owner_id = :plan_str_owner_id");
    q->SQL->Add("AND class_id = :class_id");
    q->ParamByName("plan_id")->AsInteger() = plan_id;
    q->ParamByName("plan_str_owner_id")->AsInteger() = plan_str_owner_id;
    q->ParamByName("class_id")->AsInteger() = class_id;
    q->Open();
    
    if (q->EoF()) {
        result = 0;
    } else {
        result = q->Fields[0].AsInteger();
    }
    
    delete q;
    return result;
}

void TDB_PlanStr1n::GetAllSubItem(TQueryUni *qIn, int plan_id, int pokaz_owner_id, int class_id) {
    qIn->SQL->Clear();
    qIn->SQL->Add("SELECT * FROM Plan_str1n a, Pokaztl b");
    qIn->SQL->Add("WHERE a.pokaz_id = b.pokaz_id");
    qIn->SQL->Add("AND (a.plan_id = 1 OR a.plan_id = :plan_id)");
    qIn->SQL->Add("AND a.plan_str_owner_id = :pokaz_owner_id");
    qIn->SQL->Add("AND a.class_id = :class_id");
    qIn->SQL->Add("ORDER BY plan_str_order");
    qIn->ParamByName("plan_id")->AsInteger() = plan_id;
    qIn->ParamByName("pokaz_owner_id")->AsInteger() = pokaz_owner_id;
    qIn->ParamByName("class_id")->AsInteger() = class_id;
    qIn->Open();
}

void TDB_PlanStr1n::GetAllSubItemYear(TQueryUni *qIn, int plan_id, int pokaz_owner_id, int class_id, int god) {
    qIn->SQL->Clear();

    qIn->SQL->Add("SELECT * FROM Plan_str1n a, Pokaztl b");
    qIn->SQL->Add("WHERE plan_id = " + IntToStr(plan_id));
    qIn->SQL->Add("AND plan_str_owner_id = " + IntToStr(pokaz_owner_id));
    qIn->SQL->Add("AND a.pokaz_id = b.pokaz_id");
    qIn->SQL->Add("AND a.class_id = " + IntToStr(class_id));
    switch (GetDBType()) {
        case TDBType::MSSQL:
            qIn->SQL->Add("AND " + IntToStr(god) + " BETWEEN YEAR(b.pokaz_beg_date) AND YEAR(b.pokaz_end_date)");
            break;
        case TDBType::Postgre:
            qIn->SQL->Add("AND " + IntToStr(god) + " BETWEEN date_part(''year'', b.pokaz_beg_date) AND date_part(''year'', b.pokaz_end_date)");
            break;
    }
    qIn->SQL->Add("ORDER BY plan_str_order");
    qIn->Open();
}

void TDB_PlanStr1n::delElem(int pokaz_id) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    
    q->SQL->Add("DELETE FROM Plan_str1n");
    q->SQL->Add("WHERE pokaz_id = " + IntToStr(pokaz_id));
    q->ExecSQL();
    
    delete q;
}

void TDB_PlanStr1n::delElem(int plan_id, int plan_str_owner_id, int pokaz_id, int class_id) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    
    q->SQL->Add("DELETE FROM Plan_str1n");
    q->SQL->Add("WHERE plan_id = :plan_id");
    q->SQL->Add("AND plan_str_owner_id = :plan_str_owner_id");
    q->SQL->Add("AND pokaz_id = :pokaz_id");
    q->SQL->Add("AND class_id = :class_id");
    q->ParamByName("plan_id")->AsInteger() = plan_id;
    q->ParamByName("plan_str_owner_id")->AsInteger() = plan_str_owner_id;
    q->ParamByName("pokaz_id")->AsInteger() = pokaz_id;
    q->ParamByName("class_id")->AsInteger() = class_id;
    q->ExecSQL();
    
    delete q;
}

bool TDB_PlanStr1n::GetPlanStr(TQueryUni *qIn, int plan_id, int pokaz_id, int class_id) {
    bool Result = true;
    qIn->SQL->Clear();
    qIn->SQL->Add("SELECT * FROM Plan_str1n");
    qIn->SQL->Add("WHERE plan_id = :plan_id");
    qIn->SQL->Add("AND pokaz_id = :pokaz_id");
    qIn->SQL->Add("AND class_id = :class_id");
    qIn->ParamByName("plan_id")->AsInteger() = plan_id;
    qIn->ParamByName("pokaz_id")->AsInteger() = pokaz_id;
    qIn->ParamByName("class_id")->AsInteger() = class_id;
    qIn->Open();
    if (qIn->EoF()) {
        Result = false;
    }
    return Result;
}

bool TDB_PlanStr1n::GetPlanStr(TQueryUni *qIn, int plan_id, int plan_str_owner_id, int pokaz_id, int class_id) {
    bool Result = true;
    qIn->SQL->Clear();
    qIn->SQL->Add("SELECT * FROM Plan_str1n");
    qIn->SQL->Add("WHERE plan_id = :plan_id");
    qIn->SQL->Add("AND pokaz_id = :pokaz_id");
    qIn->SQL->Add("AND plan_str_owner_id = :plan_str_owner_id");
    qIn->SQL->Add("AND class_id = :class_id");
    qIn->ParamByName("plan_id")->AsInteger() = plan_id;
    qIn->ParamByName("pokaz_id")->AsInteger() = pokaz_id;
    qIn->ParamByName("plan_str_owner_id")->AsInteger() = plan_str_owner_id;
    qIn->ParamByName("class_id")->AsInteger() = class_id;
    qIn->Open();
    if (qIn->EoF()) {
        Result = false;
    }
    return Result;
}

int TDB_PlanStr1n::getOrderByPokazId(int plan_id, int plan_str_owner_id, int pokaz_id, int class_id) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    
    if (TDB_PlanStr1n::GetPlanStr(q, plan_id, plan_str_owner_id, pokaz_id, class_id)) {
        int order = q->FieldByName("plan_str_order")->AsInteger();
        delete q;
        return order;
    } else {
        delete q;
        return 0;
    }
}

void TDB_PlanStr1n::InsertElemByNum(int plan_id, int plan_str_owner_id, int plan_str_order,
         int pokaz_id, int class_id, bool isNew) {
    int InsertOrder = 1;
    TQueryUni *qWrk = new TQueryUni(DefDBconnect.Conn);
    TDB_PlanStr1n::GetAllSubItem(qWrk, plan_id, plan_str_owner_id, class_id);
    if (!qWrk->EoF()) {
        qWrk->Last();
        while (!qWrk->BoF()) {
            if (qWrk->FieldByName("plan_str_order")->AsInteger() == 1000) {
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
            TDB_PlanStr1n::ChangePlan_str_order(plan_id, qWrk->FieldByName("plan_str_owner_id")->AsInteger(),
               qWrk->FieldByName("plan_str_order")->AsInteger(), qWrk->FieldByName("plan_str_order")->AsInteger() + 1);
            qWrk->Prior();
        }
    }
    delete qWrk;
    if (isNew) {
        TDB_PlanStr1n::AddElemByNum(plan_id, plan_str_owner_id, InsertOrder, pokaz_id, class_id);
    } else {
        TDB_PlanStr1n::ChangePlan_str_order2(plan_id, plan_str_owner_id, pokaz_id, class_id, plan_str_order);
    }
}

void TDB_PlanStr1n::ChangePlan_str_order(int plan_id, int plan_str_owner_id,
          int plan_str_order, int plan_str_orderNew) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    q->SQL->Clear();
    q->SQL->Add("UPDATE Plan_str1n");
    q->SQL->Add("SET plan_str_order = :plan_str_orderNew");
    q->SQL->Add("WHERE plan_id = :plan_id");
    q->SQL->Add("AND plan_str_owner_id = :plan_str_owner_id");
    q->SQL->Add("AND plan_str_order = :plan_str_order");
    q->ParamByName("plan_str_orderNew")->AsInteger() = plan_str_orderNew;
    q->ParamByName("plan_id")->AsInteger() = plan_id;
    q->ParamByName("plan_str_owner_id")->AsInteger() = plan_str_owner_id;
    q->ParamByName("plan_str_order")->AsInteger() = plan_str_order;
    q->ExecSQL();
    delete q;
}

void TDB_PlanStr1n::ChangePlan_str_order2(int plan_id, int plan_str_owner_id,
          int pokaz_id, int class_id, int plan_str_orderNew) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    q->SQL->Clear();
    q->SQL->Add("UPDATE Plan_str1n");
    q->SQL->Add("SET plan_str_order = :plan_str_orderNew");
    q->SQL->Add("WHERE plan_id = :plan_id");
    q->SQL->Add("AND plan_str_owner_id = :plan_str_owner_id");
    q->SQL->Add("AND pokaz_id = :pokaz_id");
    q->SQL->Add("AND class_id = :class_id");
    q->ParamByName("plan_str_orderNew")->AsInteger() = plan_str_orderNew;
    q->ParamByName("plan_id")->AsInteger() = plan_id;
    q->ParamByName("plan_str_owner_id")->AsInteger() = plan_str_owner_id;
    q->ParamByName("pokaz_id")->AsInteger() = pokaz_id;
    q->ParamByName("class_id")->AsInteger() = class_id;
    q->ExecSQL();
    delete q;
}

void TDB_PlanStr1n::AddElemByNum(int plan_id, int plan_str_owner_id, int plan_str_order,
         int pokaz_id, int class_id) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    q->SQL->Clear();
    q->SQL->Add("INSERT INTO Plan_str1n");
    q->SQL->Add("(plan_id, plan_str_owner_id, plan_str_order, pokaz_id, class_id)");
    q->SQL->Add("VALUES (:plan_id, :plan_str_owner_id, :plan_str_order, :pokaz_id, :class_id)");
    q->ParamByName("plan_id")->AsInteger() = plan_id;
    q->ParamByName("pokaz_id")->AsInteger() = pokaz_id;
    q->ParamByName("plan_str_owner_id")->AsInteger() = plan_str_owner_id;
    q->ParamByName("plan_str_order")->AsInteger() = plan_str_order;
    q->ParamByName("class_id")->AsInteger() = class_id;
    q->ExecSQL();
    delete q;
}

int TDB_PlanStr1n::getPlanStrOwnerId(int plan_id, int pokaz_id, int class_id) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    TDB_PlanStr1n::GetPlanStr(q, plan_id, pokaz_id, class_id);
    int planStrOwnerId = q->FieldByName("plan_str_owner_id")->AsInteger();
    delete q;
    return planStrOwnerId;
}

int TDB_PlanStr1n::getPlanStrOwnerIdByGod(int plan_id, int pokaz_id, int class_id, int god) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    TDB_PlanStr1n::GetPlanStr(q, plan_id, pokaz_id, class_id);
    int planStrOwnerId = 0;
    while (!q->EoF()) {
        int wPokaz_id = q->FieldByName("plan_str_owner_id")->AsInteger();
        TDateTime pokaz_beg_date, pokaz_end_date;
        int level_sign;
        TDB_Pokaztl::GetBegEndLevel(wPokaz_id, pokaz_beg_date, pokaz_end_date, level_sign);
        if (YearOf(pokaz_beg_date) == god) {
            planStrOwnerId = wPokaz_id;
            break;
        }
        q->Next();
    }
    delete q;
    return planStrOwnerId;
}
