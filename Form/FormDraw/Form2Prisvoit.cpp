#include "../SystFuncs/SystFuncsLogic.h"
#include "../DB/unDefDBconnect.h"
#include "../DB/unDBType.h"
#include "../Form/unProgress.h"
#include "FormDopInp.h"
#include "Form1.h"
#include "../DB/DB_Plan.h"
#include "Form2Prisvoit.h"
#include "../system/ClassesUtils.h"

void TForm2Prisvoit::doForm2Prisvoit(int aOwnerNomRow, int newCurrPlanId, std::string strName, std::string note2,
                     int level_sign, bool Is_open, bool readonly, bool is_hidden, bool is_filter2,
                     int ColOnFilter2, int nStolbec, std::string font, unsigned char h, bool is_bold,
                     unsigned char Align_vert, unsigned char Align_gor, TQueryUni* q, unsigned char isRow)
{
        TracerManager::Instance().StartFunctionTrace("TForm2Prisvoit::doForm2Prisvoit");
        if (nStolbec > 0) {
            strName = strName+'#'+IntToStr(nStolbec)+'#';
        }

        FormLogicF1->CurrInputRow = TForm2::AddNewRow(FormLogicF1, aOwnerNomRow, 0, 0);
        int RowVid = 14;
        TFormDopInp::SetListRowsItemAtr(FormLogicF1, FormLogicF1->CurrInputRow, RowVid, level_sign, 0, 0, 0, 0, "");
        // для Присвоить значение из описания строки cbIsRow кладем в Tocnost
        TFormDopInp::SetTocnostOnRow(FormLogicF1, FormLogicF1->CurrInputRow, isRow);
        TFormatStyle formatStyle(font, h, is_bold, Align_vert, Align_gor);
        TFormDopInp::SetFormatStyle(FormLogicF1, FormLogicF1->CurrInputRow, formatStyle);
        TDopAtrCol DopAtrCol;
        TFormDopInp::SetDopAtrToAtrRow(FormLogicF1, FormLogicF1->CurrInputRow, &DopAtrCol);

        TForm2::SetTextNameCol(FormLogicF1, FormLogicF1->CurrInputRow, strName, 0);

        PAtrRow atrRow = TFormDopInp::GetListRowsItem(FormLogicF1, FormLogicF1->CurrInputRow);
        atrRow->str_id = Is_open; // это значение ToCol

        if (q->FieldByName("is_group")->AsInteger() != 0)
            atrRow->IsUpd = true; // значит Присвоение только при вводе значения

        TForm2::p->IsStmntPrisvoit = true;
        int is_sort = FormLogicF1->quForms->FieldByName("is_sort").AsInteger();
        // если строка скрытная, то не сортир 24.01.2007
		if ((is_hidden == 0) && (is_sort == 1))
		      TFormDopInp::SetInSort(FormLogicF1, FormLogicF1->CurrInputRow);


        for (int i = 1; i <= p->MaxHrCol; ++i) {
            if (i == TFormDopInp::GetNameCol(FormLogicF1))
                continue;

            PCellValueAtr wvd = TFormDopInp::AddValueCellAtr(FormLogicF1, FormLogicF1->CurrInputRow, i,
               0, 0, 0, VidOtch, FormLogicF1->CurrPlanId, 0, 0, 0, 0, 0, 0, 0, 0);
            FormLogicF1->f1Forms->Protection_Set(true, true);
            // вставляем формулу в F1
            std::string value = TFormDopInp::ReplaceNRowNCol(FormLogicF1->CurrInputRow, i, q->FieldByName("note2")->AsString());
            TFormatStyle colFormatStyle;
            TFormDopInp::getColFrmtrAtr(FormLogicF1, 1, i, colFormatStyle);
            colFormatStyle.setToNull();
            colFormatStyle.align_vert = Align_vert;
            colFormatStyle.align_gor = Align_gor;
            colFormatStyle.font = font;
            colFormatStyle.h = h;
            colFormatStyle.is_bold = is_bold;

            std::string ws = TFormDopInp::GetFormulaForColpar(value, i);
            if (!value.empty() && value[0] == '^' && value[1] == '[') { // когда задан список формул
               TForm1::SetCellValue(FormLogicF1, FormLogicF1->f1Forms, p->MaxHrCol, FormLogicF1->CurrInputRow, i,
                  FormLogicF1->CurrInputRow, i, ws, 0, &colFormatStyle);
                if (!ws.empty())
                    wvd->EvalType = -1; // Для того, чтобы в соотств ячейке след строки установ ReadOnly
            }
            if (ws.empty()) {
                std::string stmntW = p->ListStmnt[i].Stmnt;
                if (stmntW == "NamePlanВстроке" && atrRow->plan_id != 0) {
                    ws = TDB_Plan::GetPlanName(atrRow->plan_id);
                    TForm1::SetCellValue(FormLogicF1, FormLogicF1->f1Forms, p->MaxHrCol, FormLogicF1->CurrInputRow, i,
                      FormLogicF1->CurrInputRow, i, ws, 2, &colFormatStyle);
                }
                else if (stmntW == "OKTMOPlanВстроке" && atrRow->plan_id != 0) {
                    ws = TDB_Plan::GetOKATO_kod(atrRow->plan_id);
                    TForm1::SetCellValue(FormLogicF1, FormLogicF1->f1Forms, p->MaxHrCol, FormLogicF1->CurrInputRow, i,
                       FormLogicF1->CurrInputRow, i, ws, 2, &colFormatStyle);
                }
                else if (stmntW == "НАИМПОКАЗ") {
                    ws = char16ToUTF8(FormLogicF1->f1Forms->TextRC(FormLogicF1->CurrInputRow, p->PokazCol));
                    colFormatStyle.align_gor = 1;
                    TForm1::SetCellValue(FormLogicF1, FormLogicF1->f1Forms, p->MaxHrCol, FormLogicF1->CurrInputRow, i, FormLogicF1->CurrInputRow, i,
                      ws, 2, &colFormatStyle);
                }
                else {
                    TForm1::SetCellValue(FormLogicF1, FormLogicF1->f1Forms, p->MaxHrCol, FormLogicF1->CurrInputRow, i,
                       FormLogicF1->CurrInputRow, i, "", 0, &colFormatStyle);
                }
            }
        }

        DoReadonlyHidden(FormLogicF1->CurrInputRow, FormLogicF1->CurrInputRow, readonly, is_hidden);
        TracerManager::Instance().EndFunctionTrace();
    }