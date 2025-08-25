#ifndef DB_CODECELSUBS_H
#define DB_CODECELSUBS_H

#include <string>
#include "DB_Common.h"

#include "FormTypes.h"

class TDB_CodeCelSubs {
public:
    static std::string getCodeCelSubsByIstFinId(int ist_fin_id);
    static bool findSubsidPokazCellId(std::string codeCelSubs, int& SubsidPokazId, int& SubsidCellIdKV, int& SubsidCellIdGod);
    static bool findSummaDgvrSubsidCellId(int class_idZakupki, int SummaDgvrPokazId, int ist_fin_id, std::string codeCelSubs, int SubsidPokazId, int& SummaDgvrCellIdKV, int& SummaDgvrCellIdGod);
    static int GetIstFinIdBySubs(int pokaz_idSubsid);
    static void UpdDgvrSumZakontrAll(int plan_id, int god, int class_idZakupki, int objectid, double aDelta);
    static void UpdDgvrVsSubsZakontrAll(int plan_id, int god, int class_idZakupki, int objectid, double aDelta);
    static void UpdDgvrVsOstZakontrAll(int plan_id, int god, int class_idZakupki, int objectid, double aDelta);
    static double GetSumKVforDogovor(int plan_id, int objectid, TDateTime Date1, TDateTime Date2);
    static double GetSumKVforDogovorSubsid(int plan_id, int objectid, TDateTime Date1, TDateTime Date2, std::string codeCelSubs);
    static double GetSumKVforDogovorVsegoSubsid(int godOperDay, int plan_id, int objectid, TDateTime Date1, TDateTime Date2);
    static double GetSumKVforDogovorVsegoOstatk(int godOperDay, int plan_id, int objectid, TDateTime Date1, TDateTime Date2);
    static double GetSumKVforMeropr(int plan_id, int meropr_id, TDateTime Date1, TDateTime Date2);
    static double GetSumKVforMeroprSubsid(int plan_id, int meropr_id, TDateTime Date1, TDateTime Date2, std::string codeCelSubs);
    static double GetSumKVforMeroprVsegoSubsid(int godOperDay, int plan_id, int meropr_id, TDateTime Date1, TDateTime Date2);
    static double GetSumKVforMeroprVsegoOstatk(int godOperDay, int plan_id, int meropr_id, TDateTime Date1, TDateTime Date2);
    static void setNewValuePokazSubsid(int SubsidCellIdKv, int plan_id, TDateTime Date2, double summaPP, int vid_otc);
    static int evalGodFinansir(std::string codecelsubs);
    static double GetSumKVforMmeroprOldYear(int plan_id, int god, int class_idZakupki, int meropr_idOldYear, int aCell_id, int aVid_otc, TDateTime aValue_date);
    static void evalSubsidOnUpdMmeropr(std::string& EditString, double oldValueMrpr, PCellValueAtr vd, int& plan_idHandle, int& cell_idHandle, int& objectidHandle, int& vid_otcHandle, TDateTime& value_dateHandle);
};

#endif // DB_CODECELSUBS_H
