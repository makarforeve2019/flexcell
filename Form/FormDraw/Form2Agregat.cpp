#include "Form2Agregat.h"
#include "../SystFuncs/SystFuncsLogic.h"
#include "../DB/unDefDBconnect.h"
#include "unFormLogicF1.h"
#include "../DB/unDBType.h"
// #include "unProgress.h" //Убрал пока
#include "FormDopInp.h"

void TForm2Agregat::doForm2Agregat(const int aOwnerNomRow, const int newCurrPlanId, const std::string strName,
    const std::string note2, const int level_sign, const int readonly, const int is_hidden, const int is_filter2,
    const int ColOnFilter2, const int nStolbec, const std::string font, const unsigned char h, const unsigned char is_bold, TQueryUni* q)
{
    TracerManager::Instance().StartFunctionTrace("TForm2Agregat::doForm2Agregat");
    TQueryUni *quPlanStr = new TQueryUni(DefDBconnect.Conn);
    quPlanStr->SQL->Add("SELECT * FROM Plan_str a, Pokaztl b");
    if (newCurrPlanId == 0)
        quPlanStr->SQL->Add("WHERE plan_id IN (1, " + IntToStr(FormLogicF1->CurrPlanId) + ")");
    else
        quPlanStr->SQL->Add("WHERE plan_id IN (1, " + IntToStr(newCurrPlanId) + ")");
    quPlanStr->SQL->Add("AND b.pokaz_id = " + IntToStr(q->FieldByName("field_id")->AsInteger()));
    quPlanStr->SQL->Add("AND a.pokaz_id = b.pokaz_id");
    
    switch (GetDBType())
    {
        case TDBType::MSSQL:
            quPlanStr->SQL->Add("AND " + IntToStr(FormLogicF1->CurrYear) + " BETWEEN YEAR(b.pokaz_beg_date) AND YEAR(b.pokaz_end_date)");
            break;
        case TDBType::Postgre:
            quPlanStr->SQL->Add("AND " + IntToStr(FormLogicF1->CurrYear) + " BETWEEN date_part('year', b.pokaz_beg_date) AND date_part('year', b.pokaz_end_date)");
            break;
    }
    
    quPlanStr->SQL->Add("ORDER BY CASE WHEN plan_id > 1 THEN plan_str_order + 400 WHEN plan_id = 1 and plan_str_order = 1000 THEN plan_str_order + 10000 ELSE plan_str_order END");
    quPlanStr->Open();
    
    if (!quPlanStr->EoF())
    {
        std::string s = quPlanStr->FieldByName("pokaz_name")->AsString();
        if (!strName.empty())
            s = strName;
        if (nStolbec > 0)
            s += "#" + std::to_string(nStolbec) + "#";
        
        FormLogicF1->CurrInputRow = TForm2::AddNewRow(FormLogicF1, aOwnerNomRow, 0, 0);
        int RowVid = 12;
        TFormDopInp::SetListRowsItemAtr(FormLogicF1, FormLogicF1->CurrInputRow, RowVid, level_sign, 0,
                                         q->FieldByName("pokaz_id")->AsInteger(), q->FieldByName("field_id")->AsInteger(), 0, "");
        TDopAtrCol DopAtrCol;
        TFormDopInp::SetDopAtrToAtrRow(FormLogicF1, FormLogicF1->CurrInputRow, &DopAtrCol);
        TFormDopInp::SetFilter2OnRow(FormLogicF1, FormLogicF1->CurrInputRow, is_filter2, ColOnFilter2, 0);
        TFormDopInp::SetFormatStyle(FormLogicF1, FormLogicF1->CurrInputRow, TFormatStyle(font, h, is_bold, 0, 0));
        TForm2::SetTextNameCol(FormLogicF1, FormLogicF1->CurrInputRow, s, 0);

        // Progress indication
        // TunProgress::ShowText(FormLogicF1, s);

        if (is_hidden == 0) // если строка скрытная, то не сортир 24.01.2007
          if (FormLogicF1->quForms->FieldByName("is_sort")->AsInteger() == 1) //не 2, главное, что сортировка
            TFormDopInp::SetInSort(FormLogicF1, FormLogicF1->CurrInputRow);

        CurrPokazId = q->FieldByName("field_id")->AsInteger();
        CurrObject.pokaz_id = 0;
        ExecPokaz(aOwnerNomRow, quPlanStr, q->FieldByName("plan_id")->AsInteger(), &DopAtrCol, q->FieldByName("stmnt")->AsString());
        this->DoReadonlyHidden(FormLogicF1->CurrInputRow, FormLogicF1->CurrInputRow, readonly, is_hidden);
        TFormDopInp::SetToTerminal(FormLogicF1, FormLogicF1->CurrInputRow);

        for (int i = 1; i <= p->MaxHrCol; ++i)
        {
            PCellValueAtr vd = TFormDopInp::GetCellValueAtrByRC(FormLogicF1, FormLogicF1->CurrInputRow, i);
            if (vd == NULL)
                continue;
            
            std::string stmntCol = p->ListStmnt[i].Stmnt;
            if (vd->pokaz_type != 1)
            {
                if (stmntCol == "КОДБКРздл" || stmntCol == "КОДБКРздл1" || stmntCol == "КОДБКЦелСт" ||
                    stmntCol == "КОДБКЦелСт1" || stmntCol == "КОДБКВидРасх" || stmntCol == "КОДБКЭкКл")
                {
                    vd->EnumId = quPlanStr->FieldByName("enum_id")->AsInteger(); // это kbk_id
                    continue;
                }
            }

            if (stmntCol == "ЗНАЧЕНИЕСВ" || stmntCol == "ЗНАЧЕНИЕСВНИ" || stmntCol == "ЗНАЧЕНИЕСВИР")
            {
                vd->DataType = 23 + deltaDType; // Агрегат для 'ЗНАЧЕНИЕСВ' и 'ЗНАЧЕНИЕСВНИ' и 'ЗНАЧЕНИЕСВИР'
                vd->EnumId = quPlanStr->FieldByName("enum_id")->AsInteger(); // это kbk_id
                vd->OwnerId = q->FieldByName("pokaz_id")->AsInteger();
                
                if (!note2.empty())
                {
                    int wotkuda = TFormDopInpMF::GetPokazIdForCol(note2, vd->Col);
                    if (wotkuda > 10)
                        vd->OwnerId = wotkuda;
                }
                
                if (q->FieldByName("is_svod")->AsInteger() == 1)
                    vd->PlanId = 1; // это признак свода по всем субъектам
            }
            else if (stmntCol == "КБК-Дгвр:Сумма№БО" || stmntCol == "КБК-Дгвр:СуммаБО" ||
                     stmntCol == "Мрпр-Дгвр:Сумма№БО" || stmntCol == "Мрпр-Дгвр:СуммаБО")
            {
                vd->EnumId = quPlanStr->FieldByName("enum_id")->AsInteger(); // это kbk_id
                vd->OwnerId = q->FieldByName("pokaz_id")->AsInteger();

                if (!note2.empty())
                {
                    int wotkuda = TFormDopInpMF::GetPokazIdForCol(note2, vd->Col);
                    if (wotkuda > 10)
                        vd->OwnerId = wotkuda;
                }

                if (q->FieldByName("is_svod")->AsInteger() == 1)
                    vd->PlanId = 1;  // это признак свода по всем субъектам

                if (stmntCol == "КБК-Дгвр:Сумма№БО" || stmntCol == "КБК-Дгвр:СуммаБО")
                    vd->DataType = 61 + deltaDType;
                else if (stmntCol == "Мрпр-Дгвр:Сумма№БО" || stmntCol == "Мрпр-Дгвр:СуммаБО")
                    vd->DataType = 62 + deltaDType;
            }
        }
    }
    delete quPlanStr;
    TracerManager::Instance().EndFunctionTrace();
}