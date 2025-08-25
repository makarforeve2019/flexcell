#ifndef DB_DATEUTVPLANS_H
#define DB_DATEUTVPLANS_H


#include "DB_Common.h"

class TDB_DateUtvPlans {
public:
    static void GetAllGodPokazd(TQueryUni *qIn);
    static void GetAllGodPokazd2(TQueryUni *qIn, int god);
    static bool GetGodPlanCast(TQueryUni *qIn, int plan_id, int cast_id, int god);
    static void GetAllItems(TQueryUni *qIn, int God, int pokaz_id);
    static void AddOrUpdItem(int plan_id, int cast_id, int god, TDateTime date_utv, TDateTime date_copy);
    static void AddItem(int plan_id, int cast_id, int god, TDateTime date_utv, TDateTime date_copy);
    static void AddItem2(int plan_id, int cast_id, int god, TDateTime date_utv, int month,  std::string podpisFIO,  std::string dlgnost);
    static void UpdItem(int plan_id, int cast_id, int god, TDateTime date_utv);
    static int FindItem(int plan_id, int cast_id, int god);
    static int FindItem1(int plan_id, int cast_id, int god, int month, TDateTime &date_utv);
    static void DelItem(int god, int cast_id);
    static void DelItem1(int plan_id, int cast_id, int god);
};

#endif // DB_DATEUTVPLANS_H
