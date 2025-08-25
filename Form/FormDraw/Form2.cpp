
#include "Form2.h"
#include "unDefDBconnect.h"
#include "Form1.h"
#include "DB_Pokaztl.h"
#include "Form2Ierarx.h"
#include "DB_FormStr.h"
#include "DB_TmpPokaztl.h"
#include "FormGF_Logic.h"
#include "DB_EdIzm.h"
#include "Form2Object.h"
#include "Form2PlusPatternKBK.h"
#include "Form2loadValue.h"
#include "unIDsClassFieldCell.h"
#include "DB_ComonNPokaz.h"
#include "DB_Cell.h"
#include "ClassesUtils.h"
#include "SystFuncsLogic.h"
#include "DB_bkMetaDef.h"
#include "DB_bkKBK.h"
#include "DB_Plan.h"
#include "Form2Korteg.h"
#include "DB_ClassofPart.h"
#include "Form2Owner1N.h"
#include "unProgress.h"
#include "Form2SelectCond.h"
#include "DB_FormStrTMP.h"
#include "FormSsilkaNaStr.h"
#include "Form2Agregat.h"
#include "Form2Uslovie.h"
#include "Form2Prisvoit.h"
#include "Form2Pologit.h"
#include "Form2PokazKorteg.h"
#include "Form2PokazOtlog.h"
#include "Form2F1.h"
#include "TT_Korteg.h"

//int TForm2::CurrPokazId = 0;
// если IsParamForm = 1, то для параметрической формы, если 0, то нет
// если IsParamForm = 3, то открыть повторно
// если IsParamForm = 2, то для одного объекта ParamObjectId: сгенерировать описания ячеек
// и ввести данные в строку nRowParamObjectId
// ParamObjectId содержит ид объекта конкретизации
// nRowParamObjectId - номер строки в форме, в которой расположен объект ParamObjectId
void TForm2::InitFormRows(TInfoFormLogic *InfoFormLogic, TFormLogicF1 *FormLogicF1, int IsParamForm, int ParamObjectId,
                          int nRowParamObjectId) {
    TForm2 vrForm2;
    vrForm2.InfoFormLogic = InfoFormLogic;
    vrForm2.FormLogicF1 = FormLogicF1;
    vrForm2.p = TFormDopInp::GetTww(FormLogicF1);
    vrForm2.IsParamForm = IsParamForm;
    vrForm2.ParamObjectId = ParamObjectId;
    vrForm2.nRowParamObjectId = nRowParamObjectId;
//    vrForm2.qDefPokazList = new TStringList();
    //vrForm2.qDefPokazList.Sorted = true;
//    vrForm2.slClassIds = new TStringList();
    //vrForm2.slClassIds.Sorted = true;
    vrForm2.doInitFormRows();

    for (int i = 0; i < vrForm2.qDefPokazList.Count(); i++) {
        TQueryUni *qDefPokaz = new TQueryUni(DefDBconnect.Conn);
        delete qDefPokaz;
    }
}

// Добавляет новую строку
// Увеличивает на 1 FormLogicF1.CurrInputRow
int TForm2::AddNewRow(TFormLogicF1 *FormLogicF1, int aOwnerNomRow, int OwnerId, int ObjectId) {
    TwwStringList *p = TFormDopInp::GetTww(FormLogicF1);
    int newEndRow = TF1toDE::CreateNewRowCells(p, FormLogicF1);
    int Result = newEndRow;

    TFormDopInp::ListRowsSetAtrRow(FormLogicF1, newEndRow, false, aOwnerNomRow, OwnerId, ObjectId);

    return Result;
}

void UpdFormatStyle(int aRzd1MergeN, std::string aRzd1font, int aRzd1h, int aRzd1is_bold, int aRzd1fontColor,
                    int aRzd1FonColor, int aRzd1VerAlign, int aRzd1HorAlign, TFormatStyle &FormatStyle, int &endCol) {
    if (aRzd1MergeN > 0)
        endCol += aRzd1MergeN - 1;
    if (!aRzd1font.empty())
        FormatStyle.font = aRzd1font;
    if (aRzd1h > 0)
        FormatStyle.h = aRzd1h;
    if (aRzd1is_bold > 0)
        FormatStyle.is_bold = aRzd1is_bold;
    if (aRzd1fontColor != 0)
        FormatStyle.FontColor = aRzd1fontColor;
    if (aRzd1FonColor != 0)
        FormatStyle.fonColor = aRzd1FonColor;
    if (aRzd1VerAlign > 0)
        FormatStyle.align_vert = aRzd1VerAlign;
    if (aRzd1HorAlign > 0)
        FormatStyle.align_gor = aRzd1HorAlign;
}

// заполняет колонку наименования показателя
// возвращает номер колонки с наименованием, стандартный или измененный
int TForm2::SetTextNameCol(TFormLogicF1 *FormLogicF1, int aRow, std::string Value, int edIzm_id) {
    TwwStringList *p = TFormDopInp::GetTww(FormLogicF1);
    int nL = Value.length();
    std::string s;
    int newPokazCol = 0;
    int code;
    if (nL > 0 && Value[nL - 1] == '#') {
        size_t n = Pos("#", Value);
        if (n != std::string::npos) { // Используем корректную проверку
            s = AnsiRightStr(Value, nL - n + 1);
            s = AnsiReplaceStr(s, "#", ""); // Заменяем '#' на пустую строку
            Val(s, newPokazCol, code);      //++
            Value = AnsiLeftStr(Value, n - 1);
        }
    }

    Value = Value + TFormDopInp::GetSSedIzmName(FormLogicF1, edIzm_id); // убрал s
    int PokazCol = TFormDopInp::GetNameCol(FormLogicF1);

    if (newPokazCol != 0) {
        PokazCol = newPokazCol; //++s
    }
    TFormatStyle *FormatStyle = new TFormatStyle();
    if (!TFormDopInp::getColNameFrmtrAtr(FormLogicF1, PokazCol, *FormatStyle)) {
        FormatStyle->setToNull();
        FormatStyle->align_vert = 1;
        FormatStyle->align_gor = 1;
    }

    int endCol = PokazCol;
    TStyleOforml *StyleOforml = p->StyleOforml;
    if (StyleOforml != NULL) {
        PAtrRow AtrRow = TFormDopInp::GetListRowsItem(FormLogicF1, aRow);
        if (AtrRow->CurRowVid == 1) {
            switch (AtrRow->NomUrovRazdela) {
                case 1:
                    UpdFormatStyle(StyleOforml->Rzd1MergeN, StyleOforml->Rzd1font, StyleOforml->Rzd1h,
                                   StyleOforml->Rzd1is_bold,
                                   StyleOforml->Rzd1fontColor, StyleOforml->Rzd1FonColor, StyleOforml->Rzd1VerAlign,
                                   StyleOforml->Rzd1HorAlign, *FormatStyle, endCol);
                    break;
                case 2:
                    UpdFormatStyle(StyleOforml->Rzd2MergeN, StyleOforml->Rzd2font, StyleOforml->Rzd2h,
                                   StyleOforml->Rzd2is_bold,
                                   StyleOforml->Rzd2fontColor, StyleOforml->Rzd2FonColor, StyleOforml->Rzd2VerAlign,
                                   StyleOforml->Rzd2HorAlign, *FormatStyle, endCol);
                    break;
                default:
                    UpdFormatStyle(StyleOforml->Rzd3MergeN, StyleOforml->Rzd3font, StyleOforml->Rzd3h,
                                   StyleOforml->Rzd3is_bold,
                                   StyleOforml->Rzd3fontColor, StyleOforml->Rzd3FonColor, StyleOforml->Rzd3VerAlign,
                                   StyleOforml->Rzd3HorAlign, *FormatStyle, endCol);
                    break;
            }
        }
        if (int N = AtrRow->CurRowVid; N == 3 || N == 5) {
            switch (AtrRow->NomUrovRazdela) {
                case 1:
                    UpdFormatStyle(StyleOforml->Itg1MergeN, StyleOforml->Itg1font, StyleOforml->Itg1h,
                                   StyleOforml->Itg1is_bold,
                                   StyleOforml->Itg1fontColor, StyleOforml->Itg1FonColor, StyleOforml->Itg1VerAlign,
                                   StyleOforml->Itg1HorAlign, *FormatStyle, endCol);
                    break;
                case 2:
                    UpdFormatStyle(StyleOforml->Itg2MergeN, StyleOforml->Itg2font, StyleOforml->Itg2h,
                                   StyleOforml->Itg2is_bold,
                                   StyleOforml->Itg2fontColor, StyleOforml->Itg2FonColor, StyleOforml->Itg2VerAlign,
                                   StyleOforml->Itg2HorAlign, *FormatStyle, endCol);
                    break;
                default:
                    UpdFormatStyle(StyleOforml->Itg3MergeN, StyleOforml->Itg3font, StyleOforml->Itg3h,
                                   StyleOforml->Itg3is_bold,
                                   StyleOforml->Itg3fontColor, StyleOforml->Itg3FonColor, StyleOforml->Itg3VerAlign,
                                   StyleOforml->Itg3HorAlign, *FormatStyle, endCol);
                    break;
            }
        }
        if (AtrRow->CurRowVid == 2 && StyleOforml->isVTomCisleKursiv && AtrRow->pokaz_vert != 1) {
            PAtrRow wAtrRow = TFormDopInp::GetListRowsItem(FormLogicF1, AtrRow->OwnerNomRow);
            if (wAtrRow != nullptr && wAtrRow->pokaz_vert == 1)
                FormatStyle->is_bold |= 0x02;
        }
    }

    TForm1::SetCellValue(FormLogicF1, FormLogicF1->f1Forms, p->MaxHrCol, aRow, PokazCol, aRow, endCol,
                         Value, 200, FormatStyle);
    delete FormatStyle;
    return PokazCol;
}

int TForm2::SetTextNameColVertStr(TFormLogicF1 *FormLogicF1, int aRow, std::string Value) {
    TwwStringList *p = TFormDopInp::GetTww(FormLogicF1);
    int PokazCol = TFormDopInp::GetNameCol(FormLogicF1);
    TFormatStyle *FormatStyle = new TFormatStyle();
    if (!TFormDopInp::getColNameFrmtrAtr(FormLogicF1, PokazCol, *FormatStyle)) {
        FormatStyle->setToNull();
        FormatStyle->align_vert = 1;
        FormatStyle->align_gor = 1;
    }

    TForm1::SetCellValue(FormLogicF1, FormLogicF1->f1Forms, p->MaxHrCol, aRow, PokazCol, aRow, PokazCol,
                         Value, 200, FormatStyle);
    return PokazCol;
}

void TForm2::doInitFormRows() {
    TracerManager::Instance().StartFunctionTrace("TForm2::doInitFormRows");
    int NSubIerarx1;
    bool isCreateOrderCP, isIerarhij, isObjPeredIer, is_sort;
    std::string classIdlist;
    TExecuteSxemaIerarx ExSxIerarx;
    int nBlockIerarxii;
    bool ifDO;
    TQueryUni *qFstr;

    TQueryUni *qSvjzClass = new TQueryUni(DefDBconnect.Conn);
    wDopAtrCol.SetDopAtrColNull();

    BeginRow = FormLogicF1->CurrInputRow;
    work_with = FormLogicF1->quForms->FieldByName("work_with")->AsInteger();
    // не аналитическая форма и не объектная форма (work_with=2)
    if (!(work_with == 2 || work_with == 10 || work_with == 11 || work_with == 12)) {
        if (IsParamForm != 2) {//{не работаем в уже открытой форме с отдельной строкой}
            TFormDopInp::CreateListStmnt(FormLogicF1);// создаем список операторов столбцов
        }
    }
    if (work_with != 2) {
        if (IsParamForm != 2) {//{не работаем в уже открытой форме с отдельной строкой}
            TForm2Plus::SetColHidden(FormLogicF1); // создаем столбцы невидимыми
        }
    }
    if (IsParamForm != 2) {//{не работаем в уже открытой форме с отдельной строкой}
        TFormDopInp::CreateListRows(FormLogicF1); // создает пустой список ListRows
    }
    RootPokazId = FormLogicF1->quForms->FieldByName("pokaz_id")->AsInteger();
    FormLogicF1->FormDM->quPokaz->ParamByName("PokazID")->AsInteger() = RootPokazId;
    FormLogicF1->FormDM->quPokaz->ParamByName("God")->AsInteger() = FormLogicF1->CurrYear;
    isOutPutIdRow = true;
    isSortedForm = false;
    switch (work_with) {
        case 0:
        case 1: {//{с показателями (часть 0, 30)}
            if (RootPokazId == 0) { // список строк
                if (FormLogicF1->quForms->FieldByName("is_sort")->AsInteger() > 0) {
                    SelectSpStrok(0, FormLogicF1->quForms->FieldByName("form_id")->AsInteger(), work_with, true,
                                  FormLogicF1->CurrInputRow);
                    isOutPutIdRow = false;
                    isSortedForm = true;
                } else {
                    SelectSpStrok(0, FormLogicF1->quForms->FieldByName("form_id")->AsInteger(), work_with, false,
                                  FormLogicF1->CurrInputRow);
                }
            }
            if (RootPokazId != 0) {// через владельца
                if (FormLogicF1->quForms->FieldByName("is_sort")->AsInteger() > 0) {
                    SelectSortsPokaz(FormLogicF1->CurrInputRow, FormLogicF1->FormDM->quPokaz, &wDopAtrCol, 0, 0, 0);
                    isOutPutIdRow = false;
                    isSortedForm = true;
                } else {
                    SelectOwnerPokaz(FormLogicF1->CurrInputRow, "", "", FormLogicF1->FormDM->quPokaz, false, 0, 0, 0,
                                     &wDopAtrCol, "", "", 4);
                }
            }
            break;
        }
        case 2: {//с одним объектом (часть 31)}
            RootPokazId = 0;
            if (FormLogicF1->quForms->FieldByName("pokaz_id")->AsInteger() > 0) {
                CurrObject.pokaz_id = FormLogicF1->quForms->FieldByName("pokaz_id")->AsInteger();
            } else {
                CurrObject.pokaz_id = ParamObjectId;
            }
            CurrObject.pokaz_is_razd = TDB_Pokaztl::GetPokazIsRazd(CurrObject.pokaz_id);
            TForm2Object(this).SelectSpField(FormLogicF1->CurrInputRow, &wDopAtrCol);
            isOutPutIdRow = false;
            break;
        }
        case 3:
        case 5: {//{со списком объектов (часть 32)}
            FormLogicF1->FormDM->quPokaz->Close();
            FormLogicF1->FormDM->quPokaz->ParamByName("PokazID")->AsInteger() = FormLogicF1->quForms->FieldByName(
                    "class_id")->AsInteger();
            FormLogicF1->FormDM->quPokaz->Open();
            switch (FormLogicF1->FormDM->quPokaz->FieldByName("pokaz_type")->AsInteger()) {
                case 2:
                case 32: {// {владелец - часть для хранения объектов}
                    RootPokazId = FormLogicF1->quForms->FieldByName("class_id")->AsInteger();
                    SelectAllField(); // готовит и выполняет quFormColmn по всем полям-столбцам
                    if (FormLogicF1->quForms->FieldByName("is_sort")->AsInteger() > 0) {
                        SelectSortsPokaz(FormLogicF1->CurrInputRow, FormLogicF1->FormDM->quPokaz, &wDopAtrCol, 0, 0, 0);
                        isOutPutIdRow = false;
                        isSortedForm = true;
                    } else {
                        SelectOwnerPokaz(FormLogicF1->CurrInputRow, "", "", FormLogicF1->FormDM->quPokaz, false, 0, 0,
                                         0, &wDopAtrCol, "", "", 4);
                    }
                    break;
                }
                case 1:
                case 31: {// {владелец - класс объектов}
                    if (IsParamForm == 1) {
                        RootPokazId = ParamObjectId;
                        FormLogicF1->FormDM->quPokaz->Close();
                        FormLogicF1->FormDM->quPokaz->ParamByName("PokazID")->AsInteger() = RootPokazId;
                        FormLogicF1->FormDM->quPokaz->Open();
                        SelectAllField(); // готовит и выполняет quFormColmn по всем полям-столбцам
                        SelectOwnerPokaz(FormLogicF1->CurrInputRow, "", "", FormLogicF1->FormDM->quPokaz, false, 0, 0,
                                         0, &wDopAtrCol, "", "", 4);
                    } else {
                        // это список строк
                        SelectAllField(); // готовит и выполняет quFormColmn по всем полям-столбцам
                        if (FormLogicF1->quForms->FieldByName("is_sort")->AsInteger() > 0) {
                            SelectSpStrok(0, FormLogicF1->quForms->FieldByName("form_id")->AsInteger(), work_with, true,
                                          FormLogicF1->CurrInputRow);
                            isOutPutIdRow = false;
                            isSortedForm = true;
                        } else {
                            SelectSpStrok(0, FormLogicF1->quForms->FieldByName("form_id")->AsInteger(), work_with,
                                          false, FormLogicF1->CurrInputRow);
                        }
                    }
                    break;
                }
            }
            break;
        }
        case 4: {//для иерархии объектов (часть 32)}
            nBlockIerarxii = 0;
            isIerarhij = false;
            isObjPeredIer = false;
            is_sort = (FormLogicF1->quForms->FieldByName("is_sort")->AsInteger() > 0);

            TQueryUni *qFstr = new TQueryUni(DefDBconnect.Conn);
            TDB_FormStr::GetItemsN1N2(qFstr, FormLogicF1->quForms->FieldByName("form_id")->AsInteger(), 1500, 9999);
            // сначала проверяем, это список иерархии или нет
            while (!qFstr->EoF()) {
                if (qFstr->FieldByName("stmnt")->AsString() == "ИЕРАРХИЯ") {
                    isIerarhij = true;
                    break;
                }
                if (qFstr->FieldByName("stmnt")->AsString() == "КЛАСС") {
                    isObjPeredIer = true;
                    break;
                }
                qFstr->Next();
            }
            if (isIerarhij && isObjPeredIer) {
                //throw Exception("В списке строк имеет место как единственная иерархия, так и список иерархий");
            }
            qFstr->First();
            while (!qFstr->EoF()) {
                classIdlist = "";
                ExSxIerarx.setNULL();
                wDopAtrCol.SetDopAtrColNull();
                if (qFstr->FieldByName("stmnt")->AsString() == "ИЕРАРХИЯ") {
                    nBlockIerarxii = qFstr->FieldByName("form_str_nSub")->AsInteger();
                    ifDO = true;
                    wDopAtrCol.SetDopAtrColQuery(qFstr);
                } else if (!isIerarhij && qFstr->FieldByName("stmnt")->AsString() == "КЛАСС") {
                    if (nBlockIerarxii == 0) {
                        ifDO = true;
                        nBlockIerarxii = -1; // чтобы не повторить
                    } else {
                        ifDO = false;
                    }
                }
                if (isIerarhij && qFstr->FieldByName("stmnt")->AsString() == "КЛАСС") {
                    ifDO = false;
                }
                if (ifDO) {
                    // загрузить в ExSxIerarx исполнительную схему иерархии классов
                    // построить список id подчиненных классов заданному владельцу - это 1
                    TForm2Ierarx::PrepareData(this, FormLogicF1, IsParamForm, ParamObjectId, ExSxIerarx, classIdlist,
                                              NSubIerarx1, nBlockIerarxii);
                    if (classIdlist != "") {
                        // ВСТАВЛЯЕМ СТРОКИ Перед Иерархией
                        TForm2Ierarx(this).strokiPeredIerarxii(FormLogicF1->CurrInputRow, ExSxIerarx, &wDopAtrCol);

                        // вставляем строки иерархии
                        SelectObjectsIerarx(is_sort, FormLogicF1->CurrInputRow, "", "",
                                            FormLogicF1->FormDM->quPokaz, classIdlist, ExSxIerarx, 1, NSubIerarx1,
                                            &wDopAtrCol);
                        // ВСТАВЛЯЕМ СТРОКИ После Иерархии
                        TForm2Ierarx(this).strokiPosleIerarxii(FormLogicF1->CurrInputRow, ExSxIerarx, &wDopAtrCol);
                    }
                }
                qFstr->Next();
            }
            delete qFstr;
            break;
        }
        case 10://{сравнительный анализ}
        case 11://{мониторинг}
        case 12: {//{идентификация тенденций}
            SelectSpStrok(0, FormLogicF1->quForms->FieldByName("form_id")->AsInteger(), work_with, false,
                          FormLogicF1->CurrInputRow);
            break;
        }
    }
    isCreateOrderCP = TDB_TmpPokaztl::DropAllTmpOrderCP(); // будут удалены все, если были созданы
    if (IsParamForm == 2) {
        TForm2loadValue::SetInputValue(FormLogicF1, p, true, nRowParamObjectId);
    } else {
        TForm2loadValue::SetInputValue(FormLogicF1, p, false, 0);
    }
    TForm2Plus::SetFactNoVvod(FormLogicF1); // установка "не чтения" для утвержд фактич данных
    if (isOutPutIdRow) {
        // в таблицу f1Forms перед наименованием прописывает идентификацию строки
        TForm2Plus::SetIdRow(FormLogicF1, TFormDopInp::GetNameCol(FormLogicF1), BeginRow);
    }
    switch (FormLogicF1->quForms->FieldByName("work_with_gf")->AsInteger()) {
        case 10:
        case 13:
        case 14: {// составление графика финансирования, уточнение к плану-графику финансирования
            TFormGF_Logic::DoForm10_13_14(InfoFormLogic, FormLogicF1,
                                          FormLogicF1->quForms->FieldByName("work_with_gf")->AsInteger());
            break;
        }
        case 11: {// фактическое финансирование
            TFormGF_Logic::DoForm11(FormLogicF1);
            break;
        }
        case 12: {// фактическое финансирование
            TFormGF_Logic::DoForm12(FormLogicF1);
            break;
        }
    }
    switch (p->is_dublir_itogo) {
        case 2:
        case 3: {//{скрыть пустые разделы}
            TForm2Plus::HiddenEmptyRazdel(FormLogicF1, p->is_dublir_itogo);
            break;
        }
    }
    if (isCreateOrderCP) {
        TForm2Plus::doSvodForCelPrgrm(FormLogicF1);
    }
    if (p->work_with != 2) {
        // на самом деле метод TForm2Plus.SetNumberPP - это есть исполнение оператора <№ п/п>
        TForm2Plus::SetNumberPP(FormLogicF1, p->form_begBodyRow, FormLogicF1->f1Forms->LastRow);
    }
    // здесь в таблицу F1Book добавляяется новая строка - свод в соотвествии с описанием формы
    if (FormLogicF1->quForms->FieldByName("is_svod")->AsInteger() == 1) {
        TForm2Plus::SvodForm(FormLogicF1);
    }
    delete qSvjzClass;
    TracerManager::Instance().EndFunctionTrace();
}

void TForm2::GetCoef_Pokaz(TQueryUni *aQPokaztl) {
    // это надо так и оставить, т.к. настройка идет через форм-ый параметр quPokaztl
    coef_Pokaz = TDB_EdIzm::GetCoef_PokazOpt(aQPokaztl->FieldByName("edizm_id")->AsInteger());
}

bool TForm2::GetNaturVid() {
    FormLogicF1->FormDM->quPokaztl->ParamByName("PokazId")->AsInteger() = PokazId;
    FormLogicF1->FormDM->quPokaztl->ParamByName("NPVIDId")->AsInteger() = FormLogicF1->FormDM->quFormStr->FieldByName(
            "natur_vid")->AsInteger();
    FormLogicF1->FormDM->quPokaztl->Open();
    if (!FormLogicF1->FormDM->quPokaztl->EoF()) {
        PokazId = FormLogicF1->FormDM->quPokaztl->FieldByName("pokaz_id")->AsInteger();
        PokazType = FormLogicF1->FormDM->quPokaztl->FieldByName("pokaz_type")->AsInteger();
        DataType = PokazType;
        EnumId = FormLogicF1->FormDM->quPokaztl->FieldByName("enum_id")->AsInteger();
        GetCoef_Pokaz(FormLogicF1->FormDM->quPokaztl);
        return true;
    } else {
        return false;
    }
    FormLogicF1->FormDM->quPokaztl->Close();
}

// готовит элемент в списке TCellValueAtr для значения показателя или вычисления
// параметр содержит описание показателя или характеристики

// ВАЖНО!!!!!!!!!!!!!
// AtrStmnt:PAtrStmnt и AtrRow:PAtrRow передаются параметром
// внутри метода их НЕ ВЫЧИСЛЯТЬ
// Для идентификации Range пользоваться только параметрами nRow: integer; nClmn: integer;

void
TForm2::SetValueData(int nRow, int nClmn, PAtrStmnt AtrStmnt, PAtrRow AtrRow, std::string aStmnt, TQueryUni *aQDefPokaz,
                     PDopAtrCol aDopAtrCol) {
    int Year, Month, Day;
    TDateTime PokazDate;
    PCellValueAtr vd, wvd, wwvd;
    int wcell_is_eval, wobjectid;
    PAtrRow wAtrRow;

    FormLogicF1->quCell->Params2 = IstFinGradId;
    FormLogicF1->quCell->Open();
    if (!FormLogicF1->quCell->EoF()) {
        DataType = PokazType;
        Value = "";
        ValueD = 0;
        ValueD2 = 0;
        PokazDate = EncodeDate(FormLogicF1->CurrYear, 1, 31);
        if (Kv > 0)
            PokazDate = IncMonth(PokazDate, Kv * 3 - 1);
        else if (Mes > 0) {
            if (Mes > 1)
                PokazDate = IncMonth(PokazDate, Mes - 1);
        } else
            PokazDate = EncodeDate(FormLogicF1->CurrYear, 12, 31);

        // добавление для столбцов с заданной датой
        if (aDopAtrCol->is_UstPeriod) {
            switch (aDopAtrCol->Period) {
                case 4: // квартал
                    if (aDopAtrCol->ValuePeriod > 0)
                        PokazDate = GetLastDayDate(FormLogicF1->CurrYear, aDopAtrCol->ValuePeriod, 0);
                    break;
                case 6: // месяц
                    if (aDopAtrCol->ValuePeriod > 0)
                        PokazDate = GetLastDayDate(FormLogicF1->CurrYear, 0, aDopAtrCol->ValuePeriod);
                    break;
            }
        } else {
            switch (period) {
                case 4: // квартал
                    if (ValuePeriod > 0)
                        PokazDate = GetLastDayDate(FormLogicF1->CurrYear, ValuePeriod, 0);
                    break;
                case 6: // месяц
                    if (ValuePeriod > 0)
                        PokazDate = GetLastDayDate(FormLogicF1->CurrYear, 0, ValuePeriod);
                    break;
            }
        }

        // обработка установки года
        if (aDopAtrCol->is_UstGoda) {
            switch (aDopAtrCol->is_set_god) {
                case 0: // наследуется
                    if (aDopAtrCol->god_plus != 0) {
                        DecodeDate(PokazDate, Year, Month, Day);
                        PokazDate = EncodeDate(FormLogicF1->CurrYear + aDopAtrCol->god_plus, Month, Day);
                    }
                    break;
                case 1: // задается
                    DecodeDate(PokazDate, Year, Month, Day);
                    PokazDate = EncodeDate(aDopAtrCol->god_ravno, Month, Day);
                    break;
            }
        } else {
            switch (AtrStmnt->is_set_god) {
                case 0: // наследуется
                    if (AtrStmnt->god_plus != 0) {
                        DecodeDate(PokazDate, Year, Month, Day);
                        PokazDate = EncodeDate(FormLogicF1->CurrYear + AtrStmnt->god_plus, Month, Day);
                    }
                    break;
                case 1: // задается
                    DecodeDate(PokazDate, Year, Month, Day);
                    PokazDate = EncodeDate(AtrStmnt->god_ravno, Month, Day);
                    break;
            }
        }

        // добавление для показателей с периодичностью Условно-постоянно
        if (FormLogicF1->quCell->cell_period == 0) {
            PokazDate = EncodeDate(FormLogicF1->CurrYear, 1, 2); // это изменение 18 марта 2011 // сделал на 2 число
        }

        // далее устанавливается DataType, чтобы показать, что ячейка вычисляется, а не просто читается
        if (aStmnt == "ЗНАЧЕНИЕСВ") {
            if (aQDefPokaz->FieldByName("pokaz_is_mes")->AsInteger() == 0 && PeriodType == 3 && PeriodType > PT)
                DataType = 11 + deltaDType; // расчетный показатель за месяц
        } else if (aStmnt == "ДатаСоздОбъекта") {
            DataType = 13 + deltaDType;
        } else if (aStmnt == "ЗНАЧЕНИЕСВНИ" || aStmnt == "ЗНАЧЕНИЕНВНИ") {
            DataType = 14 + deltaDType;
        } else if (aStmnt == "КБК-Дгвр:Сумма№БО" || aStmnt == "КБК-Дгвр:СуммаБО" || aStmnt == "КБК-Дгвр:Сумма№БОоз") {
            DataType = 16 + deltaDType;
        } else if (aStmnt == "ОТКЛОНЕНИЕСВНачКВ" || aStmnt == "ОТКЛОНЕНИЕНВНачКВ") {
            DataType = 17 + deltaDType;
        } else if (aStmnt == "ЗНАЧЕНИЕСВИР" || aStmnt == "ЗНАЧЕНИЕНВИР") {
            DataType = 18 + deltaDType;
        } else if (aStmnt == "УТОЧНЕНИЕСВ" || aStmnt == "УТОЧНЕНИЕСВНИ") {
            DataType = 19 + deltaDType;
        } else if (aStmnt == "Мрпр-Дгвр:Сумма№БО" || aStmnt == "Мрпр-Дгвр:СуммаБО") {
            DataType = 20 + deltaDType;
        } else if (aStmnt == "ПлнОСТАТОК") {
            DataType = 21 + deltaDType;
        } else if (aStmnt == "ФктОСТАТОК") {
            DataType = 22 + deltaDType;
        } else if (aStmnt == "ПлнОСТАТОКНИ") {
            DataType = 24 + deltaDType;
        } else if (aStmnt == "УТОЧНЕНИЕСВПосле") {
            DataType = 25 + deltaDType;
        } else if (aStmnt == "ФактВсегоВНачале") {
            DataType = 26 + deltaDType;
            DecodeDate(PokazDate, Year, Month, Day);
            switch (period) {
                case 4: // квартал
                    Kv = GetNKv(Month);
                    PokazDate = EncodeDate(Year, 1, 1);
                    PokazDate = IncMonth(PokazDate, Kv * 3 - 3);
                    break;
                case 6: // месяц
                    PokazDate = EncodeDate(Year, Month, 1);
                    break;
            }
        } else if (aStmnt == "СуммаПриход") {
            DataType = 27 + deltaDType;
        } else if (aStmnt == "СуммаВозврат") {
            DataType = 28 + deltaDType;
        } else if (aStmnt ==
                   "СуммаПлГрНИВНачале") { // семантика сейчас такая 'Сумма планов-гр. уже отправленных в течение квартала'
            DataType = 29 + deltaDType;
            DecodeDate(PokazDate, Year, Month, Day);
            switch (period) {
                case 4: // квартал
                    Kv = GetNKv(Month);
                    PokazDate = EncodeDate(Year, 1, 1);
                    PokazDate = IncMonth(PokazDate, Kv * 3 - 3);
                    break;
                case 6: // месяц
                    PokazDate = EncodeDate(Year, Month, 1);
                    break;
            }
        } else if (aStmnt == "СуммаПлГрНИГод") {
            DataType = 53 + deltaDType;
            DecodeDate(PokazDate, Year, Month, Day);
            switch (period) {
                case 4: // квартал
                    Kv = GetNKv(Month);
                    PokazDate = EncodeDate(Year, 1, 1);
                    PokazDate = IncMonth(PokazDate, Kv * 3 - 3);
                    break;
                case 6: // месяц
                    PokazDate = EncodeDate(Year, Month, 1);
                    break;
            }
        } else if (aStmnt == "ПДГ_УТ_КассПл") {
            DataType = 31 + deltaDType;
        } else if (aStmnt == "УТВ_УТ_КассПл") {
            DataType = 32 + deltaDType;
        } else if (aStmnt == "ПоступлВсегоВНачале") {
            DataType = 40 + deltaDType;
        } else if (aStmnt == "КБК-Дгвр:СуммаКЗ") {
            DataType = 41 + deltaDType;
        } else if (aStmnt == "ФактКвДату") {
            DataType = 44 + deltaDType;
        } else if (aStmnt == "СуммаПлГрНИВМесяце") {
            DataType = 45 + deltaDType;
        } else if (aStmnt == "УточПланВклДату") {
            DataType = 51 + deltaDType;
        } else if (aStmnt == "ХарактСВобъекта") {
            DataType = 55 + deltaDType;
            //В случае 'ХарактСВобъекта' pokaz_type не изменяется, остается как у характеристики связанного объекта
        } else if (aStmnt == "КонкатСтрок") {
            DataType = 56 + deltaDType;
        } else if (aStmnt == "КонкатСтрокСВобъек") {
            DataType = 57 + deltaDType;
            //В случае 'КонкатСтрокСВобъек' pokaz_type не изменяется, остается как у характеристики связанного объекта
        } else if (aStmnt == "Экономия/Резерв") {
            DataType = 58 + deltaDType;
        } else if (aStmnt == "отчетCO_RP_015") {
            DataType = 59 + deltaDType;
        } else if (aStmnt == "СведенияОбЭкономии") {
            DataType = 60 + deltaDType;
        } else if (aStmnt == "СВОДпоСУБЪвАльб" || aStmnt == "СВОДпоСУБЪвА/n") {
            DataType = 63 + deltaDType;
        }

        wcell_is_eval = FormLogicF1->quCell->cell_is_eval;

        if (aQDefPokaz->FieldByName("objekt_class_id")->AsInteger() > 0 &&
            aQDefPokaz->FieldByName("isSurgtName")->AsInteger() == 1)
            wcell_is_eval = 1;

        if (CurrObject.pokaz_id > 0)
            if (CurrObject.pokaz_is_razd == 5)
                wcell_is_eval = 1;

        if (aStmnt == "КонкатСтрок" || aStmnt == "КонкатСтрокСВобъек")
            wcell_is_eval = 1;

        wobjectid = CurrObject.pokaz_id;
        if (CurrObject.pokaz_is_razd == 2)
            wobjectid = 0;

        wvd = TFormDopInp::AddValueCellAtr(FormLogicF1, nRow, nClmn, FormLogicF1->quCell->cell_id, PokazDate, DataType,
                                           VidOtch, PlanId, coef_Pokaz, coef_Forma, wobjectid, PokazId, wcell_is_eval,
                                           PokazType, FormLogicF1->quCell->cell_period,
                                           FormLogicF1->quCell->ist_fin_id);

        if (wobjectid > 0 && DataType == 1) {
            if (FormLogicF1->FormDM->quFormColmn->FieldByName("pokaz_vsego_id")->AsInteger() != 0) {
                wvd->kakAgregirUpByField = FormLogicF1->FormDM->quFormColmn->FieldByName("pokaz_vsego_id")->AsInteger();
                if (TDB_Pokaztl::getTypeEntity(wobjectid) == 1)
                    wvd->kakAgregirUpByField = 0;

                if (wvd->kakAgregirUpByField == 8) {
                    AtrRow = TFormDopInp::GetListRowsItem(FormLogicF1, nRow);
                    wAtrRow = TFormDopInp::GetListRowsItem(FormLogicF1, AtrRow->OwnerNomRow);
                    if (wAtrRow != NULL && wAtrRow->CurRowVid == 2) {
                        wwvd = TFormDopInp::GetCellValueAtrByRC(FormLogicF1, wAtrRow->Row, nClmn);
                        if (wwvd != NULL) {
                            wwvd->EvalType = 1;
                            FormLogicF1->f1Forms->Selection_Set(wwvd->Row, wwvd->Col, wwvd->Row, wwvd->Col);
                            FormLogicF1->f1Forms->Protection_Set(true, true);
                            FormLogicF1->f1Forms->Selection_Set(wvd->Row, wvd->Col, wvd->Row, wvd->Col);
                        }
                    }
                }
            }
        }

        if (PokazType == 1 || PokazType == 10)
            TFormDopInp::setCellSizeFixed(FormLogicF1, nClmn);

        if (aStmnt == "ИСТЗНАЧВВОДПК") {
            vd = TFormDopInp::GetCellValueAtrByRC(FormLogicF1, nRow, nClmn);
            FormLogicF1->f1Forms->Protection_Set(true, true);
            vd->EvalType = 3;
            switch (p->modeNumber) {
                case 1:
                    vd->pokaz_type = 1;
                    vd->DataType = 1;
                    break;
                case 2:
                case 3:
                    vd->pokaz_type = 2;
                    vd->DataType = 2;
                    break;
            }
        }

        if (CurrObject.isSimvR)
            FormLogicF1->f1Forms->Protection_Set(true, true); // в TFormDopInp.AddValueCellAtr ячейка уже выбрана
        // установка ReadONly для характеристики агрегатора
        if (aQDefPokaz->FieldByName("pokaz_vsego_id")->AsInteger() == 4) {//{признак агрегатора}
            AtrRow = TFormDopInp::GetListRowsItem(FormLogicF1, nRow);
            if (AtrRow->CurRowVid == 4 && AtrRow->pokaz_vert == 1)//{в том числе}
                FormLogicF1->f1Forms->Protection_Set(true, true);
        }
        // End of -> установка ReadONly для характеристики агрегатора
        // спец обработка для столбца - бюджетополучаетля
        vd = TFormDopInp::GetCellValueAtrByRC(FormLogicF1, nRow, nClmn);
        if (PokazType == 4) {// пока сделал для натур
            if (vd != NULL)
                vd->EnumId = EnumId;
            EnumId = 0;
        } else if (PokazType == 7) {
            if (vd != NULL)
                vd->EnumId = aQDefPokaz->FieldByName("enum_id")->AsInteger(); // это id связанного Класса
        } else if (PokazType == 11) { // измерение
            if (vd != NULL)
                vd->EnumId = EnumId; // это id измерения
            EnumId = 0;
        }
        if (aStmnt == "ХарактСВобъекта" || aStmnt == "КонкатСтрокСВобъек") {
            if (vd != NULL) {
                vd->DPId = class_idXarakterSvjzObjecta; // в случае 'ХарактСВобъекта' это class_idXarakterSvjzObjecta - id связанного класса
            }
        }
        if (aQDefPokaz->FieldByName("objekt_class_id")->AsInteger() > 0) {
            vd = TFormDopInp::GetCellValueAtrByRC(FormLogicF1, nRow, nClmn);
            if (vd != NULL) {
                if (vd->DataType < 4 && vd->ObjectId > 0) {
                    vd->class_id_ForKey = aQDefPokaz->FieldByName("objekt_class_id")->AsInteger();
                }
            }
        }
    } else {
        // формула уже вставлена
    }
    FormLogicF1->quCell->Close();
}

void TForm2::SetValueDataForComNP(int nClmn, std::string aStmnt, PDopAtrCol aDopAtrCol) {
    int comnp_id;
    int16_t god;
    uint8_t kvrtl;
    uint8_t mesjc;
    int16_t den;
    int cell_is_eval;
    int16_t vid_period;
    int wperiod, wValuePeriod, wPlanId;
    std::string toTblName;
    PCellValueAtr vd;

    comnp_id = p->ListStmnt[nClmn].natur_vid; // comnp_id лежит в natur_vid
    god = FormLogicF1->CurrYear;
    kvrtl = 0;
    mesjc = 0;
    den = 0;
    wperiod = p->ListStmnt[nClmn].PeriodType;
    wValuePeriod = p->ListStmnt[nClmn].ValuePeriod;

    if (Kv > 0)
        kvrtl = Kv;
    if (Mes > 0)
        mesjc = Mes;
    if (wperiod == 1) {
        kvrtl = 0;
        mesjc = 0;
    }
    if (wperiod == 0) {// усл постоянное
        kvrtl = 0;
        mesjc = 0;
    }
    // добавление для столбцов с заданной датой
    if (aDopAtrCol->is_UstPeriod) {
        switch (aDopAtrCol->Period) {
            case 4:
                if (aDopAtrCol->ValuePeriod > 0)
                    kvrtl = aDopAtrCol->ValuePeriod;
                break;
            case 6:
                if (aDopAtrCol->ValuePeriod > 0)
                    mesjc = aDopAtrCol->ValuePeriod;
                break;
        }
    } else {
        switch (wperiod) {
            case 4:
                if (wValuePeriod > 0)
                    kvrtl = wValuePeriod;
                else
                    kvrtl = FormLogicF1->CurrQuarter;
                break;
            case 6:
                if (wValuePeriod > 0)
                    mesjc = wValuePeriod;
                else
                    mesjc = FormLogicF1->CurrMonth;
                break;
        }
    }
    // обработка установки года
    if (aDopAtrCol->is_UstGoda) {
        switch (aDopAtrCol->is_set_god) {
            case 0:
                if (aDopAtrCol->god_plus != 0)
                    god = FormLogicF1->CurrYear + aDopAtrCol->god_plus;
                break;
            case 1:
                god = aDopAtrCol->god_ravno;
                break;
        }
    } else {
        switch (p->ListStmnt[nClmn].is_set_god) {
            case 0:
                if (p->ListStmnt[nClmn].god_plus != 0)
                    god = FormLogicF1->CurrYear + p->ListStmnt[nClmn].god_plus;
                break;
            case 1:
                god = p->ListStmnt[nClmn].god_ravno;
                break;
        }
    }

    DataType = 54 + deltaDType; // 'ЗНАЧЕНИЕобщНП'

    if (aStmnt == "СВОДобщНПпоСУБЪвАльб" || aStmnt == "СВОДобщНПпоСУБЪвА/n")
        DataType = 64 + deltaDType;

    coef_Pokaz = TDB_EdIzm::GetCoef_PokazOpt(TIDsClassFieldCell::getCNPedizm(comnp_id));
    if (PokazType == 0)
        PokazType = TIDsClassFieldCell::getCNPtype(comnp_id);
    cell_is_eval = 0;
    if (p->ListStmnt[nClmn].IsFieldsSprvcnik == 1)
        cell_is_eval = 1;
    PokazType = TIDsClassFieldCell::getCNPtype(comnp_id);

    switch (p->ListStmnt[nClmn].PeriodType) {
        case 0:
            vid_period = 0;
            break;
        case 1:
            vid_period = 1;
            break;
        case 4:
            vid_period = 2;
            break;
        case 6:
            vid_period = 3;
            break;
    }

    toTblName = p->ListStmnt[nClmn].toTblName;

    wPlanId = PlanId;
    if (aStmnt == "ЗНАЧЕНИЕобщНПИР")
        wPlanId = 1;

    if (PokazId == 0)
        return;

    vd = TFormDopInp::AddValueCellAtrComNP(FormLogicF1, FormLogicF1->CurrInputRow, nClmn, comnp_id,
                                           god, kvrtl, mesjc, den, DataType, VidOtch, wPlanId, coef_Pokaz, coef_Forma,
                                           0, PokazId, cell_is_eval, PokazType, vid_period, toTblName);

    if (DataType == 74 && PokazType == 7) {
        if (vd != nullptr)
            vd->EnumId = TDB_ComonNPokaz::GetEnum_id(comnp_id);
    }

    if (CurrObject.isSimvR)
       FormLogicF1->f1Forms->Protection_Set(true, true);
}

void TForm2::SetValueDataForPriznak(int nClmn, PDopAtrCol aDopAtrCol) {
    int field_id;
    int Year, Month, Day;
    int cell_id, cell_is_eval;
    int16_t vid_period;
    int wperiod, wValuePeriod, wPlanId;
    std::string toTblName;

    field_id = p->ListStmnt[nClmn].field_id;

    PokazDate = EncodeDate(FormLogicF1->CurrYear, 1, 31);
    if (Kv > 0)
        PokazDate = IncMonth(PokazDate, Kv * 3 - 1);
    else if (Mes > 0) {
        if (Mes > 1)
            PokazDate = IncMonth(PokazDate, Mes - 1);
    } else
        PokazDate = EncodeDate(FormLogicF1->CurrYear, 12, 31);

    if (aDopAtrCol->is_UstPeriod) {
        switch (aDopAtrCol->Period) {
            case 4:
                if (aDopAtrCol->ValuePeriod > 0)
                    PokazDate = GetLastDayDate(FormLogicF1->CurrYear, aDopAtrCol->ValuePeriod, 0);
                break;
            case 6:
                if (aDopAtrCol->ValuePeriod > 0)
                    PokazDate = GetLastDayDate(FormLogicF1->CurrYear, 0, aDopAtrCol->ValuePeriod);
                break;
        }
    } else {
        switch (period) {
            case 4:
                if (ValuePeriod > 0)
                    PokazDate = GetLastDayDate(FormLogicF1->CurrYear, ValuePeriod, 0);
                break;
            case 6:
                if (ValuePeriod > 0)
                    PokazDate = GetLastDayDate(FormLogicF1->CurrYear, 0, ValuePeriod);
                break;
        }
    }

    if (aDopAtrCol->is_UstGoda) {
        switch (aDopAtrCol->is_set_god) {
            case 0:
                if (aDopAtrCol->god_plus != 0) {
                    DecodeDate(PokazDate, Year, Month, Day);
                    PokazDate = EncodeDate(FormLogicF1->CurrYear + aDopAtrCol->god_plus, Month, Day);
                }
                break;
            case 1:
                DecodeDate(PokazDate, Year, Month, Day);
                PokazDate = EncodeDate(aDopAtrCol->god_ravno, Month, Day);
                break;
        }
    } else {
        switch (p->ListStmnt[nClmn].is_set_god) {
            case 0:
                if (p->ListStmnt[nClmn].god_plus != 0) {
                    DecodeDate(PokazDate, Year, Month, Day);
                    PokazDate = EncodeDate(FormLogicF1->CurrYear + p->ListStmnt[nClmn].god_plus, Month, Day);
                }
                break;
            case 1:
                DecodeDate(PokazDate, Year, Month, Day);
                PokazDate = EncodeDate(p->ListStmnt[nClmn].god_ravno, Month, Day);
                break;
        }
    }

    if (p->ListStmnt[nClmn].PeriodType == 0)
        PokazDate = EncodeDate(FormLogicF1->CurrYear, 1, 2);

    cell_is_eval = 1;

    switch (p->ListStmnt[nClmn].PeriodType) {
        case 0:
            vid_period = 0;
            break;
        case 1:
            vid_period = 1;
            break;
        case 4:
            vid_period = 2;
            break;
        case 6:
            vid_period = 3;
            break;
    }

    cell_id = TDB_Cell::GetCellId(field_id, vid_period, 0);

    TFormDopInp::AddValueCellAtr(FormLogicF1, FormLogicF1->CurrInputRow, nClmn,
                                 cell_id, PokazDate, 65 + deltaDType, VidOtch, PlanId, 0,
                                 0, 0, field_id,
                                 2, 1, vid_period, 0);

    FormLogicF1->f1Forms->Protection_Set(true, true);
}

//вставить кнопку
void TForm2::InputButton(TFormLogicF1 *FormLogicF1, int nRow, int nCol, int Height, std::string aCaption) {
    std::string objName;
    int N;

    // F1ObjButton = $00000007
    objName = "7" + aCaption; // sName(objName) определяем специальным образом (не константно 'ЛЕСХОЗ'),
    //  причем первый символ будет задавать тип объекта - как в F1 type F1ObjTypeConstants
    //  F1ObjButton = $00000007
    FormLogicF1->f1Forms->InputButton(nRow - 1, nCol - 1, nRow, nCol, utf8_to_utf16(aCaption).c_str(),
                                      utf8_to_utf16(objName).c_str());
    N = TwipsToPixel(Height);
    FormLogicF1->f1Forms->RowHeight[nRow] = N;
}

// ВАЖНО!!!!!!!!!!!!!
// AtrStmnt:PAtrStmnt и AtrRow:PAtrRow передаются параметром
// внутри метода их НЕ ВЫЧИСЛЯТЬ
// Для идентификации Range пользоваться только параметрами nRow: integer; nClmn: integer;

void TForm2::InputWithDinIzm(int nRow, int nClmn, PAtrStmnt AtrStmnt, PAtrRow AtrRow, std::string aStmnt,
                             TQueryUni *aQDefPokaz, PDopAtrCol aDopAtrCol) {
    std::string DinIzmCode;
    int DinIzmId, DinIzmGradId;

    DinIzmCode = FormLogicF1->quCell->Params3;
    DinIzmId = Pos("_", DinIzmCode);
    if (DinIzmId >= 0 && DinIzmCode[0] == '#') {
        try {
            DinIzmGradId = std::stoi(DinIzmCode.substr(DinIzmId + 1));
            if (DinIzmGradId == 0) {
                DinIzmCode = DinIzmCode.substr(1, DinIzmId - 1);
                DinIzmId = std::stoi(DinIzmCode);
                FormLogicF1->FormDM->quDinIzm->params[0].AsInteger() = DinIzmId;
                FormLogicF1->FormDM->quDinIzm->Open();
                if (!FormLogicF1->FormDM->quDinIzm->EoF()) {
                    while (!FormLogicF1->FormDM->quDinIzm->EoF()) {
                        FormLogicF1->quCell->Params3 = "#" + DinIzmCode + "_" + std::to_string(
                                FormLogicF1->FormDM->quDinIzm->FieldByName("din_izm_id")->AsInteger());
                        SetValueData(nRow, nClmn, AtrStmnt, AtrRow, aStmnt, aQDefPokaz, aDopAtrCol);
                        FormLogicF1->FormDM->quDinIzm->Next();
                    }
                }
                FormLogicF1->FormDM->quDinIzm->Close();
            } else {
                SetValueData(nRow, nClmn, AtrStmnt, AtrRow, aStmnt, aQDefPokaz, aDopAtrCol);
            }
        } catch (std::exception &e) {
            TracerManager::Instance().AddSpanLog("Exception.TForm2::InputWithDinIzm", {{"errordesr", e.what()}},
                                                 TracerManager::LOG_LEVEL_ERROR);
        }
    } else {
        SetValueData(nRow, nClmn, AtrStmnt, AtrRow, aStmnt, aQDefPokaz, aDopAtrCol);
    }
}

void TForm2::doStmnt(int nClmn, int nRow, TQueryUni *aQOwnerPlanStr, std::string stmnt, int wCurrCol) {
    PAtrStmnt AtrStmnt = &(p->ListStmnt[nClmn]);
    TFormatStyle FormatStyle;
    TFormDopInp::getColFrmtrAtr(FormLogicF1, 0, nClmn, FormatStyle);
    std::string Value = "";
    PAtrRow AtrRow = TFormDopInp::GetListRowsItem(FormLogicF1, nRow);
    int wn = 2;
    if (stmnt == "КОДБК" || stmnt == "КОДБК1") {
        Value = aQOwnerPlanStr->FieldByName("pokaz_bk_code")->AsString();
        if (stmnt == "КОДБК1")
            Value = SetCodeBK_rzdltl(Value, FormLogicF1->CurrYear);
    } else if (stmnt == "НАИМПОКАЗ") {

        int nNameCol = TFormDopInp::GetNameCol(FormLogicF1);
        Value = char16ToUTF8(FormLogicF1->f1Forms->TextRC_Get(nRow, nNameCol));
        p->PokazColother = wCurrCol;
        if (Value == "")
            Value = char16ToUTF8(FormLogicF1->f1Forms->TextRC_Get(nRow, wCurrCol));
    } else if (stmnt == "forUnikIdentif") {
        Value = aQOwnerPlanStr->FieldByName("forUnikIdentif")->AsString();
    } else if (stmnt == "ПолныйНомер") {
        Value = aQOwnerPlanStr->FieldByName("level_text")->AsString();
    } else if (stmnt == "ДатаСоздОбъекта") {
        Value = FormatDateTime("dd.mm.yyyy", aQOwnerPlanStr->FieldByName("create_date").AsDateTime());
    } else if (stmnt == "НАИМЦелСт") {
        Value = aQOwnerPlanStr->FieldByName("pokaz_bk_code")->AsString();
        Value = TDB_bkMetaDef::GetSubKBKByVid(Value, 1, FormLogicF1->CurrYear);
        Value = TDB_bkKBK::GetNameByCodeKBK(Value);
    } else if (stmnt == "КОДБКРздл" || stmnt == "КОДБКРздл1" || stmnt == "КОДБКЦелСт" || stmnt == "КОДБКЦелСт1" ||
               stmnt == "КОДБКВидРасх" || stmnt == "КОДБКЭкКл") {
        Value = aQOwnerPlanStr->FieldByName("pokaz_bk_code")->AsString();
        if (stmnt == "КОДБКРздл" || stmnt == "КОДБКРздл1") {
            Value = TDB_bkMetaDef::GetSubKBKByVid(Value, 0, FormLogicF1->CurrYear);
            if (stmnt == "КОДБКРздл1")
                Value = SetCodeBK_RZD_rzdltl(Value, AtrStmnt->natur_vid);
            if (AtrRow->CurRowVid == 12) {
                int wi = TDB_bkMetaDef::GetVidById(
                        TFormDopInp::GetCellValueAtrByRC(FormLogicF1, nRow, wCurrCol)->EnumId);
                if (wi > 0)
                    Value = "";
            }
        } else if (stmnt == "КОДБКЦелСт" || stmnt == "КОДБКЦелСт1") {
            Value = TDB_bkMetaDef::GetSubKBKByVid(Value, 1, FormLogicF1->CurrYear);
            if (stmnt == "КОДБКЦелСт1")
                Value = SetCodeBK_CST_rzdltl(Value, FormLogicF1->CurrYear, AtrStmnt->natur_vid);
        } else if (stmnt == "КОДБКВидРасх") {
            Value = TDB_bkMetaDef::GetSubKBKByVid(Value, 2, FormLogicF1->CurrYear);
        } else if (stmnt == "КОДБКЭкКл") {
            Value = TDB_bkMetaDef::GetSubKBKByVid(Value, 3, FormLogicF1->CurrYear);
        }
    } else if (stmnt == "Владелец") {
        if (CurrObject.pokaz_id > 0) {
            if (TDB_Pokaztl::GetPokazIsRazd(aQOwnerPlanStr->FieldByName("pokaz_owner_id")->AsInteger()) == 1)
                Value = TDB_Pokaztl::GetPokazName(aQOwnerPlanStr->FieldByName("pokaz_owner_id")->AsInteger());
        } else if (PokazId > 0) {
            if (TDB_Pokaztl::GetPokazIsRazd(aQOwnerPlanStr->FieldByName("pokaz_owner_id")->AsInteger()) == 1)
                Value = TDB_Pokaztl::GetPokazName(aQOwnerPlanStr->FieldByName("pokaz_owner_id")->AsInteger());
        }
    } else if (stmnt == "Ведомство") {
        if (PokazId > 0) {
            Value = Trim(aQOwnerPlanStr->FieldByName("Vedomstvo")->AsString());
            if (Value == "")
                Value = TDB_Plan::GetPPPOkrug();
        } else if (CurrObject.pokaz_id > 0) {
            Value = Trim(aQOwnerPlanStr->FieldByName("Vedomstvo")->AsString());
            if (Value == "")
                Value = TDB_Plan::GetPPPOkrug();
        }
    } else if (stmnt == "Примечание") {
        Value = aQOwnerPlanStr->FieldByName("Primecanie")->AsString();
    } else if (stmnt == "ЕДИЗМ" || stmnt == "ЕДИЗМНВ" || stmnt == "ЕДИЗМобщНП") {
        Value = TDB_EdIzm::GetEdIzmNameOpt(aQOwnerPlanStr->FieldByName("edizm_id")->AsInteger());
    } else if (stmnt == "СистХарОбъекта" || stmnt == "СистХарПоказтля") {
        wn = 1;
        switch (AtrStmnt->field_id) {
            case 1:  // наименование
                Value = aQOwnerPlanStr->FieldByName("pokaz_name")->AsString();
                wn = 0;
                break;
            case 2:  // код
                Value = aQOwnerPlanStr->FieldByName("pokaz_code")->AsString();
                break;
            case 3:  // Id класса объекта
                Value = aQOwnerPlanStr->FieldByName("objekt_class_id")->AsString();
                break;
            case 4:  // Вид показателя/объекта
                Value = TForm2::evalVidPokazObj(p, nRow, aQOwnerPlanStr);
                break;
            case 5:  // вид строки
                Value = std::to_string(AtrRow->CurRowVid);
                break;
            case 6:  // Тип
                Value = aQOwnerPlanStr->FieldByName("pokaz_type")->AsString();
                break;
            case 7:  // Номер строки владельца
                Value = std::to_string(AtrRow->OwnerNomRow);
                break;
        }
    }
    TForm1::SetCellValue(FormLogicF1, FormLogicF1->f1Forms, p->MaxHrCol, nRow, wCurrCol, nRow, wCurrCol, Value, wn,
                         &FormatStyle);
}

// обрабатывая оператор столбец создает ячейку в списке TValueData,
// оператор не исполняется,
// в CurrPokazId лежит текущий pokaz_id
void TForm2::ExecPokaz(int aOwnerNomRow, TQueryUni *aQOwnerPlanStr, int aPlan_id, PDopAtrCol aDopAtrCol,
                       std::string aStmntRow) {
    int nRow, nClmn;
    std::unique_ptr<TForm2Korteg> Form2Korteg;
    std::unique_ptr<TForm2ExecPokaz1clmn> Form2ExecPokaz1clmn;
    PAtrRow AtrRow;
    PAtrStmnt AtrStmnt;

    nRow = FormLogicF1->CurrInputRow;
    if (aStmntRow == "КОРТЕЖ") {
        Form2Korteg = std::make_unique<TForm2Korteg>(this);
        Form2Korteg->doForm2Korteg(aQOwnerPlanStr, aPlan_id, aDopAtrCol, aStmntRow);
        return;
    }

    AtrRow = TFormDopInp::GetListRowsItem(FormLogicF1, nRow);

    for (nClmn = 1; nClmn <= p->MaxHrCol; nClmn++) {
        if (nClmn == p->PokazCol)
            continue;
        Form2ExecPokaz1clmn = std::make_unique<TForm2ExecPokaz1clmn>(this);
        AtrStmnt = &(p->ListStmnt[nClmn]);
        Form2ExecPokaz1clmn->ExecPokaz1clmn(nRow, nClmn, AtrStmnt, AtrRow, aQOwnerPlanStr, aPlan_id, aDopAtrCol,
                                            aStmntRow);
    }
}

void TForm2::ExecCelPrgrm(int kbk_id) {
    std::string Stmnt; // текущий обрабатываемый в ExecPokaz показатель
    std::string ws;
    TFormatStyle *FormatStyle = new TFormatStyle();
    int wCurrCol;
    PCellValueAtr ValueData;

    FormLogicF1->FormDM->quFormStr->First(); // начинает чтение столбцов формы с начала и до конца
    while (!FormLogicF1->FormDM->quFormStr->EoF()) {
        Stmnt = FormLogicF1->FormDM->quFormStr->FieldByName("stmnt")->AsString();
        if (TDB_FormStr::GetIndexByStmnt(Stmnt) < 0) // значит не столбец-значение
            goto LNext;
        wCurrCol = FormLogicF1->FormDM->quFormStr->FieldByName("attr_1")->AsInteger(); // CurrCol
        if (Stmnt == "КОДБКЦелСт" || Stmnt == "КОДБКЦелСт1") {
            ValueData = TFormDopInp::AddValueCellAtr(FormLogicF1, FormLogicF1->CurrInputRow, wCurrCol, // CurrCol
                                                     0, // CellId
                                                     0, // ValueDate
                                                     0, // DataType
                                                     0, // VidOtch
                                                     0, // PlanId
                                                     0, // coef_Pokaz
                                                     0, // coef_Forma
                                                     0, // ObjectId
                                                     0, // PokazId
                                                     1, // cell_is_eval
                                                     0, 0, // cell_period
                                                     0); // ist_fin_id
            ValueData->EnumId = kbk_id;
        } else if (Stmnt == "ФОРМУЛАF1") {
            // вставляем формулу в F1
            ws = p->ListStmnt[wCurrCol].note1;
            Value = TFormDopInp::ReplaceNRowNCol(FormLogicF1->CurrInputRow, wCurrCol, ws);
            TFormDopInp::getColFrmtrAtr(FormLogicF1, 1, wCurrCol, *FormatStyle);
            TForm1::SetCellValue(FormLogicF1, FormLogicF1->f1Forms, p->MaxHrCol, FormLogicF1->CurrInputRow, wCurrCol,
                                 FormLogicF1->CurrInputRow, wCurrCol, Value, 0, // DataType
                                 FormatStyle);
        }

        LNext:
        FormLogicF1->FormDM->quFormStr->Next();
    }
}

// выбор подчиненных показателей/объектов, заданных в описании формы через владельца
// aLevel_sign - как определени идентификаиция у владельца, если 6 - значит наследуется, будет так, как у объекта
void TForm2::SelectOwnerPokaz(const int aOwnerNomRow, std::string OwnerLevelText, std::string VertStr,
                              TQueryUni *aOwnerPlanStr, int bSkipRow, int isFilter, int isFilter2, int ColOnFilter2,
                              PDopAtrCol aDopAtrCol, std::string aStmntRow, std::string shablon, int aLevel_sign,
                              int aNomUrovRazdela) {

    bool bBold, bItalic;
    std::string LevelText, ws, aAndWhere;
    short LevelType;
    bool IsParamRoot;
    bool IsOgrTipom;
    bool IsExecPokaz;
    int RowVid, wPlanId;
    PAtrRow wAtrRow;
    int wpokaz_owner_id = 0, wCurrPokazId, wYear;
    int aNomUrovRazdela_;
    bool isPodcinMrprRaskrit; // true - если подчиненные мероприятия раскрыть
    bool isVstavka1N; // true - если была вставка объектов по связи 1N
    int inVstavka1NOwnerRow; // каким был OwnerRow в случае такой вставки

    if (!aOwnerPlanStr->Active()) {
        aOwnerPlanStr->Open();
    }
    try {
        if (VertStr != "") {
            if (!aOwnerPlanStr->EoF())
                wpokaz_owner_id = aOwnerPlanStr->FieldByName("pokaz_owner_id")->AsInteger();
            wAtrRow = TFormDopInp::GetListRowsItem(FormLogicF1, aOwnerNomRow);
            if (wAtrRow != NULL) {
                wpokaz_owner_id = wAtrRow->ObjectId;
            }
            int ObjectId;
            if (!aOwnerPlanStr->EoF()) {
                ObjectId = aOwnerPlanStr->FieldByName("pokaz_id")->AsInteger();
            } else {
                ObjectId = 0;
            }

            FormLogicF1->CurrInputRow = TForm2::AddNewRow(FormLogicF1, aOwnerNomRow, wpokaz_owner_id, ObjectId);
            TFormDopInp::SetFormatStyle(FormLogicF1, FormLogicF1->CurrInputRow,
                                        TFormatStyle(aDopAtrCol->font, aDopAtrCol->H, aDopAtrCol->Is_bold,
                                                     aDopAtrCol->FontColor, aDopAtrCol->fonColor));
            TForm2::SetTextNameColVertStr(FormLogicF1, FormLogicF1->CurrInputRow, VertStr);
            TForm2Plus::doOutputVTomCisle(FormLogicF1, FormLogicF1->CurrInputRow, VertStr);
        }
        if (!aOwnerPlanStr->EoF()) {
            while (!aOwnerPlanStr->EoF()) {
                if (aDopAtrCol->isVPredelaxDiapazonaKBK) {
                    if (int N = aOwnerPlanStr->FieldByName("pokaz_is_razd")->AsInteger(); N == 2 || N == 4) {
                        if (!aDopAtrCol->compareKBK(p, aOwnerPlanStr->FieldByName("pokaz_bk_code")->AsString())) {
                            goto LNext;
                        }
                    }
                }
                aNomUrovRazdela_ = aNomUrovRazdela;
                CurrObject.pokaz_id = 0;
                CurrPokazId = 0;
                if (IsParamForm == 2 && aOwnerPlanStr->Tag == 1) {
                    if (aOwnerPlanStr->FieldByName("pokaz_id")->AsInteger() != ParamObjectId) {
                        goto LNext;
                    }
                }
                switch (FormLogicF1->quForms->FieldByName("work_with")->AsInteger()) {
                    case 0:
                    case 1:
                        CurrPokazId = aOwnerPlanStr->FieldByName("pokaz_id")->AsInteger();
                        break;
                    case 2://{с одним объектом (часть 31)}
                        break;
                    case 3:
                    case 5:
                        CurrObject.pokaz_id = aOwnerPlanStr->FieldByName(
                                "pokaz_id")->AsInteger();// это часть или объект
                        CurrObject.pokaz_is_razd = aOwnerPlanStr->FieldByName("pokaz_is_razd")->AsInteger();
                        if (aOwnerPlanStr->FieldByName("pokaz_is_razd")->AsInteger() ==
                            2) {//{это показатель, а не объект}
                            CurrObject.pokaz_id = 0;
                            CurrPokazId = aOwnerPlanStr->FieldByName("pokaz_id")->AsInteger();
                        }
                        break;
                }
                if (aLevel_sign == 6) {//{наследуется}
                    LevelType = aOwnerPlanStr->FieldByName("level_sign")->AsInteger();
                } else {
                    LevelType = aLevel_sign;
                }
                LevelText = aOwnerPlanStr->FieldByName("level_text")->AsString();
                if (Pos(".", OwnerLevelText) < 0 && Pos(".", LevelText) >= 0 &&
                    aOwnerPlanStr->FieldByName("pokaz_is_razd")->AsInteger() == 2) {
                } else {
                    bBold = false;
                    bItalic = false;
                }
                switch (aOwnerPlanStr->FieldByName("pokaz_is_razd")->AsInteger()) {
                    case 1:
                    case 3:
                    case 5:
                        bBold = true;
                        bItalic = false;
                        if (int N = aOwnerPlanStr->FieldByName("pokaz_is_razd")->AsInteger(); N == 3 || N == 5) {
                            bItalic = true;
                        }
                        break;
                }
                if (LevelType == 3) {
                    bItalic = true;
                }
                if (LevelType == 1) {
                    LevelText = OwnerLevelText + LevelText;
                }
                VertStr = aOwnerPlanStr->FieldByName("vert_str")->AsString();
                IsParamRoot = false;
                if (IsParamForm == 1) { // для параметрической формы
                    if (RootPokazId == aOwnerPlanStr->FieldByName("pokaz_id")->AsInteger()) {// голова раскрутки
                        IsParamRoot = true;
                    }
                }
                IsOgrTipom = true;
                if (int N = FormLogicF1->quForms->FieldByName("work_with")->AsInteger(); N == 3 || N == 5) {
                    if (FormLogicF1->quForms->FieldByName("ogr_class_id")->AsInteger() > 0) {
                        if (!IsParamRoot && FormLogicF1->quForms->FieldByName("ogr_class_id")->AsInteger() !=
                                            aOwnerPlanStr->FieldByName("objekt_class_id")->AsInteger()) {
                            IsOgrTipom = false;
                        }
                        int N = aOwnerPlanStr->FieldByName("pokaz_is_razd")->AsInteger();
                        if (N == 1 || N == 3 || N == 5) {
                            // если раздел, итого, то вставляем
                            IsOgrTipom = true;
                        }
                    }
                }
                IsExecPokaz = false;
                if (isFilter > 0) {
                    switch (isFilter) {
                        case 1:
                        case 14:// {только КОСГУ, Только КОСГУ но полностью}
                            IsOgrTipom = false; // чтобы пропустить запись
                            if (aOwnerPlanStr->FieldByName("pokaz_vert")->AsInteger() == 0) {
                                IsOgrTipom = true;
                            }
                            if (aOwnerPlanStr->FieldByName("pokaz_vert")->AsInteger() == 3) {
                                IsOgrTipom = true;
                            }
                            break;
                        case 2:// {только Вид расходов}
                            if (aOwnerPlanStr->FieldByName("pokaz_bk_code")->AsString().length() <
                                TDB_bkMetaDef::GetDl_R_CS_VR(FormLogicF1->CurrYear)) {
                                IsOgrTipom = false;
                            } else if (aOwnerPlanStr->FieldByName("pokaz_bk_code")->AsString().length() >
                                       TDB_bkMetaDef::GetDl_R_CS_VR(FormLogicF1->CurrYear)) {
                                return;
                            } else {// {длина = 14 - это вид расходов}
                                if (TDB_Pokaztl::getLengthKBKforSubPokaz(
                                        aOwnerPlanStr->FieldByName("pokaz_id")->AsInteger()) ==
                                    TDB_bkMetaDef::GetDl_R_CS_VR(FormLogicF1->CurrYear)) {
                                    IsOgrTipom = false;//{пропускаем уровень внутри вида расходов}
                                } else {
                                    IsOgrTipom = true;
                                    if (Copy(aOwnerPlanStr->FieldByName("pokaz_bk_code")->AsString(), 1, 4) ==
                                        "0001") {// {специально для кассового плана}
                                        IsOgrTipom = false;
                                    }
                                }
                            }
                            break;
                        case 3:// Заданный КБК
                            if (int N = aOwnerPlanStr->FieldByName("pokaz_is_razd")->AsInteger(); N == 1 || N == 3 ||
                                                                                                  N == 5) {
                                IsOgrTipom = false;// если раздел, итого, то не вставляем
                            }
                            if (aOwnerPlanStr->FieldByName("pokaz_bk_code")->AsString() != shablon) {
                                IsOgrTipom = false; // чтобы пропустить запись
                            }
                            break;
                        case 9:// {только раздел/подраздел}
                            if (aOwnerPlanStr->FieldByName("pokaz_bk_code")->AsString().length() < 4) {
                                IsOgrTipom = false;
                            } else if (aOwnerPlanStr->FieldByName("pokaz_bk_code")->AsString().length() > 4) {
                                return;
                            } else {
                                if (TDB_Pokaztl::getLengthKBKforSubPokaz(
                                        aOwnerPlanStr->FieldByName("pokaz_id")->AsInteger()) > 4) {
                                    IsOgrTipom = true;
                                } else {
                                    IsOgrTipom = false;
                                }
                                if (Copy(aOwnerPlanStr->FieldByName("pokaz_bk_code")->AsString(), 1, 4) ==
                                    "0001") {// {специально для кассового плана}
                                    IsOgrTipom = false;
                                }
                            }
                            break;
                        case 10:
                        case 13:// {шаблон КБК} {наследуемый шаблон}
                            IsOgrTipom = false;
                            if (aDopAtrCol->compareKBKbyStep(aOwnerPlanStr->FieldByName("pokaz_bk_code")->AsString(),
                                                             shablon, FormLogicF1->CurrYear)) {
                                if (aOwnerPlanStr->FieldByName("pokaz_vert")->AsInteger() == 0 ||
                                    aOwnerPlanStr->FieldByName("pokaz_vert")->AsInteger() == 3) {
                                    IsOgrTipom = true;
                                }
                            }
                            break;
                        case 11://{До вида расходов включительно}
                            if (aOwnerPlanStr->FieldByName("pokaz_bk_code")->AsString().length() >
                                TDB_bkMetaDef::GetDl_R_CS_VR(FormLogicF1->CurrYear)) {
                                return;
                            } else {
                                IsOgrTipom = true;
                                if (Copy(aOwnerPlanStr->FieldByName("pokaz_bk_code")->AsString(), 1, 4) == "0001") {
                                    IsOgrTipom = false;
                                }
                            }
                            break;
                        case 12:// {До целевой статьи включительно}
                            if (aOwnerPlanStr->FieldByName("pokaz_bk_code")->AsString().length() >
                                TDB_bkMetaDef::GetDl_R_CS(FormLogicF1->CurrYear)) {
                                return;
                            } else {
                                IsOgrTipom = true;
                                if (Copy(aOwnerPlanStr->FieldByName("pokaz_bk_code")->AsString(), 1, 4) == "0001") {
                                    IsOgrTipom = false;
                                }
                            }
                            break;
                        case 15://{Только разделы и итого по разделам}
                            if (int N = aOwnerPlanStr->FieldByName("pokaz_is_razd")->AsInteger(); N == 1 || N == 3 ||
                                                                                                  N == 5) {
                                IsOgrTipom = true;
                            } else {
                                IsOgrTipom = false;
                            }
                            break;
                    }
                }
                if (IsOgrTipom && !IsParamRoot && (bSkipRow == 0 || bSkipRow == 2) &&
                    aOwnerPlanStr->FieldByName("pokaz_is_razd")->AsInteger() > 0) {
                    if (IsParamForm == 2 && aOwnerPlanStr->Tag == 1) {
                        FormLogicF1->CurrInputRow = nRowParamObjectId;
                    } else {
                        FormLogicF1->CurrInputRow = TForm2::AddNewRow(FormLogicF1, aOwnerNomRow,
                                                                      aOwnerPlanStr->FieldByName(
                                                                              "pokaz_owner_id")->AsInteger(),
                                                                      aOwnerPlanStr->FieldByName(
                                                                              "pokaz_id")->AsInteger());
                    }
                    RowVid = aOwnerPlanStr->FieldByName("pokaz_is_razd")->AsInteger();
                    if (RowVid == 1) {
                        aNomUrovRazdela_ = aNomUrovRazdela_ + 1;
                    }
                    if (aStmntRow == "КОРТЕЖ") {
                        RowVid = 13;
                    }
                    TFormDopInp::SetListRowsItemAtr(FormLogicF1, FormLogicF1->CurrInputRow, RowVid, LevelType,
                                                    aOwnerPlanStr->FieldByName("pokaz_vert")->AsInteger(),
                                                    aOwnerPlanStr->FieldByName("pokaz_owner_id")->AsInteger(),
                                                    aOwnerPlanStr->FieldByName("pokaz_id")->AsInteger(),
                                                    aOwnerPlanStr->FieldByName("objekt_class_id")->AsInteger(),
                                                    aOwnerPlanStr->FieldByName("pokaz_bk_code")->AsString(),
                                                    aNomUrovRazdela_);
                    TFormDopInp::SetDopAtrToAtrRow(FormLogicF1, FormLogicF1->CurrInputRow, aDopAtrCol);
                    TFormDopInp::SetFilter2OnRow(FormLogicF1, FormLogicF1->CurrInputRow, isFilter2, ColOnFilter2,
                                                 isFilter);
                    TFormDopInp::SetFormatStyle(FormLogicF1, FormLogicF1->CurrInputRow,
                                                TFormatStyle(aDopAtrCol->font, aDopAtrCol->H, aDopAtrCol->Is_bold,
                                                             aDopAtrCol->FontColor, aDopAtrCol->fonColor));
                    TForm2::SetTextNameCol(FormLogicF1, FormLogicF1->CurrInputRow,
                                           aOwnerPlanStr->FieldByName("pokaz_name")->AsString(),
                                           aOwnerPlanStr->FieldByName("edizm_id")->AsInteger());
                    if (int N = FormLogicF1->quForms->FieldByName("work_with")->AsInteger(); N == 1 || N == 3 ||
                                                                                             N == 5) {
                        if (p->plan_id > 1) {
                            ExecPokaz(aOwnerNomRow, aOwnerPlanStr, p->plan_id, aDopAtrCol, aStmntRow);
                        } else {
                            TForm2::ExecPokaz(aOwnerNomRow, aOwnerPlanStr, 0, aDopAtrCol, aStmntRow);
                        }
                        IsExecPokaz = true;
                    }
                }
                switch (FormLogicF1->quForms->FieldByName("work_with")->AsInteger()) {
                    case 0:
                    case 1:
                        isSelectSubItem = true;
                        break;
                    case 2://{с одним объектом (часть 31)}
                        break;
                    case 3:
                    case 5:
                        int N = aOwnerPlanStr->FieldByName("pokaz_is_razd")->AsInteger();
                        isSelectSubItem = (N == 0 || N == 1);//{раскручиваем только часть и раздел}
                        if (IsParamRoot) {// для параметрической формы  // голова раскрутки
                            isSelectSubItem = true;
                        } else {
                            if (FormLogicF1->quForms->FieldByName("ogr_class_id")->AsInteger() > 0 &&
                                FormLogicF1->quForms->FieldByName("ogr_class_id")->AsInteger() !=
                                aOwnerPlanStr->FieldByName("objekt_class_id")->AsInteger()) {
                                isSelectSubItem = true;
                            }
                            if (FormLogicF1->quForms->FieldByName("ogr_class_id")->AsInteger() > 0 &&
                                FormLogicF1->quForms->FieldByName("ogr_class_id")->AsInteger() ==
                                aOwnerPlanStr->FieldByName("objekt_class_id")->AsInteger() &&
                                aOwnerPlanStr->FieldByName("pokaz_vert")->AsInteger() == 1) {
                                isSelectSubItem = true;
                            }
                        }
                        if (aOwnerPlanStr->FieldByName("pokaz_is_razd")->AsInteger() == 4 &&
                            aOwnerPlanStr->FieldByName("pokaz_vert")->AsInteger() > 0) {
                            if (TDB_ClassofPart::IsElem(aOwnerPlanStr->FieldByName("objekt_class_id")->AsInteger(),
                                                        aOwnerPlanStr->FieldByName("objekt_class_id")->AsInteger())) {
                                // Это иерархия объектов одного и того же класса
                                isSelectSubItem = true;
                            }
                        }
                        if (aOwnerPlanStr->FieldByName("pokaz_is_razd")->AsInteger() == 2 &&
                            aOwnerPlanStr->FieldByName("pokaz_vert")->AsInteger() > 0) {
                            isSelectSubItem = true;
                            if (aOwnerPlanStr->FieldByName("pokaz_vert")->AsInteger() == 3) { // если включает
                                if (TDB_bkMetaDef::GetVidById(aOwnerPlanStr->FieldByName("enum_id")->AsInteger()) ==
                                    3) {// если КОСГУ
                                    isSelectSubItem = false;
                                }
                            }
                        }
                        break;
                }
                if (aOwnerPlanStr->FieldByName("pokaz_is_razd")->AsInteger() > 1 &&
                    (aOwnerPlanStr->FieldByName("pokaz_vert")->AsInteger() == 0 ||
                     aOwnerPlanStr->FieldByName("pokaz_vert")->AsInteger() == 3)) {
                    isSelectSubItem = false;
                }
                if (aOwnerPlanStr->FieldByName("pokaz_vert")->AsInteger() == 3) { // если включает
                    if (TDB_bkMetaDef::GetVidById(aOwnerPlanStr->FieldByName("enum_id")->AsInteger()) ==
                        3) {// если КОСГУ
                        isSelectSubItem = false;
                    }
                }
                if (isSelectSubItem) {
                    isVstavka1N = false;
                    if (aOwnerPlanStr->FieldByName("pokaz_vert")->AsInteger() == 1 && work_with == 5 &&
                        aDopAtrCol->OtkudaOwner == 2) {
                        wAtrRow = TFormDopInp::GetListRowsItem(FormLogicF1, FormLogicF1->CurrInputRow);
                        if (wAtrRow != NULL) {
                            aDopAtrCol->owner_id_1N = wAtrRow->ObjectId;
                            if ((work_with = 5) && (isFilter = 15) &&
                                (aOwnerPlanStr->FieldByName("pokaz_is_razd").AsInteger() == 2))
                                aDopAtrCol->owner_id_1N = aOwnerPlanStr->FieldByName(
                                        "pokaz_id").AsInteger(); // Для раскрутки только по разделам
                            // раскручиваем
                            wCurrPokazId = CurrPokazId; // 06.06.2022
                            isVstavka1N = true; //была вставка объектов по связи 1N
                            inVstavka1NOwnerRow = FormLogicF1->CurrInputRow; // каким был OwnerRow в случае такой вставки
                            TForm2Owner1N(this).doForm2Owner1N(aDopAtrCol->plan_id, aDopAtrCol,
                                                               FormLogicF1->CurrInputRow);
                            CurrPokazId = wCurrPokazId;  // 06.06.2022 т.к. метод doForm2Owner1N изменяет CurrPokazId
                        }
                    }
                    TQueryUni *quPlanStr = new TQueryUni(DefDBconnect.Conn);
                    quPlanStr->Tag = 1;
                    quPlanStr->SQL->Add("SELECT b.*, a.plan_id FROM Plan_str a, Pokaztl b");
                    int N = FormLogicF1->quForms->FieldByName("work_with")->AsInteger();
                    if ((N == 1 || N == 3) && (p->plan_id > 1)) {
                        quPlanStr->SQL->Add("WHERE plan_id IN (1, " + IntToStr(p->plan_id) + ")");
                    } else {
                        quPlanStr->SQL->Add("WHERE plan_id IN (1, " + IntToStr(FormLogicF1->CurrPlanId) + ")");
                    }
                    switch (FormLogicF1->quForms->FieldByName("work_with")->AsInteger()) {
                        case 0:
                        case 1://{с показателями (часть 30)}
                            quPlanStr->SQL->Add("AND plan_str_owner_id = " + IntToStr(CurrPokazId));
                            break;
                        case 2:
                            break;
                        case 3:
                        case 5:
                            if (aOwnerPlanStr->FieldByName("pokaz_is_razd")->AsInteger() == 2) {
                                quPlanStr->SQL->Add("AND plan_str_owner_id = " + IntToStr(CurrPokazId));
                            } else {
                                quPlanStr->SQL->Add("AND plan_str_owner_id = " + IntToStr(CurrObject.pokaz_id));
                            }
                            break;
                    }
                    quPlanStr->SQL->Add("AND a.pokaz_id = b.pokaz_id");
                    wYear = FormLogicF1->CurrYear;
                    if (aDopAtrCol->is_UstGoda) {
                        switch (aDopAtrCol->is_set_god) {
                            case 0:
                                wYear = wYear + aDopAtrCol->god_plus;
                                break;
                            case 1:
                                wYear = aDopAtrCol->god_ravno;
                                break;
                        }
                    }
                    switch (GetDBType()) {
                        case TDBType::MSSQL:
                            quPlanStr->SQL->Add("AND " + IntToStr(wYear) +
                                                " BETWEEN YEAR(b.pokaz_beg_date) AND YEAR(b.pokaz_end_date)");
                            break;
                        case TDBType::Postgre:
                            quPlanStr->SQL->Add("AND " + IntToStr(wYear) +
                                                " BETWEEN date_part('year', b.pokaz_beg_date) AND date_part('year', b.pokaz_end_date)");
                            break;
                    }
                    if (isFilter2 == 6 || isFilter2 == 7) { // если заданы условия выборки по столбцам
                        int N = FormLogicF1->quForms->FieldByName("work_with")->AsInteger();
                        if ((N == 1 || N == 3) && (p->plan_id > 1)) {
                            wPlanId = p->plan_id;
                        } else {
                            wPlanId = FormLogicF1->CurrPlanId;
                        }
                        if (aDopAtrCol->is_planId) {
                            wPlanId = aDopAtrCol->plan_id;
                        }
                        if (TForm2SelectCond(this).doForm2SelectCond(wPlanId, aAndWhere)) {
                            quPlanStr->SQL->Add(aAndWhere);
                        }
                    }
                    quPlanStr->SQL->Add(
                            "ORDER BY CASE WHEN plan_id > 1 THEN plan_str_order+400 WHEN plan_id = 1 and plan_str_order = 1000 THEN plan_str_order+10000 ELSE plan_str_order END");
                    isPodcinMrprRaskrit = true;
                    if (aOwnerPlanStr->FieldByName("pokaz_is_razd")->AsInteger() == 2 &&
                        aOwnerPlanStr->FieldByName("pokaz_vert")->AsInteger() == 1 && bSkipRow == 2) {
                        isPodcinMrprRaskrit = false;
                    }
                    if (isPodcinMrprRaskrit) {
                        if (IsExecPokaz) {
                            if (isVstavka1N) {
                                wpokaz_owner_id = inVstavka1NOwnerRow;
                            } else {
                                wpokaz_owner_id = FormLogicF1->CurrInputRow;
                            }
                            SelectOwnerPokaz(wpokaz_owner_id, LevelText, VertStr, quPlanStr, bSkipRow, isFilter,
                                             isFilter2, ColOnFilter2, aDopAtrCol, aStmntRow, shablon, LevelType,
                                             aNomUrovRazdela_);
                        } else {
                            SelectOwnerPokaz(aOwnerNomRow, LevelText, "", quPlanStr, bSkipRow, isFilter, isFilter2,
                                             ColOnFilter2, aDopAtrCol, aStmntRow, shablon, LevelType, aNomUrovRazdela_);
                        }
                    }
                    delete quPlanStr;
                } else {
                    if (IsExecPokaz) {
                        if (VertStr != "") {
                            wpokaz_owner_id = aOwnerPlanStr->FieldByName("pokaz_owner_id")->AsInteger();
                            wAtrRow = TFormDopInp::GetListRowsItem(FormLogicF1, aOwnerNomRow);
                            if (wAtrRow != NULL) {
                                wpokaz_owner_id = wAtrRow->ObjectId;
                            }
                            FormLogicF1->CurrInputRow = TForm2::AddNewRow(FormLogicF1, aOwnerNomRow, wpokaz_owner_id,
                                                                          aOwnerPlanStr->FieldByName(
                                                                                  "pokaz_id")->AsInteger());
                            TFormDopInp::SetFormatStyle(FormLogicF1, FormLogicF1->CurrInputRow,
                                                        TFormatStyle(aDopAtrCol->font, aDopAtrCol->H,
                                                                     aDopAtrCol->Is_bold, aDopAtrCol->FontColor,
                                                                     aDopAtrCol->fonColor));
                            TForm2::SetTextNameColVertStr(FormLogicF1, FormLogicF1->CurrInputRow, VertStr);
                        }
                        if (work_with == 5) {
                            if (aDopAtrCol->OtkudaOwner ==
                                2) { // значит Owner задается внутренне, оператором Показатель
                                wAtrRow = TFormDopInp::GetListRowsItem(FormLogicF1, FormLogicF1->CurrInputRow);
                                aDopAtrCol->owner_id_1N = wAtrRow->ObjectId;
                                // раскручиваем
                                TForm2Owner1N(this).doForm2Owner1N(aDopAtrCol->plan_id, aDopAtrCol,
                                                                   FormLogicF1->CurrInputRow);
                            }
                        }
                    } else {
                        // Для раскрутки только по разделам 20.10.2023
                        // Сюда придем, посколку мероприятие не вставили
                        if (work_with == 5 && isFilter == 15 &&
                            aOwnerPlanStr->FieldByName("pokaz_is_razd")->AsInteger() == 2) {
                            if (aDopAtrCol->OtkudaOwner == 2) {
                                aDopAtrCol->owner_id_1N = aOwnerPlanStr->FieldByName("pokaz_id")->AsInteger();
                                TForm2Owner1N(this).doForm2Owner1N(aDopAtrCol->plan_id, aDopAtrCol,
                                                                   FormLogicF1->CurrInputRow);
                            }
                        }
                    }
                }
                LNext:
                aOwnerPlanStr->Next();
            }
        } else {
            TFormDopInp::SetToTerminal(FormLogicF1, FormLogicF1->CurrInputRow);
        }
    } catch (exception &E) {
        TracerManager::Instance().AddSpanLog("Exception.TForm2::SelectOwnerPokaz", {{"errordesr", E.what()}},
                                             TracerManager::LOG_LEVEL_ERROR);
        FormLogicF1->FormDM->quValue->Close();
        FormLogicF1->quCell->Close();
        aOwnerPlanStr->Close();
        //ws = E;
        //throw Exception(ws);
    }
    aOwnerPlanStr->Close();
}

// выбор подчиненных показателей/объектов, заданных в описании формы через владельца
// в ПОРЯКЕ, определяемой целевыми программами
void
TForm2::SelectPokazOrderCP(int aRootPokazId, const int aOwnerNomRow, std::string OwnerLevelText, std::string VertStr,
                           bool bSkipRow, int isFilter, int isFilter2, int ColOnFilter2, PDopAtrCol aDopAtrCol,
                           std::string aStmntRow, std::string shablon, int isOpen) {

    bool bBold, bItalic;
    std::string LevelText;
    short LevelType;
    bool IsParamRoot, IsOgrTipom, IsExecPokaz;
    int RowVid;
    std::string nameTmpTable;

    TQueryUni *quPlanStr = new TQueryUni(DefDBconnect.Conn);

    switch (GetDBType()) {
        case TDBType::MSSQL:
            nameTmpTable = "#PTB_InitSelect" + IntToStr(aRootPokazId);
            break;
        case TDBType::Postgre:
            nameTmpTable = "PTB_InitSelect" + IntToStr(aRootPokazId);
            break;
    }

    TQueryUni *guPTB = new TQueryUni(DefDBconnect.Conn);
    TDB_TmpPokaztl::CreateTmpOrderCP(nameTmpTable, FormLogicF1->CurrYear, aRootPokazId);

    switch (isFilter) {
        case 4:
            TDB_TmpPokaztl::addPrgrm(nameTmpTable, shablon, FormLogicF1->CurrYear, isOpen);
            break;
        case 5:
            TDB_TmpPokaztl::addSubPrgrm(nameTmpTable, shablon, FormLogicF1->CurrYear, isOpen);
            break;
    }

    guPTB->SQL->Add("SELECT * FROM " + nameTmpTable);
    switch (GetDBType()) {
        case TDBType::MSSQL:
            guPTB->SQL->Add("WHERE LEN(cs) > 0");
            break;
        case TDBType::Postgre:
            guPTB->SQL->Add("WHERE LENGTH(cs) > 0");
            break;
    }
    guPTB->SQL->Add("ORDER BY cs, rd, vr, ku");
    guPTB->Open();
    while (!guPTB->EoF()) {
        if (shablon != "") {
            if (Pos(shablon, guPTB->FieldByName("cs").AsString()) != 0)
                guPTB->Next();
            continue;
        }
        CurrObject.pokaz_id = 0;
        CurrPokazId = 0;
        switch (FormLogicF1->quForms->FieldByName("work_with")->AsInteger()) {
            case 0:
            case 1:
                CurrPokazId = guPTB->FieldByName("pokaz_id")->AsInteger();
                break;
            case 2:
                break;
            case 3:
            case 5:
                CurrObject.pokaz_id = guPTB->FieldByName("pokaz_id")->AsInteger();// это часть или объект
                CurrObject.pokaz_is_razd = guPTB->FieldByName("pokaz_is_razd")->AsInteger();
                break;
        }

        if (aDopAtrCol->isVPredelaxDiapazonaKBK) {
            if (int N = guPTB->FieldByName("pokaz_is_razd")->AsInteger(); N == 2 || N == 4) {
                if (!aDopAtrCol->compareKBK(p, guPTB->FieldByName("pokaz_bk_code")->AsString()))
                    guPTB->Next();
                continue;
            }
        }

        // 18{Это целевая программа}, 19{Это подпрограмма}
        int N = guPTB->FieldByName("pokaz_is_razd")->AsInteger();
        if (N == 18 || N == 19 || N == 20 || N == 21) {
            if (isOpen == 2 || isOpen == 3)
                goto LNext;
            // kbk_id - это есть CurrPokazId
            FormLogicF1->CurrInputRow = TForm2::AddNewRow(FormLogicF1, 0, 0, 0);
            RowVid = guPTB->FieldByName("pokaz_is_razd")->AsInteger();
            // в случае целевой программы RowVid = 18, подпрограммы = 19
            TFormDopInp::SetListRowsItemAtr(FormLogicF1, FormLogicF1->CurrInputRow, RowVid, 0, 0, 0, CurrPokazId, 0,
                                            "");
            TFormDopInp::SetFilter2OnRow(FormLogicF1, FormLogicF1->CurrInputRow, isFilter2, ColOnFilter2, isFilter);
            TFormDopInp::SetFormatStyle(FormLogicF1, FormLogicF1->CurrInputRow,
                                        TFormatStyle(aDopAtrCol->font, aDopAtrCol->H,
                                                     aDopAtrCol->Is_bold, aDopAtrCol->FontColor, aDopAtrCol->fonColor));
            TFormDopInp::SetDopAtrToAtrRow(FormLogicF1, FormLogicF1->CurrInputRow, aDopAtrCol);
            TForm2::SetTextNameCol(FormLogicF1, FormLogicF1->CurrInputRow, TDB_bkKBK::GetName(CurrPokazId), 0);
            TForm2::ExecCelPrgrm(CurrPokazId);
            TFormDopInp::SetToTerminal(FormLogicF1, FormLogicF1->CurrInputRow);
            goto LNext;
        }

        quPlanStr->SQL->Clear();
        quPlanStr->SQL->Add("SELECT * FROM Plan_str a, Pokaztl b");
        if (int N = FormLogicF1->quForms->FieldByName("work_with")->AsInteger(); N == 1 || N == 3 ||
                                                                                 N == 5 && p->plan_id > 1)
            quPlanStr->SQL->Add("WHERE plan_id IN (1, " + std::to_string(p->plan_id) + ")");
        else
            quPlanStr->SQL->Add("WHERE plan_id IN (1, " + std::to_string(FormLogicF1->CurrPlanId) + ")");
        switch (FormLogicF1->quForms->FieldByName("work_with")->AsInteger()) {
            case 0:
            case 1:
                quPlanStr->SQL->Add("AND b.pokaz_id = " + std::to_string(CurrPokazId));
                break;
            case 2:
                break;
            case 3:
            case 5:
                quPlanStr->SQL->Add("AND b.pokaz_id = " + std::to_string(CurrObject.pokaz_id));
                break;
        }
        quPlanStr->SQL->Add("AND a.pokaz_id = b.pokaz_id");
        switch (GetDBType()) {
            case TDBType::MSSQL:
                quPlanStr->SQL->Add("AND " + std::to_string(FormLogicF1->CurrYear) +
                                    " BETWEEN YEAR(b.pokaz_beg_date) AND YEAR(b.pokaz_end_date)");
                break;
            case TDBType::Postgre:
                quPlanStr->SQL->Add("AND " + std::to_string(FormLogicF1->CurrYear) +
                                    " BETWEEN date_part('year', b.pokaz_beg_date) AND date_part('year', b.pokaz_end_date)");
                break;
        }
        quPlanStr->Open();

        LevelType = quPlanStr->FieldByName("level_sign")->AsInteger();
        LevelText = quPlanStr->FieldByName("level_text")->AsString();
        if (Pos(".", OwnerLevelText) < 0 && Pos(".", LevelText) >= 0 &&
            quPlanStr->FieldByName("pokaz_is_razd")->AsInteger() == 2) {
            bBold = true;
            bItalic = true;
        } else {
            bBold = false;
            bItalic = false;
        }
        switch (quPlanStr->FieldByName("pokaz_is_razd")->AsInteger()) {
            case 1:
            case 3:
            case 5:
                bBold = true;
                if (int N = quPlanStr->FieldByName("pokaz_is_razd")->AsInteger(); N == 3 || N == 5)
                    bItalic = true;
                break;
        }
        if (LevelType == 3)
            bItalic = true;
        if (LevelType == 1)
            LevelText = OwnerLevelText + LevelText;
        VertStr = quPlanStr->FieldByName("vert_str")->AsString();
        IsParamRoot = false;
        if (IsParamForm == 1) // для параметрической формы
            if (RootPokazId == quPlanStr->FieldByName("pokaz_id")->AsInteger())
                IsParamRoot = true;
        IsOgrTipom = true;
        if (int N = FormLogicF1->quForms->FieldByName("work_with")->AsInteger(); (N == 3 || N == 5)
                                                                                 && FormLogicF1->quForms->FieldByName(
                "ogr_class_id")->AsInteger() > 0) {
            if (!IsParamRoot &&
                FormLogicF1->quForms->FieldByName("ogr_class_id")->AsInteger() !=
                quPlanStr->FieldByName("objekt_class_id")->AsInteger())
                IsOgrTipom = false;
            if (int N = quPlanStr->FieldByName("pokaz_is_razd")->AsInteger(); N == 1 || N == 3 || N == 5)
                IsOgrTipom = true;
        }
        IsExecPokaz = false;
        switch (isOpen) {
            case 2:
                if (quPlanStr->FieldByName("pokaz_vert")->AsInteger() == 0 ||
                    quPlanStr->FieldByName("pokaz_vert")->AsInteger() == 3)
                    IsOgrTipom = true;
                else
                    IsOgrTipom = false;
                break;
            case 3:
                if (quPlanStr->FieldByName("pokaz_bk_code")->AsString().length() <
                    TDB_bkMetaDef::GetDl_R_CS_VR(FormLogicF1->CurrYear))
                    IsOgrTipom = false;
                else if (quPlanStr->FieldByName("pokaz_bk_code")->AsString().length() >
                         TDB_bkMetaDef::GetDl_R_CS_VR(FormLogicF1->CurrYear))
                    IsOgrTipom = false;
                else
                    IsOgrTipom = true;
                break;
            case 4:
                if (quPlanStr->FieldByName("pokaz_bk_code")->AsString().length() >
                    TDB_bkMetaDef::GetDl_R_CS_VR(FormLogicF1->CurrYear))
                    IsOgrTipom = false;
                else
                    IsOgrTipom = true;
                break;
        }
        if (IsOgrTipom && !IsParamRoot && !bSkipRow && quPlanStr->FieldByName("pokaz_is_razd")->AsInteger() > 0) {
            TunProgress::ShowText(FormLogicF1, quPlanStr->FieldByName("pokaz_name")->AsString());
            FormLogicF1->CurrInputRow = TForm2::AddNewRow(FormLogicF1, aOwnerNomRow,
                                                          quPlanStr->FieldByName("pokaz_owner_id")->AsInteger(),
                                                          quPlanStr->FieldByName("pokaz_id")->AsInteger());
            RowVid = quPlanStr->FieldByName("pokaz_is_razd")->AsInteger();
            if (aStmntRow == "КОРТЕЖ")
                RowVid = 13;
            TFormDopInp::SetListRowsItemAtr(FormLogicF1, FormLogicF1->CurrInputRow, RowVid,
                                            quPlanStr->FieldByName("level_sign")->AsInteger(),
                                            quPlanStr->FieldByName("pokaz_vert")->AsInteger(),
                                            quPlanStr->FieldByName("pokaz_owner_id")->AsInteger(),
                                            quPlanStr->FieldByName("pokaz_id")->AsInteger(),
                                            quPlanStr->FieldByName("objekt_class_id")->AsInteger(),
                                            quPlanStr->FieldByName("pokaz_bk_code")->AsString());
            TFormDopInp::SetFilter2OnRow(FormLogicF1, FormLogicF1->CurrInputRow, isFilter2, ColOnFilter2, isFilter);
            TFormDopInp::SetFormatStyle(FormLogicF1, FormLogicF1->CurrInputRow,
                                        TFormatStyle(aDopAtrCol->font, aDopAtrCol->H, aDopAtrCol->Is_bold,
                                                     aDopAtrCol->FontColor, aDopAtrCol->fonColor));
            TForm2::SetTextNameCol(FormLogicF1, FormLogicF1->CurrInputRow,
                                   quPlanStr->FieldByName("pokaz_name")->AsString(),
                                   quPlanStr->FieldByName("edizm_id")->AsInteger());
            if (int N = FormLogicF1->quForms->FieldByName("work_with")->AsInteger(); (N == 1 || N == 3 || N == 5) &&
                                                                                     p->plan_id > 1)
                ExecPokaz(aOwnerNomRow, quPlanStr, p->plan_id, aDopAtrCol, aStmntRow);
            else
                ExecPokaz(aOwnerNomRow, quPlanStr, 0, aDopAtrCol, aStmntRow);
            IsExecPokaz = true;
            TFormDopInp::SetToTerminal(FormLogicF1, FormLogicF1->CurrInputRow);
        }
        quPlanStr->Close();

        LNext:
        guPTB->Next();
    }
    //MAK guPTB->Free();
    delete guPTB;
    //quPlanStr->Free();
    delete quPlanStr;
}

// выбор показателей только КОСГУ
void TForm2::SelectOnlyKOSGU(int aRootPokazId, int aOwnerNomRow, std::string OwnerLevelText, std::string VertStr,
                             bool bSkipRow, int isFilter, int isFilter2, int ColOnFilter2, PDopAtrCol aDopAtrCol,
                             std::string aStmntRow) {

    TQueryUni *quPlanStr = new TQueryUni(DefDBconnect.Conn);

    std::string tmpTblName;
    switch (GetDBType()) {
        case TDBType::MSSQL:
            tmpTblName = "#PTB_byKOSGU" + std::to_string(aRootPokazId);
            break;
        case TDBType::Postgre:
            tmpTblName = "PTB_byKOSGU" + std::to_string(aRootPokazId);
            break;
    }
    // выборка только тех, что актуальны по дате
    TDB_TmpPokaztl::CreateTmpByKOSGU(isFilter, tmpTblName, FormLogicF1->CurrYear, aRootPokazId);

    quPlanStr->SQL->Add("SELECT b.*, a.plan_id FROM Plan_str a, Pokaztl b, " + tmpTblName + " t");
    if (int N = FormLogicF1->quForms->FieldByName("work_with")->AsInteger(); (N == 1 || N == 3 || N == 5) &&
                                                                             (p->plan_id > 1))
        quPlanStr->SQL->Add("WHERE a.plan_id IN (1, " + std::to_string(p->plan_id) + ")");
    else
        quPlanStr->SQL->Add("WHERE a.plan_id IN (1, " + std::to_string(FormLogicF1->CurrPlanId) + ")");
    quPlanStr->SQL->Add("AND b.pokaz_id = t.pokaz_id");
    quPlanStr->SQL->Add("AND a.pokaz_id = b.pokaz_id");
    // проверять корректность по дате не надо, это сделано в TDB_TmpPokaztlDr.CreateTmpByKOSGU

    if (isFilter2 == 6 || isFilter2 == 7) {// если заданы условия выборки по столбцам
        int wPlanId;
        if (int N = FormLogicF1->quForms->FieldByName("work_with")->AsInteger(); (N == 1 || N == 3 || N == 5) &&
                                                                                 (p->plan_id > 1))
            wPlanId = p->plan_id;
        else
            wPlanId = FormLogicF1->CurrPlanId;
        if (aDopAtrCol->is_planId)
            wPlanId = aDopAtrCol->plan_id;
        std::string aAndWhere;
        if (TForm2SelectCond(this).doForm2SelectCond(wPlanId, aAndWhere))
            quPlanStr->SQL->Add(aAndWhere);
    }

    quPlanStr->SQL->Add("ORDER BY rd, cs, vr, ku");
    quPlanStr->Open();

    while (!quPlanStr->EoF()) {
        CurrObject.pokaz_id = 0;
        CurrPokazId = 0;
        switch (FormLogicF1->quForms->FieldByName("work_with")->AsInteger()) {
            case 0:
            case 1:
                CurrPokazId = quPlanStr->FieldByName("pokaz_id").AsInteger();
                break;
            case 2:
                break;
            case 3:
            case 5:
                CurrObject.pokaz_id = quPlanStr->FieldByName("pokaz_id").AsInteger();
                CurrObject.pokaz_is_razd = quPlanStr->FieldByName("pokaz_is_razd").AsInteger();
                break;
        }

        if (aDopAtrCol->isVPredelaxDiapazonaKBK) {
            if (int N = quPlanStr->FieldByName("pokaz_is_razd").AsInteger(); N == 2 || N == 4) {
                if (!aDopAtrCol->compareKBK(p, quPlanStr->FieldByName("pokaz_bk_code").AsString()))
                    quPlanStr->Next();
                continue;
            }
        }

        int LevelType = quPlanStr->FieldByName("level_sign").AsInteger();
        std::string LevelText = quPlanStr->FieldByName("level_text").AsString();
        bool bBold = false, bItalic = false;
        if (Pos(".", OwnerLevelText) < 0 && Pos(".", LevelText) >= 0 &&
            (quPlanStr->FieldByName("pokaz_is_razd").AsInteger() == 2)) {
            // bBold = true;
            // bItalic = true;
        } else {
            bBold = false;
            bItalic = false;
        }
        switch (quPlanStr->FieldByName("pokaz_is_razd").AsInteger()) {
            case 1:
            case 3:
            case 5:
                bBold = true;
                if (int N = quPlanStr->FieldByName("pokaz_is_razd").AsInteger(); N == 3 || N == 5)
                    bItalic = true;
                break;
        }
        if (LevelType == 3)
            bItalic = true;
        if (LevelType == 1)
            LevelText = OwnerLevelText + LevelText;
        VertStr = quPlanStr->FieldByName("vert_str").AsString();
        bool IsParamRoot = false;
        if (IsParamForm == 1) // для параметрической формы
            if (RootPokazId == quPlanStr->FieldByName("pokaz_id").AsInteger()) // голова раскрутки
                IsParamRoot = true;
        bool IsOgrTipom = true;
        if (int N = FormLogicF1->quForms->FieldByName("work_with")->AsInteger(); (N == 3 || N == 5) &&
                                                                                 (FormLogicF1->quForms->FieldByName(
                                                                                         "ogr_class_id")->AsInteger() >
                                                                                  0)) {
            if (!IsParamRoot &&
                (FormLogicF1->quForms->FieldByName("ogr_class_id")->AsInteger() !=
                 quPlanStr->FieldByName("objekt_class_id")->AsInteger()))
                IsOgrTipom = false;
            if (int N = quPlanStr->FieldByName("pokaz_is_razd")->AsInteger(); N == 1 || N == 3 || N ==
                                                                                                  5) // если раздел, итого, то вставляем
                IsOgrTipom = true;
        }

        if (Copy(quPlanStr->FieldByName("pokaz_bk_code").AsString(), 1, 4) == "0001")
            IsOgrTipom = false;

        bool IsExecPokaz = false;
        if (IsOgrTipom && !IsParamRoot && !bSkipRow && (quPlanStr->FieldByName("pokaz_is_razd").AsInteger() > 0)) {
            TunProgress::ShowText(FormLogicF1, quPlanStr->FieldByName("pokaz_name").AsString());
            FormLogicF1->CurrInputRow = TForm2::AddNewRow(FormLogicF1, aOwnerNomRow,
                                                          quPlanStr->FieldByName("pokaz_owner_id").AsInteger(),
                                                          quPlanStr->FieldByName("pokaz_id").AsInteger());
            int RowVid = quPlanStr->FieldByName("pokaz_is_razd").AsInteger();
            if (aStmntRow == "КОРТЕЖ")
                RowVid = 13;
            TFormDopInp::SetListRowsItemAtr(FormLogicF1, FormLogicF1->CurrInputRow, RowVid,
                                            quPlanStr->FieldByName("level_sign").AsInteger(),
                                            quPlanStr->FieldByName("pokaz_vert").AsInteger(),
                                            quPlanStr->FieldByName("pokaz_owner_id").AsInteger(),
                                            quPlanStr->FieldByName("pokaz_id").AsInteger(),
                                            quPlanStr->FieldByName("objekt_class_id").AsInteger(),
                                            quPlanStr->FieldByName("pokaz_bk_code").AsString());
            TFormDopInp::SetFilter2OnRow(FormLogicF1, FormLogicF1->CurrInputRow, isFilter2, ColOnFilter2, isFilter);
            TFormDopInp::SetFormatStyle(FormLogicF1, FormLogicF1->CurrInputRow,
                                        TFormatStyle(aDopAtrCol->font, aDopAtrCol->H, aDopAtrCol->Is_bold,
                                                     aDopAtrCol->FontColor, aDopAtrCol->fonColor));
            TForm2::SetTextNameCol(FormLogicF1, FormLogicF1->CurrInputRow,
                                   quPlanStr->FieldByName("pokaz_name").AsString(),
                                   quPlanStr->FieldByName("edizm_id").AsInteger());
            TFormDopInp::SetDopAtrToAtrRow(FormLogicF1, FormLogicF1->CurrInputRow, aDopAtrCol);
            if (int N = FormLogicF1->quForms->FieldByName("work_with")->AsInteger(); (N == 1 || N == 3 || N == 5) &&
                                                                                     (p->plan_id > 1))
                ExecPokaz(aOwnerNomRow, quPlanStr, p->plan_id, aDopAtrCol, aStmntRow);
            else
                ExecPokaz(aOwnerNomRow, quPlanStr, 0, aDopAtrCol, aStmntRow);
            IsExecPokaz = true;
            TFormDopInp::SetToTerminal(FormLogicF1, FormLogicF1->CurrInputRow);
        }

        LNext:
        quPlanStr->Next();
    }
    quPlanStr->Free();
}

// выбор подчиненных показателей/объектов, заданных в описании формы через владельца
// только указанного вида
void TForm2::SelectPokazOnlyVid(const int aOwnerNomRow, std::string OwnerLevelText, std::string VertStr,
                                TQueryUni *aOwnerPlanStr, bool bSkipRow, int isFilter, int isFilter2, int ColOnFilter2,
                                PDopAtrCol aDopAtrCol, std::string aStmntRow, int aVid) {
    TQueryUni *quPlanStr;
    bool bBold, bItalic;
    std::string LevelText, ws;
    short LevelType;
    bool IsParamRoot;
    bool IsOgrTipom;
    bool IsExecPokaz;
    int RowVid;
    aOwnerPlanStr->Open();
    try {
        if (VertStr != "") {
            FormLogicF1->CurrInputRow = TForm2::AddNewRow(FormLogicF1, aOwnerNomRow,
                                                          aOwnerPlanStr->FieldByName("pokaz_owner_id")->AsInteger(),
                                                          aOwnerPlanStr->FieldByName("pokaz_id")->AsInteger());
            TFormDopInp::SetFormatStyle(FormLogicF1, FormLogicF1->CurrInputRow,
                                        TFormatStyle(aDopAtrCol->font, aDopAtrCol->H, aDopAtrCol->Is_bold,
                                                     aDopAtrCol->FontColor, aDopAtrCol->fonColor));
            TForm2::SetTextNameCol(FormLogicF1, FormLogicF1->CurrInputRow, VertStr, 0);
        }
        if (!aOwnerPlanStr->EoF()) {
            while (!aOwnerPlanStr->EoF()) {
                CurrObject.pokaz_is_razd = 0;
                CurrPokazId = 0;
                if (aOwnerPlanStr->FieldByName("pokaz_vert")->AsInteger() == 0 ||
                    aOwnerPlanStr->FieldByName("pokaz_vert")->AsInteger() == 3) {
                    if (aDopAtrCol->isVPredelaxDiapazonaKBK) {
                        if (int N = aOwnerPlanStr->FieldByName("pokaz_is_razd")->AsInteger(); N == 2 || N == 4) {
                            if (!aDopAtrCol->compareKBK(p, aOwnerPlanStr->FieldByName("pokaz_bk_code")->AsString())) {
                                goto LNext;
                            }
                        }
                    }
                }
                switch (FormLogicF1->quForms->FieldByName("work_with")->AsInteger()) {
                    case 0:
                    case 1:
                        CurrPokazId = aOwnerPlanStr->FieldByName("pokaz_id")->AsInteger();
                        break;
                    case 2:
                        break;
                    case 3:
                    case 5:
                        CurrObject.pokaz_id = aOwnerPlanStr->FieldByName("pokaz_id")->AsInteger();
                        CurrObject.pokaz_is_razd = aOwnerPlanStr->FieldByName("pokaz_is_razd")->AsInteger();
                        if (aOwnerPlanStr->FieldByName("pokaz_is_razd")->AsInteger() == 2) {
                            CurrObject.pokaz_id = 0;
                            CurrPokazId = aOwnerPlanStr->FieldByName("pokaz_id")->AsInteger();
                        }
                        break;
                }
                LevelType = aOwnerPlanStr->FieldByName("level_sign")->AsInteger();
                LevelText = aOwnerPlanStr->FieldByName("level_text")->AsString();
                if (Pos(".", OwnerLevelText) < 0 && Pos(".", LevelText) >= 0 &&
                    aOwnerPlanStr->FieldByName("pokaz_is_razd")->AsInteger() == 2) {
                    bBold = true;
                    bItalic = true;
                } else {
                    bBold = false;
                    bItalic = false;
                }
                switch (aOwnerPlanStr->FieldByName("pokaz_is_razd")->AsInteger()) {
                    case 1:
                    case 3:
                    case 5:
                        bBold = true;
                        bItalic = false;
                        if (int N = aOwnerPlanStr->FieldByName("pokaz_is_razd")->AsInteger(); N == 3 || N == 5) {
                            bItalic = true;
                        }
                        break;
                }
                if (LevelType == 3) {
                    bItalic = true;
                }
                if (LevelType == 1) {
                    LevelText = OwnerLevelText + LevelText;
                }
                VertStr = aOwnerPlanStr->FieldByName("vert_str")->AsString();
                IsParamRoot = false;
                if (IsParamForm == 1) {
                    if (RootPokazId == aOwnerPlanStr->FieldByName("pokaz_id")->AsInteger()) {
                        IsParamRoot = true;
                    }
                }
                IsOgrTipom = true;
                if (int N = FormLogicF1->quForms->FieldByName("work_with")->AsInteger(); N == 3 || N == 5) {
                    if (FormLogicF1->quForms->FieldByName("ogr_class_id")->AsInteger() > 0) {
                        if (!IsParamRoot && FormLogicF1->quForms->FieldByName("ogr_class_id")->AsInteger() !=
                                            aOwnerPlanStr->FieldByName("objekt_class_id")->AsInteger()) {
                            IsOgrTipom = false;
                        }
                        if (int N = aOwnerPlanStr->FieldByName("pokaz_is_razd")->AsInteger(); N == 1 || N == 3 || N ==
                                                                                                                  5) { // если раздел, итого, то вставляем
                            IsOgrTipom = true;
                        }
                    }
                }
                IsExecPokaz = false;
                if (isFilter > 0) {
                    switch (isFilter) {
                        case 1:
                        case 14:
                            IsOgrTipom = false;
                            if (aOwnerPlanStr->FieldByName("pokaz_vert")->AsInteger() == 0 ||
                                aOwnerPlanStr->FieldByName("pokaz_vert")->AsInteger() == 3) {
                                IsOgrTipom = true;
                            }
                            break;
                        case 2:
                            if (aOwnerPlanStr->FieldByName("pokaz_bk_code")->AsString().length() <
                                TDB_bkMetaDef::GetDl_R_CS_VR(FormLogicF1->CurrYear)) {
                                IsOgrTipom = false;
                            } else if (aOwnerPlanStr->FieldByName("pokaz_bk_code")->AsString().length() >
                                       TDB_bkMetaDef::GetDl_R_CS_VR(FormLogicF1->CurrYear)) {
                                return;
                            } else {
                                if (TDB_Pokaztl::getLengthKBKforSubPokaz(
                                        aOwnerPlanStr->FieldByName("pokaz_id")->AsInteger()) ==
                                    TDB_bkMetaDef::GetDl_R_CS_VR(FormLogicF1->CurrYear)) {
                                    IsOgrTipom = false;
                                } else {
                                    IsOgrTipom = true;
                                }
                                if (Copy(aOwnerPlanStr->FieldByName("pokaz_bk_code")->AsString(), 1, 4) == "0001") {
                                    IsOgrTipom = false;
                                }
                            }
                            break;
                        case 3:
                            break;
                    }
                }
                if (aOwnerPlanStr->FieldByName("pokaz_is_razd")->AsInteger() != aVid) {
                    IsOgrTipom = false;
                }
                if (isFilter == 8) {
                    if (aOwnerPlanStr->FieldByName("plan_id")->AsInteger() == 1) {
                        IsOgrTipom = false;
                    }
                }
                if (IsOgrTipom && !IsParamRoot && !bSkipRow &&
                    aOwnerPlanStr->FieldByName("pokaz_is_razd")->AsInteger() > 0) {
                    TunProgress::ShowText(FormLogicF1, aOwnerPlanStr->FieldByName("pokaz_name")->AsString());
                    FormLogicF1->CurrInputRow = TForm2::AddNewRow(FormLogicF1, aOwnerNomRow, aOwnerPlanStr->FieldByName(
                            "pokaz_owner_id")->AsInteger(), aOwnerPlanStr->FieldByName("pokaz_id")->AsInteger());
                    RowVid = aOwnerPlanStr->FieldByName("pokaz_is_razd")->AsInteger();
                    if (aStmntRow == "КОРТЕЖ") {
                        RowVid = 13;
                    }
                    TFormDopInp::SetListRowsItemAtr(FormLogicF1, FormLogicF1->CurrInputRow, RowVid,
                                                    aOwnerPlanStr->FieldByName("level_sign")->AsInteger(),
                                                    aOwnerPlanStr->FieldByName("pokaz_vert")->AsInteger(),
                                                    aOwnerPlanStr->FieldByName("pokaz_owner_id")->AsInteger(),
                                                    aOwnerPlanStr->FieldByName("pokaz_id")->AsInteger(),
                                                    aOwnerPlanStr->FieldByName("objekt_class_id")->AsInteger(),
                                                    aOwnerPlanStr->FieldByName("pokaz_bk_code")->AsString());
                    TFormDopInp::SetFilter2OnRow(FormLogicF1, FormLogicF1->CurrInputRow, isFilter2, ColOnFilter2,
                                                 isFilter);
                    TFormDopInp::SetFormatStyle(FormLogicF1, FormLogicF1->CurrInputRow,
                                                TFormatStyle(aDopAtrCol->font, aDopAtrCol->H, aDopAtrCol->Is_bold,
                                                             aDopAtrCol->FontColor, aDopAtrCol->fonColor));
                    TFormDopInp::SetDopAtrToAtrRow(FormLogicF1, FormLogicF1->CurrInputRow, aDopAtrCol);
                    TForm2::SetTextNameCol(FormLogicF1, FormLogicF1->CurrInputRow,
                                           aOwnerPlanStr->FieldByName("pokaz_name")->AsString(),
                                           aOwnerPlanStr->FieldByName("edizm_id")->AsInteger());
                    if (int N = FormLogicF1->quForms->FieldByName("work_with")->AsInteger();
                            (N == 1 || N == 3 || N == 5) && p->plan_id > 1) {
                        ExecPokaz(aOwnerNomRow, aOwnerPlanStr, p->plan_id, aDopAtrCol, aStmntRow);
                    } else {
                        ExecPokaz(aOwnerNomRow, aOwnerPlanStr, 0, aDopAtrCol, aStmntRow);
                    }
                }
                IsExecPokaz = true;
            }
            switch (FormLogicF1->quForms->FieldByName("work_with")->AsInteger()) {
                case 0:
                case 1:
                    isSelectSubItem = true;
                    break;
                case 2:
                    break;
                case 3:
                case 5:
                    int N = aOwnerPlanStr->FieldByName("pokaz_is_razd")->AsInteger();
                    isSelectSubItem = (N == 0 || N == 1 || N == 4);
                    if (IsParamRoot) {
                        isSelectSubItem = true;
                    } else {
                        if (FormLogicF1->quForms->FieldByName("ogr_class_id")->AsInteger() > 0 &&
                            FormLogicF1->quForms->FieldByName("ogr_class_id")->AsInteger() !=
                            aOwnerPlanStr->FieldByName("objekt_class_id")->AsInteger()) {
                            isSelectSubItem = true;
                        }
                        if (FormLogicF1->quForms->FieldByName("ogr_class_id")->AsInteger() > 0 &&
                            FormLogicF1->quForms->FieldByName("ogr_class_id")->AsInteger() ==
                            aOwnerPlanStr->FieldByName("objekt_class_id")->AsInteger() &&
                            aOwnerPlanStr->FieldByName("pokaz_vert")->AsInteger() == 1) {
                            isSelectSubItem = true;
                        }
                    }
                    if (aOwnerPlanStr->FieldByName("pokaz_is_razd")->AsInteger() == 2 &&
                        aOwnerPlanStr->FieldByName("pokaz_vert")->AsInteger() > 0) {
                        isSelectSubItem = true;
                        if (aOwnerPlanStr->FieldByName("pokaz_vert")->AsInteger() == 3) {
                            if (TDB_bkMetaDef::GetVidById(aOwnerPlanStr->FieldByName("enum_id")->AsInteger()) == 3) {
                                isSelectSubItem = false;
                            }
                        }
                    }
                    break;
            }
            if (aOwnerPlanStr->FieldByName("pokaz_is_razd")->AsInteger() > 1 &&
                (aOwnerPlanStr->FieldByName("pokaz_vert")->AsInteger() == 0 ||
                 aOwnerPlanStr->FieldByName("pokaz_vert")->AsInteger() == 3)) {
                isSelectSubItem = false;
            }
            if (isSelectSubItem) {
                TQueryUni *quPlanStr = new TQueryUni(DefDBconnect.Conn);
                quPlanStr->SQL->Add("SELECT * FROM Plan_str a, Pokaztl b");
                if (int N = FormLogicF1->quForms->FieldByName("work_with")->AsInteger(); N == 1 || N == 3 ||
                                                                                         N == 5 && p->plan_id > 1) {
                    quPlanStr->SQL->Add("WHERE plan_id IN (1, " + IntToStr(p->plan_id) + ")");
                } else {
                    quPlanStr->SQL->Add("WHERE plan_id IN (1, " + IntToStr(FormLogicF1->CurrPlanId) + ")");
                }
                switch (FormLogicF1->quForms->FieldByName("work_with")->AsInteger()) {
                    case 0:
                    case 1:
                        quPlanStr->SQL->Add("AND plan_str_owner_id = " + IntToStr(CurrPokazId));
                        break;
                    case 2:
                        break;
                    case 3:
                    case 5:
                        if (aOwnerPlanStr->FieldByName("pokaz_is_razd")->AsInteger() == 2) {
                            quPlanStr->SQL->Add("AND plan_str_owner_id = " + IntToStr(CurrPokazId));
                        } else {
                            quPlanStr->SQL->Add("AND plan_str_owner_id = " + IntToStr(CurrObject.pokaz_id));
                        }
                        break;
                }
                quPlanStr->SQL->Add("AND a.pokaz_id = b.pokaz_id");
                switch (GetDBType()) {
                    case TDBType::MSSQL:
                        quPlanStr->SQL->Add("AND " + IntToStr(FormLogicF1->CurrYear) +
                                            " BETWEEN YEAR(b.pokaz_beg_date) AND YEAR(b.pokaz_end_date)");
                        break;
                    case TDBType::Postgre:
                        quPlanStr->SQL->Add("AND " + IntToStr(FormLogicF1->CurrYear) +
                                            " BETWEEN date_part('year', b.pokaz_beg_date) AND date_part('year', b.pokaz_end_date)");
                        break;
                }
                quPlanStr->SQL->Add(
                        "ORDER BY CASE WHEN plan_id > 1 THEN plan_str_order+400 WHEN plan_id = 1 and plan_str_order = 1000 THEN plan_str_order+10000 ELSE plan_str_order END");
                if (IsExecPokaz) {
                    SelectPokazOnlyVid(FormLogicF1->CurrInputRow, LevelText, VertStr, quPlanStr, false, isFilter,
                                       isFilter2, ColOnFilter2, aDopAtrCol, aStmntRow, aVid);
                } else {
                    SelectPokazOnlyVid(aOwnerNomRow, LevelText, "", quPlanStr, false, isFilter, isFilter2, ColOnFilter2,
                                       aDopAtrCol, aStmntRow, aVid);
                }
                delete quPlanStr;
            }
            if (RootPokazId == aOwnerPlanStr->FieldByName("pokaz_owner_id")->AsInteger()) {
                TunProgress::doProcessMessages();
            }
            LNext:
            aOwnerPlanStr->Next();

        } else {
            TFormDopInp::SetToTerminal(FormLogicF1, FormLogicF1->CurrInputRow);
        }
    } catch (exception &E) {
        TracerManager::Instance().AddSpanLog("Exception.TForm2::SelectPokazOnlyVid", {{"errordesr", E.what()}},
                                             TracerManager::LOG_LEVEL_ERROR);
        FormLogicF1->FormDM->quValue->Close();
        FormLogicF1->quCell->Close();
        aOwnerPlanStr->Close();
        //ws = E;
        //throw Exception(ws);
    }
    aOwnerPlanStr->Close();
}

// выбор сортированных подчиненных показателей, заданных в описании формы через владельца
void
TForm2::SelectSortsPokaz(const int aOwnerNomRow, TQueryUni *prOwnerPlanStr, PDopAtrCol aDopAtrCol, const int isFilter,
                         const int isFilter2, const int ColOnFilter2) {
    TQueryUni *quPlanStr;
    bool IsParamRoot;
    bool IsOgrTipom;
    bool IsExecPokaz;
    int wPlanId;
    std::string aAndWhere;
    prOwnerPlanStr->Open();
    try {
        if (!prOwnerPlanStr->EoF()) {
            while (!prOwnerPlanStr->EoF()) {
                CurrObject.pokaz_id = 0;
                CurrPokazId = 0;
                bItalic = false;
                bBold = false;
                switch (FormLogicF1->quForms->FieldByName("work_with")->AsInteger()) {
                    case 0:
                    case 1:
                        CurrPokazId = prOwnerPlanStr->FieldByName("pokaz_id")->AsInteger();
                        break;
                    case 2:
                        break;
                    case 3:
                    case 5:
                        CurrObject.pokaz_id = prOwnerPlanStr->FieldByName("pokaz_id")->AsInteger();
                        CurrObject.pokaz_is_razd = prOwnerPlanStr->FieldByName("pokaz_is_razd")->AsInteger();
                        break;
                }
                IsParamRoot = false;
                if (IsParamForm == 1) {
                    if (RootPokazId == prOwnerPlanStr->FieldByName("pokaz_id")->AsInteger()) {
                        IsParamRoot = true;
                    }
                }
                IsOgrTipom = true;
                if (int N = FormLogicF1->quForms->FieldByName("work_with")->AsInteger(); N == 3 || N == 5) {
                    if (FormLogicF1->quForms->FieldByName("ogr_class_id")->AsInteger() > 0) {
                        if (!IsParamRoot && FormLogicF1->quForms->FieldByName("ogr_class_id")->AsInteger() !=
                                            prOwnerPlanStr->FieldByName("objekt_class_id")->AsInteger()) {
                            IsOgrTipom = false;
                        }
                        if (prOwnerPlanStr->FieldByName("pokaz_is_razd")->AsInteger() == 1) {
                            IsOgrTipom = true;
                        }
                    }
                }
                IsExecPokaz = false;
                if (isFilter > 0) {
                    switch (isFilter) {
                        case 1:
                        case 14:
                            IsOgrTipom = false;
                            if (TDB_bkMetaDef::GetVidById(prOwnerPlanStr->FieldByName("enum_id")->AsInteger()) == 3 &&
                                (prOwnerPlanStr->FieldByName("pokaz_vert")->AsInteger() == 0 ||
                                 prOwnerPlanStr->FieldByName("pokaz_vert")->AsInteger() == 3)) {
                                IsOgrTipom = true;
                            }
                            break;
                        case 2:
                            if (prOwnerPlanStr->FieldByName("pokaz_bk_code")->AsString().length() <
                                TDB_bkMetaDef::GetDl_R_CS_VR(FormLogicF1->CurrYear)) {
                                IsOgrTipom = false;
                            } else if (prOwnerPlanStr->FieldByName("pokaz_bk_code")->AsString().length() >
                                       TDB_bkMetaDef::GetDl_R_CS_VR(FormLogicF1->CurrYear)) {
                                return;
                            } else {
                                if (TDB_Pokaztl::getLengthKBKforSubPokaz(
                                        prOwnerPlanStr->FieldByName("pokaz_id")->AsInteger()) ==
                                    TDB_bkMetaDef::GetDl_R_CS_VR(FormLogicF1->CurrYear)) {
                                    IsOgrTipom = false;
                                } else {
                                    IsOgrTipom = true;
                                    if (prOwnerPlanStr->FieldByName("pokaz_bk_code")->AsString().substr(0, 4) ==
                                        "0001") {
                                        IsOgrTipom = false;
                                    }
                                }
                            }
                            break;
                        case 9:
                            if (prOwnerPlanStr->FieldByName("pokaz_bk_code")->AsString().length() < 4) {
                                IsOgrTipom = false;
                            } else if (prOwnerPlanStr->FieldByName("pokaz_bk_code")->AsString().length() > 4) {
                                return;
                            } else {
                                if (TDB_Pokaztl::getLengthKBKforSubPokaz(
                                        prOwnerPlanStr->FieldByName("pokaz_id")->AsInteger()) > 4) {
                                    IsOgrTipom = true;
                                } else {
                                    IsOgrTipom = false;
                                }
                                if (prOwnerPlanStr->FieldByName("pokaz_bk_code")->AsString().substr(0, 4) == "0001") {
                                    IsOgrTipom = false;
                                }
                            }
                            break;
                    }
                }
                if (int N = prOwnerPlanStr->FieldByName("pokaz_is_razd")->AsInteger(); IsOgrTipom && !IsParamRoot &&
                                                                                       (N == 2 || N == 4)) {
                    TunProgress::ShowText(FormLogicF1, prOwnerPlanStr->FieldByName("pokaz_name")->AsString());
                    FormLogicF1->CurrInputRow = TForm2::AddNewRow(FormLogicF1, aOwnerNomRow,
                                                                  prOwnerPlanStr->FieldByName(
                                                                          "pokaz_owner_id")->AsInteger(),
                                                                  prOwnerPlanStr->FieldByName("pokaz_id")->AsInteger());
                    TFormDopInp::SetListRowsItemAtr(FormLogicF1, FormLogicF1->CurrInputRow,
                                                    prOwnerPlanStr->FieldByName("pokaz_is_razd")->AsInteger(),
                                                    prOwnerPlanStr->FieldByName("level_sign")->AsInteger(),
                                                    prOwnerPlanStr->FieldByName("pokaz_vert")->AsInteger(),
                                                    prOwnerPlanStr->FieldByName("pokaz_owner_id")->AsInteger(),
                                                    prOwnerPlanStr->FieldByName("pokaz_id")->AsInteger(),
                                                    prOwnerPlanStr->FieldByName("objekt_class_id")->AsInteger(),
                                                    prOwnerPlanStr->FieldByName("pokaz_bk_code")->AsString());
                    TFormDopInp::SetDopAtrToAtrRow(FormLogicF1, FormLogicF1->CurrInputRow, aDopAtrCol);
                    TFormDopInp::SetFilter2OnRow(FormLogicF1, FormLogicF1->CurrInputRow, isFilter2, ColOnFilter2,
                                                 isFilter);
                    TFormDopInp::SetFormatStyle(FormLogicF1, FormLogicF1->CurrInputRow,
                                                TFormatStyle(aDopAtrCol->font, aDopAtrCol->H, aDopAtrCol->Is_bold,
                                                             aDopAtrCol->FontColor, aDopAtrCol->fonColor));
                    TForm2::SetTextNameCol(FormLogicF1, FormLogicF1->CurrInputRow,
                                           prOwnerPlanStr->FieldByName("pokaz_name")->AsString(),
                                           prOwnerPlanStr->FieldByName("edizm_id")->AsInteger());
                    TFormDopInp::SetInSort(FormLogicF1, FormLogicF1->CurrInputRow);
                    ExecPokaz(aOwnerNomRow, prOwnerPlanStr, 0, aDopAtrCol, "");
                    IsExecPokaz = true;
                }
                switch (FormLogicF1->quForms->FieldByName("work_with")->AsInteger()) {
                    case 0:
                    case 1:
                        isSelectSubItem = true;
                        break;
                    case 2:
                        break;
                    case 3:
                    case 5:
                        int N = prOwnerPlanStr->FieldByName("pokaz_is_razd")->AsInteger();
                        isSelectSubItem = (N == 0 || N == 1);
                        if (IsParamRoot) {
                            isSelectSubItem = true;
                        } else {
                            if (FormLogicF1->quForms->FieldByName("ogr_class_id")->AsInteger() > 0 &&
                                FormLogicF1->quForms->FieldByName("ogr_class_id")->AsInteger() !=
                                prOwnerPlanStr->FieldByName("objekt_class_id")->AsInteger()) {
                                isSelectSubItem = true;
                            }
                            if (FormLogicF1->quForms->FieldByName("ogr_class_id")->AsInteger() > 0 &&
                                FormLogicF1->quForms->FieldByName("ogr_class_id")->AsInteger() ==
                                prOwnerPlanStr->FieldByName("objekt_class_id")->AsInteger() &&
                                prOwnerPlanStr->FieldByName("pokaz_vert")->AsInteger() == 1) {
                                isSelectSubItem = true;
                            }
                        }
                        break;
                }
                if (prOwnerPlanStr->FieldByName("pokaz_is_razd")->AsInteger() > 1 &&
                    (prOwnerPlanStr->FieldByName("pokaz_vert")->AsInteger() == 0 ||
                     prOwnerPlanStr->FieldByName("pokaz_vert")->AsInteger() == 3)) {
                    isSelectSubItem = false;
                }
                if (isSelectSubItem) {
                    TQueryUni *quPlanStr = new TQueryUni(DefDBconnect.Conn);
                    quPlanStr->SQL->Add("SELECT * FROM Plan_str a, Pokaztl b");
                    quPlanStr->SQL->Add("WHERE plan_id IN (1, " + std::to_string(FormLogicF1->CurrPlanId) + ")");
                    switch (FormLogicF1->quForms->FieldByName("work_with")->AsInteger()) {
                        case 0:
                        case 1:
                            quPlanStr->SQL->Add("AND plan_str_owner_id = " + std::to_string(CurrPokazId));
                            break;
                        case 2:
                            break;
                        case 3:
                        case 5:
                            quPlanStr->SQL->Add("AND plan_str_owner_id = " + std::to_string(CurrObject.pokaz_id));
                            break;
                    }
                    quPlanStr->SQL->Add("AND a.pokaz_id = b.pokaz_id");
                    switch (GetDBType()) {
                        case TDBType::MSSQL:
                            quPlanStr->SQL->Add("AND " + std::to_string(FormLogicF1->CurrYear) +
                                                " BETWEEN YEAR(b.pokaz_beg_date) AND YEAR(b.pokaz_end_date)");
                            break;
                        case TDBType::Postgre:
                            quPlanStr->SQL->Add("AND " + std::to_string(FormLogicF1->CurrYear) +
                                                " BETWEEN date_part('year', b.pokaz_beg_date) AND date_part('year', b.pokaz_end_date)");
                            break;
                    }
                    if (isFilter2 == 6 || isFilter2 == 7) {
                        int N = FormLogicF1->quForms->FieldByName("work_with")->AsInteger();
                        if ((N == 1 || N == 3) && (p->plan_id > 1)) {
                            wPlanId = p->plan_id;
                        } else {
                            wPlanId = FormLogicF1->CurrPlanId;
                        }
                        if (aDopAtrCol->is_planId) {
                            wPlanId = aDopAtrCol->plan_id;
                        }
                        if (TForm2SelectCond(this).doForm2SelectCond(wPlanId, aAndWhere)) {
                            quPlanStr->SQL->Add(aAndWhere);
                        }
                    }
                    quPlanStr->SQL->Add(
                            "ORDER BY CASE WHEN plan_id > 1 THEN plan_str_order+400 WHEN plan_id = 1 and plan_str_order = 1000 THEN plan_str_order+10000 ELSE plan_str_order END");
                    if (IsExecPokaz) {
                        SelectSortsPokaz(FormLogicF1->CurrInputRow, quPlanStr, aDopAtrCol, isFilter, isFilter2,
                                         ColOnFilter2);
                    } else {
                        SelectSortsPokaz(aOwnerNomRow, quPlanStr, aDopAtrCol, isFilter, isFilter2, ColOnFilter2);
                    }
                    delete quPlanStr;
                }
                prOwnerPlanStr->Next();
            }
        } else {
            TFormDopInp::SetToTerminal(FormLogicF1, FormLogicF1->CurrInputRow);
        }
    } catch (std::exception &e) {
        TracerManager::Instance().AddSpanLog("Exception.TForm2::SelectSortsPokaz", {{"errordesr", e.what()}},
                                             TracerManager::LOG_LEVEL_ERROR);
        FormLogicF1->FormDM->quValue->Close();
        FormLogicF1->quCell->Close();
        prOwnerPlanStr->Close();
        //throw Exception("Не идентифицированная ошибка");
    }
    prOwnerPlanStr->Close();
}

void TForm2::DoReadonlyHidden(int nRowFrom, int nRowTo, int aReadonly, int aIs_hidden) {
    int wi;
    int pRow1, pCol1, pRow2, pCol2;
    PAtrRow AtrRow;

    FormLogicF1->f1Forms->Selection_Get(0, pRow1, pCol1, pRow2, pCol2);

    for (wi = nRowFrom; wi <= nRowTo; wi++) {
        AtrRow = TFormDopInp::GetListRowsItem(FormLogicF1, wi);

        switch (aReadonly) {
            case 0:
                AtrRow->isReadOnly = false;
                AtrRow->isRefresh = false;
                break;
            case 1:
                AtrRow->isReadOnly = true;
                AtrRow->isRefresh = false;
                break;
            case 2:
                AtrRow->isReadOnly = true;
                AtrRow->isRefresh = true;
                p->isRefreshRows = true;
                break;
        }

        FormLogicF1->f1Forms->Selection_Set(wi, 1, wi, 256);

        if (aReadonly == 1) {
            if (AtrRow->isRow1n) {
                if (AtrRow->isRow1nReadOnly) {
                    FormLogicF1->f1Forms->Protection_Set(true, true);
                }
            } else {
                FormLogicF1->f1Forms->Protection_Set(true, true);
            }
        }

        if (aIs_hidden == 1) {
            if (AtrRow->isRow1n) {
                if (AtrRow->isRow1nhidden) {
                    FormLogicF1->f1Forms->RowHidden[wi] = true;
                }
            } else {
                FormLogicF1->f1Forms->RowHidden[wi] = true;
                AtrRow->isRowHidden = 1;
            }
        }
    }

    FormLogicF1->f1Forms->Selection_Set(pRow1, pCol1, pRow2, pCol2);
}

void TForm2::SelectSpStrok(int blockLevelUr, int aForm_id, int aWork_with, bool aIs_sort, const int aOwnerNomRow,
                           int isFilter2Block, int ColOnFilter2Block, int plan_idBlock) {
    TracerManager::Instance().StartFunctionTrace("TForm2::SelectSpStrok");
    TQueryUni *quPlanStr;
    string s, stmntCol, stmntW, ws;
    string strName, strPokazId, shablon, oldshablon;
    int i, n, RowVid, edIzm_id, saveCurrPlanId, newCurrPlanId, wCol, wn, newPokazCol;
    PCellValueAtr vd, wvd;
    int ist_fin_grad_id, level_sign, Is_open, readonly, is_hidden, OldRow, noPrint, is_filter;
    int is_filter2, ColOnFilter2, is_filter2W, ColOnFilter2W, Filter2Variant;
    int nStolbec;
    TDopAtrCol DopAtrCol;
    TFormatStyle *FormatStyle = new TFormatStyle();
    unsigned char Align_vert, Align_gor;
    string font;
    unsigned char h;
    unsigned char is_bold;
    PAtrRow AtrRow, wAtrRow;
    int plan_id_2podvedomstv, wotkuda;
    int tocnost;
    json JsonSerializer;

    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    if (q) {
        if (aWork_with == 10 || aWork_with == 11 || aWork_with == 12) {
            TDB_FormStrTMP::GetItemsAll(q, aForm_id);
        } else {
            TDB_FormStr::GetItemsAll(q, aForm_id);
        }
        if (blockLevelUr == 0) {
            TFormSsilkaNaStr::CreateSL();
        }
        while (!q->EoF()) {
            if (q->FieldByName("form_str_n")->AsInteger() < 1500 || q->FieldByName("form_str_n")->AsInteger() > 9999) {
                if (AnsiUpperCase(q->FieldByName("stmnt")->AsString()) == "ODINAKOVZNAC") {
                    p->stmntODINAKOVZNAC = q->FieldByName("note2")->AsString();
                }
                q->Next();
                continue;
            }
            strName = q->FieldByName("note1")->AsString();
            note2 = q->FieldByName("note2")->AsString();
            nStolbec = q->FieldByName("attr_2")->AsInteger();
            level_sign = q->FieldByName("attr_3")->AsInteger();
            Is_open = q->FieldByName("attr_4")->AsInteger();
            readonly = q->FieldByName("readonly")->AsInteger();
            is_hidden = q->FieldByName("is_hidden")->AsInteger();
            CurrPokazId = q->FieldByName("pokaz_id")->AsInteger();
            saveCurrPlanId = FormLogicF1->CurrPlanId;
            if (blockLevelUr == 1 && plan_idBlock != 0) {
                newCurrPlanId = plan_idBlock;
            } else {
                newCurrPlanId = q->FieldByName("plan_id")->AsInteger();
                p->wwPlan_id = q->FieldByName("plan_id")->AsInteger();
            }
            if (newCurrPlanId == 1) {
                newCurrPlanId = TDB_Plan::GetPodVedomstvId(FormLogicF1->CurrPlanId, false, plan_id_2podvedomstv);
            }
            Align_vert = q->FieldByName("align_vert")->AsInteger();
            Align_gor = q->FieldByName("align_gor")->AsInteger();
            font = q->FieldByName("font")->AsString();
            h = q->FieldByName("h")->AsInteger();
            is_bold = q->FieldByName("is_bold")->AsInteger();
            noPrint = q->FieldByName("is_group")->AsInteger();
            is_filter = q->FieldByName("is_filter")->AsInteger();
            oldshablon = shablon;
            shablon = q->FieldByName("shablon")->AsString();
            if (is_filter == 10) {
                shablon = TForm2PlusPatternKBK::createLikePattern(FormLogicF1->CurrYear, shablon);
            }
            if (is_filter == 13) {
                if (FormLogicF1->aShablonKBK == "") {
                    FormLogicF1->aShablonKBK = "%";
                }
                shablon = FormLogicF1->aShablonKBK;
            }
            if (!(is_filter == 3 || is_filter == 4 || is_filter == 5 || is_filter == 6 || is_filter == 10 ||
                  is_filter == 13)) {
                oldshablon = "";
                shablon = "";
            }
            if (blockLevelUr == 1 && isFilter2Block != 0) {
                is_filter2 = isFilter2Block;
            } else {
                is_filter2 = q->FieldByName("is_filter2")->AsInteger();
            }
            if (blockLevelUr == 1 && ColOnFilter2Block != 0) {
                ColOnFilter2 = ColOnFilter2Block;
            } else {
                ColOnFilter2 = q->FieldByName("ColOnFilter2")->AsInteger();
            }
            DopAtrCol.SetDopAtrColQuery(q);
            if (is_filter2 == 6 || is_filter2 == 7) {
                Filter2Variant = q->FieldByName("isorientation")->AsInteger();
                TForm2SelectCond(this).doEvalAtrSelectCond(&DopAtrCol, FormLogicF1->CurrYear, FormLogicF1->CurrVidOtch,
                                                           FormLogicF1->CurrQuarter, FormLogicF1->CurrMonth,
                                                           Filter2Variant);
            }
            if (blockLevelUr == 1 && plan_idBlock != 0) {
                DopAtrCol.plan_id = plan_idBlock;
                DopAtrCol.is_planId = true;
            }
            if (q->FieldByName("stmnt")->AsString() == "БЛОК") {
                if (blockLevelUr > 0) {
                    is_filter2W = is_filter2;
                    ColOnFilter2W = ColOnFilter2;
                } else {
                    is_filter2W = q->FieldByName("is_filter2")->AsInteger();
                    ColOnFilter2W = q->FieldByName("ColOnFilter2")->AsInteger();
                }
                SelectSpStrok(1, q->FieldByName("field_id")->AsInteger(), 0, aIs_sort, aOwnerNomRow, is_filter2W,
                              ColOnFilter2W, q->FieldByName("plan_id")->AsInteger());
            } else if (q->FieldByName("stmnt")->AsString() == "АГРЕГАТ") {
                TForm2Agregat(this).doForm2Agregat(aOwnerNomRow, newCurrPlanId, strName, note2, level_sign, readonly,
                                                   is_hidden, is_filter2, ColOnFilter2, nStolbec, font, h, is_bold, q);
            } else if (q->FieldByName("stmnt")->AsString() == "УСЛОВИЕ") {
                TForm2Uslovie(this).doForm2Uslovie(aOwnerNomRow, newCurrPlanId, strName, note2, level_sign, readonly,
                                                   is_hidden, is_filter2, ColOnFilter2, nStolbec, font, h, is_bold, q);
            } else if (q->FieldByName("stmnt")->AsString() == "ПРИСВОИТЬ") {
                TForm2Prisvoit(this).doForm2Prisvoit(aOwnerNomRow, newCurrPlanId, strName, note2, level_sign, Is_open,
                                                     readonly, is_hidden, is_filter2, ColOnFilter2, nStolbec, font, h,
                                                     is_bold, Align_vert, Align_gor, q,
                                                     q->FieldByName("isGroupSelect")->AsInteger());
            } else if (q->FieldByName("stmnt")->AsString() == "ПОЛОЖИТЬ") {
                TForm2Pologit(this).doForm2Pologit(aOwnerNomRow, newCurrPlanId, strName, note2, level_sign, Is_open,
                                                   readonly, is_hidden, is_filter2, ColOnFilter2, nStolbec, font, h,
                                                   is_bold, Align_vert, Align_gor, q);
            } else if (q->FieldByName("stmnt")->AsString() == "СУММАшаблонКБК") {
                if (nStolbec > 0) {
                    strName = strName + "#" + IntToStr(nStolbec) + "#";
                }
                FormLogicF1->CurrInputRow = TForm2::AddNewRow(FormLogicF1, aOwnerNomRow, 0, 0);
                RowVid = 15;
                TFormDopInp::SetListRowsItemAtr(FormLogicF1, FormLogicF1->CurrInputRow, RowVid, level_sign, 0, 0,
                                                q->FieldByName("pokaz_id")->AsInteger(), 0, "");
                TFormDopInp::SetFormatStyle(FormLogicF1, FormLogicF1->CurrInputRow,
                                            TFormatStyle(font, h, is_bold, 0, 0));
                TFormDopInp::SetFilter2OnRow(FormLogicF1, FormLogicF1->CurrInputRow, is_filter2, ColOnFilter2,
                                             is_filter);
                TForm2::SetTextNameCol(FormLogicF1, FormLogicF1->CurrInputRow, strName, 0);
                AtrRow = TFormDopInp::GetListRowsItem(FormLogicF1, FormLogicF1->CurrInputRow);
                AtrRow->str_id = q->FieldByName("form_str_n")->AsInteger();
                //MAK AtrRow->pokaz_bk_code = TForm2PlusPatternKBK::createLikePattern(FormLogicF1->CurrYear, q->FieldByName("shablon")->AsString());
                if (is_hidden == 0) {
                    if (FormLogicF1->quForms->FieldByName("is_sort")->AsInteger() == 1) {
                        TFormDopInp::SetInSort(FormLogicF1, FormLogicF1->CurrInputRow);
                    }
                }
                TForm2PlusPatternKBK::ExecPokazPatternKBK(&DopAtrCol, FormLogicF1, FormLogicF1->CurrInputRow, p, q);
                DoReadonlyHidden(FormLogicF1->CurrInputRow, FormLogicF1->CurrInputRow, readonly, is_hidden);
            } else if (q->FieldByName("stmnt")->AsString() == "ПОКАЗАТЕЛЬ" ||
                       q->FieldByName("stmnt")->AsString() == "КОРТЕЖ") {
                TForm2PokazKorteg(this).doForm2PokazKorteg(aIs_sort, aOwnerNomRow, newCurrPlanId, saveCurrPlanId,
                                                           strName, note2, shablon, oldshablon, level_sign, Is_open,
                                                           readonly, is_hidden, is_filter2, ColOnFilter2, noPrint,
                                                           is_filter, nStolbec, font, h, is_bold, Align_vert, Align_gor,
                                                           &DopAtrCol, q);
            } else if (q->FieldByName("stmnt")->AsString() == "ССЫЛКАНАСТР") {
                TFormSsilkaNaStr::AddSsilkaNaStr(strName, FormLogicF1->CurrInputRow + 1);
            } else if (q->FieldByName("stmnt")->AsString() == "ЗАМЕНА") {
                TForm2Plus::doZamenaInForm2(p, FormLogicF1, strName, note2);
            } else if (q->FieldByName("stmnt")->AsString() == "КНОПКА") {
                FormLogicF1->CurrInputRow = TForm2::AddNewRow(FormLogicF1, aOwnerNomRow, 0, 0);
                FormLogicF1->f1Forms->RowHeight[FormLogicF1->CurrInputRow] = PixelsToTwips(h);
                AtrRow = TFormDopInp::GetListRowsItem(FormLogicF1, FormLogicF1->CurrInputRow);
                AtrRow->msg = q->FieldByName("msg")->AsString();
                for (i = 1; i <= p->MaxHrCol; i++) {
                    ws = TFormDopInp::GetFormulaForColpar(note2, i);
                    ws = TFormDopInp::ReplaceNRowNCol(FormLogicF1->CurrInputRow, i, ws);
                    if (ws != "") {
                        FormatStyle->setToNull();
                        FormatStyle->Align_vert = Align_vert;
                        FormatStyle->align_gor = Align_gor;
                        TForm1::SetCellValue(FormLogicF1, FormLogicF1->f1Forms, p->MaxHrCol, FormLogicF1->CurrInputRow,
                                             i, FormLogicF1->CurrInputRow, i, ws, 0, FormatStyle);
                        AtrRow->ColOnFilter2 = i;
                    }
                }
                InputButton(FormLogicF1, FormLogicF1->CurrInputRow, q->FieldByName("attr_1")->AsInteger(),
                            PixelsToTwips(h), q->FieldByName("note1")->AsString());
            } else if (q->FieldByName("stmnt")->AsString() == "ПОКАЗАТЕЛЬотложен") {
                TForm2PokazOtlog(this).doForm2PokazOtlog(aIs_sort, aOwnerNomRow, newCurrPlanId, saveCurrPlanId, strName,
                                                         note2, shablon, oldshablon, level_sign, Is_open, readonly,
                                                         is_hidden, is_filter2, ColOnFilter2, noPrint, is_filter,
                                                         nStolbec, font, h, is_bold, Align_vert, Align_gor, &DopAtrCol,
                                                         q);
            } else if (q->FieldByName("stmnt")->AsString() == "ОбъектыПоСвязи1:N") {
                if (work_with != 5) {
                    //throw Exception("Оператор \"ОбъектыПоСвязи1:N\" может быть использован только в форме Список объектов и показателей");
                }
                DopAtrCol.readonly = readonly;
                DopAtrCol.is_hidden = is_hidden;
                DopAtrCol.is_filter = is_filter;
                DopAtrCol.is_filter2 = is_filter2;
                DopAtrCol.ColOnFilter2 = ColOnFilter2;
                DopAtrCol.OtkudaOwner = q->FieldByName("attr_4")->AsInteger();
                if (saveCurrPlanId == 0) {
                    DopAtrCol.plan_id = p->wwPlan_id;
                } else {
                    DopAtrCol.plan_id = saveCurrPlanId;
                }
                DopAtrCol.class_idItems = p->ogr_class_id;
                switch (DopAtrCol.OtkudaOwner) {
                    case 1: {
                        DopAtrCol.owner_id_1N = FormLogicF1->aId_Owner_link1n;
                        TForm2Owner1N(this).doForm2Owner1N(DopAtrCol.plan_id, &DopAtrCol, FormLogicF1->CurrInputRow);
                        break;
                    }
                    case 2: {
                        DopAtrCol.isIgnor = (DopAtrCol.is_set_god == 2);
                        break;
                    }
                }
            } else if (q->FieldByName("stmnt")->AsString() == "Стиль оформления") {
                if (p->StyleOforml != NULL) {
                    delete p->StyleOforml;
                }
                p->StyleOforml = TStyleOforml::Deserialize(q->FieldByName("note2")->AsString());
            } else {
                if (nStolbec > 0) {
                    strName = strName + "#" + IntToStr(nStolbec) + "#";
                }
                FormLogicF1->CurrInputRow = TForm2::AddNewRow(FormLogicF1, aOwnerNomRow, 0, 0);
                RowVid = 10;
                if (q->FieldByName("stmnt")->AsString() == "ПРОИЗВСТР") {
                    if (q->FieldByName("note1")->AsString() ==
                        "::При активации формы обновить \"только-чтение\" ячейки показателей") {
                        p->isOnActivForm_RefreshROcells = true;
                    } else if (q->FieldByName("note1")->AsString() ==
                               "::После обработки момент-ых присваиваний выполнить все вспомогательные формы") {
                        p->isExecAllVspomogForms = true;
                    }
                }
                if (q->FieldByName("stmnt")->AsString() == "ФОРМУЛАF1") {
                    RowVid = 11;
                    if (q->FieldByName("is_svod")->AsInteger() == 1) {
                        RowVid = 23;
                    }
                    if (AnsiStartsText("::Тест на \"Только чтение\"", strName)) {
                        p->nRowOfStmntTestReadOnly = FormLogicF1->CurrInputRow;
                    }
                }
                TFormDopInp::SetListRowsItemAtr(FormLogicF1, FormLogicF1->CurrInputRow, RowVid, level_sign, 0, 0, 0, 0,
                                                "");
                if (RowVid == 10) {
                    TFormDopInp::SetDopAtrToAtrRow(FormLogicF1, FormLogicF1->CurrInputRow, &DopAtrCol);
                }
                AtrRow = TFormDopInp::GetListRowsItem(FormLogicF1, FormLogicF1->CurrInputRow);
                if (q->FieldByName("stmnt")->AsString() == "ПРОИЗВСТР" &&
                    q->FieldByName("note1")->AsString() == "::Приглашение к вставке новой строки") {
                    if (TFormDopInp::getUpRowVTomCisle(FormLogicF1, FormLogicF1->CurrInputRow, AtrRow->OwnerId,
                                                       AtrRow->OwnerNomRow)) {
                        p->isPriglashKVstavke = true;
                        wAtrRow = TFormDopInp::GetListRowsItem(FormLogicF1, AtrRow->OwnerNomRow);
                        wAtrRow->isPriglashKVstavke = true;
                    }
                }
                TFormDopInp::SetFormatStyle(FormLogicF1, FormLogicF1->CurrInputRow,
                                            TFormatStyle(DopAtrCol.font, DopAtrCol.H, DopAtrCol.Is_bold,
                                                         DopAtrCol.FontColor, DopAtrCol.fonColor));
                newPokazCol = TForm2::SetTextNameCol(FormLogicF1, FormLogicF1->CurrInputRow, strName, 0);
                if (is_hidden == 0) {
                    if (FormLogicF1->quForms->FieldByName("is_sort")->AsInteger() == 1) {
                        TFormDopInp::SetInSort(FormLogicF1, FormLogicF1->CurrInputRow);
                    }
                }
                if (q->FieldByName("stmnt")->AsString() == "ФОРМУЛАF1") {
                    try {
                        tocnost = q->FieldByName("tocnost")->AsInteger();
                    } catch (...) {
                        tocnost = 0;
                    }
                    FormatStyle->setToNull();
                    FormatStyle->Align_vert = Align_vert;
                    FormatStyle->align_gor = Align_gor;
                    FormatStyle->font = font;
                    FormatStyle->h = h;
                    FormatStyle->Is_bold = is_bold;
                    TForm2F1(this).SelectF1(newPokazCol, note2, FormatStyle, tocnost, is_filter2, ColOnFilter2,
                                            &DopAtrCol);
                }
                DoReadonlyHidden(FormLogicF1->CurrInputRow, FormLogicF1->CurrInputRow, readonly, is_hidden);
            }
            q->Next();
        }
        if (blockLevelUr == 0) {
            TFormSsilkaNaStr::FreeSL();
        }
        TDB_TmpPokaztl::DropTmpByKOSGUall();
    }
    delete q;
    delete FormatStyle;
    T_Korteg::DeleteListKBK_Plc_Rsp();
    TracerManager::Instance().EndFunctionTrace();
}


void TForm2::SelectObjectsIerarx(bool is_sort, int aOwnerNomRow, std::string OwnerLevelText,
                                 std::string VertStr, TQueryUni *prOwnerPlanStr,
                                 std::string classIdlist, TExecuteSxemaIerarx &ExSxIerarx,
                                 int NOwner, int NSubIerarx1, PDopAtrCol aDopAtrCol) {
    TForm2Ierarx form2Ierarx(this);
    form2Ierarx.SelectObjectsIerarx(is_sort, aOwnerNomRow, OwnerLevelText, VertStr, prOwnerPlanStr, classIdlist,
                                    ExSxIerarx,
                                    NOwner, NSubIerarx1, aDopAtrCol);
}

// готовит и выполняет quFormColmn по всем полям-столбцам
void TForm2::SelectAllField() {
    int id, Offset;
    std::string s;

    FormLogicF1->FormDM->quFormStr->First(); // начинает чтение столбцов формы с начала и до конца
    s = "(";
    while (!FormLogicF1->FormDM->quFormStr->EoF()) {
        if (FormLogicF1->FormDM->quFormStr->FieldByName("attr_1")->AsInteger() !=
            TFormDopInp::GetNameCol(FormLogicF1)) {
            if (FormLogicF1->quForms->FieldByName("work_with")->AsInteger() == 4) {
                Offset = 1;
                while (true) {
                    id = getFieldId2(Offset, FormLogicF1->FormDM->quFormStr->FieldByName("note2")->AsString());
                    if (id != 0)
                        s += std::to_string(id) + ",";
                    else
                        break;
                }
            } else {
                id = FormLogicF1->FormDM->quFormStr->FieldByName("field_id")->AsInteger();
                if (id != 0)
                    s += std::to_string(id) + ",";
            }
        }
        FormLogicF1->FormDM->quFormStr->Next();
    }
    if (s[s.length() - 1] == ',') s[s.length() - 1] = ' ';
    s += ")";
    if (s == "()") s = "(0)";
    FormLogicF1->FormDM->quFormColmn->SQL->SetPart(1, "WHERE pokaz_id IN " + s);
    FormLogicF1->FormDM->quFormColmn->Open();
}

void TForm2::doGenerirFormula1(int aOwnerNomRow, int nRowFrom, int nRowTo, int level_sign, std::string aStrName,
                               PDopAtrCol aDopAtrCol) {
    std::string stmnt, Value;
    int i;
    TFormatStyle *FormatStyle = new TFormatStyle();

    FormLogicF1->CurrInputRow = TForm2::AddNewRow(FormLogicF1, aOwnerNomRow, 0 /* OwnerId */, 0 /* ObjectId */);
    TFormDopInp::SetListRowsItemAtr(FormLogicF1, FormLogicF1->CurrInputRow, 11 /* RowVid - 'ФОРМУЛАF1' */, level_sign,
                                    0, 0, 0, 0, /* false, */ "");
    TFormDopInp::SetFormatStyle(FormLogicF1, FormLogicF1->CurrInputRow,
                                TFormatStyle(aDopAtrCol->font, aDopAtrCol->H, aDopAtrCol->Is_bold,
                                             aDopAtrCol->FontColor, aDopAtrCol->fonColor));
    TForm2::SetTextNameCol(FormLogicF1, FormLogicF1->CurrInputRow, aStrName, 0);
    FormLogicF1->f1Forms->RowHidden[FormLogicF1->CurrInputRow] = true;

    if (nRowFrom > nRowTo)
        Value = "0";

    for (i = 1; i <= p->MaxHrCol; i++) {
        if (i == TFormDopInp::GetNameCol(FormLogicF1)) continue;
        stmnt = p->ListStmnt[i].Stmnt;
        if (!(stmnt == "ЗНАЧЕНИЕобщНП" || stmnt == "ЗНАЧЕНИЕСВ" || stmnt == "ЗНАЧЕНИЕСВНИ" ||
              stmnt == "УточПланВклДату" || stmnt == "ФОРМУЛАF1"))
            continue;

        if (nRowFrom <= nRowTo) {
            std::string Value =
                    "SUM(" + std::string("#Col") + std::to_string(nRowFrom) + ":" + "#Col" + std::to_string(nRowTo) +
                    ")";

            Value = TFormDopInp::ReplaceNRowNCol(FormLogicF1->CurrInputRow, i, Value);
        }

        FormatStyle->setToNull();
        TForm1::SetCellValue(FormLogicF1, FormLogicF1->f1Forms, p->MaxHrCol, FormLogicF1->CurrInputRow, i,
                             FormLogicF1->CurrInputRow, i, Value, 0 /* DataType */, FormatStyle);
    }
}

int TForm2::findRowByClassId(int aClass_id, int aRow) {
    std::string key = std::to_string(aClass_id);
    int Index;
    if (slClassIds.Find(key, Index)) {
        return *std::get<std::shared_ptr<int>>(slClassIds.GetObject(Index));
    } else {
        slClassIds.AddObject(key, aRow);
        return 0;
    }
}

// вычислиь Вид показателя/объекта - для системной характеристики
// Возвращает:
// 0 - не показатель, не объект
// 1 - просто показатель
// 2 - раздел КБК
// 3 - цел статья КБК
// 4 - вид расх КБК
// 5 - косгу КБК из сметы
// 6 - косгу 000 из сметы
// 7 - косгу 999 из под сметы
// 8 - косгу из под сметы
// 10 - объект
std::string TForm2::evalVidPokazObj(TwwStringList *p, int aRow, TQueryUni *q) {
    int pokaz_is_razd, kbk_id, vid;

//    AtrRow = p->ListRows.Items[aRow];
    PAtrRow AtrRow = ((*p->ListRows)[aRow]);

    if (AtrRow == nullptr)
        return "0";

    pokaz_is_razd = q->FieldByName("pokaz_is_razd")->AsInteger();
    switch (pokaz_is_razd) {
        case 2: {
            if (q->FieldByName("pokaz_type")->AsInteger() != 1) {
                AtrRow->isRowHidden = 1;
                return "1";
            }
            kbk_id = q->FieldByName("enum_id")->AsInteger();
            vid = TDB_bkMetaDef::GetVidById(kbk_id); // 0 - раздел, 1 - ЦС, 2 - ВР, 3 - КОСГУ
            switch (vid) {
                case 0: {
                    AtrRow->isRowHidden = 2;
                    return "2";
                }
                case 1: {
                    AtrRow->isRowHidden = 3;
                    return "3";
                }
                case 2: {
                    AtrRow->isRowHidden = 4;
                    return "4";
                }
                case 3: {
                    if (q->FieldByName("pokaz_vert")->AsInteger() == 3) {
                        AtrRow->isRowHidden = 6;
                        return "6";
                    } else {
//                        AtrRowPred = p->ListRows.Items[aRow - 1];
                        PAtrRow AtrRowPred = ((*p->ListRows)[aRow - 1]);

                        if (AtrRowPred == nullptr) {
                            AtrRow->isRowHidden = 5;
                            return "5";
                        }
                        switch (AtrRowPred->isRowHidden) {
                            case 5: {
                                AtrRow->isRowHidden = 5;
                                return "5";
                            }
                            case 6: {
                                AtrRow->isRowHidden = 7;
                                return "7";
                            }
                            case 7: {
                                AtrRow->isRowHidden = 8;
                                return "8";
                            }
                            case 8: {
                                if (AtrRow->OwnerId == AtrRowPred->OwnerId) {
                                    AtrRow->isRowHidden = 8;
                                    return "8";
                                } else {
                                    AtrRow->isRowHidden = 5;
                                    return "5";
                                }
                            }
                        }
                    }
                }
            }
            break;
        }
        case 4: {
            AtrRow->isRowHidden = 10;
            return "10";
        }
        default: {
            AtrRow->isRowHidden = 1;
            return "1";
        }
    }
    return "0";
}














