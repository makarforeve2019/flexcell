#include "Form2Pologit.h"
#include "../SystFuncs/SystFuncsLogic.h"
#include "../DB/unDefDBconnect.h"
#include "../DB/unDBType.h"
#include "FormDopInp.h"
#include "Form1.h"

void TForm2Pologit::doForm2Pologit(const int aOwnerNomRow, int newCurrPlanId, std::string strName, std::string note2,
                                   short level_sign, short Is_open, short readonly, short is_hidden, short is_filter2,
                                   short ColOnFilter2, short nStolbec, std::string font, unsigned char h, unsigned char is_bold,
                                   unsigned char Align_vert, unsigned char Align_gor, TQueryUni *q)
{
    TDopAtrCol DopAtrCol;
    if (nStolbec > 0) {
        strName += "#" + std::to_string(nStolbec) + "#";
    }

    FormLogicF1->CurrInputRow = TForm2::AddNewRow(FormLogicF1, aOwnerNomRow, 0, 0);
    int RowVid = 16;
    TFormDopInp::SetListRowsItemAtr(FormLogicF1, FormLogicF1->CurrInputRow, RowVid, level_sign, 0, 0, 0, 0, "");
    TFormDopInp::SetFormatStyle(FormLogicF1, FormLogicF1->CurrInputRow, TFormatStyle(font, h, is_bold, 0, 0));
    TFormDopInp::SetDopAtrToAtrRow(FormLogicF1, FormLogicF1->CurrInputRow, &DopAtrCol);
    
    TForm2::SetTextNameCol(FormLogicF1, FormLogicF1->CurrInputRow, strName, 0);

    PAtrRow AtrRow = TFormDopInp::GetListRowsItem(FormLogicF1, FormLogicF1->CurrInputRow);

    AtrRow->str_id = Is_open; // это значение ToCol
    p->IsStmntPrisvoit = true;

    // если строка скрытная, то не сортир 24.01.2007
    if (is_hidden == 0 && FormLogicF1->quForms->FieldByName("is_sort")->AsInteger() == 1) {
        TFormDopInp::SetInSort(FormLogicF1, FormLogicF1->CurrInputRow);
    }

    for (int i = 1; i <= p->MaxHrCol; ++i) {
        if (i == TFormDopInp::GetNameCol(FormLogicF1)) continue;
        FormLogicF1->f1Forms->Protection_Set(true, true);
        // вставляем формулу в F1
        std::string Value = TFormDopInp::ReplaceNRowNCol(FormLogicF1->CurrInputRow, i, q->FieldByName("note2")->AsString());
        TFormatStyle FormatStyle;
        TFormDopInp::getColFrmtrAtr(FormLogicF1, 1, i, FormatStyle);
        FormatStyle.setToNull();
        FormatStyle.Align_vert = Align_vert;
        FormatStyle.align_gor = Align_gor;
        FormatStyle.font = font;
        FormatStyle.h = h;
        FormatStyle.Is_bold = is_bold;
        if (!Value.empty() && Value[0] == '^' && Value[1] == '[') {
            TForm1::SetCellValue(FormLogicF1, FormLogicF1->f1Forms, p->MaxHrCol, FormLogicF1->CurrInputRow, i,
                                 FormLogicF1->CurrInputRow, i, TFormDopInp::GetFormulaForColpar(Value, i), 0, &FormatStyle);
        }
    }

    this->DoReadonlyHidden(FormLogicF1->CurrInputRow, FormLogicF1->CurrInputRow, readonly, is_hidden);
}