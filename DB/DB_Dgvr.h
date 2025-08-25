#ifndef DB_DgvrH
#define DB_DgvrH

#include <string>
#include "DB_Common.h"


class TDB_Dgvr {
public:
    static void SetCodeBK(int object_id, std::string pokaz_bk_code);
    static int GetIdClassDgvr();
    static int GetIdClass(std::string& class_name);
    static int GetIdClassDgvr1();
    static int GetIdFieldMeropr();
    static int GetIdFieldObjemFin();
    static void GetIdFieldUslOplati(int &aUslOplati_id, int &aUslOplati_cellid);
    static void GetIdFieldEAIST(int &aEAIST_id, int &aEAIST_cellid);
    static int GetField_id(TQueryUni *qFld, std::string field_name);
    static void GetIdsFieldsDGVR(int &aN_BO_id, int &aN_BO_cellid, int &aN_dgvr_id, int &aN_dgvr_cellid, int &aPostUslug_id, int &aPostUslug_cellid, int &afield_Finansir_id, int &afield_Finansir_cellidTB_KV, int &aData_zakluc_id, int &aData_zakluc_cellid, int &aSrok_deistv_id, int &aSrok_deistv_cellid, int &asoderg_dgvr_id, int &asoderg_dgvr_cellid, int &aINN_id, int &aINN_cellid);
    static int GetFieldBP(int ObjectId, int &cell_id);
    static void GetDgvrByKBK(const std::string& KBK, int plan_id, int god, TStringList* aSl);
    static int GetKolDgvrByKBK(const std::string& KBK, int plan_id, int god);
    static int GetKolDgvrByKBK_NBO(const std::string& KBK, int plan_id, int god);
    static double GetSumBOByMrpr(int pokaz_id, int plan_id, int god, int vid_otc);
    static double GetSumNumBOByMrpr(int pokaz_id, int plan_id, int god, int vid_otc);
    static bool GetField_id_forNDgvr(int &aN_dgvr_id, int &aN_dgvr_cellid);
    static bool GetField_id_forOFinansir(int &aFinansir_id, int &aFinansir_cellidTB_KV);
    static std::string getNmbrDgvr(int aCurPokazId, int aobjekt_class_id, int aCurYear, int aPlan_id);
    static bool setNmbrDgvr(int aCurPokazId, int aobjekt_class_id, int aCurYear, int aPlan_id, unsigned char aCurrVidOtch, std::string &nmbrDgvr);
    static void saveNmbrDgvr(const std::string& fullNmbrDgvr, int aCurPokazId, int aobjekt_class_id, int aCurYear, int aPlan_id, unsigned char aCurrVidOtch);
    static void GetAllPartForDgvr(TQueryUni *qIn, int ObjectId);
    static int GetKolPartForDgvr(int ObjectId);
    static void UpdNameDgvrKlons(int ObjectId, const std::string nameDgvr);
    static void getInfoByPegim(bool isGbu, int &class_id, int &field_Finansir_id, int &link_pokaz_id);
    static double GetSumNumBOByKBK(int pokaz_id, int plan_id, const TDateTime value_date, int vid_otc, short cell_period, int ist_fin_id);
    static double GetSumNumDOByKBK(int pokaz_id, int plan_id, const TDateTime value_date, int vid_otc, short cell_period, int ist_fin_id);
    static double GetSumObjFinByKBK(int pokaz_id, int plan_id, const TDateTime value_date, int vid_otc, short cell_period, int ist_fin_id);
};

static int cell_id_FinansirMes[20];

#endif
