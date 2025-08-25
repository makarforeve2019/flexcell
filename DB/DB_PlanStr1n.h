#ifndef DB_PLANSTR1N_H
#define DB_PLANSTR1N_H

#include "DB_Common.h"


class TDB_PlanStr1n {
public:
    static void AddElem(int plan_id, int plan_str_owner_id, int pokaz_id, int class_id);
    static int GetMaxOrder(int plan_id, int plan_str_owner_id, int class_id);
    static void GetAllSubItem(TQueryUni *qIn, int plan_id, int pokaz_owner_id, int class_id);
    static void GetAllSubItemYear(TQueryUni *qIn, int plan_id, int pokaz_owner_id, int class_id, int god);
    static void delElem(int pokaz_id);
    static void delElem(int plan_id, int plan_str_owner_id, int pokaz_id, int class_id);
    static bool GetPlanStr(TQueryUni *qIn, int plan_id, int plan_str_owner_id, int pokaz_id, int class_id);
    static bool GetPlanStr(TQueryUni *qIn, int plan_id, int pokaz_id, int class_id);
    static int getOrderByPokazId(int plan_id, int plan_str_owner_id, int pokaz_id, int class_id);
    static void InsertElemByNum(int plan_id, int plan_str_owner_id, int plan_str_order, int pokaz_id, int class_id, bool isNew);
    static void ChangePlan_str_order(int plan_id, int plan_str_owner_id, int plan_str_order, int plan_str_orderNew);
    static void ChangePlan_str_order2(int plan_id, int plan_str_owner_id, int pokaz_id, int class_id, int plan_str_orderNew);
    static void AddElemByNum(int plan_id, int plan_str_owner_id, int plan_str_order, int pokaz_id, int class_id);
    static int getPlanStrOwnerId(int plan_id, int pokaz_id, int class_id);
    static int getPlanStrOwnerIdByGod(int plan_id, int pokaz_id, int class_id, int god);
};

#endif // DB_PLANSTR1N_H
