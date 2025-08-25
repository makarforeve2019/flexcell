#ifndef DB_RP_067evalH
#define DB_RP_067evalH

#include "ClassesUtils.h"



class TDB_RP_067eval {
public:
    int id_obmena;
    TDateTime po_naDatu;
    bool isLoad;
    TStringList valList;

    static void createObj();
    static void freeObj();
    static double doEval(int plan_id, int pokaz_id, int VidOtc, int god, int Nvalue);
    static double doEvalByKBK(int plan_id, std::string KBK, int god, int Nvalue);
    static double doEvalUni(bool isKeyByPlanId, int plan_id, int pokaz_id, std::string KBK, int VidOtc, int god, int Nvalue);
    void LoadToValList(int god, bool isKeyByPlanId);
};
#endif