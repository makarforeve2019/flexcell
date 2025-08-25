#include "Form2Uslovie.h"
#include "../SystFuncs/SystFuncsLogic.h"
#include "../DB/unDefDBconnect.h"
#include "../DB/unDBType.h"
#include "FormDopInp.h"
#include "Form1.h"

void TForm2Uslovie::doForm2Uslovie(const int aOwnerNomRow, int newCurrPlanId, std::string strName, std::string note2,
    int level_sign, int readonly, int is_hidden, int is_filter2, int ColOnFilter2, int nStolbec,
    std::string font, int h, int is_bold, TQueryUni *q)
{
    TracerManager::Instance().StartFunctionTrace("TForm2Uslovie::doForm2Uslovie");
    TQueryUni *quPlanStr = new TQueryUni(DefDBconnect.Conn);
    quPlanStr->SQL->Add("SELECT * FROM Plan_str a, Pokaztl b");
    if (newCurrPlanId == 0)
        quPlanStr->SQL->Add("WHERE plan_id IN (1, " + IntToStr(FormLogicF1->CurrPlanId) + ")");
    else
        quPlanStr->SQL->Add("WHERE plan_id IN (1, " + IntToStr(newCurrPlanId) + ")");
    quPlanStr->SQL->Add("AND b.pokaz_name LIKE :patern");
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

    quPlanStr->SQL->Add("ORDER BY CASE WHEN plan_id > 1 THEN plan_str_order+400 WHEN plan_id = 1 and plan_str_order = 1000 THEN plan_str_order+10000 ELSE plan_str_order END");
    quPlanStr->ParamByName("patern")->AsString() = strName + "%";
    quPlanStr->Open();

    while (!quPlanStr->EoF())
    {
        std::string s = quPlanStr->FieldByName("pokaz_name")->AsString();
        if (nStolbec > 0)
            s += "#" + IntToStr(nStolbec) + "#";
        FormLogicF1->CurrInputRow = TForm2::AddNewRow(FormLogicF1, aOwnerNomRow, 0, 0);
        int RowVid = quPlanStr->FieldByName("pokaz_is_razd")->AsInteger();
        TFormDopInp::SetListRowsItemAtr(FormLogicF1, FormLogicF1->CurrInputRow, RowVid,
            quPlanStr->FieldByName("level_sign")->AsInteger(),
            quPlanStr->FieldByName("pokaz_vert")->AsInteger(),
            quPlanStr->FieldByName("pokaz_owner_id")->AsInteger(),
            quPlanStr->FieldByName("pokaz_id")->AsInteger(),
            quPlanStr->FieldByName("objekt_class_id")->AsInteger(),
            quPlanStr->FieldByName("pokaz_bk_code")->AsString());
        TFormDopInp::SetFormatStyle(FormLogicF1, FormLogicF1->CurrInputRow, TFormatStyle(font, h, is_bold, 0, 0));
        TForm2::SetTextNameCol(FormLogicF1, FormLogicF1->CurrInputRow, s, 0);

        if (is_hidden == 0)
        {
            if (FormLogicF1->quForms->FieldByName("is_sort")->AsInteger() == 1)
                TFormDopInp::SetInSort(FormLogicF1, FormLogicF1->CurrInputRow);
        }

        int CurrPokazId = quPlanStr->FieldByName("pokaz_id")->AsInteger();
        CurrObject.pokaz_id = 0;

        int wn = quPlanStr->FieldByName("plan_id")->AsInteger();
        if (wn == 1)
            wn = FormLogicF1->CurrPlanId;
        if (newCurrPlanId > 0)
            wn = newCurrPlanId;
        TDopAtrCol *DopAtrCol = nullptr;
        this->ExecPokaz(aOwnerNomRow, quPlanStr, wn, DopAtrCol, q->FieldByName("stmnt")->AsString());

        TFormatStyle* FormatStyle = new TFormatStyle();
        FormatStyle->setToNull();
        FormatStyle->font = font;
        FormatStyle->h = h;
        FormatStyle->is_bold = is_bold;
        if (q->FieldByName("attr_4")->AsInteger() > 0)
        {
            TForm1::SetCellValue(FormLogicF1, FormLogicF1->f1Forms, p->MaxHrCol, FormLogicF1->CurrInputRow,
                q->FieldByName("attr_4")->AsInteger(), FormLogicF1->CurrInputRow, q->FieldByName("attr_4")->AsInteger(),
                strName, 0, FormatStyle);
        }
//        TForm2 *tf = TForm2(this);
        TForm2::DoReadonlyHidden(FormLogicF1->CurrInputRow, FormLogicF1->CurrInputRow, readonly, is_hidden);
        TFormDopInp::SetToTerminal(FormLogicF1, FormLogicF1->CurrInputRow);
        quPlanStr->Next();
    }

    delete quPlanStr;
    TracerManager::Instance().EndFunctionTrace();
}