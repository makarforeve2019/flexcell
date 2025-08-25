
#ifndef DB_BPolucH
#define DB_BPolucH

#include "DB_Common.h"

class TDB_BPoluc {
public:
    static int GetId();
    static int AddBP(std::string short_name, std::string name_, std::string rasc_scet,
                     std::string bank_rekviz, std::string rasc_scet2, int plan_id2, std::string INN,
                     std::string JurAddres, int god_beg, int god_end, std::string CodeInSvReestr);
    static void UpdBP(int budget_poluc_id, std::string short_name, std::string name_, std::string rasc_scet,
                      std::string bank_rekviz, std::string rasc_scet2, int plan_id2, std::string INN,
                      std::string JurAddres, int god_beg, int god_end, std::string CodeInSvReestr,
                      std::string KPP);
    static void GetAllBPs(TQueryUni *qIn);
    static void GetAllBPsRaj(TQueryUni *qIn, int plan_id);
    static bool GetBP(TQueryUni *qIn, int budget_poluc_id);
    static std::string GetNameBP(int budget_poluc_id);
    static std::string GetRascScetBP(int budget_poluc_id);
    static std::string GetRascScet1(int PlanId);
    static std::string GetRascScet1Name(int PlanId, std::string &NameBP);
    static std::string GetRascScet2(int plan_id);
    static std::string GetRascScet2Name(int plan_id, std::string &NameBP, std::string &INN);
    static std::string GetINNBP(int budget_poluc_id);
    static std::string GetJurAddresBP(int budget_poluc_id);
    static int GetId_NPVid();
    static int GetIdBP(int pokaz_id, int np_vid_id, int Year, int plan_id);
    static void GetMrprbyBP(int pokaz_id, TQueryUni *qIn, int budget_poluc_id, int Year, int plan_id);
    static bool IsRasporjd(std::string rasc_scet);
    static bool IsPolucat(std::string rasc_scet);
    static int GetBPatr_byLicScet(std::string rasc_scet, std::string &nameBPL, std::string &INN, int &plan_id);
};

static int id_npVidBP = 0;

#endif
