#ifndef EkonomijBPSevalH
#define EkonomijBPSevalH

#include <vector>
#include <string>
#include "DB_Common.h"
#include "unDBType.h"
#include "unEkonomijBPSColDef.h"
#include "ClassesUtils.h"


struct TEkonBPS {
    int plan_id;
    int pokaz_id;
    double Ekonomij;
    double cenaKontrakts;
};
typedef struct TEkonBPS* PEkonBPS;


struct TKntrkt {
    int pokaz_id;
    int plan_id;
    std::string nBO;
    std::string KBK;
    double dZakluc;
    double dBO;
    double dKassRasxBO;
};
typedef struct TKntrkt* PKntrkt;

struct TKntrkt15 {
    double dBO15;
    double dKassRasxBO15;
};

typedef TKntrkt15* PKntrkt15;


struct TTorgi {
    std::string KBK;
    int plan_id;
    double SummaLota;
};
typedef struct TTorgi* PTorgi;

struct TDgvr15 {
    std::string KBK;
    double dDgvr15;
};

typedef struct TDgvr15 *PDgvr15;

//int LUnc = 0;

class TEkonomijBPSeval {
public:
    int id_obmena;
    TDateTime lastdate_15fcu;
    TDateTime po_naDatu;
    bool isLoad;
    TStringList* valList;
    int reestrDgvrov_id;
    int class_dgvr_id;
    int N_BO_id, N_BO_cellid, N_dgvr_id, N_dgvr_cellid;
    int PostUslug_id, PostUslug_cellid, field_Finansir_id, field_Finansir_cellidTB_KV;
    int Data_zakluc_id, Data_zakluc_cellid;
    int Srok_deistv_id, Srok_deistv_cellid;
    int soderg_dgvr_id, soderg_dgvr_cellid;
    int INN_id, INN_cellid;

    static TStringList* kontrList;
    static TStringList* kbk15List;
    static TStringList* dgvrList;
    static TStringList* torgiList;

    static TEkonomijBPSeval* createObj();
    static void freeObj(TEkonomijBPSeval* obj);

    static double doEval(int plan_id, int pokaz_id, int VidOtc, int god, int Nvalue);
    static double doEvalEconom(int plan_id, int pokaz_id, int VidOtc, int god, int Nvalue);

    void LoadToValList(int god);

    static void SelectKONTRAKTs(std::string plan_idSS, TStringList& kontrList, int plan_id, int pokaz_owner_id, int god);
    static void SelectKONTRAKTsDalee(std::string plan_idSS, std::shared_ptr<TList<PKntrkt>> wList, int plan_id, int pokaz_owner_id, int god);

    static void evalKONTRAKTsByDB(TList<PKntrkt> wList, int god, std::string KBK, double& dZakluc, double& dZaklucNoBO);
    static void doKONTRAKTby15(TList<PKntrkt> wList, int god, std::string KBK, double& dBO, double& dKassRasxBO);

    static void evalBO15_KassRasxBO15(double& dBO15, double& dKassRasxBO15, int god, std::string KBK, int plan_id, TDateTime lastdate_15fcu);
    static void evalBO15_KassRasxBO15all(double& dBO15all, double& dKassRasxBO15all, int god, std::string pokaz_bk_code, TDateTime lastdate_15fcu);

    static double evalDgvr15(TList<PDgvr15>& wList, const std::string KBK);
    static void evalDgvr15allValues(std::string plan_idSS, int plan_id, std::shared_ptr<TList<PDgvr15>> wList, int god, TDateTime lastdate_15fcu);

    static void evalTorgiLoadValues(bool isAllTorgi, int plan_id, std::shared_ptr<TList<TTorgi*>> wList, int god);
    static double evalTorgi(std::shared_ptr<TList<PTorgi>> wList, std::string KBK);
};
#endif