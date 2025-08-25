#include "../SystFuncs/SystFuncsLogic.h"
#include "../DB/unDefDBconnect.h"
#include "../DB/unDBType.h"
#include "FormDopInp.h"
#include "../Form/unProgress.h"
#include "Form1.h"
#include "Form2SelectCond.h"
#include "Form2PokazKorteg.h"

void TForm2PokazKorteg::doForm2PokazKorteg(bool aIs_sort, int aOwnerNomRow, int newCurrPlanId, int saveCurrPlanId,
                                           std::string strName, std::string note2, std::string shablon, std::string oldshablon,
                                           int level_sign, int Is_open, int readonly, int is_hidden, int is_filter2, int ColOnFilter2,
                                           int noPrint, int is_filter, int nStolbec, std::string font, unsigned char h, unsigned char is_bold,
                                           unsigned char Align_vert, unsigned char Align_gor, PDopAtrCol aDopAtrCol, TQueryUni* q) {
    TracerManager::Instance().StartFunctionTrace("TForm2PokazKorteg::doForm2PokazKorteg");
    int wPlanId;
    int OldRow;
    TQueryUni *quPlanStr = new TQueryUni(DefDBconnect.Conn);
    quPlanStr->SQL->Add("SELECT b.*, a.plan_id FROM Plan_str a, Pokaztl b");
    if (newCurrPlanId == 0)
        quPlanStr->SQL->Add("WHERE plan_id IN (1, " + std::to_string(FormLogicF1->CurrPlanId) + ")");
    else
        quPlanStr->SQL->Add("WHERE plan_id IN (1, " + std::to_string(newCurrPlanId) + ")");
    quPlanStr->SQL->Add("AND b.pokaz_id = " + std::to_string(CurrPokazId));
    quPlanStr->SQL->Add("AND a.pokaz_id = b.pokaz_id");
    int wYear = FormLogicF1->CurrYear;
    if (aDopAtrCol->is_UstGoda) {
        switch (aDopAtrCol->is_set_god) {
            case 0: // наследуется
                wYear += aDopAtrCol->god_plus;
                break;
            case 1: // задается
                wYear = aDopAtrCol->god_ravno;
                break;
        }
    }
    switch (GetDBType()) {
        case TDBType::MSSQL:
            quPlanStr->SQL->Add("AND " + std::to_string(wYear) + " BETWEEN YEAR(b.pokaz_beg_date) AND YEAR(b.pokaz_end_date)");
            break;
        case TDBType::Postgre:
            quPlanStr->SQL->Add("AND " + std::to_string(wYear) + " BETWEEN date_part('year', b.pokaz_beg_date) AND date_part('year', b.pokaz_end_date)");
            break;
    }
    if (Is_open == 0) {
        if (is_filter2 == 6 || is_filter2 == 7) { // если заданы условия выборки по столбцам
            wPlanId = (newCurrPlanId == 0) ? FormLogicF1->CurrPlanId : newCurrPlanId;
            if (aDopAtrCol->is_planId)
                wPlanId = aDopAtrCol->plan_id;
            std::string aAndWhere;
            if (TForm2SelectCond(this).doForm2SelectCond(wPlanId, aAndWhere))
                quPlanStr->SQL->Add(aAndWhere);
        }
    }
    quPlanStr->SQL->Add("ORDER BY CASE WHEN plan_id > 1 THEN plan_str_order+400 WHEN plan_id = 1 and plan_str_order = 1000 THEN plan_str_order+10000 ELSE plan_str_order END");
    quPlanStr->Open();
    if (!quPlanStr->EoF()) {
        switch (quPlanStr->FieldByName("pokaz_is_razd")->AsInteger()) {
            case 0:
                if (newCurrPlanId > 0)
                    FormLogicF1->CurrPlanId = newCurrPlanId;
                OldRow = FormLogicF1->CurrInputRow;
                if (aIs_sort)
                    SelectSortsPokaz(BeginRow, quPlanStr, aDopAtrCol, is_filter, is_filter2, ColOnFilter2);
                else {
                    switch (is_filter) {
                        case 1:
                        case 14:
                            SelectOnlyKOSGU(CurrPokazId, BeginRow, "", "", false, is_filter, is_filter2, ColOnFilter2, aDopAtrCol, q->FieldByName("stmnt")->AsString());
                            break;
                        case 4:
                        case 5:
                        case 6:
                            SelectPokazOrderCP(CurrPokazId, BeginRow, "", "", false, is_filter, is_filter2, ColOnFilter2, aDopAtrCol, q->FieldByName("stmnt")->AsString(), shablon, Is_open);
                            break;
                        case 7:
                            SelectPokazOnlyVid(BeginRow, "", "", quPlanStr, false, is_filter, is_filter2, ColOnFilter2, aDopAtrCol, q->FieldByName("stmnt")->AsString(), 2);
                            break;
                        case 8:
                            SelectPokazOnlyVid(BeginRow, "", "", quPlanStr, false, is_filter, is_filter2, ColOnFilter2, aDopAtrCol, q->FieldByName("stmnt")->AsString(), 4);
                            break;
                        default:
                            SelectOwnerPokaz(BeginRow, "", "", quPlanStr, 0, is_filter, is_filter2, ColOnFilter2, aDopAtrCol, q->FieldByName("stmnt")->AsString(), shablon, level_sign);
                            break;
                    }
                }
                DoReadonlyHidden(OldRow + 1, FormLogicF1->CurrInputRow, readonly, is_hidden);
                FormLogicF1->CurrPlanId = saveCurrPlanId;
                break;
            case 1:
                if (newCurrPlanId > 0)
                    FormLogicF1->CurrPlanId = newCurrPlanId;
                OldRow = FormLogicF1->CurrInputRow;
                if (aIs_sort)
                    SelectSortsPokaz(BeginRow, quPlanStr, aDopAtrCol, is_filter, is_filter2, ColOnFilter2);
                else {
                    switch (is_filter) {
                        case 4:
                        case 5:
                        case 6:
                            SelectPokazOrderCP(CurrPokazId, BeginRow, "", "", (noPrint == 1), is_filter, is_filter2, ColOnFilter2, aDopAtrCol, q->FieldByName("stmnt")->AsString(), shablon, Is_open);
                            break;
                        case 7:
                            SelectPokazOnlyVid(BeginRow, "", "", quPlanStr, (noPrint == 1), is_filter, is_filter2, ColOnFilter2, aDopAtrCol, q->FieldByName("stmnt")->AsString(), 2);
                            break;
                        case 8:
                            SelectPokazOnlyVid(BeginRow, "", "", quPlanStr, (noPrint == 1), is_filter, is_filter2, ColOnFilter2, aDopAtrCol, q->FieldByName("stmnt")->AsString(), 4);
                            break;
                        default:
                            SelectOwnerPokaz(BeginRow, "", "", quPlanStr, noPrint, is_filter, is_filter2, ColOnFilter2, aDopAtrCol, q->FieldByName("stmnt")->AsString(), shablon, level_sign);
                            break;
                    }
                }
                DoReadonlyHidden(OldRow + 1, FormLogicF1->CurrInputRow, readonly, is_hidden);
                FormLogicF1->CurrPlanId = saveCurrPlanId;
                break;
            case 2:{;}
            case 3:{;}//{итого по разделу}
            case 4:{;}
            case 5:{;}
                if (Is_open > 0) {
                    if (newCurrPlanId > 0)
                        FormLogicF1->CurrPlanId = newCurrPlanId;
                    int OldRow = FormLogicF1->CurrInputRow;
                    if (aIs_sort)
                        SelectSortsPokaz(BeginRow, quPlanStr, aDopAtrCol, is_filter, is_filter2, ColOnFilter2);
                    else
                        SelectOwnerPokaz(BeginRow, "", "", quPlanStr, noPrint, is_filter, is_filter2, ColOnFilter2, aDopAtrCol, q->FieldByName("stmnt")->AsString(), shablon, level_sign);
                    DoReadonlyHidden(OldRow + 1, FormLogicF1->CurrInputRow, readonly, is_hidden);
                    FormLogicF1->CurrPlanId = saveCurrPlanId;
                } else {
                    int edIzm_id = 0;
                    std::string s = quPlanStr->FieldByName("pokaz_name")->AsString();
                    if (strName == "")
                        edIzm_id = quPlanStr->FieldByName("edizm_id")->AsInteger();
                    if (strName != "")
                        s = strName;
                    if (nStolbec > 0)
                        s += "#" + std::to_string(nStolbec) + "#";
                    CurrObject.pokaz_id = 0;
                    if (int N = quPlanStr->FieldByName("pokaz_is_razd")->AsInteger(); N == 4 || N == 5) {
                        CurrObject.pokaz_id = quPlanStr->FieldByName("pokaz_id")->AsInteger();
                        CurrObject.pokaz_is_razd = quPlanStr->FieldByName("pokaz_is_razd")->AsInteger();
                    }
                    FormLogicF1->CurrInputRow = TForm2::AddNewRow(FormLogicF1, aOwnerNomRow, 0, 0);
                    int RowVid = quPlanStr->FieldByName("pokaz_is_razd")->AsInteger();
                    if (q->FieldByName("stmnt")->AsString() == "КОРТЕЖ")
                        RowVid = 13;
                    TFormDopInp::SetListRowsItemAtr(FormLogicF1, FormLogicF1->CurrInputRow, RowVid,
                                                    level_sign, quPlanStr->FieldByName("pokaz_vert")->AsInteger(),
                                                    quPlanStr->FieldByName("pokaz_owner_id")->AsInteger(),
                                                    quPlanStr->FieldByName("pokaz_id")->AsInteger(),
                                                    quPlanStr->FieldByName("objekt_class_id")->AsInteger(),
                                                    quPlanStr->FieldByName("pokaz_bk_code")->AsString());
                    TFormatStyle FormatStyle =  TFormatStyle(font, h, is_bold, 0, 0);
                    TFormDopInp::SetFormatStyle(FormLogicF1, FormLogicF1->CurrInputRow, FormatStyle);
                    TFormDopInp::SetDopAtrToAtrRow(FormLogicF1, FormLogicF1->CurrInputRow, aDopAtrCol);
                    TFormDopInp::SetFilter2OnRow(FormLogicF1, FormLogicF1->CurrInputRow, is_filter2, ColOnFilter2, is_filter);


                    TForm2::SetTextNameCol(FormLogicF1, FormLogicF1->CurrInputRow, s, edIzm_id);
                    //--TunProgress::ShowText(FormLogicF1, s);
                    if (int N = FormLogicF1->quForms->FieldByName("work_with")->AsInteger(); N == 10 || N == 11 || N == 12)
                        TFormDopInp::SetToStrIdTEMP(FormLogicF1, FormLogicF1->CurrInputRow, q->FieldByName("str_id")->AsInteger());
                    if (is_hidden == 0)// если строка скрытная, то не сортир 24.01.2007
                        if (FormLogicF1->quForms->FieldByName("is_sort")->AsInteger() > 0)//{простые показатели в списке всегда сортируются}
                            TFormDopInp::SetInSort(FormLogicF1, FormLogicF1->CurrInputRow);
                    if (quPlanStr->FieldByName("pokaz_is_razd")->AsInteger() > 1) {
                        ExecPokaz(aOwnerNomRow, quPlanStr, q->FieldByName("plan_id")->AsInteger(), aDopAtrCol, q->FieldByName("stmnt")->AsString());
                        DoReadonlyHidden(FormLogicF1->CurrInputRow, FormLogicF1->CurrInputRow, readonly, is_hidden);
                    }
                    TFormDopInp::SetToTerminal(FormLogicF1, FormLogicF1->CurrInputRow);
                }
                break;
        }
        if (is_filter == 3) { // если был Фильтр по КБК
            if (shablon == oldshablon)
                doGenerirFormula1(aOwnerNomRow, OldRow, FormLogicF1->CurrInputRow, level_sign, "-#" + std::to_string(nStolbec) + "#", aDopAtrCol);
            else
                doGenerirFormula1(aOwnerNomRow, OldRow + 1, FormLogicF1->CurrInputRow, level_sign, "-#" + std::to_string(nStolbec) + "#", aDopAtrCol);
        }
    }
    delete quPlanStr;
    TracerManager::Instance().EndFunctionTrace();
}