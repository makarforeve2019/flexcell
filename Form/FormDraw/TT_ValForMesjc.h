
#ifndef TT_VALFORMESJC_H
#define TT_VALFORMESJC_H

#include <string>
#include "unFormLogicF1.h"
#include "../FormTypes.h"
#include "../DB/DB_Common.h"
#include "../DB/unDBType.h"



class TValForMesjc {
public:
    TStringList* sl;
    static void CreateObjValForMesjc();
    static void FreeObjValForMesjc();
    static std::string CreateKey(int plan_id, int objectid, int pokaz_id, int vid_otc, int god, int nMesjc, int ist_fin_id);
    void Addrcrd(int plan_id, int objectid, int pokaz_id, int vid_otc, int god, int nMesjc, int ist_fin_id, int cell_id, double value);
    double Val_Addrcrd_Det(int plan_id, int objectid, int pokaz_id, int vid_otc, int god, int nMesjc, int ist_fin_id, int cell_id);
    double Val_Addrcrd_VtomCisle(int plan_id, int objectid, int pokaz_id, int vid_otc, int god, int nMesjc, int ist_fin_id, int cell_id);
    double Val_Addrcrd_Itogo(int plan_id, int objectid, int pokaz_id, int vid_otc, int god, int nMesjc, int ist_fin_id, int cell_id);
    double Val_forOwner(int plan_id, int objectid, int pokaz_owner_id, int vid_otc, int god, int nMesjc, int ist_fin_id);
    double Getrcrd(int plan_id, int objectid, int pokaz_id, int vid_otc, int god, int nMesjc, int ist_fin_id, int cell_id, int pokaz_is_razd, int pokaz_vert);
    static double GetZaPeriod(TFormLogicF1* FormLogicF1, PCellValueAtr vd);
    static double GetPlGrFinZaPeriod(TFormLogicF1* FormLogicF1, int plan_id, int objectid, int pokaz_id, int vid_otc, int god, int nMesjc, int ist_fin_id, int cell_id, bool isGod);
    static double GetPlGrFinZaKvrtl(TFormLogicF1* FormLogicF1, int plan_id, int objectid, int pokaz_id, int vid_otc, int god, int nMesjc, int ist_fin_id, int cell_id);
    static double GetPlGrFinZaMesjc(TFormLogicF1* FormLogicF1, int plan_id, int objectid, int pokaz_id, int vid_otc, int god, int nMesjc, int ist_fin_id, int cell_id);
};


#endif // TT_VALFORMESJC_H
