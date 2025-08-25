
#include "unActionView.h"
#include "unF1FlexCelFormulas.h"
#include "TF1FlexCel.h"
#include "TracerManager.h"

/////////////// ��� ����������� ������ �������� /////////////////
/// FormLogicF1.ActionLog �������� ������ ��� ������ 'Actions' - ��� ����� ��������

// ������ �������� ����� �������� � ���� ������ �������
//  ����� ������ (isSaveAction = false) - ���������� �������� ������ ��� ������� ������ (����������, ��������)
//  ����� ������ (isSaveAction = true) -  ���������� ��� �������� ��� �������, �������, ��������

//void TActionView::doActionInterp() {
//    int index;
//    for (int nSheet = 1; nSheet <= TActionView.flexCel.NumSheets(); ++nSheet) {
//        std::string key = std::to_string(nSheet);
//        TActionView.slFlexCelFormulas.find(key, index);
//        if (index != -1) {
//            TF1FlexCelFormulas* flexCelFormulas = dynamic_cast<TF1FlexCelFormulas>(TActionView.slFlexCelFormulas.Objects[index]);
//            TActionView.flexCel.Sheet = nSheet;
//            try {
//                flexCelFormulas->ApplyChanges();
//            } catch (...) {
//                // Exception handling
//            }
//            delete flexCelFormulas; // Freeing memory
//            ActionView.slFlexCelFormulas.delete(index);
//        }
//    }
//}

// ������������� � ��������� ��������� ������ ActionView:
// - ������ ������ ��������
// - ������������� isSaveAction � False
void TActionView::doBeginSost(bool isBookEmpty_) {
    isBookEmpty = isBookEmpty_;
    FActions.clear();  // ������� ������ ��������
    isSaveAllAction = false;
}
void TActionView::doBeginSostSaveAllAction() {
    doBeginSost(false); // isBookEmpty_ �������� �� false
    setSaveAllAction();
}
// ������������� � ������������ ��������� ������ ActionView:
// - ������ �������� �� ��������.
// - ������������� isSaveAction � False
void TActionView::doSubBeginSost() {
    isSaveAllAction = false;
}

// ������������� �������, ��� � ������� �������� Action ������� ���������
//  ��� ������������ �������� ������ � �������, �������
void TActionView::setSaveAllAction() {
    TracerManager::Instance().StartFunctionTrace("TActionView::setSaveAllAction");
    std::string key = std::to_string(flexCel.Sheet);
    int index;

    // ������� ����� ������������ ������ �� �����
    if (slFlexCelFormulas.Find(key, index)) {
        // ������� ������ ������ (������������� �������������, ��� ��� ������������ std::shared_ptr)
        slFlexCelFormulas.Delete(index);
    }

    // �������� ������ ������� TF1FlexCelFormulas
    auto flexCelFormulas = std::make_shared<TF1FlexCelFormulas>(this, &flexCel);


    // ����� ������ ��� ������������� ������
    flexCelFormulas->RememberCellValues();

    // ���������� ������ ������� � ������ � ������
    slFlexCelFormulas.AddObject(key, flexCelFormulas);

    // ��������� ����� ��������
    isSaveAllAction = true;
    TracerManager::Instance().EndFunctionTrace();
}



void TActionView::BeginAlbum() {
    json curElement;
    FActions.push_back(curElement);
    curElement.emplace("BeginAlbum", nullptr);
}

void TActionView::EndAlbum() {
    json curElement;
    FActions.push_back(curElement);
    curElement.emplace("EndAlbum", nullptr);
}

// ������������ ������� ����, ����������� � ������� �����, � ������� ����������
// NSheet - ����� �������� �����
// form_is_main - ���������� ����� � �������: 0 - �������, 1 - �������, 2 - ���������������
// formObozn - ����������� � ��������
// vid_form - ��� �����: = 0 - ���������, = 1 - �����������, = 2 - ������ ��������������, = 3 - �������������, = 4 - �����-��������
// deltaRow - ���������� ����� � ���������
// form_k1 - ���������� ����� � �����
// maxCol - ���������� ��������
// ---- pokazCol - ����� ������� ��� ������������ ���������� ����� 31.08.2020
// fixedCols - ���������� ������������ ��������

json TActionView::InsertSheet(int NSheet, int form_is_main, std::string formObozn,
                                                    int vid_form, int deltaRow, int form_k1, int maxCol,
                                                    int fixedCols) {
    json curElement;
    FActions.push_back(curElement);
    curElement.emplace("InsSheet", NSheet);
    curElement.emplace("form_is_main", form_is_main);
    curElement.emplace("formObozn", formObozn);
    curElement.emplace("vid_form", vid_form);
    curElement.emplace("deltaRow", deltaRow);
    curElement.emplace("form_k1", form_k1);
    curElement.emplace("maxCol", maxCol);
    curElement.emplace("fixedCols", fixedCols);
    return curElement;
}

void TActionView::ModifInsertSheet(json curElement, int aCol_SizeFixed) {
    json ColSizeFixed = curElement["ColSizeFixed"];
    if (ColSizeFixed == nullptr) {
        curElement.emplace("ColSizeFixed", ColSizeFixed);
    }
    ColSizeFixed.push_back(aCol_SizeFixed);
}


// ��������� ������ ������� ������� ������, ����������� �� ������� �����
void TActionView::DeleteSheet(int NSheet) {
    int count = static_cast<int>(FActions.size());
    json curElement = nullptr;
    json Arr = nullptr;

    if (count > 0) {
        curElement = FActions[count - 1];
        Arr = curElement["DelSheet"];
        if (Arr == nullptr) {
            curElement = nullptr;
        }
    }

    if (curElement == nullptr) {
        FActions.push_back(curElement);
        curElement.emplace("DelSheet", Arr);
    }

    Arr.push_back(NSheet);
}

// Type = 1 - Number, 2 - string, 3 - Formula
void TActionView::CellValue(int NSheet, int nRow, int nCol, int aType) {
    if (!isSaveAllAction) {
        return;
    }

    json* curElement = nullptr;
    int count = FActions.size();

    if (count > 0) {
        curElement = &FActions.back();
        if (!((*curElement)["CellValue"].is_number_integer() && (*curElement)["CellValue"].get<int>() == NSheet)) {
            curElement = nullptr;
        }
    }

    if (curElement == nullptr) {
        json newElement;
        newElement["CellValue"] = NSheet;
        newElement["Row"] = json::array();
        newElement["Col"] = json::array();
        newElement["Type"] = json::array();
        FActions.push_back(newElement);
        curElement = &FActions.back();
    }

    (*curElement)["Row"].push_back(nRow);
    (*curElement)["Col"].push_back(nCol);
    (*curElement)["Type"].push_back(aType);
}

void TActionView::SetActiveSheet(int NSheet) {
    if (!isSaveAllAction)
        return;

    json curElement;
    FActions.push_back(curElement);
    curElement.emplace("SetActiveSheet", NSheet);
}

void TActionView::SetActiveCell(int NSheet, int nRow, int nCol) {
    if (!isSaveAllAction)
        return;

    json curElement;
    FActions.push_back(curElement);
    curElement.emplace("SetActiveCell", NSheet);
    curElement.emplace("Row", nRow);
    curElement.emplace("Col", nCol);
}

void TActionView::ShowActiveCell(int NSheet) {
    if (!isSaveAllAction)
        return;

    json curElement ;
    FActions.push_back(curElement);
    curElement.emplace("ShowActiveCell", NSheet);
}

void TActionView::Row(int nRow) {
    if (!isSaveAllAction)
        return;

    json curElement;
    FActions.push_back(curElement);
    curElement.emplace("Row", nRow);
}

void TActionView::Col(int nCol) {
    if (!isSaveAllAction)
        return;

    json curElement;
    FActions.push_back(curElement);
    curElement.emplace("Col", nCol);
}

void TActionView::InsertRange(int NSheet, int nRow1, int nCol1, int nRow2, int nCol2, int InsertType) {
    if (!isSaveAllAction)
        return;

    json curElement;
    FActions.push_back(curElement);
    curElement.emplace("InsertRange", NSheet);
    curElement.emplace("Row1", nRow1);
    curElement.emplace("Col1", nCol1);
    curElement.emplace("Row2", nRow2);
    curElement.emplace("Col2", nCol2);
    curElement.emplace("InsertType", InsertType);
}

void TActionView::SetBorder(int NSheet, int nRow, int nCol) {
    if (!isSaveAllAction)
        return;

    json curElement;
    FActions.push_back(curElement);
    curElement.emplace("SetBorder", NSheet);
    curElement.emplace("Row", nRow);
    curElement.emplace("Col", nCol);
}

void TActionView::SetPattern(int NSheet, int nRow, int nCol) {
    if (!isSaveAllAction)
        return;

    json curElement;
    FActions.push_back(curElement);
    curElement.emplace("SetPattern", NSheet);
    curElement.emplace("Row", nRow);
    curElement.emplace("Col", nCol);
}

void TActionView::MaxRow(int Value) {
    if (!isSaveAllAction)
        return;

    json curElement;
    FActions.push_back(curElement);
    curElement.emplace("MaxRow", Value);
}

void TActionView::MaxCol(int Value) {
    if (!isSaveAllAction)
        return;

    json curElement;
    FActions.push_back(curElement);
    curElement.emplace("MaxCol", Value);
}

void TActionView::RowHeight(int nRow, short Value) {
    if (!isSaveAllAction)
        return;

    json curElement;
    FActions.push_back(curElement);
    curElement.emplace("RowHeight", nRow);
    curElement.emplace("Value", Value);
}

void TActionView::SetProtection(int NSheet, int nRow, int nCol, bool bLocked, bool bHidden) {
    if (!isSaveAllAction)
        return;

    json curElement;
    FActions.push_back(curElement);
    curElement.emplace("SetProtection", NSheet);
    curElement.emplace("Row", nRow);
    curElement.emplace("Col", nCol);
    curElement.emplace("bLocked", bLocked);
    curElement.emplace("bHidden", bHidden);
}

void TActionView::SetCellFormat(int NSheet, int nRow, int nCol) {
    if (!isSaveAllAction)
        return;

    json curElement;
    FActions.push_back(curElement);
    curElement.emplace("SetCellFormat", NSheet);
    curElement.emplace("Row", nRow);
    curElement.emplace("Col", nCol);
}

void TActionView::SetNumberFormat(int NSheet, int nRow, int nCol, string NumberFormat) {
    if (!isSaveAllAction)
        return;

    json curElement;
    FActions.push_back(curElement);
    curElement.emplace("SetNumberFormat", NSheet);
    curElement.emplace("Row", nRow);
    curElement.emplace("Col", nCol);
    curElement.emplace("NumberFormat", NumberFormat);
}

void TActionView::RowHidden(int NSheet, int nRow, bool Value) {
    if (!isSaveAllAction)
        return;

    json curElement;
    FActions.push_back(curElement);
    curElement.emplace("RowHidden", NSheet);
    curElement.emplace("Row", nRow);
    curElement.emplace("Value", Value);
}

void TActionView::ColHidden(int NSheet, int nCol, bool Value) {
    if (!isSaveAllAction)
        return;

    json curElement;
    FActions.push_back(curElement);
    curElement.emplace("ColHidden", NSheet);
    curElement.emplace("Col", nCol);
    curElement.emplace("Value", Value);
}

void TActionView::WordWrap(int NSheet, int nRow1, int nCol1, int nRow2, int nCol2, bool Value) {
    if (!isSaveAllAction)
        return;

    json curElement;
    FActions.push_back(curElement);
    curElement.emplace("WordWrap", NSheet);
    curElement.emplace("Row1", nRow1);
    curElement.emplace("Col1", nCol1);
    curElement.emplace("Row2", nRow2);
    curElement.emplace("Col2", nCol2);
    curElement.emplace("Value", Value);
}

void TActionView::HAlign(int NSheet, int nRow1, int nCol1, int nRow2, int nCol2, int Value) {
    if (!isSaveAllAction)
        return;

    json curElement;
    FActions.push_back(curElement);
    curElement.emplace("HAlign", NSheet);
    curElement.emplace("Row1", nRow1);
    curElement.emplace("Col1", nCol1);
    curElement.emplace("Row2", nRow2);
    curElement.emplace("Col2", nCol2);
    curElement.emplace("Value", Value);
}

void TActionView::VAlign(int NSheet, int nRow1, int nCol1, int nRow2, int nCol2, int Value) {
    if (!isSaveAllAction)
        return;

    json curElement;
    FActions.push_back(curElement);
    curElement.emplace("VAlign", NSheet);
    curElement.emplace("Row1", nRow1);
    curElement.emplace("Col1", nCol1);
    curElement.emplace("Row2", nRow2);
    curElement.emplace("Col2", nCol2);
    curElement.emplace("Value", Value);
}

void TActionView::FontBold(int NSheet, int nRow1, int nCol1, int nRow2, int nCol2, bool Value) {
    if (!isSaveAllAction)
        return;

    json curElement;
    FActions.push_back(curElement);
    curElement.emplace("FontBold", NSheet);
    curElement.emplace("Row1", nRow1);
    curElement.emplace("Col1", nCol1);
    curElement.emplace("Row2", nRow2);
    curElement.emplace("Col2", nCol2);
    curElement.emplace("Value", Value);
}

void TActionView::MergeCells(int NSheet, int nRow1, int nCol1, int nRow2, int nCol2, bool Value) {
    if (!isSaveAllAction)
        return;

    json curElement;
    FActions.push_back(curElement);
    curElement.emplace("MergeCells", NSheet);
    curElement.emplace("Row1", nRow1);
    curElement.emplace("Col1", nCol1);
    curElement.emplace("Row2", nRow2);
    curElement.emplace("Col2", nCol2);
    curElement.emplace("Value", Value);
}

void TActionView::SetBackgroundColor(int NSheet, int nRow1, int nCol1, int nRow2, int nCol2, unsigned int crBG) {
    if (!isSaveAllAction)
        return;

    json curElement;
    FActions.push_back(curElement);
    curElement.emplace("SetBackgroundColor", NSheet);
    curElement.emplace("Row1", nRow1);
    curElement.emplace("Col1", nCol1);
    curElement.emplace("Row2", nRow2);
    curElement.emplace("Col2", nCol2);
    curElement.emplace("Value", crBG);
}

void TActionView::DeleteRange(int NSheet, int nRow1, int nCol1, int nRow2, int nCol2, int ShiftType) {
    if (!isSaveAllAction)
        return;

    json curElement;
    FActions.push_back(curElement);
    curElement.emplace("DeleteRange", NSheet);
    curElement.emplace("Row1", nRow1);
    curElement.emplace("Col1", nCol1);
    curElement.emplace("Row2", nRow2);
    curElement.emplace("Col2", nCol2);
    curElement.emplace("ShiftType", ShiftType);
}

void TActionView::ClearRange(int NSheet, int nRow1, int nCol1, int nRow2, int nCol2) {
    if (!isSaveAllAction)
        return;

    json curElement;
    FActions.push_back(curElement);
    curElement.emplace("ClearRange", NSheet);
    curElement.emplace("Row1", nRow1);
    curElement.emplace("Col1", nCol1);
    curElement.emplace("Row2", nRow2);
    curElement.emplace("Col2", nCol2);
}

void TActionView::DrawRow(int NSheet, int nRow) {
    if (!isSaveAllAction)
        return;

    json curElement;
    FActions.push_back(curElement);
    curElement.emplace("DrawRow", NSheet);
    curElement.emplace("Row1", nRow);
}

void TActionView::PostMessage(unsigned int aHandle, std::string ActionType, int ActionTag) {
    if (!isSaveAllAction)
        return;

    json curElement;
    FActions.push_back(curElement);
    curElement.emplace("PostMessage", nullptr);
    curElement.emplace("handle", aHandle);
    curElement.emplace("ActionType", ActionType);
    curElement.emplace("ActionTag", ActionTag);
}

void TActionView::Button(bool isCheckSaveAction, int NSheet, int nRow, int nCol, std::string aCaption, int aHeight) {
    if (isCheckSaveAction && !isSaveAllAction)
        return;

    json curElement;
    FActions.push_back(curElement);
    curElement.emplace("Button", nullptr);
    curElement.emplace("NSheet", NSheet);
    curElement.emplace("Row", nRow);
    curElement.emplace("Col", nCol);
    curElement.emplace("Caption", aCaption);
    curElement.emplace("Height", aHeight); // height in pixels
}

void TActionView::Event(bool isCheckSaveAction, std::string aNameOfEventEndInterp) {
    if (isCheckSaveAction && !isSaveAllAction)
        return;

    json curElement;
    FActions.push_back(curElement);
    curElement.emplace("Event", aNameOfEventEndInterp);
}

json TActionView::getFActions() {
    return FActions;
}
