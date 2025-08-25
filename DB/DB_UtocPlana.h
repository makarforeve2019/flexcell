#ifndef DB_UTOCPLANA_H
#define DB_UTOCPLANA_H

#include "DB_Common.h"
#include "unDBType.h"
#include "../Form/FormTypes.h"

struct TUtoc_plana {
    int plan_id;
    int god;
    TDateTime utoc_date;
    std::string n_protocol;  
    int prdlg2_id;
    unsigned char kv_otkuda;
    int pokaz_id_otkuda;
    int cell_id_otkuda;
    unsigned char kv_kuda;
    int pokaz_id_kuda;
    int cell_id_kuda;
    int budget_poluc_id_otkuda;
    int budget_poluc_id_kuda;
    double summa_kuda;
    double summa_otkuda;
};

typedef TUtoc_plana* pUtoc_plana;

class TDB_UtocPlana {
public:
    static void GetDetStr(TQueryUni *qIn, int utoc_plana_id);
    static void SetOwnerId(int utoc_plana_id, int owner_id);
    static double GetSumUtocPosleKvartala(PCellValueAtr vd);
    static double GetSumUtocPosleKvartalaRAZD(PCellValueAtr vd);
    static void UpdPrdlg2_id(int utoc_plana_id, int prdlg2_id);
    static bool IsUrovenGU_IS_Raj(int prdlg_id, int &parent_id);
    static int IsUrovenGU_IS_Raj2(int prdlg_id, int plan_idRaj);
    static int AddToGurnal(PCellValueAtr aVd, pUtoc_plana up, int owner_id);
    static bool doUpdDBbyValue1(PCellValueAtr vd, double val);
    static std::string getNDocSmeta(int pokaz000, int god, int planId);
    static int AddToGurnalSubSmeta(PCellValueAtr aVd, double delta, double valuePokaztl);
    static void SetDocumInfo(int utoc_plana_id, std::string DocumNum, TDateTime DocumDate);
    static int getDocumInfo(std::string DocumNum, TDateTime DocumDate);
    static void updPrimec(int utoc_plana_id, std::string substr);
};

#endif // DB_UTOCPLANA_H
