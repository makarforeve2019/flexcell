#ifndef TT_PostuplForPeriodH
#define TT_PostuplForPeriodH

#include "../DB/DB_Common.h"
#include "../DB/unDBType.h"
#include <string>

class TPostuplForPeriod
{
public:
    TStringList* sl;
    static void CreateObjValForPeriod();
    static void FreeObjValForPeriod();
    static std::string CreateKey(int plan_id, int pokaz_id, int vid_otc, int god, int nMesjc, int day, int ist_fin_id);
    void Addrcrd(int plan_id, int pokaz_id, int vid_otc, int god, int nMesjc, int day, int ist_fin_id,
                 int cell_id, double value);
    double Val_Addrcrd_Det5(int plan_id, int pokaz_id, int vid_otc, TDateTime BegDate, int god, int nMesjc, int day,
                             int ist_fin_id, int cell_id);
    double Val_Addrcrd_VtomCisle(int plan_id, int pokaz_id, int vid_otc, TDateTime BegDate, int god, int nMesjc, int day,
                                  int ist_fin_id, int cell_id);
    double Val_Addrcrd_Itogo(int plan_id, int pokaz_id, int vid_otc, TDateTime BegDate, int god, int nMesjc, int day,
                              int ist_fin_id, int cell_id);
    double Val_forOwner(int plan_id, int pokaz_owner_id, int vid_otc, TDateTime BegDate, int god, int nMesjc, int day,
                        int ist_fin_id);
    double Getrcrd(int plan_id, int pokaz_id, int vid_otc, TDateTime BegDate, int god, int nMesjc, int day,
                   int ist_fin_id, int cell_id, int pokaz_is_razd, int pokaz_vert);    
};

//struct TrcrdFM
//{
//    int plan_id;
//    int objectid;
//    int pokaz_id;
//    int vid_otc;
//    int god;
//    int nMesjc;
//    int day;
//    int ist_fin_id; // ist_fin_id_CBRTOkruga, ist_fin_id_TrBudg are defined in DB_IstFin,
//                    // ist_fin_id_CelSubsid are defined in DB_IstFin
//    int cell_id;
//    double valueUtCB;
//    double valueUtTB;
//    double valueUtCS; // ist_fin_id_CelSubsid
//    double valueFcCB;
//    double valueFcTB;
//    double valueFcCS; // ist_fin_id_CelSubsid
//};
//typedef TrcrdFM* PrcrdFM;

#endif
