#ifndef CO_RP_015evalH
#define CO_RP_015evalH

#include "DB_Common.h"

class TCO_RP_015eval {
public:
    TDateTime lastdate_15fcu;
    bool isLoad;
    TStringList valList;

    static void createObj();

    static void freeObj();

    static double doEval(int plan_id, int pokaz_id, int VidOtc, int god, int Nvalue);
    void LoadToValList(int god);
};
#endif