
#ifndef DB_CELLVALUE_H
#define DB_CELLVALUE_H

#
#include <string> // Include for std::string
#include "DB_Common.h"
#include "../Form/FormTypes.h"


class TDB_CellValue {
public:
    static bool IsValue(int objectid, int cell_id, int plan_id, int vid_otc, TDateTime value_date);
    static bool IsValue(int PokazId, int ComnpId, int VidPeriod, int God, int Kvrtl, int Mesjc, int Den, int PlanId, int VidOtch);
    static void GetDataForObject(TQueryUni *qIn, int objectid, int plan_id, std::string spCellId);
    static double GetValue(int objectid, int cell_id, int plan_id, int vid_otc, TDateTime value_date);
    static double GetValuePeriod(int objectid, int cell_id, int plan_id, int vid_otc, int aYear, TDateTime value_date);
    static std::string GetStrValue(int objectid, int cell_id, int plan_id, int vid_otc, TDateTime value_date);
    static std::string GetNI_byStr(TDateTime PokazDate, int objectid, int CurrCellId, int CurrPlanId, int CurrVidOtch, int CurrYear);
    static void SetValue(double value, int objectid, int cell_id, int plan_id, int vid_otc, TDateTime value_date);
    static void SetValueNote(double value, std::string Note, int objectid, int cell_id, int plan_id, int vid_otc, TDateTime value_date);
    static void SetValue2(double value, int objectid, int cell_id, int plan_id, int vid_otc, TDateTime value_date, int coef);
    static bool SetStrValue(PCellValueAtr vd, std::string str_value, int objectid, int cell_id, int plan_id, int vid_otc, TDateTime value_date);
    static void SetStrValue2(std::string str_value, int objectid, int cell_id, int plan_id, int vid_otc, TDateTime value_date);
    static void SetStrValue2note(std::string str_value, std::string Note, int objectid, int cell_id, int plan_id, int vid_otc, TDateTime value_date);
    static void SetDcum_id(std::string docum_id, int objectid, int cell_id, int plan_id, int vid_otc, TDateTime value_date);
    static std::string GetDcum_id(int objectid, int cell_id, int plan_id, int vid_otc, TDateTime value_date);
    static std::string GetValueUslPost(PCellValueAtr vd, int &Id_Znac, TDateTime &ValueDate);
    static std::string GetValueUslPostBP(int objectid, int pokaz_id, int PlanId, int VidOtch, TDateTime ValueDate, int &Id_Znac);
    static double GetNumValueUslPost(int objectid, int cell_id, int plan_id, int vid_otc, TDateTime value_date);
    static std::string GetStrValueUslPost(int objectid, int cell_id, int plan_id, int vid_otc, TDateTime value_date);
    static bool SetValueUslPost(std::string EditString, PCellValueAtr vd);
    static bool IsSumma(int &vid_otc, int god, int pokaz_id, double &summa);
    static double GetSummaUtPlNI(int plan_id, int pokaz_id, int ist_fin_id, int fmForm_CurrYear);
    static void DelOnlyValue(int objectid, int cell_id, int plan_id, int vid_otc, TDateTime value_date);
    static void DelForClear(int pokaz_id, int vid_otc, int god);
    static void DeleteDocums(TQueryUni *q);
    static void SetNote(std::string Note, int objectid, int cell_id, int plan_id, int vid_otc, TDateTime value_date);
    static std::string GetNote(int objectid, int cell_id, int plan_id, int vid_otc, TDateTime value_date);
};

#endif // DB_CELLVALUE_H
