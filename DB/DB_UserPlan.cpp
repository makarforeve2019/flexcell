#include "DB_UserPlan.h"
#include "unDefDBconnect.h"
#include "DB_Plan.h"
#include "../GlobalVars/dmGlobalVars.h"
#include "../SystFuncs/SystFuncsLogic.h"

#include <stdexcept>

int TDB_UserPlan::GetUserPlan_Perm(int plan_id) {
    int result = 2;

    if (plan_id == 0) {
        result = 0;
        return result;
    }

    if (plan_id == 1) {
    }

    if (!GlobalVars->isArLoad) {
        TDB_UserPlan::LoadPlanPermForCurUser();
    }

    if (plan_id >= 1 && plan_id <= 5000) {
        result = GlobalVars->arUserPlanPerm[plan_id];
    } else {
        throw std::out_of_range("Parameter plan_id of function TDB_UserPlan::GetUserPlan_Perm is greater than 5000");
    }

    return result;
}

int TDB_UserPlan::GetUserPlan_Perm(int user_id, int plan_id) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    int result = 0;
    q->SQL->Clear();
    q->SQL->Add("SELECT user_perm FROM User_plan");
    q->SQL->Add("WHERE user_id = :user_id");
    q->SQL->Add("AND plan_id = :plan_id");
    q->ParamByName("user_id")->AsInteger() = user_id;
    q->ParamByName("plan_id")->AsInteger() = plan_id;
    q->Open();
    
    if (!q->EoF()) {
        result = q->FieldByName("user_perm")->AsInteger();
    }
    delete q;
    return result;
}

int TDB_UserPlan::GetUserPref_Perm() {
    int planIdPref = TDB_Plan::GetIdPrefOkrug();
    return TDB_UserPlan::GetUserPlan_Perm(planIdPref);
}

void TDB_UserPlan::LoadPlanPermForCurUser(TQueryUni *q, int user_id) {
//    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    q->SQL->Clear();
    q->SQL->Add("SELECT plan_id, user_perm FROM User_plan");
    q->SQL->Add("WHERE user_id = :user_id");
    q->ParamByName("user_id")->AsInteger() = user_id;
    q->Open();
}

void TDB_UserPlan::LoadPlanPermForCurUser() {
    int user_id = GetUserId();
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);

    q->SQL->Clear();
    q->SQL->Add("SELECT plan_id, user_perm FROM User_plan");
    q->SQL->Add("WHERE user_id = :user_id");
    q->ParamByName("user_id")->AsInteger() = user_id;
    q->Open();

    while (!q->EoF()) {
        GlobalVars->arUserPlanPerm[q->FieldByName("plan_id")->AsInteger()] = q->FieldByName("user_perm")->AsInteger();
        q->Next();
    }

    delete q;
    GlobalVars->isArLoad = true;
}

void TDB_UserPlan::GetUserPerm(TQueryUni *qIn, int user_id) {
    qIn->SQL->Clear(); 
    qIn->SQL->Add("SELECT user_perm, User_plan.plan_id, plan_name FROM User_plan, Plans");
    qIn->SQL->Add("WHERE user_id = :user_id");
    qIn->SQL->Add("AND Plans.plan_id = User_plan.plan_id");
    qIn->SQL->Add("ORDER BY plan_order");
    qIn->ParamByName("user_id")->AsInteger() = user_id;
    qIn->Open();
}

void TDB_UserPlan::DelPlansForUser(int user_id) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);

    q->SQL->Clear();
    q->SQL->Add("DELETE FROM User_plan");
    q->SQL->Add("WHERE user_id = :user_id");
    q->ParamByName("user_id")->AsInteger() = user_id;
    q->ExecSQL();

    delete q;
}

bool TDB_UserPlan::isUserPlan(int user_id, int plan_id) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);

    q->SQL->Clear();
    q->SQL->Add("SELECT user_perm FROM User_plan");
    q->SQL->Add("WHERE user_id = :user_id");
    q->SQL->Add("AND plan_id = :plan_id");
    q->ParamByName("user_id")->AsInteger() = user_id;
    q->ParamByName("plan_id")->AsInteger() = plan_id;;
    q->Open();

    bool result = !q->EoF();
    q->Close();
    delete q;
    return result;
}

void TDB_UserPlan::DelUserPlan(int user_id, int plan_id) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);

    q->SQL->Clear();
    q->SQL->Add("DELETE FROM User_plan");
    q->SQL->Add("WHERE user_id = :user_id");
    q->SQL->Add("AND plan_id = :plan_id");
    q->ParamByName("user_id")->AsInteger() = user_id;
    q->ParamByName("plan_id")->AsInteger() = plan_id;;
    q->ExecSQL();

    delete q;
}

void TDB_UserPlan::AddUserPerm(int user_id, int plan_id, int user_perm) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);

    q->SQL->Clear();
    q->SQL->Add("INSERT INTO User_plan");
    q->SQL->Add("(user_id, plan_id, user_perm)");
    q->SQL->Add("VALUES (:user_id, :plan_id, :user_perm)");
    q->ParamByName("user_id")->AsInteger() = user_id;
    q->ParamByName("plan_id")->AsInteger() = plan_id;;
    q->ParamByName("user_perm")->AsInteger() = user_perm;;
    q->ExecSQL();

    delete q;
}

void TDB_UserPlan::UpdUserPerm(int user_id, int plan_id, int user_perm) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);

    q->SQL->Clear();
    q->SQL->Add("UPDATE User_plan");
    q->SQL->Add("SET user_perm = :user_perm");
    q->SQL->Add("WHERE user_id = :user_id");
    q->SQL->Add("AND plan_id = :plan_id");
    q->ParamByName("user_id")->AsInteger() = user_id;
    q->ParamByName("plan_id")->AsInteger() = plan_id;;
    q->ParamByName("user_perm")->AsInteger() = user_perm;;
    q->ExecSQL();

    delete q;
}
