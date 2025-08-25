#include "../SystFuncs/SystFuncsLogic.h"
#include "../DB/unDefDBconnect.h"
#include "../Form/unProgress.h"
#include "../DB/DB_FormStr.h"
#include "../DB/DB_PlanStr.h"
#include "FormDopInp.h"
#include "Form2Plus.h"
#include "Form2F1.h"
#include "FormSsilkaNaStr.h"
#include "../DB/unDBType.h"
#include "../calc/unIspolGrupu_AlbumLogic.h"
#include "Form2Prisvoit.h"
#include "Form2SelectCond.h"
#include "Form2Ierarx.h"
#include "../DB/unDBType.h"

// выбор подчиненных объектов, заданных в описании формы посредством иерархии
void TForm2Ierarx::SelectObjectsIerarx(bool is_sort, int aOwnerNomRow, std::string OwnerLevelText, 
    std::string VertStr, TQueryUni* prOwnerPlanStr,//{здесь в текущй записи - текущий корневой объект или часть} 
    std::string classIdlist, TExecuteSxemaIerarx& ExSxIerarx, 
    int NOwner,//{номер владельца, по подчиненным узлам которого построен список classIdlist} 
    int NSubIerarx1,//{номер первого подчиненного из classIdlist} 
    PDopAtrCol aDopAtrCol)
 {
    TQueryUni * quPlanStr;
    bool bBold, bItalic;
    std::string LevelText;
    short LevelType;
    bool IsExecPokaz;
    int wwCurrInputRow;
    int newCurrPlanId;
    int NOwnerNew;
    std::string classIdlistNew;
    std::vector<int> planIds;
    int countPlanIds, i;

    newCurrPlanId = FormLogicF1->quForms->FieldByName("plan_id")->AsInteger(); // 28.01.2019
    if (newCurrPlanId == 0 || newCurrPlanId == 1) {
        if (aDopAtrCol->is_planId) {
            newCurrPlanId = aDopAtrCol->plan_id;
        }
    }
    if (newCurrPlanId == 0 || newCurrPlanId == 1) {
        newCurrPlanId = p->plan_id; // 11.12.2023
    }

    countPlanIds = 0;
    if (newCurrPlanId == 0) {
        countPlanIds = 1;
        planIds.resize(countPlanIds);
        planIds[0] = 0;
    } else if (newCurrPlanId == -1) {
        TIspolGrupu_AlbumLogic::evalPlanIds(FormLogicF1->CurrAlbumId, countPlanIds, planIds);
    } else {
        countPlanIds = 1;
        planIds.resize(countPlanIds);
        planIds[0] = newCurrPlanId;
    }


    for (i = 0; i < countPlanIds; i++) {
        CurrPokazId = 0;
        CurrObject.pokaz_id = prOwnerPlanStr->FieldByName("pokaz_id")->AsInteger();
        CurrObject.pokaz_is_razd = prOwnerPlanStr->FieldByName("pokaz_is_razd")->AsInteger();

        SelectObjectsIerarxDalee(is_sort, aOwnerNomRow, OwnerLevelText, VertStr, prOwnerPlanStr, classIdlist, ExSxIerarx, NOwner, NSubIerarx1, aDopAtrCol, planIds[i]);
    }
}

void TForm2Ierarx::SelectObjectsIerarxDalee(bool is_sort, int aOwnerNomRow, std::string OwnerLevelText,
                                            std::string VertStr, TQueryUni* prOwnerPlanStr, std::string classIdlist, TExecuteSxemaIerarx& ExSxIerarx,
                                            int NOwner, int NSubIerarx1, PDopAtrCol aDopAtrCol, int aNewCurrPlanId)
{
    bool bBold, bItalic;
    std::string LevelText;
    short LevelType;
    bool IsExecPokaz;
    int wwCurrInputRow;
    int newCurrPlanId;
    int NIerarxNew1, NOwnerNew;
    std::string classIdlistNew, aAndWhere;
    int wYear, wPlanId;

    p->ParamClassIdlist = classIdlist;
    TQueryUni *quPlanStr = new TQueryUni(DefDBconnect.Conn);
    wYear = FormLogicF1->CurrYear; // 11.01.2023
    if (aDopAtrCol->is_UstGoda)
    {
        switch (aDopAtrCol->is_set_god)
        {
        case 0: // наследуется
            wYear += aDopAtrCol->god_plus;
            break;
        case 1: // задается
            wYear = aDopAtrCol->god_ravno;
            break;
        }
    }

    if (ExSxIerarx.IerarxS[NSubIerarx1].is_link == 0) // подчинение
    {
        quPlanStr->SQL->Add("SELECT * FROM Plan_str a, Pokaztl b");
        if (FormLogicF1->CurrPlanId > 0)
            quPlanStr->SQL->Add("WHERE plan_id IN (1, " + std::to_string(FormLogicF1->CurrPlanId) + ")");
        else
        {
            if (aNewCurrPlanId == 0)
                newCurrPlanId = TDB_PlanStr::getPlanidByPokazid(CurrObject.pokaz_id);
            else
                newCurrPlanId = aNewCurrPlanId;

            aDopAtrCol->is_planId = true;
            aDopAtrCol->plan_id = newCurrPlanId; // 18 февр 2018
            quPlanStr->SQL->Add("WHERE plan_id IN (1, " + std::to_string(newCurrPlanId) + ")");
        }
        quPlanStr->SQL->Add("AND plan_str_owner_id = " + std::to_string(CurrObject.pokaz_id)); // с показателями (часть 30)
        quPlanStr->SQL->Add("AND a.pokaz_id = b.pokaz_id");
        switch (GetDBType())
        {
        case TDBType::MSSQL:
            quPlanStr->SQL->Add("AND " + std::to_string(wYear) + " BETWEEN YEAR(b.pokaz_beg_date) AND YEAR(b.pokaz_end_date)");
            break;
        case TDBType::Postgre:
            quPlanStr->SQL->Add("AND " + std::to_string(wYear) + " BETWEEN date_part('year', b.pokaz_beg_date) AND date_part('year', b.pokaz_end_date)");
            break;
        }
        quPlanStr->SQL->Add("AND (pokaz_is_razd = 1");
        quPlanStr->SQL->Add("   OR (pokaz_is_razd in (4, 5)");
        quPlanStr->SQL->Add("      AND objekt_class_id IN " + classIdlist);
        quPlanStr->SQL->Add("))");

        if (ExSxIerarx.IerarxS[NSubIerarx1].is_filter2 == 6 || ExSxIerarx.IerarxS[NSubIerarx1].is_filter2 == 7) {
            // Проверяем условие по полю 'work_with'
            int workWith = FormLogicF1->quForms->FieldByName("work_with")->AsInteger();
            if ((workWith == 1 || workWith == 3) && p->plan_id > 1) {
                wPlanId = p->plan_id;
            } else {
                wPlanId = FormLogicF1->CurrPlanId;
            }

            // Проверка дополнительного атрибута plan_id
            if (aDopAtrCol->is_planId) {
                wPlanId = aDopAtrCol->plan_id; // Обновлено 09.02.2020
            }

            // Вызов диалога или функции выбора с условиями
            if (TForm2SelectCond(this).doForm2SelectCond(wPlanId, aAndWhere)) {
                quPlanStr->SQL->Add(aAndWhere);
            }
        }

        quPlanStr->SQL->Add("ORDER BY CASE WHEN plan_id > 1 THEN plan_str_order+400 WHEN plan_id = 1 and plan_str_order = 1000 THEN plan_str_order+10000 ELSE plan_str_order END");
    }
    else // связь
    {
        quPlanStr->SQL->Add("SELECT * FROM Cell_value, Pokaztl");
        quPlanStr->SQL->Add("WHERE cell_id = :cell_id AND plan_id = :plan_id");
        quPlanStr->SQL->Add("AND vid_otc = :vid_otc AND value_date  = :value_date");
        quPlanStr->SQL->Add("AND objectid = :objectid AND Cell_value.pokaz_id  = :pokaz_id");
        quPlanStr->SQL->Add("AND Pokaztl.pokaz_id = CAST(Cell_value.value AS int)");
        quPlanStr->ParamByName("cell_id")->AsInteger() = 1;
        quPlanStr->ParamByName("plan_id")->AsInteger() = FormLogicF1->CurrPlanId;
        quPlanStr->ParamByName("vid_otc")->AsInteger() = FormLogicF1->CurrVidOtch;
        quPlanStr->ParamByName("value_date")->AsDateTime() = EncodeDate(FormLogicF1->CurrYear, 12, 31);
        quPlanStr->ParamByName("objectid")->AsInteger() = CurrObject.pokaz_id;
        quPlanStr->ParamByName("pokaz_id")->AsInteger() = ExSxIerarx.IerarxS[NSubIerarx1].field_id;
    }
    try
    {
        quPlanStr->Open();
        if (!VertStr.empty())
        {
            FormLogicF1->CurrInputRow = TForm2::AddNewRow(FormLogicF1, aOwnerNomRow, quPlanStr->FieldByName("pokaz_owner_id")->AsInteger(), 
               quPlanStr->FieldByName("pokaz_id")->AsInteger());
            TForm2::SetTextNameCol(FormLogicF1, FormLogicF1->CurrInputRow, VertStr, 0);
        }
        if (!quPlanStr->EoF())
        {
            //TunProgress::doProcessMessages();
            while (!quPlanStr->EoF())
            {
                CurrObject.pokaz_id = quPlanStr->FieldByName("pokaz_id")->AsInteger(); // это часть или объект
                CurrObject.pokaz_is_razd = quPlanStr->FieldByName("pokaz_is_razd")->AsInteger();
                LevelType = quPlanStr->FieldByName("level_sign")->AsInteger();
                LevelText = quPlanStr->FieldByName("level_text")->AsString();
                //  здесь был блок установки PosLevelText - 19 сент 2011
                if ( Pos(".", OwnerLevelText) < 0 && Pos(".", LevelText) >= 0 &&
                   (quPlanStr->FieldByName("pokaz_is_razd")->AsInteger() == 2))
                {
                    //!!bBold = true;
                    //!!bItalic = true;
                }
                else
                {
                    bBold = false;
                    bItalic = false;
                }
                if (LevelType == 3) // ' - '
                    bItalic = true;
                if (LevelType == 1)
                    LevelText = OwnerLevelText + LevelText;
                VertStr = quPlanStr->FieldByName("vert_str")->AsString();

                IsExecPokaz = false;
                if (quPlanStr->FieldByName("pokaz_is_razd")->AsInteger() > 0)
                {
                    if (quPlanStr->FieldByName("pokaz_is_razd")->AsInteger() != 2)
                        ; //!!bBold = true;
                    if (bItalic)
                        bBold = false;
                    if (int N = quPlanStr->FieldByName("pokaz_is_razd")->AsInteger(); N == 1 || N == 5)
                    {
                        bBold = true;
                        bItalic = false;
                        if (quPlanStr->FieldByName("pokaz_is_razd")->AsInteger() == 5)
                            bItalic = true;
                    }
                    // FormShowF1.fmProgress.SetMessage(quPlanStr->FieldByName("pokaz_name")->AsString());

                   // ---------------------------------------------------------
                   // Пока предполагаю, что в иерархии разделов нет
                   // поэтому случай с FieldByName('pokaz_is_razd')= 1 пока исключаю
                   // ---------------------------------------------------------
                    if (ExSxIerarx.IerarxS[NSubIerarx1].is_ForObject == 0)
                    {
                        NOwnerNew = TForm2Ierarx::evalNOwnerNew(ExSxIerarx, NOwner, quPlanStr->FieldByName("objekt_class_id")->AsInteger());
                        // Примечание: NOwnerNew есть номер класса текущего (выбранного) объекта

                        // ВСТАВЛЯЕМ СТРОКИ Перед Объектами класса

                        // генерируем строки для объекта класса
                        strokiForClassObject(is_sort, aOwnerNomRow, quPlanStr, ExSxIerarx, NOwnerNew, aDopAtrCol);
                        
                        // ВСТАВЛЯЕМ СТРОКИ После Объектов класса
                        
                        IsExecPokaz = true; // вставили объект
                    }
                }

                if (IsExecPokaz)
                {
                    wwCurrInputRow = FormLogicF1->CurrInputRow;
                    // вычисляем новый NOwner как NOwnerNew
                    // NOwnerNew вычислен выше

                    // Строим список classIdlistNew
                    classIdlistNew = TForm2Ierarx::createClassIdlist(ExSxIerarx, NOwnerNew, NIerarxNew1);
                    if (!classIdlistNew.empty())
                    {
                        SelectObjectsIerarx(is_sort, wwCurrInputRow, LevelText, VertStr, quPlanStr, classIdlistNew, ExSxIerarx, NOwnerNew, NIerarxNew1, aDopAtrCol);
                        VertStr = ""; // чтобы исключить вывод для новых подчиненных
                    }
                }
                else // строку не вставили, поэтому владелец остается прежним - ???????
                {
                    // вычисляем новый NOwner как NOwnerNew
                    NOwnerNew = TForm2Ierarx::evalNOwnerNew(ExSxIerarx, NOwner, quPlanStr->FieldByName("objekt_class_id")->AsInteger());
                    // Строим список classIdlistNew
                    classIdlistNew = TForm2Ierarx::createClassIdlist(ExSxIerarx, NOwnerNew, NIerarxNew1);
                    if (!classIdlistNew.empty())
                    {
                        SelectObjectsIerarx(is_sort, aOwnerNomRow, LevelText, "", quPlanStr, classIdlistNew, ExSxIerarx, NOwnerNew, NIerarxNew1, aDopAtrCol);
                    }
                }
                quPlanStr->Next();
            }
        }
        else
        {
            TFormDopInp::SetToTerminal(FormLogicF1, FormLogicF1->CurrInputRow);
        }
    }
    catch (...)
    {
        FormLogicF1->FormDM->quValue->Close();
        FormLogicF1->quCell->Close();
        quPlanStr->Close();
        throw std::runtime_error("Не идентифицированная ошибка");
    }
    quPlanStr->Close();
    delete(quPlanStr);
}

// генерируем строки для объекта класса
void TForm2Ierarx::strokiForClassObject(bool is_sort, int aOwnerNomRow,
        TQueryUni* quPlanStr,// {здесь в текущй записи - текущий корневой объект или часть}; 
        TExecuteSxemaIerarx& ExSxIerarx, int NSelClass, PDopAtrCol aDopAtrCol)
{
    int i, ist_fin_grad_id;
    TStringList *sl = nullptr;
    PAtrRow AtrRow = nullptr;

    if (ExSxIerarx.IerarxS[NSelClass].regimGenerac == 0) {
        // режим генерации одной строки
        FormLogicF1->CurrInputRow = TForm2::AddNewRow(FormLogicF1, aOwnerNomRow,
                          quPlanStr->FieldByName("pokaz_owner_id")->AsInteger(), // OwnerId
                          quPlanStr->FieldByName("pokaz_id")->AsInteger());
        AtrRow = TFormDopInp::GetListRowsItem(FormLogicF1, FormLogicF1->CurrInputRow);
        AtrRow->isReadOnly = false; // 28.12.2018

        if (ExSxIerarx.IerarxS[NSelClass].is_hidden == 1) {
            FormLogicF1->f1Forms->RowHidden[FormLogicF1->CurrInputRow] = true;
            AtrRow->isRowHidden = 1;
        }

        TForm2::SetTextNameCol(FormLogicF1, FormLogicF1->CurrInputRow, quPlanStr->FieldByName("pokaz_name")->AsString(),
                          quPlanStr->FieldByName("edizm_id")->AsInteger());
        TFormDopInp::SetListRowsItemAtr(FormLogicF1, FormLogicF1->CurrInputRow, quPlanStr->FieldByName("pokaz_is_razd")->AsInteger(),
                          quPlanStr->FieldByName("level_sign")->AsInteger(),
                          quPlanStr->FieldByName("pokaz_vert")->AsInteger(),
                          quPlanStr->FieldByName("pokaz_owner_id")->AsInteger(),
                          quPlanStr->FieldByName("pokaz_id")->AsInteger(),
                          quPlanStr->FieldByName("objekt_class_id")->AsInteger(),
                          quPlanStr->FieldByName("pokaz_bk_code")->AsString());
        TFormDopInp::SetFilter2OnRow(FormLogicF1, FormLogicF1->CurrInputRow,
           ExSxIerarx.IerarxS[NSelClass].is_filter2, ExSxIerarx.IerarxS[NSelClass].ColOnFilter2, 0);
        TFormDopInp::SetDopAtrToAtrRow(FormLogicF1, FormLogicF1->CurrInputRow, aDopAtrCol);
        if (is_sort)
            TFormDopInp::SetInSort(FormLogicF1, FormLogicF1->CurrInputRow);
        ExecPokaz(aOwnerNomRow, quPlanStr, 0, aDopAtrCol, "");
    } else {
        sl->Text = ExSxIerarx.IerarxS[NSelClass].idsCikl;
        for (i = 0; i < sl->Count(); i++) {
            FormLogicF1->CurrInputRow = TForm2::AddNewRow(FormLogicF1, aOwnerNomRow,
                quPlanStr->FieldByName("pokaz_owner_id")->AsInteger(), // OwnerId
                quPlanStr->FieldByName("pokaz_id")->AsInteger());
            TForm2::SetTextNameCol(FormLogicF1, FormLogicF1->CurrInputRow, quPlanStr->FieldByName("pokaz_name")->AsString(),
                        quPlanStr->FieldByName("edizm_id")->AsInteger());

            if (ExSxIerarx.IerarxS[NSelClass].is_hidden == 1) {
                FormLogicF1->f1Forms->RowHidden[FormLogicF1->CurrInputRow] = true;
                AtrRow->isRowHidden = 1;
            }

            TFormDopInp::SetFilter2OnRow(FormLogicF1, FormLogicF1->CurrInputRow,
                ExSxIerarx.IerarxS[NSelClass].is_filter2, ExSxIerarx.IerarxS[NSelClass].ColOnFilter2, 0);

            TFormDopInp::SetListRowsItemAtr(FormLogicF1, FormLogicF1->CurrInputRow,
                quPlanStr->FieldByName("pokaz_is_razd")->AsInteger(),
                quPlanStr->FieldByName("level_sign")->AsInteger(),
                quPlanStr->FieldByName("pokaz_vert")->AsInteger(),
                quPlanStr->FieldByName("pokaz_owner_id")->AsInteger(),
                quPlanStr->FieldByName("pokaz_id")->AsInteger(),
                quPlanStr->FieldByName("objekt_class_id")->AsInteger(),
                quPlanStr->FieldByName("pokaz_bk_code")->AsString());

            aDopAtrCol->SetDopAtrColNull();
            ist_fin_grad_id = std::stoi(sl->Strings[i]);
            aDopAtrCol->is_UstIstFin = true;
            aDopAtrCol->ist_fin_grad_id = ist_fin_grad_id;

            ExecPokaz(aOwnerNomRow, quPlanStr, 0, aDopAtrCol, "");
        }
    }
}

// Подготовить Необходимые данные: исполнительную схему иерархии классов и список id подчиненных классов
void TForm2Ierarx::PrepareData(TForm2* Form2, TFormLogicF1* FormLogicF1, unsigned char IsParamForm, int ParamObjectId, TExecuteSxemaIerarx& ExSxIerarx,
                               std::string& classIdlist, int& NSubIerarx1, int nBlockIerarxii)
{
    // загрузить в ExSxIerarx исполнительную схему иерархии классов

    TForm2Ierarx::LoadExecuteSxemaIerarx(ExSxIerarx, FormLogicF1->quForms->FieldByName("class_id")->AsInteger(), FormLogicF1,
                                         FormLogicF1->FormDM->quFormStr->params[0].AsInteger(), nBlockIerarxii);
    FormLogicF1->FormDM->quPokaz->Close();
    FormLogicF1->FormDM->quPokaz->ParamByName("God")->AsInteger() = FormLogicF1->CurrYear;
    FormLogicF1->FormDM->quPokaz->ParamByName("PokazID")->AsInteger() = FormLogicF1->quForms->FieldByName("class_id")->AsInteger();
    FormLogicF1->FormDM->quPokaz->Open();
    switch (FormLogicF1->FormDM->quPokaz->FieldByName("pokaz_type")->AsInteger()) {
    case 2:
    case 32:
        // владелец - часть для хранения объектов
        Form2->RootPokazId = FormLogicF1->quForms->FieldByName("class_id")->AsInteger();
        Form2->SelectAllField(); // Prepares and executes quFormColmn for all fields-columns
        break;
    case 1:
    case 31:
        // владелец - класс объектов
        if (IsParamForm == 1) {
            Form2->RootPokazId = ParamObjectId;
            FormLogicF1->FormDM->quPokaz->Close();
            FormLogicF1->FormDM->quPokaz->ParamByName("PokazID")->AsInteger() = Form2->RootPokazId;
            FormLogicF1->FormDM->quPokaz->Open();
            Form2->SelectAllField(); // готовит и выполняет quFormColmn по всем полям-столбцам
        } else {
            FormLogicF1->FormDM->quPokaz->Close();
            FormLogicF1->FormDM->quPokaz->ParamByName("PokazID")->AsInteger() = Form2->RootPokazId;
            FormLogicF1->FormDM->quPokaz->Open();
            Form2->SelectAllField(); // готовит и выполняет quFormColmn по всем полям-столбцам
        }
        break;
    }
    // Bпостроить список id подчиненных классов заданному владельцу - это 1
    classIdlist = TForm2Ierarx::createClassIdlist(ExSxIerarx, 1, NSubIerarx1); // This is a comma-separated list of IDs of subordinate classes
}

// загрузить исполнительную схему иерархии классов
void Load1rowStmnt(PRowStmnt rowStmnt, TQueryUni * qFstr) 
{
    rowStmnt->form_id = qFstr->FieldByName("form_id")->AsInteger();
    rowStmnt->form_str_n = qFstr->FieldByName("form_str_n")->AsInteger();
    rowStmnt->stmnt = qFstr->FieldByName("Stmnt")->AsString();
    rowStmnt->nStolbec = qFstr->FieldByName("attr_2")->AsInteger();
    rowStmnt->strName = qFstr->FieldByName("note1")->AsString();
    rowStmnt->note2 = qFstr->FieldByName("note2")->AsString();
    rowStmnt->Plan_Id = qFstr->FieldByName("plan_id")->AsInteger();
    rowStmnt->readonly = qFstr->FieldByName("readonly")->AsInteger();
    rowStmnt->is_hidden = qFstr->FieldByName("is_hidden")->AsInteger();
    rowStmnt->is_link = qFstr->FieldByName("is_group")->AsInteger();
    rowStmnt->is_ForObject = qFstr->FieldByName("isGroupSelect")->AsInteger();
    rowStmnt->tocnost = qFstr->FieldByName("tocnost")->AsInteger();
    rowStmnt->isMergeVert = qFstr->FieldByName("is_sort")->AsInteger();
    rowStmnt->isMergeHoriz = qFstr->FieldByName("uroven_sort")->AsInteger();
    rowStmnt->is_filter2 = qFstr->FieldByName("is_filter2")->AsInteger();
    rowStmnt->ColOnFilter2 = qFstr->FieldByName("ColOnFilter2")->AsInteger();
    rowStmnt->font = qFstr->FieldByName("font")->AsString();
    rowStmnt->h = qFstr->FieldByName("h")->AsInteger();
    rowStmnt->is_bold = qFstr->FieldByName("is_bold")->AsInteger();
    rowStmnt->is_set_god = qFstr->FieldByName("is_set_god")->AsInteger();
    rowStmnt->god_plus = qFstr->FieldByName("god_plus")->AsInteger();
    rowStmnt->god_ravno = qFstr->FieldByName("god_ravno")->AsInteger();
    rowStmnt->Period = qFstr->FieldByName("Period")->AsInteger();
    rowStmnt->ValuePeriod = qFstr->FieldByName("ValuePeriod")->AsInteger();
    rowStmnt->is_ist_fin = qFstr->FieldByName("is_ist_fin")->AsInteger();
    rowStmnt->ist_fin_grad_id = qFstr->FieldByName("ist_fin_grad_id")->AsInteger();
}

int SelectItem(TExecuteSxemaIerarx& ExSxIerarx, int class_id_from, int level) {
    for (int i = 1; i < 50; i++) {
        if ((ExSxIerarx.IerarxS[i].class_id == class_id_from) && (ExSxIerarx.IerarxS[i].level == level)) {
            return i;
        }
        if (ExSxIerarx.IerarxS[i].class_id == 0) {
            return 0;
        }
    }
    return 0;
}

int Skolko(TQueryUni * qFstr, int aNatur_vid, int nBlockIerarxii) {
    int nW = 0;
    qFstr->First();
    while (!qFstr->EoF()) {
        if ((qFstr->FieldByName("natur_vid")->AsInteger() == aNatur_vid) &&
            (qFstr->FieldByName("form_str_nSub")->AsInteger() == nBlockIerarxii)) {
            nW++;
        }
        qFstr->Next();
    }
    return nW;
}

void TForm2Ierarx::LoadExecuteSxemaIerarx(TExecuteSxemaIerarx& ExSxIerarx, int class_id, TFormLogicF1* FormLogicF1, int FormId, int nBlockIerarxii) {
    TQueryUni *qFstr = new TQueryUni(DefDBconnect.Conn);
    TDB_FormStr::GetItemsN1N2(qFstr, FormId, 1500, 9999);

    int natur_vid = -1; // до Иерархии объектов,
    int n = Skolko(qFstr, natur_vid, nBlockIerarxii);
    if (n > 0) {
        ExSxIerarx.PeredIerarx.resize(n);
        qFstr->First();
        n = -1;
        while (!qFstr->EoF()) {
            if ((qFstr->FieldByName("natur_vid")->AsInteger() == natur_vid) &&
                (qFstr->FieldByName("form_str_nSub")->AsInteger() == nBlockIerarxii)) {
                n++;
                Load1rowStmnt(&ExSxIerarx.PeredIerarx[n], qFstr);
            }
            qFstr->Next();
        }
    }

    natur_vid = 1; // после Иерархии объектов,
    n = Skolko(qFstr, natur_vid, nBlockIerarxii);
    if (n > 0) {
        ExSxIerarx.PosleIerarx.resize(n);
        qFstr->First();
        n = -1;
        while (!qFstr->EoF()) {
            if ((qFstr->FieldByName("natur_vid")->AsInteger() == natur_vid) &&
                (qFstr->FieldByName("form_str_nSub")->AsInteger() == nBlockIerarxii)) {
                n++;
                Load1rowStmnt(&ExSxIerarx.PosleIerarx[n], qFstr);
            }
            qFstr->Next();
        }
    }

    n = 1;
    ExSxIerarx.IerarxS[n].class_id = class_id;
    ExSxIerarx.IerarxS[n].owner = 0;
    ExSxIerarx.IerarxS[n].level = 0;
    ExSxIerarx.IerarxS[n + 1].class_id = 0; // признак конца

    qFstr->First();
    while (!qFstr->EoF()) {
        if (qFstr->FieldByName("stmnt").AsString() == "КЛАСС") {
            int i = SelectItem(ExSxIerarx, qFstr->FieldByName("ist_fin_grad_id")->AsInteger(), // {это class_id_from}
              qFstr->FieldByName("CoefPlus1")->AsInteger() - 1);//{это его Level}
            if (i > 0) { // эсть такой узел
                n++;
                ExSxIerarx.IerarxS[n].class_id = qFstr->FieldByName("plan_id")->AsInteger();
                ExSxIerarx.IerarxS[n].field_id = qFstr->FieldByName("field_id")->AsInteger();
                ExSxIerarx.IerarxS[n].is_link = qFstr->FieldByName("is_group")->AsInteger();
                ExSxIerarx.IerarxS[n].is_ForObject = qFstr->FieldByName("isGroupSelect")->AsInteger();
                ExSxIerarx.IerarxS[n].level = qFstr->FieldByName("CoefPlus1")->AsInteger();
                ExSxIerarx.IerarxS[n].form_str_n = qFstr->FieldByName("form_str_n")->AsInteger();
                ExSxIerarx.IerarxS[n].regimGenerac = qFstr->FieldByName("vid_uslovij")->AsInteger();
                ExSxIerarx.IerarxS[n].idsCikl = qFstr->FieldByName("shablon")->AsString();
                ExSxIerarx.IerarxS[n].isMergeVert = qFstr->FieldByName("is_sort")->AsInteger();
                ExSxIerarx.IerarxS[n].isNoOutOtsutstv = qFstr->FieldByName("is_Check")->AsInteger();
                ExSxIerarx.IerarxS[n].isMergeHoriz = qFstr->FieldByName("uroven_sort")->AsInteger();
                ExSxIerarx.IerarxS[n].is_filter2 = qFstr->FieldByName("is_filter2")->AsInteger();
                ExSxIerarx.IerarxS[n].ColOnFilter2 = qFstr->FieldByName("ColOnFilter2")->AsInteger();
                ExSxIerarx.IerarxS[n].is_hidden = qFstr->FieldByName("is_hidden")->AsInteger();
                ExSxIerarx.IerarxS[n].owner = i;
                ExSxIerarx.IerarxS[n + 1].class_id = 0;  // признак конца
            }
            else if (qFstr->FieldByName("stmnt")->AsString() == "ИЕРАРХИЯ") {
                // do nothing
            }
            else {
                throw std::runtime_error("В иерархии классов имеется ссылка на несуществующий класс");
            }
        }
        qFstr->Next();
    }

    for (int i = 1; i <= 50; i++) {
        if (ExSxIerarx.IerarxS[i].class_id == 0) {
            break;
        }
        int class_idW = ExSxIerarx.IerarxS[i].class_id;

        natur_vid = -class_idW;  // до объектов класса
        n = Skolko(qFstr, natur_vid, nBlockIerarxii);
        if (n > 0) {
            ExSxIerarx.IerarxS[i].PeredClass.resize(n);
            qFstr->First();
            n = -1;
            while (!qFstr->EoF()) {
                if ((qFstr->FieldByName("natur_vid")->AsInteger() == natur_vid) &&
                    (qFstr->FieldByName("form_str_nSub")->AsInteger() == nBlockIerarxii)) {
                    n++;
                    Load1rowStmnt(&ExSxIerarx.IerarxS[i].PeredClass[n], qFstr);
                }
                qFstr->Next();
            }
        }

        natur_vid = class_idW;  // после объектов класса,
        n = Skolko(qFstr, natur_vid, nBlockIerarxii);
        if (n > 0) {
            ExSxIerarx.IerarxS[i].PosleClass.resize(n);
            qFstr->First();
            n = -1;
            while (!qFstr->EoF()) {
                if ((qFstr->FieldByName("natur_vid")->AsInteger() == natur_vid) &&
                    (qFstr->FieldByName("form_str_nSub")->AsInteger() == nBlockIerarxii)) {
                    n++;
                    Load1rowStmnt(&ExSxIerarx.IerarxS[i].PosleClass[n], qFstr);
                }
                qFstr->Next();
            }
        }
    }
    delete(qFstr);
}

// генерируем строки Перед Иерархией
void TForm2Ierarx::strokiPeredIerarxii(int aOwnerNomRow, TExecuteSxemaIerarx& ExSxIerarx, PDopAtrCol aDopAtrCol) {
    for (int i = 0; i < ExSxIerarx.PeredIerarx.size(); i++) {
        PRowStmnt rowStmnt = &(ExSxIerarx.PeredIerarx[i]);
        outRowToF1(aOwnerNomRow, rowStmnt, aDopAtrCol);
    }
}

// генерируем строки после иерархии
void TForm2Ierarx::strokiPosleIerarxii(const int aOwnerNomRow, TExecuteSxemaIerarx& ExSxIerarx, PDopAtrCol aDopAtrCol) {
    for (int i = 0; i < ExSxIerarx.PosleIerarx.size(); i++) {
        PRowStmnt rowStmnt = &(ExSxIerarx.PosleIerarx[i]);
        outRowToF1(aOwnerNomRow, rowStmnt, aDopAtrCol);
    }
}

// выводим строку в F1
void TForm2Ierarx::outRowToF1(int aOwnerNomRow, PRowStmnt rowStmnt, PDopAtrCol aDopAtrCol) {
    std::string strName = rowStmnt->strName;
    int newPokazCol;
    TFormatStyle* FormatStyle = new TFormatStyle();

    if (rowStmnt->nStolbec > 0) {
         strName = strName+"#" + to_string(rowStmnt->nStolbec) + "#";
    }

    if (rowStmnt->stmnt == "ФОРМУЛАF1") {
        FormLogicF1->CurrInputRow = TForm2::AddNewRow(FormLogicF1, aOwnerNomRow, 0, 0);
        TFormDopInp::SetListRowsItemAtr(FormLogicF1, FormLogicF1->CurrInputRow, 11, rowStmnt->level_sign, 0, 0, 0, 0, "");
        TFormDopInp::SetFormatStyle(FormLogicF1, FormLogicF1->CurrInputRow, TFormatStyle(rowStmnt->font, rowStmnt->h, rowStmnt->is_bold, 0, 0));
        newPokazCol = TForm2::SetTextNameCol(FormLogicF1, FormLogicF1->CurrInputRow, strName, 0);

        //TunProgress::ShowText(FormLogicF1, strName);
        
        FormatStyle->Align_vert = rowStmnt->align_vert;
        FormatStyle->align_gor = rowStmnt->align_gor;
        FormatStyle->font = rowStmnt->font;
        FormatStyle->h = rowStmnt->h;
        FormatStyle->Is_bold = rowStmnt->is_bold;

        TForm2F1(this).SelectF1(newPokazCol, rowStmnt->note2, FormatStyle, rowStmnt->tocnost, rowStmnt->is_filter2, rowStmnt->ColOnFilter2, aDopAtrCol);
        TForm2::DoReadonlyHidden(FormLogicF1->CurrInputRow, FormLogicF1->CurrInputRow, rowStmnt->readonly, rowStmnt->is_hidden);
    }
    else if (rowStmnt->stmnt == "ПРОИЗВСТР") {
        FormLogicF1->CurrInputRow = TForm2::AddNewRow(FormLogicF1, aOwnerNomRow, 0, 0);
        TFormDopInp::SetListRowsItemAtr(FormLogicF1, FormLogicF1->CurrInputRow, 10, rowStmnt->level_sign, 0, 0, 0, 0, "");
        TFormDopInp::SetFormatStyle(FormLogicF1, FormLogicF1->CurrInputRow, TFormatStyle(rowStmnt->font, rowStmnt->h, rowStmnt->is_bold, 0, 0));
        TForm2::SetTextNameCol(FormLogicF1, FormLogicF1->CurrInputRow, strName, 0);
        TForm2::DoReadonlyHidden(FormLogicF1->CurrInputRow, FormLogicF1->CurrInputRow, rowStmnt->readonly, rowStmnt->is_hidden);
    }
    else if (rowStmnt->stmnt == "ССЫЛКАНАСТР") {
        TFormSsilkaNaStr::AddSsilkaNaStr(strName, FormLogicF1->CurrInputRow + 1);
    }
    else if (rowStmnt->stmnt == "ЗАМЕНА") {
        TForm2Plus::doZamenaInForm2(p, FormLogicF1, strName, rowStmnt->note2);
    }
    else if (rowStmnt->stmnt == "ПРИСВОИТЬ") {
        TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
        TDB_FormStr::GetItemsN1N2(q, rowStmnt->form_id, rowStmnt->form_str_n, rowStmnt->form_str_n);
        TForm2Prisvoit(this).doForm2Prisvoit(aOwnerNomRow, rowStmnt->Plan_Id, rowStmnt->strName, rowStmnt->note2,
                                            rowStmnt->level_sign, rowStmnt->Is_open, rowStmnt->readonly, rowStmnt->is_hidden, rowStmnt->is_filter2, rowStmnt->ColOnFilter2, rowStmnt->nStolbec,
                                            rowStmnt->font, rowStmnt->h, rowStmnt->is_bold, rowStmnt->align_vert, rowStmnt->align_gor, q, rowStmnt->is_ForObject);
    }
}

// построить список id подчиненных классов заданному владельцу
// NSubIerarx1 - номер первого подчиненного
std::string TForm2Ierarx::createClassIdlist(TExecuteSxemaIerarx& ExSxIerarx, int aOwner, int& NSubIerarx1) {
    std::string classIdlist = "";  // это список id подчиненных классов, разделенных запятой
    NSubIerarx1 = 0;
    for (int i = 0; i < 50; i++) {
        if (ExSxIerarx.IerarxS[i].class_id == 0) {
            break;
        }
        if (ExSxIerarx.IerarxS[i].owner == aOwner) {
            if (NSubIerarx1 == 0) {
                NSubIerarx1 = i;
            }
            if (!classIdlist.empty()) {
                classIdlist += ",";
            }
            classIdlist += std::to_string(ExSxIerarx.IerarxS[i].class_id);
        }
    }
    if (!classIdlist.empty()) {
        classIdlist = "(" + classIdlist + ")";
    }
    return classIdlist;
}

// вычисляем новый NOwner как NOwnerNew
int TForm2Ierarx::evalNOwnerNew(TExecuteSxemaIerarx& ExSxIerarx, int NOwner, int objekt_class_id) {
    int Result = 0;
    for (int i = 0; i < 50; i++) {
        if (ExSxIerarx.IerarxS[i].class_id == 0) {
            break;
        }
        if (ExSxIerarx.IerarxS[i].owner == NOwner && ExSxIerarx.IerarxS[i].class_id == objekt_class_id) {
            Result = i;
            return Result;
        }
    }
    return Result;
}
