#ifndef DB_DENEGN_POTOK_H
#define DB_DENEGN_POTOK_H

#include <string>
#include "DB_Common.h"


const std::string CAvtoRascet = "Автом. расчет от договоров ";
const std::string CAvtoRascetMrpr = "Автом. расчет от меропр ";
const std::string CAvtoRascetMrprBdg = "Автом. расчет от договоров БюджМ ";

class TDB_DenegnPotok {
public:
    static int IsItembyCAvto(std::string wwAvtoRascet, int plan_id, int pokaz_id, int cell_id, int vid_otc, TDateTime value_date);
    static int IsItembyCAvtoMrpr(int plan_id, int pokaz_id, int cell_id, int vid_otc, TDateTime value_date);
    static int IsItembyCAvtoDgvr(int plan_id, int pokaz_id, int cell_id, int vid_otc, TDateTime value_date);
    static int GetId();
    static bool IsPlanGrDgvr(int vid_otc, int god, int objectid, double &summa);
    static bool IsPlategkaDgvrGod(int god, int objectid);
    static bool IsPlategkaPokazGod(int god, int pokaz_id);
    static int IsItembyRegNomer(std::string reg_nomer);
    static int IsItembyRegNomerKBK(std::string reg_nomer, std::string KBKfromXML, double aValue);
    static int IsItembyRegNomerTBandOther(std::string reg_nomer, TDateTime value_date, double value, std::string nom_plategki);
    static int IsPPbyInfo(int plan_id, int vid_otc, int objectid, std::string nom_plategki, TDateTime date_plategki, TDateTime value_date, double value);
    static int IsItembyPlategkaPropertyGBU(int plan_id, std::string nom_plategki, TDateTime date_plategki, TDateTime value_date, double value);
    static int IsItembyPlategkaProperty(int plan_id, std::string nom_plategki, TDateTime date_plategki, std::string BnkRS_PlatPoluc, std::string nBO, TDateTime operDay, double value);
    static void Upddate_BnkRS(int den_potok_id, TDateTime date_plategki, std::string BnkRS_PlatPoluc, std::string nBO);
    static int WriteItem(std::string reg_nomer, int objectId, int plan_id, int pokaz_id, int cell_id, int vid_otc, TDateTime value_date, double value, int coef, std::string nom_documenta, std::string attrib1, std::string attrib2, std::string attrib3, std::string fromXML, std::string nom_plategki, TDateTime date_plategki, std::string NBOneprisoed, std::string BnkRS_PlatPoluc);
    static int WriteItemDO(int objectId, int plan_id, int pokaz_id, int cell_id, int vid_otc, TDateTime value_date, double value, int coef, std::string RCV_INN, std::string RCV_CPP, std::string RCV_NAME, std::string KBK_000_KOSGU, std::string PURPOSE, std::string nom_plategki, TDateTime date_plategki, std::string nomerDO, TDateTime file_date, std::string reg_nomer, std::string nom_documenta);
    static int WriteItem2(std::string reg_nomer, int objectId, int plan_id, int pokaz_id, int cell_id, int vid_otc, TDateTime value_date, double value, int coef, std::string nom_documenta, std::string attrib1, std::string attrib2, std::string attrib3, std::string fromXML, std::string nom_plategki, TDateTime date_plategki, int budgetpokaz_id, std::string NBOneprisoed, std::string BnkRS_PlatPoluc);
    static int WriteItemBPid(std::string reg_nomer, int objectId, int plan_id, int pokaz_id, int cell_id, int vid_otc, TDateTime value_date, double value, int coef, std::string nom_documenta, std::string attrib1, std::string attrib2, std::string attrib3, std::string fromXML, std::string nom_plategki, TDateTime date_plategki, int budget_poluc_id, std::string NBOneprisoed, std::string BnkRS_PlatPoluc);
    static int WriteItemZajvka(std::string grnlGrFin_idS, std::string nPoPorjdkuS, int objectId, std::string nDO, int field_id, int cell_id, int plan_id, double value, TDateTime value_date, std::string nom_plategki, TDateTime date_plategki, std::string naznacPlatega, std::string KBK, std::string kontrAgent);
    static bool UpdateItem(int den_potok_id, int objectId, double value, std::string nom_documenta, std::string attrib1, std::string attrib3);
    static bool UpdateItem2(int den_potok_id, int objectId, double value, std::string nom_documenta, std::string attrib1, std::string attrib3, int budgetpokaz_id);
    static void UpdValue(int den_potok_id, double value);
    static void UpdIdFromPref(int den_potok_id, int id_FromPref);
    static void UpdBudgetpokaz_id(int den_potok_id, int budgetpokaz_id);
    static void UpdFromXML(int den_potok_id, std::string fromXML);
    static void UpdNPlategki(int den_potok_id, std::string nom_plategki);
    static void UpdDatePlategki(int den_potok_id, TDateTime date_plategki);
    static void UpdNBOneprisoedObj(int den_potok_id, std::string NBOneprisoed, int objectid);
    static void UpdNBOneprisoedToNULL(int den_potok_id, int objectid, int field_Finansir_id, int cell_id_FinansirMes);
    static void UpdKFO_CelSubs(int den_potok_id, std::string CODE_KFO, std::string codeCelSubs);
    static bool DeleteItem(int den_potok_id);
    static void DeleteItemByObj(int objectid);
    static void DeleteItemByCellId(int cell_id);
    static void DeleteItemByPlanCellId(int plan_id, int cell_id);
    static void DeleteDocums(TQueryUni *q);
    static bool GetItem(TQueryUni *qIn, int den_potok_id);
    static void getDataForDenPotokId(int den_potok_id, int &plan_id, int &objectid, int &pokaz_id, int &cell_id, int &budgetpokaz_id, int &budget_poluc_id, double &summa, TDateTime &value_date, std::string &attrib3, std::string &file_name, TDateTime &file_date, std::string &fromXML, std::string &reg_nomer, std::string &nom_plategki, TDateTime &date_plategki, std::string &NBOneprisoed, std::string &BnkRS_PlatPoluc, std::string &nom_documenta);
    static std::string GetAttrib3(int den_potok_id, std::string &fromXML, std::string &NBOneprisoed, std::string &BnkRS_PlatPoluc);
    static std::string GetNPlategki(int den_potok_id);
    static TDateTime GetDPlategki(int den_potok_id);
    static int GetPayerId(int den_potok_id);
    static int GetBudgetpokaz_id(int den_potok_id);
    static TDateTime GetValue_date(int den_potok_id);
    static double GetValue(int den_potok_id);
    static std::string GetNom_documenta(int den_potok_id);
    static std::string GetFromXML(int den_potok_id);
    static void GetPokazCell_id(int den_potok_id, int &pokaz_id, int &cell_id);
    static void SumByDogsForMeropr_Write(int plan_id, int pokaz_field_id, int pokaz_meropr_id, int link_pokaz_id, int vid_otc, TDateTime value_date, int cell_id_Mes_Meropr, int budgetpokaz_id);
    static double GetSumKVforMeropr(int plan_id, int pokaz_meropr_id, int cell_id_Mes_Meropr, TDateTime Date1, TDateTime Date2);
    static double GetSumZaPeriodforP_M_D(int plan_id, int pokaz_id, int cell_id, int ObjectId, TDateTime Date1, TDateTime Date2);
    static double GetValueItogoMesjc(int aPokaz_id, int aPlan_id, TDateTime aFromDate, TDateTime aToDate, int aVidOtch, int aCell_id);
    static double GetSummaFactNI(int plan_id, int pokaz_id, int ist_fin_id, int fmForm_CurrYear);
    static void SumbyMeroprsForBudgPokaz_Write(int plan_id, int budgetpokaz_id, int cell_id_Mes_Budgpokaz, int vid_otc, TDateTime value_date);
    static void SumByDogsForBudgPokaz_Write(int plan_id, int field_Finansir_cellid_Mes, int budgetpokaz_id, int cell_id_Mes_Budgpokaz, int vid_otc, TDateTime value_date, std::string KBK);
    static void SumByDogsNoMRPRforBudgPokaz_Write(int plan_id, int field_Finansir_cellid_Mes, int budgetpokaz_id, int cell_id_Mes_Budgpokaz, int vid_otc, TDateTime value_date, std::string KBK);
    static bool isFromAisUBP(std::string reg_nomer);
    static void GetItemsPokazDate(TQueryUni *qIn, int plan_id, int pokaz_id, TDateTime s, TDateTime po);
    static void GetFilenameFiledateFromAttrib3(std::string attrib3, std::string &file_name, TDateTime &file_date);
    static bool doEvalforMrpr_forBpokaz(bool isBudgetpokaz_id, int budgetpokaz_id, int plan_id, int pokaz_meropr_id, int cell_id_Mes_Meropr, TDateTime value_date, int ist_fin_grad_id);
    static bool doEvalforDgvr_forMrpr_forBpokaz(bool isBudgetpokaz_id, std::string KBK, int plan_id, int objectid, TDateTime value_date, int ist_fin_grad_id, int &outBudgetpokaz_id);
    static double GetValueMonth(int objectid, int cell_id, int plan_id, int vid_otc, int aYear, int aMonth);
    static int fromNeOdnoznPlategkaToDenegn_potokKBK(int neOdnozn_id, int plan_id, int budgetpokaz_id, int cell_id_Mes_budgetpokaz);
    static int fromNeOdnoznPlategkaToDenegn_potokObject(int objectId, int neOdnozn_id, int plan_id, int field_Finansir_id, int cell_id_FinansirMes);
    static int fromDenegn_potokToNeOdnoznPlategka(int den_potok_id, std::string rasc_scet);
};

#endif // DB_DENEGN_POTOK_H
