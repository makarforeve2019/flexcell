//
// Created by Alex on 17.04.2024.
//

#include "RS_Plan.h"
#include "../DB/unDefDBconnect.h"
#include "../DB/DB_Common.h"
#include "../DB/DB_Plan.h"
#include "../DB/DB_UserPlan.h"
#include <../nlohmann/json.hpp>

using json = nlohmann::json;

std::string Plan::GetAllPlanForUser(int user_id) {
    std::string result = "Not Result";
    try {
        TQueryUni qTmp(DefDBconnect.Conn);
        TDB_Plan::GetAllPlan_forUser(&qTmp, user_id);
        json root;
        json contentArray = json::array();

        while (!qTmp.EoF()) {
            json planObject;
            planObject["plan_id"] = qTmp.FieldByName("plan_id")->AsInteger();
            planObject["plan_name"] = qTmp.FieldByName("plan_name")->AsString();
            planObject["plan_owner_id"] = qTmp.FieldByName("plan_owner_id")->AsInteger();
            planObject["plan_short_name"] = qTmp.FieldByName("plan_short_name")->AsString();
            planObject["plan_is_rajon"] = qTmp.FieldByName("plan_is_rajon")->AsInteger();
            planObject["god_beg"] = qTmp.FieldByName("god_beg")->AsString();
            planObject["god_end"] = qTmp.FieldByName("god_end")->AsString();
            planObject["user_perm"] = qTmp.FieldByName("user_perm")->AsInteger();
            contentArray.push_back(planObject);
            qTmp.Next();
        }

        root["Plan"] = contentArray;

        result = root.dump();
    } catch (const std::exception& e) {
        std::cerr << "Ошибка при выполнении запроса: " << e.what() << std::endl;
    }
    return result;
}

std::string Plan::LoadPlanPermForCurUser(int user_id) {
    std::string result = "Not Result";
    try {
        TQueryUni qTmp(DefDBconnect.Conn);
        TDB_UserPlan::LoadPlanPermForCurUser(&qTmp, user_id);
        json root;
        json contentArray = json::array();

        while (!qTmp.EoF()) {
            json planObject;
            planObject["plan_id"] = qTmp.FieldByName("plan_id")->AsInteger();
            planObject["user_perm"] = qTmp.FieldByName("user_perm")->AsInteger();
            contentArray.push_back(planObject);
            qTmp.Next();
        }

        root["Plan"] = contentArray;
        result = root.dump();
    } catch (const std::exception& e) {
        std::cerr << "Ошибка при выполнении запроса: " << e.what() << std::endl;
    }
    return result;
}


