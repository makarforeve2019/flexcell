#ifndef ACTION_VIEW_H
#define ACTION_VIEW_H

#include <../nlohmann/json.hpp>
#include "../system/ClassesUtils.h"
#include "TF1FlexCel.h"

using json = nlohmann::json;

using namespace F1DevExpress::unF1FlexCel;

class TActionView
{
private:
    json FActionLog = json::object();
    json FActions = json::array();

public:
    bool isBookEmpty;
    bool isSaveAllAction;
    TStringList slFlexCelFormulas;
    F1DevExpress::unF1FlexCel::TF1FlexCel flexCel;

    TActionView(){
        FActionLog.emplace("Actions", FActions);
//        slFlexCelFormulas.sorted = true;
    };
    virtual ~TActionView(){
        slFlexCelFormulas.Clear();
    };

    void doBeginSost(bool isBookEmpty_);
    void doSubBeginSost();
    void setSaveAllAction();
    void doBeginSostSaveAllAction();

    void BeginAlbum();
    void EndAlbum();
    json InsertSheet(int NSheet, int form_is_main, std::string formObozn,
        int vid_form, int deltaRow, int form_k1, int maxCol, int fixedCols);
    void ModifInsertSheet(json curElement, int aCol_SizeFixed);
    void DeleteSheet(int NSheet);
    void CellValue(int NSheet, int nRow, int nCol, int aType);
    void SetActiveSheet(int NSheet);
    void SetActiveCell(int NSheet, int nRow, int nCol);
    void ShowActiveCell(int NSheet);
    void Row(int nRow);
    void Col(int nCol);
    void InsertRange(int NSheet, int nRow1, int nCol1, int nRow2, int nCol2, int InsertType);
    void SetBorder(int NSheet, int nRow, int nCol);
    void SetPattern(int NSheet, int nRow, int nCol);
    void MaxRow(int Value);
    void MaxCol(int Value);
    void RowHeight(int nRow, short Value);
    void SetProtection(int NSheet, int nRow, int nCol, bool bLocked, bool bHidden);
    void SetCellFormat(int NSheet, int nRow, int nCol);
    void SetNumberFormat(int NSheet, int nRow, int nCol, std::string NumberFormat);
    void RowHidden(int NSheet, int nRow, bool Value);
    void ColHidden(int NSheet, int nCol, bool Value);
    void WordWrap(int NSheet, int nRow1, int nCol1, int nRow2, int nCol2, bool Value);
    void HAlign(int NSheet, int nRow1, int nCol1, int nRow2, int nCol2, int Value);
    void VAlign(int NSheet, int nRow1, int nCol1, int nRow2, int nCol2, int Value);
    void FontBold(int NSheet, int nRow1, int nCol1, int nRow2, int nCol2, bool Value);
    void MergeCells(int NSheet, int nRow1, int nCol1, int nRow2, int nCol2, bool Value);
    void SetBackgroundColor(int NSheet, int nRow1, int nCol1, int nRow2, int nCol2, unsigned crBG);
    void DeleteRange(int NSheet, int nRow1, int nCol1, int nRow2, int nCol2, int ShiftType);
    void ClearRange(int NSheet, int nRow1, int nCol1, int nRow2, int nCol2);
    void DrawRow(int NSheet, int nRow);
    void PostMessage(unsigned aHandle, std::string ActionType, int ActionTag);
    void Button(bool isCheckSaveAction, int NSheet, int nRow, int nCol, std::string aCaption, int aHeight);
    void Event(bool isCheckSaveAction, std::string aNameOfEventEndInterp);

    json getFActions();
//
//    void doActionInterp();
};

#endif // ACTION_VIEW_H
