
#ifndef UPDDBBYVALUE_H
#define UPDDBBYVALUE_H
#include "../system/ClassesUtils.h"
#include "../DB/DB_Common.h"
#include "json.hpp"

class TUpdDBbyValue {
public:
    static bool UpdDBbyValue1(int aCell_id, int aPlan_id, unsigned char aVid_otc, TDateTime aDate, double aValue, int objectid, bool aIsMsg);
    static bool UpdDBbyValue1(int aCell_id, int aPlan_id, unsigned char aVid_otc, TDateTime aDate, double aValue, int objectid, bool aIsMsg, nlohmann::json& JSONresult);
    static bool UpdDBbyValue2(int aCell_id, int aPlan_id, unsigned char aVid_otc, TDateTime aDate, double aValue, int objectid, bool aIsMsg);
    static bool UpdDBbyValue3(int aCell_id, int aPlan_id, unsigned char aVid_otc, TDateTime aDate, double aValue, int objectid, bool aIzNix, bool aIsMsg);
    static bool UpdDBbyValueX(int aCell_id, int aPlan_id, unsigned char aVid_otc, TDateTime aDate, double aValue, bool bAddRec, bool aIzNix, bool aIsMsg, int aObjectid);

    static void SetOwnerObjectItogo(int aObjectid, TQueryUni* vCell, int aPlan_id, int aVid_otc, int aGod, int aKv, int aMes, double aDelta, double aValue, bool bAddRec, std::string idsOwnerFieldItogo, bool aIsMsg);

    static int getOwnerFieldId(int OwnerClass_id, std::string idsOwnerFieldItogo);

    static double Set3DValue(TQueryUni* vCell, TQueryUni* vPokaztl, int aPlan_id, int aVid_otc, int aGod, int aKv,
                             int aMes, double aValue, bool bDelta, bool bAddRec, bool IsKvFormula, bool IsGodFormula, int aObjectid, bool aIsMsg);

    static void ExecFormula(int aIst_fin_id, int aCell_period, TQueryUni* vPokaztl, int aCell_id, int aPlan_id, int aVid_otc, TDateTime aDate, double aValue, int aObjectid, bool aIsMsg);
    static bool CheckIzNixBase(bool aIsValue, int aPokaz_id, std::string aPokaz_name, int aPlan_id, unsigned char aVid_otc, TDateTime aValue_date, double aValue, int aCell_period, int aIst_fin_id, std::string aCell_din_izm, int objectid);
    static bool CheckVklucaetBase(bool aIsValue, int aPokaz_id, std::string aPokaz_name, int aPlan_id, unsigned char aVid_otc, TDateTime aValue_date, double aDelta, double aValue, int aCell_period, int aIst_fin_id, std::string aCell_din_izm, int objectid, bool aIsMsg);
    static bool CheckIzNixNatur(bool aIsValue, std::string aPokaz_name, int aNp_vid_id, int aPokaz_base_id, int aPlan_id, unsigned char aVid_otc, TDateTime aValue_date, double aValue, int aCell_period, int aIst_fin_id, std::string aCell_din_izm);
    static double SetValue(int aIst_fin_id, int aCell_period, TQueryUni* vPokaztl, int aCell_id, int aPlan_id, int aVid_otc, TDateTime aDate, double aValue, bool bDelta, bool bAddRec, int aObjectid, bool aIsMsg);
    static double SetYearValue(int aIst_fin_id, int aCell_period, TQueryUni* vPokaztl, 
                               int aCell_id, int aPlan_id, int aVid_otc, int aGod, int aKv,
                               int aMes, double aValue, bool bDelta, bool bAddRec, bool IsKvFormula,
                               bool IsGodFormula, int aObjectid, bool aIsMsg);

    static bool SetOwnerBase(TQueryUni* vCell, TQueryUni* vPokaztl, int aPlan_id, int aVid_otc, int aGod, int aKv, int aMes, double aDelta, double aValue, bool bAddRec, bool aIzNix, bool aIsMsg);
    static void SetOwnerBaseMulti(bool aB, TQueryUni* vCell, int aItogo_idMulti, int aPlan_id, int aVid_otc, int aGod, int aKv, int aMes, double aDelta, bool bAddRec, bool aIsMsg);

    static void SetOwnerObjectAgregat(int aObjectid, TQueryUni* vCell, int aPlan_id, int aVid_otc, int aGod, int aKv, int aMes, double aDelta, double aValue, bool bAddRec, bool aIsMsg);
    static void SetOwnerNatur(TQueryUni* vCell, TQueryUni* vPokaztl, int aPlan_id, int aVid_otc, int aGod, int aKv, int aMes, double aDelta, bool bAddRec, bool aIzNix, bool aIsMsg);
};

#endif // UPDDBBYVALUE_H

