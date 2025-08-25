#include "Form2F1.h"
#include "FormDopInp.h"
#include "Form1.h"
#include "Form2Plus.h"
#include "../DB/DB_Plan.h"

void TForm2F1::SelectF1(int newPokazCol, std::string note2, PFormatStyle aFormatStyle, int tocnost,
                        int is_filter2, int ColOnFilter2, PDopAtrCol aDopAtrCol)
{
    PAtrRow AtrRow;
    TFormatStyle* FormatStyle = new TFormatStyle();
    std::string stmntW, ws;

    TFormDopInp::SetDopAtrToAtrRow(FormLogicF1, FormLogicF1->CurrInputRow, aDopAtrCol);
    TFormDopInp::SetFilter2OnRow(FormLogicF1, FormLogicF1->CurrInputRow, is_filter2, ColOnFilter2, 0);
    TFormDopInp::SetTocnostOnRow(FormLogicF1, FormLogicF1->CurrInputRow, tocnost);

    if (newPokazCol == 0) // если не задан, то стандартный вариант
        newPokazCol = TFormDopInp::GetNameCol(FormLogicF1);

    for (int i = 1; i <= p->MaxHrCol; ++i)
    {
        if (i == newPokazCol)
            continue;
        AtrRow = TFormDopInp::GetListRowsItem(FormLogicF1, FormLogicF1->CurrInputRow);
        TFormDopInp::AddValueCellAtr(FormLogicF1, FormLogicF1->CurrInputRow, i, 0, 0, 0, VidOtch, FormLogicF1->CurrPlanId,
                                      0, 0, 0, 0, 0, 0, 0, 0);
        FormLogicF1->f1Forms->Protection_Set(true, true);
        // вставляем формулу в F1
        Value = TFormDopInp::ReplaceNRowNCol(FormLogicF1->CurrInputRow, i, note2);
        TFormDopInp::getColFrmtrAtr(FormLogicF1, 1, i, *FormatStyle);
        if (!((Value.empty()) && (Value[0] == '^') && (Value[1] == '[') )) // когда задан список формул
        {
            TForm1::SetCellValue(FormLogicF1, FormLogicF1->f1Forms, p->MaxHrCol, FormLogicF1->CurrInputRow, i,
                                 FormLogicF1->CurrInputRow, i, TFormDopInp::GetFormulaForColpar(Value, i), 0,
                                 FormatStyle);
        }
        else
        {
            stmntW = p->ListStmnt[i].Stmnt;

            FormatStyle->setToNull();
            FormatStyle->Align_vert = aFormatStyle->Align_vert;
            FormatStyle->align_gor = aFormatStyle->align_gor;
            if (!(stmntW == "КОДБК" || stmntW == "КОДБК1" || stmntW == "Примечание" || stmntW == "Владелец" ||
                  stmntW == "КОДБКРздл" || stmntW == "КОДБКРздл1" || stmntW == "КОДБКЦелСт" || stmntW == "КОДБКЦелСт1" ||
                  stmntW == "КОДБКВидРасх" || stmntW == "КОДБКЭкКл"))
            {
                TForm1::SetCellValue(FormLogicF1, FormLogicF1->f1Forms, p->MaxHrCol, FormLogicF1->CurrInputRow, i,
                                     FormLogicF1->CurrInputRow, i, Value, 0,
                                     FormatStyle);
            }
        }
        stmntW = p->ListStmnt[i].Stmnt;
        if (stmntW == "ПРОИЗВСТОЛБ")
        {
            TForm2Plus::SetNumberFormats1(FormLogicF1, FormLogicF1->CurrInputRow, i, p);
        }
        else if (stmntW == "NamePlanВстроке" && AtrRow->plan_id != 0)
        {
            ws = TDB_Plan::GetPlanName(AtrRow->plan_id);
            FormatStyle->setToNull();
            FormatStyle->Align_vert = aFormatStyle->Align_vert;
            FormatStyle->align_gor = aFormatStyle->align_gor;
            TForm1::SetCellValue(FormLogicF1, FormLogicF1->f1Forms, p->MaxHrCol, FormLogicF1->CurrInputRow, i,
                                 FormLogicF1->CurrInputRow, i, ws, 2, FormatStyle);
        }
        else if (stmntW == "OKTMOPlanВстроке" && AtrRow->plan_id != 0)
        {
            ws = TDB_Plan::GetOKATO_kod(AtrRow->plan_id);
            FormatStyle->setToNull();
            FormatStyle->Align_vert = aFormatStyle->Align_vert;
            FormatStyle->align_gor = aFormatStyle->align_gor;
            TForm1::SetCellValue(FormLogicF1, FormLogicF1->f1Forms, p->MaxHrCol, FormLogicF1->CurrInputRow, i,
                                 FormLogicF1->CurrInputRow, i, ws, 2, FormatStyle);
        }
        else if (stmntW == "НАИМПОКАЗ")
        {
            std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> codecvt;
            ws = codecvt.to_bytes(FormLogicF1->f1Forms->TextRC(FormLogicF1->CurrInputRow, p->PokazCol));

            FormatStyle->setToNull();
            FormatStyle->Align_vert = aFormatStyle->Align_vert;
            FormatStyle->align_gor = aFormatStyle->align_gor;
            FormatStyle->font = aFormatStyle->font;
            FormatStyle->h = aFormatStyle->h;
            FormatStyle->Is_bold = aFormatStyle->Is_bold;
            TForm1::SetCellValue(FormLogicF1, FormLogicF1->f1Forms, p->MaxHrCol, FormLogicF1->CurrInputRow, i,
                                 FormLogicF1->CurrInputRow, i, ws, 2, FormatStyle);
        }
    }
    delete FormatStyle;
}