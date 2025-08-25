#include "unTF1CellFormat.h"
#include "unF1toDE.h"



void TF1toDE::CreateClmnCells(TwwStringList* p, TFormLogicF1* FormLogicF1, int nCol) {
    for (int wRow = 1; wRow <= FormLogicF1->f1Forms->LastRow; wRow++) {
        TF1toDE::WorkSetBorder(p, FormLogicF1, wRow, nCol);
    }
}

void TF1toDE::ColWidth(TFormLogicF1* F1Book1, int nCol, int WidthTwips, int WidthPixels) {
    F1Book1->f1Forms->ColWidth_Set(nCol, WidthPixels);
}

void TF1toDE::CreateNewRowsCells(TwwStringList* p, TFormLogicF1* FormLogicF1, int nRow1, int nRow2) {
    for (int wRow = nRow1; wRow <= nRow2; wRow++) {
        for (int wCol = 1; wCol <= p->MaxHrCol; wCol++) {
            TF1toDE::WorkSetBorder(p, FormLogicF1, wRow, wCol);
        }
    }
}

int TF1toDE::CreateNewRowCells(TwwStringList* p, TFormLogicF1* FormLogicF1) {
    //mak int Result = FormLogicF1->f1Forms->LastRow + 1;

    int Result = FormLogicF1->CurrInputRow + 1;
    for (int wRow = Result; wRow <= Result; wRow++) {
        for (int wCol = 1; wCol <= p->MaxHrCol; wCol++) {
            TF1toDE::WorkSetBorder(p, FormLogicF1, wRow, wCol);
        }
    }
    return Result;
}

void TF1toDE::InsertRowsCells(TwwStringList* p, TFormLogicF1* FormLogicF1, int nRow1, int nRow2) {
    // Расширяем тип перед выполнением арифметической операции, чтобы избежать переполнения
    auto shiftFlags = static_cast<uint64_t>(F1ShiftTypeConstants::F1ShiftRows) +
                      static_cast<uint64_t>(F1ShiftTypeConstants::F1FixupAppend);

    // Приводим результат к unsigned int после вычисления
    FormLogicF1->f1Forms->InsertRange(nRow1, -1, nRow2, -1, static_cast<unsigned int>(shiftFlags));

    for (int wRow = nRow1; wRow <= nRow2; wRow++) {
        for (int wCol = 1; wCol <= p->MaxHrCol; wCol++) {
            TF1toDE::WorkSetBorder(p, FormLogicF1, wRow, wCol);
        }
    }
}


void TF1toDE::DeleteRows(TFormLogicF1* F1Book1, int nRow1, int nRow2) {
    F1Book1->f1Forms->DeleteRange(nRow1, -1, nRow2, -1, static_cast<unsigned int>(F1ShiftTypeConstants::F1ShiftRows));
}

void TF1toDE::WorkSetBorder(TwwStringList* p, TFormLogicF1* FormLogicF1, int aRow, int aCol) {
    int R1, C1, R2, C2;
    int StyleTopBorder, StyleLeftBorder, StyleBottomBorder, StyleRightBorder;

    if ((p->form_endBodyRow > 0) && (aRow > p->form_endBodyRow)) {
        return;
    }

    FormLogicF1->f1Forms->Selection_Get(0, R1, C1, R2, C2);
    FormLogicF1->f1Forms->Selection_Set(aRow, aCol, aRow, aCol);



    if (aRow > p->MaxHrRow) {
        if (aRow == (p->MaxHrRow + 1)) {
            StyleTopBorder = static_cast<int>(F1BorderStyleConstants::F1BorderMedium);
        } else {
            StyleTopBorder = static_cast<int>(F1BorderStyleConstants::F1BorderThin);
        }
        StyleLeftBorder = static_cast<int>(F1BorderStyleConstants::F1BorderThin);
        StyleBottomBorder = static_cast<int>(F1BorderStyleConstants::F1BorderThin);
        StyleRightBorder = static_cast<int>(F1BorderStyleConstants::F1BorderThin);
        FormLogicF1->f1Forms->BorderStyle_Set(StyleTopBorder, StyleLeftBorder, StyleBottomBorder, StyleRightBorder);
    } else if ((aRow >= p->DeltaRow + 1) && (aRow <= p->MaxHrRow)) {
        StyleTopBorder = static_cast<int>(F1BorderStyleConstants::F1BorderMedium);
        StyleLeftBorder = static_cast<int>(F1BorderStyleConstants::F1BorderMedium);
        StyleBottomBorder = static_cast<int>(F1BorderStyleConstants::F1BorderMedium);
        StyleRightBorder = static_cast<int>(F1BorderStyleConstants::F1BorderMedium);
        FormLogicF1->f1Forms->BorderStyle_Set(StyleTopBorder, StyleLeftBorder, StyleBottomBorder, StyleRightBorder);
    }

    FormLogicF1->f1Forms->Selection_Set(R1, C1, R2, C2);
}

