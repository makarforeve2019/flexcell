//
// Created by Alex on 21.04.2024.
//

#ifndef MWSRESTAPI_FORMDOPUPDLOGIC_H
#define MWSRESTAPI_FORMDOPUPDLOGIC_H



// C++ translation

#include "unFormLogicF1.h"
#include "FormTypes.h"
//#include <Math.h>
#include "DB_Common.h"
#include "DB_Enum.h"
#include "DB_GurnProtokol.h"
#include "evalComonNPokaz_Logic.h"
#include "unDocumentValueLogic.h"
#include "json.hpp"

using json = nlohmann::json;

class TFormDopUpdLogic {
public:

    static bool doUpdExecuteCells(int nSheet, TFormLogicF1* FormLogicF1, int aNRow, int aNCol, bool noSetActiveCell, nlohmann::json& JSONresult);
//    static bool doForPrisvoitUpd(TFormLogicF1* FormLogicF1);
    static bool doForPrisvoitUpd(TFormLogicF1* FormLogicF1, nlohmann::json& JSONresult);
//    static bool doPrisvoitOnly(TFormLogicF1* FormLogicF1, TwwStringList* p);
    static bool doPrisvoitOnly(TFormLogicF1* FormLogicF1, TwwStringList* p, nlohmann::json& JSONresult);
    static void doEvalCells(TFormLogicF1* FormLogicF1, TwwStringList* p);
    static bool doStmntAssignByVvodPokaztl(TFormLogicF1* FormLogicF1, TwwStringList* p, int nRow, int nCol, int nColVvod);
    static void doRefreshComputedCells(bool isComNP, TFormLogicF1* FormLogicF1, TwwStringList* p, PCellValueAtr vd, double d0edIzm);
    static void doAgregirUpByField(TwwStringList* p, PCellValueAtr vd);
    static bool CellValueAtr(TFormLogicF1* FormLogicF1, int nSheet, int nLocRow, int nLocCol, json& JSONresult);
    static bool CellEndingEdit(TFormLogicF1* FormLogicF1, int nSheet, int nRow, int nCol, std::string editValue, int valueID, nlohmann::json& JSONresult);
    static void AddDefMrprBack(int nSheet, TFormLogicF1* FormLogicF1,
                                          int plan_id, bool isItogo, int pokaz_is_razd, int pokaz_owner_id,
                                          int in_cast, const std::string& pokaz_name, const std::string& Primecanie,
                                          int is_list, int level_sign, const std::string& level_text, int pokaz_vert,
                                          const std::string& vert_str, int pokaz_type, const std::string& pokaz_code,
                                          const std::string& pokaz_bk_code, const TDateTime& pokaz_beg_date,
                                          const TDateTime& pokaz_end_date, int enum_id, int edizm_id,
                                          int pokaz_is_const, int pokaz_is_god, int pokaz_is_kv, int pokaz_is_mes,
                                          int pokaz_is_ist_fin, int pokaz_is_din_izm, int& pokaz_id);
    static int findNRowIns(TFormLogicF1* FormLogicF1, int nRowSel, bool isSub);
    static void insertRowEndDataToForm(TFormLogicF1* FormLogicF1, int nSheet,
                                                  TwwStringList* p, int nRowIns, int aColSelect,
                                                  int pokaz_id, int cast_id);
    static void insertEmptyNewRow(TFormLogicF1* FormLogicF1, int nSheet, int insertRow,
                                             TwwStringList* p, int OwnerNomRow, int Ownerid, int pokaz_id,
                                             int level_sign, int L, int CurRowVid);
    static void insertEmptyRow(TFormLogicF1* FormLogicF1, int insertRow, TwwStringList* p,
                                          int OwnerNomRow, int OwnerId, int pokaz_id, int level_sign,
                                          int L, int CurRowVid);


    static bool ClickButton(TFormLogicF1* FormLogicF1, int nSheet, int nRow, nlohmann::json& JSONresult);
    static void RecalcFormulas(TFormLogicF1* FormLogicF1, nlohmann::json& JSONresult);

    static bool CellsImport(TFormLogicF1* formLogic, const nlohmann::json& jsonObj, nlohmann::json& resultJson);

};





#endif //MWSRESTAPI_FORMDOPUPDLOGIC_H
