#ifndef DB_UnikNmbrDgvrH
#define DB_UnikNmbrDgvrH


#include "DB_Common.h"


class TDB_UnikNmbrDgvr {
public:
    static int GetId();

    static int GetNextUnikNmbr(int god, int plan_id);

    static int GetCurUnikNmbr(int god, int plan_id, int& aundgvr_id);

    static int AddNewItem(int god, int plan_id);

    static void UpdItem(int undgvr_id, int curUnikNmbrDgvr);
};

#endif
