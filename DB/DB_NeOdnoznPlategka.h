#ifndef DB_NeOdnoznPlategkaH
#define DB_NeOdnoznPlategkaH

#include <string>
#include "DB_Common.h"

const int KO_NeprImjBP = 1;
const int KO_OtsSpr = 2;
const int KO_NePravStr = 3;
const int KO_NePravStrDate = 4;
const int KO_NePoSpravke = 5;
const int KO_MnogoSpravok = 6;
const int KO_inDbNoRascScet = 7;
const int KO_BPotherRascScet = 8;
const int KO_RascScetNeUnik = 9;
const int KO_OtsBO = 10;
const int KO_NoKey = 11;
const int KO_NoPlan = 12;
const int KO_NoPlGraf = 13;
const int KO_inDbBP = 14;
const int KO_NoMrpr = 15;
const int KO_NoPlGrafMrpr = 16;
const int KO_NoCellid = 17;
const int KO_NoPokazInPlan = 18;
const int KO_NoSum = 19;
const int KO_NoSredstv = 20;

class TDB_NeOdnoznPlategka {
public:
    static int IsPlategka(std::string KBKfromXML, std::string nom_plategki, TDateTime value_date,
      double ValueSumma, std::string rasc_scet);
    static void DelPlategka(int neOdnozn_id);
    static void UpdPlategka(int neOdnozn_id, int budget_poluc_id, int kod_oshibki);
    static void UpdPayerID(int neOdnozn_id, int payer_id);
    static int SaveInNeOdnozn(std::string NLimSprav, std::string kodAU,
      double ValueSumma, std::string fromXML, TDateTime value_date, std::string file_name,
      TDateTime file_date, std::string reg_nomer, std::string short_name, int budget_poluc_id,
      int kod_oshibki, std::string rasc_scet, std::string nomer_BO, std::string nom_plategki,
      TDateTime date_plategki, std::string pokaz_bk_code, std::string NBOneprisoed,
      std::string BnkRS_PlatPoluc, std::string nom_documenta);
    static int GetId();
    static TDateTime getValue_date(int neOdnozn_id, double &ValueSumma);
    static void UpdNote(int neOdnozn_id, std::string Note);
    static void UpdErrorMsg(int neOdnozn_id, std::string aMsg);
    static void UpdKOSGU(int neOdnozn_id, std::string CODE_KVR, std::string KOSGU);

};

#endif
