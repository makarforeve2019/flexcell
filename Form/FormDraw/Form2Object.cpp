#include "../SystFuncs/SystFuncsLogic.h"
#include "../DB/unDefDBconnect.h"
#include "Form2F1.h"
#include "../DB/DB_FormStr.h"
#include "../DB/DB_Pokaztl.h"
#include "Form1.h"
#include "Form2Plus.h"
#include "../DB/unDBType.h"
#include "../DB/DB_Cell.h"
#include "unTF1CellFormat.h"
#include "Form2Object.h"
#include "unTF1CellFormat.h"
//ky: Объектная форма

void TForm2Object::SelectSpField(int aOwnerNomRow, PDopAtrCol aDopAtrCol)
{
    PAtrStmnt atrStmnt = nullptr;
    TAtrRow *atrRow = nullptr;
    TDopAtrCol *dopAtrCol = nullptr;
    TwwStringList *p = TFormDopInp::GetTww(FormLogicF1);



    FormLogicF1->f1Forms->DisableShrink = true;
    FormLogicF1->f1Forms->DisableAutoSize = true;

    int maxRow = FormLogicF1->quForms->FieldByName("form_k1")->AsInteger();
    int maxCol = FormLogicF1->quForms->FieldByName("form_k2")->AsInteger();
    if (maxRow == 0) {
        maxRow = 257;
    }
    if (maxCol == 0) {
        maxCol = 257;
    }

    FormLogicF1->f1Forms->MaxRow = maxRow;
    FormLogicF1->f1Forms->MaxCol = maxCol;
    for (int col = 1; col <= FormLogicF1->f1Forms->MaxCol; col++) {
        FormLogicF1->f1Forms->ColWidth[col] = 15;
    }

    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    TDB_FormStr::GetItemsN1N2(q, FormLogicF1->quForms->FieldByName("form_id")->AsInteger(), 1500, cMaxStrN);
    while (!q->EoF()) {
        int row1 = q->FieldByName("attr_1")->AsInteger();
        int col1 = q->FieldByName("attr_2")->AsInteger();
        int row2 = q->FieldByName("attr_3")->AsInteger();
        int col2 = q->FieldByName("attr_4")->AsInteger();
        std::string stmnt = q->FieldByName("Stmnt")->AsString();

        FormLogicF1->f1Forms->Selection_Set(row1, col1, row2, col2);
        if (row1 != row2 || col1 != col2) {
            FormLogicF1->f1Forms->MergeCells = true;
        }

        std::string fontName = q->FieldByName("font")->AsString();
        if (fontName == "") {
            fontName = "Arial";
        }
        FormLogicF1->f1Forms->FontName = utf8_to_utf16(fontName).c_str();

        int fontSize = q->FieldByName("h")->AsInteger();
        if (fontSize == 0) {
            fontSize = 10;
        }
        FormLogicF1->f1Forms->FontSize = fontSize;

        int isBold = q->FieldByName("is_bold")->AsInteger();
        FormLogicF1->f1Forms->FontBold = (isBold & 0x01) != 0;
        FormLogicF1->f1Forms->FontItalic = (isBold & 0x02) != 0;
        FormLogicF1->f1Forms->FontUnderline = (isBold & 0x04) != 0;
        FormLogicF1->f1Forms->FontStrikeout = (isBold & 0x08) != 0;

        int fontColor = q->FieldByName("fontColor")->AsInteger();
        if (fontColor == SystFuncs::SystFuncsAltWin::GetColorFromRGB(255, 255, 255)) {
            fontColor = SystFuncs::SystFuncsAltWin::GetColorFromRGB(0, 0, 0);
        }
        FormLogicF1->f1Forms->FontColor = fontColor;

        int backgroundColor = q->FieldByName("patternBGcolor")->AsInteger();
        if (backgroundColor == 0) {
            backgroundColor = SystFuncs::SystFuncsAltWin::GetColorFromRGB(255, 255, 255);
        }
        FormLogicF1->f1Forms->BackgroundColor_Set(backgroundColor);

        switch (q->FieldByName("align_vert")->AsInteger()) {
            case 1:
                FormLogicF1->f1Forms->VAlign = static_cast<int16_t>(F1VAlignConstants::F1VAlignTop);
                break;
            case 2:
                FormLogicF1->f1Forms->VAlign = static_cast<int16_t>(F1VAlignConstants::F1VAlignCenter);
                break;
            case 3:
                FormLogicF1->f1Forms->VAlign = static_cast<int16_t>(F1VAlignConstants::F1VAlignBottom);
                break;
        }
        switch (q->FieldByName("align_gor")->AsInteger()) {
            case 1:
                FormLogicF1->f1Forms->HAlign = static_cast<int16_t>(F1HAlignConstants::F1HAlignLeft);
                break;
            case 2:
                FormLogicF1->f1Forms->HAlign = static_cast<int16_t>(F1HAlignConstants::F1HAlignCenter);
                break;
            case 3:
                FormLogicF1->f1Forms->HAlign = static_cast<int16_t>(F1HAlignConstants::F1HAlignRight);
                break;
        }

        FormLogicF1->f1Forms->WordWrap = true;

        int borderStyle = q->FieldByName("borderStyle")->AsInteger();
        FormLogicF1->f1Forms->BorderStyle_Set(borderStyle, borderStyle, borderStyle, borderStyle);

        int borderColor = q->FieldByName("borderColor")->AsInteger();
        FormLogicF1->f1Forms->BorderColor_Set(borderColor, borderColor, borderColor, borderColor);

        if (stmnt == "ТЕКСТ") {
            FormLogicF1->f1Forms->Text = utf8_to_utf16(q->FieldByName("note1")->AsString()).c_str();
        }
        else if (stmnt == "ЗНАЧЕНИЕСВ") {
            int field_id = q->FieldByName("field_id")->AsInteger();
            TQueryUni *qOwnerPlanStr = new TQueryUni(DefDBconnect.Conn);
            qOwnerPlanStr->SQL->Add("SELECT b.*, a.plan_id FROM Plan_str a, Pokaztl b");
            qOwnerPlanStr->SQL->Add("WHERE b.pokaz_id = " + IntToStr(field_id));

            qOwnerPlanStr->SQL->Add("AND a.pokaz_id = b.pokaz_id");
            int year = FormLogicF1->CurrYear;
            if (aDopAtrCol->is_UstGoda) {
                if (aDopAtrCol->is_set_god == 1) {
                    year = aDopAtrCol->god_ravno;
                }
            }
            switch (GetDBType()) {
                case TDBType::MSSQL:
                    qOwnerPlanStr->SQL->Add("AND " + IntToStr(year) + " BETWEEN YEAR(b.pokaz_beg_date) AND YEAR(b.pokaz_end_date)");
                    break;
                case TDBType::Postgre:
                    qOwnerPlanStr->SQL->Add("AND " + IntToStr(year) + " BETWEEN date_part('year', b.pokaz_beg_date) AND date_part('year', b.pokaz_end_date)");
                    break;
            }
            qOwnerPlanStr->Open();

            atrStmnt->form_id=FormLogicF1->quForms->FieldByName("form_id")->AsInteger();
            atrStmnt->form_str_n=q->FieldByName("form_str_n")->AsInteger();
            atrStmnt->Stmnt = stmnt;

            // форматирование, заданное при описании формы
            atrStmnt->FormatStyle.align_vert=q->FieldByName("align_vert")->AsInteger();
            atrStmnt->FormatStyle.align_gor=q->FieldByName("borderStyle")->AsInteger();
            atrStmnt->FormatStyle.font=fontName;
            atrStmnt->FormatStyle.h=fontSize;
            atrStmnt->FormatStyle.is_bold=q->FieldByName("is_bold")->AsInteger();
            atrStmnt->FormatStyle.FontColor=fontColor;
            atrStmnt->FormatStyle.fonColor=backgroundColor;
            atrStmnt->FormatStyle.borderStyle=q->FieldByName("borderStyle")->AsInteger();
            atrStmnt->FormatStyle.borderColor=q->FieldByName("borderColor")->AsInteger();
            // End of -> форматирование
            atrStmnt->read_only=false;//ky: Объектная форма:???
            atrStmnt->readonly= q->FieldByName("readonly")->AsInteger(); // что задано в описании столбца непосредственно

            atrStmnt->PeriodType=q->FieldByName("period")->AsInteger();
            atrStmnt->vid_otch=q->FieldByName("vid_otch")->AsInteger();
            atrStmnt->is_ist_fin=q->FieldByName("is_ist_fin")->AsInteger();
            atrStmnt->ist_fin_grad_id=q->FieldByName("ist_fin_grad_id")->AsInteger();

            atrStmnt->natur_vid=0;//ky: Объектная форма: уточнить
            atrStmnt->ValuePeriod=q->FieldByName("ValuePeriod")->AsInteger();

            atrStmnt->field_id=q->FieldByName("field_id")->AsInteger(); // только в случае работы с объектами - это ид характеристики
            atrStmnt->CoefPlus1=q->FieldByName("CoefPlus1")->AsInteger();
            atrStmnt->plan_id=p->OwnerFormPlanId;
            atrStmnt->pokaz_id=ParamObjectId;//ky: Объектная форма: уточнить
            atrStmnt->is_set_god=q->FieldByName("is_set_god")->AsInteger();
            atrStmnt->god_plus=q->FieldByName("god_plus")->AsInteger();
            atrStmnt->god_ravno=q->FieldByName("god_ravno")->AsInteger();
            atrStmnt->din_izm=q->FieldByName("din_izm").AsString();

            atrStmnt->nValidator=q->FieldByName("nValidator")->AsInteger();
            atrStmnt->vid_uslovij=q->FieldByName("vid_uslovij")->AsInteger();
            atrStmnt->Value1=q->FieldByName("value1")->AsInteger();
            atrStmnt->Value2=q->FieldByName("value2")->AsInteger();
            atrStmnt->Shablon=q->FieldByName("shablon")->AsString();
            atrStmnt->Msg=q->FieldByName("Msg")->AsString();

            atrRow->Row=row1;
            atrRow->CurRowVid= 4; // 4 - объект
            atrRow->pokaz_vert=0;

            dopAtrCol->SetDopAtrColNull();

            CurrPokazId=field_id;
            ExecPokaz1clmn(row1,col1, atrStmnt, atrRow, qOwnerPlanStr, 0, dopAtrCol , stmnt);

            delete qOwnerPlanStr;
            
            PCellValueAtr vd = TFormDopInp::GetCellValueAtrByRC(FormLogicF1, row1, col1);
            if (vd != nullptr) {
                vd->stmnt = stmnt;
            }

            FormLogicF1->f1Forms->NumberFormat = reinterpret_cast<const char16_t *>("# ### ### ### ##0.00;-# ### ### ### ##0.00");
        }

        q->Next();
    }
    delete q;
}

TForm2Object::TForm2Object(TForm2 *pForm2) : TForm2ExecPokaz1clmn(pForm2) {

}
