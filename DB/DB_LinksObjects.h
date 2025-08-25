#ifndef DB_LINKSOBJECTS_H
#define DB_LINKSOBJECTS_H


#include "DB_Common.h"
#include "../Form/FormTypes.h"


class TDB_LinksObjects {
public:
    static int GetId();
    static int AddLinksObjects(int plan_id, int objectid, int pokaz_id, int link_pokaz_id, TDateTime date_create);
    static int AddMultiLinksObjects(int plan_id, int objectid, int pokaz_id, int link_pokaz_id, TDateTime date_create);
    static int GetLinksObjects(int plan_id, int objectid, int pokaz_id, int link_pokaz_id);
    static int GetIdObjectPokaz(int plan_id, int objectid, int link_pokaz_id);
    static int GetdPokazByObject(int plan_id, int objectid, int link_pokaz_id, std::string &aPokazName);
    static void GetObjectsByPokaz(int plan_id, int pokaz_id, int link_pokaz_id, TStringList *aSl);
    static bool isObjectsByPokaz(int plan_id, int pokaz_id, int link_pokaz_id);
    static void UpdPokazId(int plan_id, int pokaz_id, int NewPokaz_id);
    static void DelLinksObjects(int link_id);
    static void DelMultiLinksObjects(int plan_id, int objectid, int pokaz_id, int link_pokaz_id);
    static void doAgregirUpByField(TwwStringList p, PCellValueAtr vd);
    static double doAgregirSumForSubObjects(int plan_id, int meropr_id, int link_pokaz_id,
                                             int cell_idClassField, int vid_otc, TDateTime dateKV);
    static double doAgregirSumForSubObjects2(int plan_id, int meropr_id, int link_pokaz_id,
                                              int cell_idClassField, int vid_otc, TDateTime dateKV);
    static double doAgregirSumForSubObjects3(int plan_id, int meropr_id, int link_pokaz_id,
                                              int cell_idLimitZakup, int vid_otc, int god);
};

#endif // DB_LINKSOBJECTS_H
