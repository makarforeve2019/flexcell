#include "../SystFuncs/SystFuncsLogic.h"
#include "../DB/DB_FormStr.h"
#include "FormDopInp.h"
#include "TT_Korteg.h"
#include "../DB/unDefDBconnect.h"
#include "../DB/unDBType.h"
#include "../DB/DB_Pokaztl.h"
#include "../DB/DB_PlanStr.h"
#include "../DB/DB_Plan.h"
#include "Form2find.h"
#include "../DB/DB_ComonNPokaz.h"
#include "Form1.h"
#include "../DB/DB_EdIzm.h"
#include "../Pokazateli/unIDsClassFieldCell.h"
// #include "unProgress.h"
#include "Form2Owner1N.h"

void TForm2Owner1N::doForm2Owner1N(int aPlan_id, PDopAtrCol aDopAtrCol, int aOwnerNomRow)
{
    TQueryUni *quPlanStr = new TQueryUni(DefDBconnect.Conn);
    quPlanStr->SQL->Add("SELECT * FROM Plan_str1n a, Pokaztl b");
    quPlanStr->SQL->Add("WHERE plan_id IN (" + IntToStr(aDopAtrCol->plan_id) + ")");
    quPlanStr->SQL->Add("AND plan_str_owner_id = " + IntToStr(aDopAtrCol->owner_id_1N));
    quPlanStr->SQL->Add("AND a.pokaz_id = b.pokaz_id");
    quPlanStr->SQL->Add("AND a.class_id = " + IntToStr(aDopAtrCol->class_idItems));

    if (!aDopAtrCol->isIgnor)
    {
        switch (GetDBType())
        {
            case TDBType::MSSQL:
                quPlanStr->SQL->Add("AND " + IntToStr(FormLogicF1->CurrYear) + " BETWEEN YEAR(b.pokaz_beg_date) AND YEAR(b.pokaz_end_date)");
                break;
            case TDBType::Postgre:
                quPlanStr->SQL->Add("AND " + IntToStr(FormLogicF1->CurrYear) + " BETWEEN date_part('year', b.pokaz_beg_date) AND date_part('year', b.pokaz_end_date)");
                break;
        }
    }

    quPlanStr->SQL->Add("ORDER BY CASE WHEN plan_id > 1 THEN plan_str_order+400 WHEN plan_id = 1 and plan_str_order = 1000 THEN plan_str_order+10000 ELSE plan_str_order END");
    quPlanStr->Open();
    
    bool isNoRecord = quPlanStr->EoF();
    while (!quPlanStr->EoF())
    {
        CurrObject.pokaz_id = 0;
        CurrPokazId = 0;

        CurrObject.pokaz_id = quPlanStr->FieldByName("pokaz_id")->AsInteger();// это часть или объект
        CurrObject.pokaz_is_razd = quPlanStr->FieldByName("pokaz_is_razd")->AsInteger();
        if (quPlanStr->FieldByName("pokaz_is_razd")->AsInteger() == 2) //{это показатель, а не объект}
        {
            CurrObject.pokaz_id = 0;
            CurrPokazId = quPlanStr->FieldByName("pokaz_id")->AsInteger();
        }

        bool IsParamRoot = false;

        bool IsOgrTipom = true;
        if (FormLogicF1->quForms->FieldByName("ogr_class_id")->AsInteger() > 0)
        {
            if (!IsParamRoot && (FormLogicF1->quForms->FieldByName("ogr_class_id")->AsInteger() != quPlanStr->FieldByName("objekt_class_id")->AsInteger()))
            {
                IsOgrTipom = false;
            }
            if (int N = quPlanStr->FieldByName("pokaz_is_razd")->AsInteger(); N == 1 || N  == 3 || N == 5)
            {
                IsOgrTipom = true;
            }
        }
        bool IsExecPokaz = false;

        if (IsOgrTipom && !IsParamRoot && quPlanStr->FieldByName("pokaz_is_razd")->AsInteger() > 0)
        {
            // TunProgress->ShowText(FormLogicF1, quPlanStr->FieldByName("pokaz_name")->AsString);
            FormLogicF1->CurrInputRow = TForm2::AddNewRow(FormLogicF1, aOwnerNomRow, quPlanStr->FieldByName("pokaz_owner_id")->AsInteger(), quPlanStr->FieldByName("pokaz_id")->AsInteger());
            int RowVid = quPlanStr->FieldByName("pokaz_is_razd")->AsInteger();

            TFormDopInp::SetListRowsItemAtr(FormLogicF1, FormLogicF1->CurrInputRow, RowVid,
                                             LevelType, quPlanStr->FieldByName("pokaz_vert")->AsInteger(),
                                             quPlanStr->FieldByName("pokaz_owner_id")->AsInteger(),
                                             quPlanStr->FieldByName("pokaz_id")->AsInteger(),
                                             quPlanStr->FieldByName("objekt_class_id")->AsInteger(),
                                             quPlanStr->FieldByName("pokaz_bk_code")->AsString());

            TFormDopInp::SetDopAtrToAtrRow(FormLogicF1, FormLogicF1->CurrInputRow, aDopAtrCol);
            TFormDopInp::SetDopAtrToAtrRow1n(FormLogicF1, FormLogicF1->CurrInputRow, aDopAtrCol);
            TFormDopInp::SetFilter2OnRow(FormLogicF1, FormLogicF1->CurrInputRow, aDopAtrCol->is_filter2, aDopAtrCol->ColOnFilter2, aDopAtrCol->is_filter);
            TFormDopInp::SetFormatStyle(FormLogicF1, FormLogicF1->CurrInputRow, TFormatStyle(aDopAtrCol->font, aDopAtrCol->H, aDopAtrCol->Is_bold, aDopAtrCol->FontColor, aDopAtrCol->fonColor));

            DoReadonlyHidden(FormLogicF1->CurrInputRow, FormLogicF1->CurrInputRow, aDopAtrCol->readonly, aDopAtrCol->is_hidden);

            TForm2::SetTextNameCol(FormLogicF1, FormLogicF1->CurrInputRow,
                                   quPlanStr->FieldByName("pokaz_name")->AsString(), quPlanStr->FieldByName("edizm_id")->AsInteger());

            if (int N = FormLogicF1->quForms->FieldByName("work_with")->AsInteger(); (N == 1 || N == 3 || N == 5) && p->plan_id > 1)
            {
                ExecPokaz(aOwnerNomRow, quPlanStr, p->plan_id, aDopAtrCol, "ПОКАЗАТЕЛЬ");
            }
            else
            {
                ExecPokaz(aOwnerNomRow, quPlanStr, 0, aDopAtrCol, "ПОКАЗАТЕЛЬ");
            }
            IsExecPokaz = true; // вставили объект
        }
        LNext: ;
        quPlanStr->Next();
    }
    
    if (isNoRecord)
      TFormDopInp::SetToTerminal(FormLogicF1, FormLogicF1->CurrInputRow);

    delete quPlanStr;
}