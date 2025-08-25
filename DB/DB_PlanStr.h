#ifndef DB_PlanStrH
#define DB_PlanStrH


#include "DB_Common.h"

#include <string> // Add this line

class TDB_PlanStr
{
public:
    static void AddElem(bool isItogo, int plan_id, int plan_str_owner_id, int pokaz_id);
    static void AddElemByNum(int plan_id, int plan_str_owner_id, int plan_str_order, int pokaz_id);
    static void InsertElemByNum(int plan_id, int plan_str_owner_id, int plan_str_order, int pokaz_id, bool isNew);
    static void InsertElemByNum2(int plan_id, int plan_str_owner_id, int plan_str_order, int pokaz_id, bool isNew);
    static int GetMaxOrder(int plan_id, int plan_str_owner_id);
    static bool GetPlanStr(TQueryUni *qIn, int plan_id, int pokaz_id);
    static bool GetPlanStr1(TQueryUni *qIn, int plan_id, int pokaz_id);
    static bool InPlanStrAn(int pokaz_id, int &plan_id);
    static std::string getPlanNameByPokazId(int pokaz_id, int &plan_id); // Change return type to std::string
    static int getOrderByPokazId(int plan_id, int pokaz_id);
    static int getPlanStrOwnerId(int plan_id, int pokaz_id);
    static int getPredPokazId(int plan_str_owner_id, int plan_id, int pokaz_id);
    static void ChangeOrder(int plan_id, int pokaz_id1, int pokaz_id2);
    static void ChangePlan_str_order(int plan_id, int plan_str_owner_id, int plan_str_order, int plan_str_orderNew);
    static void ChangePlan_str_order2(int plan_id, int plan_str_owner_id, int pokaz_id, int plan_str_orderNew);
    static int ChangePlan_str_order3(int plan_id, int pokaz_id, int plan_str_orderNew);
    static void ChangePlan_str4(int plan_id, int pokaz_id, int plan_str_owner_id, int plan_str_order, int Vedomstvo);
    static void ChangeOwnerId(int plan_id, int pokaz_id, int plan_str_owner_id);
    static void ChangePokazId(int plan_id, int pokaz_id, int plan_str_owner_id, int NewPokaz_id);
    static void ChangeOwnerId2(int plan_id, int pokaz_id, int plan_str_owner_id);
    static void GetAllSubItemAndOwner(TQueryUni *qIn, int plan_id, int pokaz_owner_id, TDateTime curDate);
    static void GetAllSubItem(TQueryUni *qIn, int plan_id, int pokaz_owner_id, TDateTime curDate);
    static void GetAllSubItem2(TQueryUni *qIn, int plan_id, TDateTime curDate);
    static void GetAllSubItem0(TQueryUni *qIn, int pokaz_owner_id, TDateTime year);
    static void GetAllSubItemOnlyPlanStr(TQueryUni *qIn, int plan_id, int plan_str_owner_id);
    static void GetAllSubItemAn(TQueryUni *qIn, int pokaz_owner_id, TDateTime CurDate);
    static void GetAllSubClassItem(TQueryUni *qIn, int class_id, TDateTime CurDate);
    static std::string GetVedomstvo(int plan_id, int pokaz_id); // Change return type to std::string
    static void SetVedomstvo(int plan_id, int pokaz_id, std::string Vedomstvo); // Change parameter type to std::string
    static bool EstLiPokazBK(int plan_str_owner_id, int kbk_id);
    static bool EstLiItem(int plan_id, int plan_str_owner_id, int pokaz_id, bool isShowMsg);
    static bool EstLiItemForSubject(int plan_id, int pokaz_id);
    static int getPlanidByPokazid(int pokaz_id);
    static int getPokazid(int plan_id, int plan_str_owner_id, int plan_str_order);
    static std::string GetAllSubRazd(int plan_id, int owner_id, int year);
};

#endif
