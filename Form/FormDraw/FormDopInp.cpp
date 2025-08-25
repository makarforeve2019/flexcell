#include "SystFuncsLogic.h"
#include "unDefDBconnect.h"
#include "DB_Pokaztl.h"
#include "DB_Cell.h"
#include "DB_PlanStr.h"
#include "DB_CellValue.h"
#include "DB_BPoluc.h"
#include "DB_LinksObjects.h"
#include "DB_DenegnPotok.h"
#include "DB_NMrpr.h"
#include "DB_UserPlan.h"
#include "DB_Plan.h"
#include "DB_FormStr.h"
#include "DB_ComonNPokaz.h"
#include "DB_BPolucRaj.h"
#include "Form1.h"
#include "DB_Dgvr.h"
#include "DB_Enum.h"
#include "DB_EdIzm.h"
#include "FormSsilkaNaStr.h"
#include "DB_bkMetaDef.h"
#include "DB_ClassofPart.h"
#include "FormDopInp.h"

int TFormDopInp::fSize_ = 0;
std::string TFormDopInp::fName_;
// добавляет элемент в список TValueData и устанавливает Protection ячейки формы
PCellValueAtr TFormDopInp::AddValueCellAtr(TFormLogicF1 *FormLogicF1, int CurrRow, int CurrCol, int CellId, TDateTime ValueDate,
                                           int DataType, int VidOtch, int PlanId, int coef_Pokaz, int coef_Forma, int ObjectId,
                                           int PokazId, int cell_is_eval, int pokaz_type, int cell_period, int ist_fin_id)
{
    TwwStringList * p = TFormDopInp::GetTww(FormLogicF1);
    PCellValueAtr wvd;
    PCellValueAtr vd = new TCellValueAtr;
    vd->Row = CurrRow;
    vd->Col = CurrCol;
    vd->CellId = CellId;
    vd->ObjectId = ObjectId;
    vd->PokazId = PokazId;
    vd->ValueDate = ValueDate;
    vd->DataType = DataType;
    vd->pokaz_type = pokaz_type;
    vd->VidOtch = VidOtch;
    vd->PlanId = PlanId;
    vd->EvalType = cell_is_eval;
    vd->cell_period = cell_period;
    vd->BPId = 0;
    vd->DPId = 0;
    vd->coef_Pokaz = coef_Pokaz;
    vd->coef_Forma = coef_Forma;
    vd->class_id_ForKey = 0;
    vd->read_onlyProg = false;
    std::string cellName = "R" + std::to_string(vd->Row) + "C" + std::to_string(vd->Col);
    p->AddObject(cellName, vd);

    vd->comnp_id = 0;
    vd->god = 0;
    vd->kvrtl = 0;
    vd->mesjc = 0;
    vd->den = 0;

    FormLogicF1->f1Forms->Selection_Set(vd->Row, vd->Col, vd->Row, vd->Col);

    if ((p->work_with != 2) && (cell_is_eval == 0) && 
       (!GetTww(FormLogicF1)->ListStmnt[vd->Col].read_only && 
       ((pokaz_type < 3) || (pokaz_type == 12) || (DataType >= 30 + deltaDType) || 
         (DataType <= 31 + deltaDType))))
    {
        FormLogicF1->f1Forms->Protection_Set(false, false);
    }
    else
    {
        FormLogicF1->f1Forms->Protection_Set(true, true);
    }

    // вокруг оператора ПРИСВОИТЬ
    if (p->IsStmntPrisvoit)
    {
        PAtrRow AtrRow = ((*p->ListRows)[vd->Row - 1]);
        if (AtrRow != nullptr && AtrRow->CurRowVid == 14)
        {
            wvd = TFormDopInp::GetCellValueAtrByRC(FormLogicF1, vd->Row - 1, vd->Col);
            if (wvd != nullptr && wvd->EvalType == -1)
            {
                FormLogicF1->f1Forms->Protection_Set(true, true);
            }
        }
    }

    int userPlan_perm = TDB_UserPlan::GetUserPlan_Perm(PlanId);
    switch (userPlan_perm)
    {
    case 0: // возврат: 0 - нет для пользователя такого плана
        FormLogicF1->f1Forms->Protection_Set(true, true);
        break;
    case 1: // : 1 - Только чтение
        FormLogicF1->f1Forms->Protection_Set(true, true);
        break;
    case 2: // : 2 - На запись
        // явно ничего не устанавливаем - здесь все разрешено    
        break;
    }

    vd->ist_fin_id = ist_fin_id;
    if (ist_fin_id == 1) //{ Всего }
    {
        TDB_Cell::eval2_3by1(vd->CellId, vd->cell_id2, vd->cell_id3, vd->cell_id4);
    }

    vd->is_block = false;
    vd->is_Prisv = false;
    vd->read_onlyProg = false;
    vd->kakAgregirUpByField = 0;

    return vd;
}

// добавляет элемент в список TValueData и устанавливает Protection ячейки формы - случай ЗНАЧЕНИЕобщНП
PCellValueAtr TFormDopInp::AddValueCellAtrComNP(TFormLogicF1 *FormLogicF1, int CurrRow, int CurrCol, int comnp_id, short god,
                                                int kvrtl, int mesjc, int den, int DataType, short VidOtch, int PlanId,
                                                int coef_Pokaz, int coef_Forma, int ObjectId, int PokazId, int cell_is_eval,
                                                int pokaz_type, int vid_period, std::string toTblName)
{
    PCellValueAtr vd = TFormDopInp::AddValueCellAtr(FormLogicF1, CurrRow, CurrCol, 
      0, 0, DataType, VidOtch, PlanId, coef_Pokaz, coef_Forma, ObjectId, PokazId, 
      cell_is_eval, pokaz_type, vid_period, 0);
    vd->comnp_id = comnp_id;
    vd->god = god;
    vd->kvrtl = kvrtl;
    vd->mesjc = mesjc;
    vd->den = den;
    if (vd->pokaz_type == 4)//{ элемент из... }
    { 
        FormLogicF1->f1Forms->Protection_Set(true, true);
        if (toTblName == "Budget_poluc")
        {
            vd->EnumId = 1;
        }
        else if (toTblName == "VidProdukcii")
        {
            vd->EnumId = 2;
        }
        else if (toTblName == "OKDP")
        {
            vd->EnumId = 3;
        }
        else if (toTblName == "OKVED")
        {
            vd->EnumId = 4;
        }
    }
    return vd;
}

// копирует элемент списка TCellValueAtr
void TFormDopInp::CopyCellValueAtr(TCellValueAtr &ValueData, PCellValueAtr vd)
{
    ValueData.Row = vd->Row;
    ValueData.Col = vd->Col;
    ValueData.CellId = vd->CellId;
    ValueData.PokazId = vd->PokazId;
    ValueData.ObjectId = vd->ObjectId;
    ValueData.OwnerId = vd->OwnerId;
    ValueData.ValueDate = vd->ValueDate;
    ValueData.DataType = vd->DataType;
    ValueData.pokaz_type = vd->pokaz_type;
    ValueData.EnumId = vd->EnumId;
    ValueData.VidOtch = vd->VidOtch;
    ValueData.PlanId = vd->PlanId;
    ValueData.EvalType = vd->EvalType;
    ValueData.cell_period = vd->cell_period;
    ValueData.BPId = vd->BPId;
    ValueData.DPId = vd->DPId;
    ValueData.coef_Pokaz = vd->coef_Pokaz;
    ValueData.coef_Forma = vd->coef_Forma;
    ValueData.class_id_ForKey = vd->class_id_ForKey;
    ValueData.ist_fin_id = vd->ist_fin_id;
    ValueData.comnp_id = vd->comnp_id;
    ValueData.god = vd->god;
    ValueData.kvrtl = vd->kvrtl;
    ValueData.mesjc = vd->mesjc;
    ValueData.den = vd->den;
    ValueData.is_block = vd->is_block;
    ValueData.is_Prisv = vd->is_Prisv;
    ValueData.read_onlyProg = vd->read_onlyProg;
    ValueData.kakAgregirUpByField = vd->kakAgregirUpByField;
}

// возвращает ссылку на элемент списка TValueData по индексу
//PCellValueAtr TFormDopInp::GetPCellValueAtrByIndex(TFormLogicF1 *FormLogicF1, int Index)
//{
//    TwwStringList *p = TFormDopInp::GetTww(FormLogicF1);
//    const auto &variant = p->Objects[Index];
//    if (auto vdPtr = std::get_if<PCellValueAtr>(&variant)) {
//        return *vdPtr; // Возвращаем указатель
//    }else {
//        std::cerr << "Ошибка: объект не соответствует типу PCellValueAtr." << std::endl;
//        return nullptr;
//    }
//}

PCellValueAtr TFormDopInp::GetPCellValueAtrByIndex(TFormLogicF1 *FormLogicF1, int Index)
{
    TwwStringList* p = TFormDopInp::GetTww(FormLogicF1);
//    std::cout << "GetPCellValueAtrByIndex" << endl;
    if (Index < 0 || Index >= static_cast<int>(p->items.size())) {
        std::cerr << "Ошибка: индекс выходит за пределы диапазона." << std::endl;
        return nullptr;
    }

    auto it = std::next(p->items.begin(), Index);
    const auto& variant = it->second;

    if (auto vdPtr = std::get_if<PCellValueAtr>(&variant)) {
        return *vdPtr; // Возвращаем указатель
    } else {
        std::cerr << "Ошибка: объект не соответствует типу PCellValueAtr." << std::endl;
        return nullptr;
    }
}

bool TFormDopInp::GetCellValueAtrByName(TFormLogicF1 *FormLogicF1, std::string CellName, TCellValueAtr &ValueData)
{
    TwwStringList *p = TFormDopInp::GetTww(FormLogicF1);
    PCellValueAtr vd = p->getObjectByKey<PCellValueAtr>(CellName);
//    int i = p->IndexOf(CellName);
//    if (i < 0)
//    {
//        return false;
//    }
//    PCellValueAtr vd = std::get<TCellValueAtr *>(p->Objects[i]);
    TFormDopInp::CopyCellValueAtr(ValueData, vd);
    return true;
}

// возвращает элемент списка TCellValueAtr по строке и столбцу
PCellValueAtr TFormDopInp::GetCellValueAtrByRC(TFormLogicF1 * FormLogicF1, int Row, int Col)
{
    TwwStringList *p = TFormDopInp::GetTww(FormLogicF1);
    if (p == nullptr)
    {
        return nullptr;
    }

    std::string key = "R" + std::to_string(Row) + "C" + std::to_string(Col);
    PCellValueAtr wVd = p->getObjectByKey<PCellValueAtr>(key);
    return wVd;
}

int TFormDopInp::GetICellValueAtrByRC(TFormLogicF1 *FormLogicF1, int Row, int Col)
{
    TwwStringList *p = TFormDopInp::GetTww(FormLogicF1);
    if (p == nullptr)
    {
        return -1;
    }
    std::string key = "R" + std::to_string(Row) + "C" + std::to_string(Col);
    int index = p->IndexOf(key);
    return index;
}

PCellValueAtr TFormDopInp::GetCellValueAtrByRC_p(TwwStringList *p, int Row, int Col)
{
    int i;
    PCellValueAtr Result = NULL;
    if (p == NULL)
        return Result;
    i = p->IndexOf("R" + std::to_string(Row) + "C" + std::to_string(Col));
    if (i < 0)
        return Result;
//    const auto &variant = p->items[i].second;
//    if (auto wVd = std::get_if<PCellValueAtr>(&variant)) {
//        return *wVd; // Возвращаем указатель
//    }else {
//        std::cerr << "Ошибка: объект не соответствует типу PCellValueAtr." << std::endl;
//        return nullptr;
//    }
    PCellValueAtr wVd = std::get<PCellValueAtr>(p->Objects[i]);
    return wVd;
}

// удаляет элемент списка TCellValueAtr по строке и столбцу
void TFormDopInp::DelCellValueAtrByRC(TFormLogicF1 *FormLogicF1, int Row, int Col)
{
    TwwStringList * p = TFormDopInp::GetTww(FormLogicF1);
    int i = p->IndexOf("R" + std::to_string(Row) + "C" + std::to_string(Col));
    if (i < 0)
    {
        return;
    }
    PCellValueAtr vd = std::get<PCellValueAtr>(p->Objects[i]);
    p->Delete(i);
    delete(vd);
}

// возвращает объект по строке
int TFormDopInp::GetObjectByRow(TFormLogicF1 *FormLogicF1, int Row)
{
    TwwStringList *p = TFormDopInp::GetTww(FormLogicF1);
    int Result = 0;
    for (int i = 1; i <= p->form_endBodyRow; i++)
    {
        PCellValueAtr vd = TFormDopInp::GetCellValueAtrByRC(FormLogicF1, Row, i);
        if (vd == nullptr)
            continue;
        if (vd->ObjectId != 0)
        {
            Result = vd->ObjectId;
            return Result;
        }
    }
    return Result;
}

// возвращает количество элементов
int TFormDopInp::GetCellValueAtrCount(TFormLogicF1 *FormLogicF1)
{
    TwwStringList * p = TFormDopInp::GetTww(FormLogicF1);
    return p->Count();
}

// возвращает наименование единицы измерения с запятой или без и пусто если 0
std::string TFormDopInp::GetSSedIzmName(TFormLogicF1 *FormLogicF1, int edizm_id)
{
    std::string s;
    TwwStringList * p = GetTww(FormLogicF1);
    if (int N = p->is_edIzm; !(N == 1 || N == 3))
    {
        edizm_id = 0;
    }
    if (edizm_id > 0)
    {
        s = TDB_EdIzm::GetEdIzmNameOpt(edizm_id);
        s = Trim(s);
        if (!s.empty())
        {
            return ", " + s;
        }
    }
    return "";
}

// создает список ListRows
void TFormDopInp::CreateListRows(TFormLogicF1 *FormLogicF1)
{
    TwwStringList * p = TFormDopInp::GetTww(FormLogicF1);
    p->ListRows->SetCount(10000);
    //--p->ListRows.Count = 10000;
    // Уберем это для SMALL - слишком много данных передается через веб-сервис    
}

void TFormDopInp::ListRowsSetAtrRow(TFormLogicF1 *FormLogicF1, int Row, bool IsTerminal, int OwnerNomRow, 
    int Ownerid, int ObjectId)
{
    TwwStringList * p = TFormDopInp::GetTww(FormLogicF1);
    PAtrRow AtrRow = new TAtrRow;
    // при изменении обязательно изменить CopyListRowsItem !!!!!!!!!!!
    AtrRow->Row = Row;
    AtrRow->IsTerminal = IsTerminal;
    AtrRow->OwnerNomRow = OwnerNomRow;
    AtrRow->CurRowVid = 0;
    AtrRow->level_sign = 0;
    AtrRow->L = 0;
    AtrRow->OwnerId = Ownerid;
    AtrRow->objekt_class_id = 0;
    AtrRow->ObjectId = ObjectId;
    AtrRow->inSort = false;
    AtrRow->IsUpd = false;
    AtrRow->isRowHidden = 0;
    // --AtrRow->isDoxod=false;
    AtrRow->is_filter2 = 0;
    AtrRow->ColOnFilter2 = 0;
    AtrRow->tocnost = 0;
    AtrRow->FormatStyle.setToNull();
    AtrRow->colReplValue = 0;
    AtrRow->NameReplValue = "";
    AtrRow->msg = "";
    AtrRow->isPriglashKVstavke = false;
    AtrRow->pokaz_bk_code = "";
    AtrRow->NomUrovRazdela = 0;
    AtrRow->isRow1n = false;
    AtrRow->isRow1nReadOnly = false;
    AtrRow->isRow1nhidden = false;
    if (Row > p->ListRows->Count() - 1) {
        p->ListRows->SetCount(p->ListRows->Count() + 50);
    }
    p->ListRows->items[Row] = AtrRow;
}

// копирует элемент списка ListRows
void TFormDopInp::CopyListRowsItem(TFormLogicF1 *FormLogicF1, PAtrRow AtrRow, int DblRow)
{
    TwwStringList * p = TFormDopInp::GetTww(FormLogicF1);
//    PAtrRow ar = &(p->ListRows[DblRow]);
    PAtrRow ar =  ((*p->ListRows)[DblRow]);

    AtrRow->Row = ar->Row;
    AtrRow->IsTerminal = ar->IsTerminal;
    AtrRow->OwnerNomRow = ar->OwnerNomRow;
    AtrRow->OwnerId = ar->OwnerId;
    AtrRow->ObjectId = ar->ObjectId;
    AtrRow->objekt_class_id = ar->objekt_class_id;
    AtrRow->CurRowVid = ar->CurRowVid;
    AtrRow->level_sign = ar->level_sign;
    AtrRow->pokaz_vert = ar->pokaz_vert;
    AtrRow->L = ar->L;
    AtrRow->inSort = ar->inSort;
    AtrRow->isRowHidden = ar->isRowHidden;
    AtrRow->is_filter2 = ar->is_filter2;
    AtrRow->ColOnFilter2 = ar->ColOnFilter2;
    AtrRow->tocnost = ar->tocnost;
    AtrRow->FormatStyle = ar->FormatStyle;
    AtrRow->colReplValue = ar->colReplValue;
    AtrRow->NameReplValue = ar->NameReplValue;
    AtrRow->msg = ar->msg;
    AtrRow->isPriglashKVstavke = ar->isPriglashKVstavke;
    AtrRow->NomUrovRazdela = ar->NomUrovRazdela;
    AtrRow->isRow1n = ar->isRow1n;
    AtrRow->isRow1nReadOnly = ar->isRow1nReadOnly;
    AtrRow->isRow1nhidden = ar->isRow1nhidden;
}

// удаляет элемент из список ListRows
void TFormDopInp::DelListRowsItem(TFormLogicF1 *FormLogicF1, int Row)
{
    TwwStringList * p = GetTww(FormLogicF1);
    p->ListRows->Delete(Row);

}

PAtrRow TFormDopInp::GetListRowsItem(TFormLogicF1* FormLogicF1, int row) {
    PAtrRow Result = nullptr;
    TwwStringList* p = TFormDopInp::GetTww(FormLogicF1);

    if (p != nullptr && p->ListRows != nullptr) {
        // Если индекс превышает размер списка, увеличиваем его на 50 элементов
        if (row >= p->ListRows->Count()) {
            p->ListRows->SetCount(p->ListRows->Count() + 50);
        }

        // Убедитесь, что индекс находится в допустимом диапазоне
        if (row > 0 && row < p->ListRows->Count()) {
//            Result = p->ListRows->items[row];
            Result = ((*p->ListRows)[row]);
        }
    }

    return Result;
}


// устанавливает, что текущая строка терминальная-нет подчиненных показателей
void TFormDopInp::SetToTerminal(TFormLogicF1 *FormLogicF1, int32_t Row)
{
    PAtrRow AtrRow = GetListRowsItem(FormLogicF1, Row);
    if (AtrRow != nullptr)
    {
        AtrRow->IsTerminal = true;
    }
}

// устанавливает в текущей строке адрес строки временной таблицы для аналитических форм
void TFormDopInp::SetToStrIdTEMP(TFormLogicF1 *FormLogicF1, int32_t Row, int32_t str_id)
{
    PAtrRow AtrRow = GetListRowsItem(FormLogicF1, Row);
    if (AtrRow != nullptr)
    {
        AtrRow->str_id = str_id;
    }
}

// устанавливает, что текущая строка в сортировке
void TFormDopInp::SetInSort(TFormLogicF1 *FormLogicF1, int32_t Row)
{
    PAtrRow AtrRow = GetListRowsItem(FormLogicF1, Row);
    if (AtrRow != nullptr)
    {
        AtrRow->inSort = true;
    }
}

// устанавливает атрибуты указанной строки
void TFormDopInp::SetListRowsItemAtr(TFormLogicF1 *FormLogicF1, int Row, int CurRowVid, int level_sign, int pokaz_vert, int Ownerid, int ObjectId, int objekt_class_id, std::string pokaz_bk_code, int aNomUrovRazdela)
{
    PAtrRow AtrRow = GetListRowsItem(FormLogicF1, Row);
    AtrRow->CurRowVid = CurRowVid;
    AtrRow->level_sign = level_sign;
    AtrRow->pokaz_vert = pokaz_vert;
    AtrRow->OwnerId = Ownerid;
    AtrRow->ObjectId = ObjectId;
    AtrRow->objekt_class_id = objekt_class_id;
    AtrRow->pokaz_bk_code = pokaz_bk_code;
    AtrRow->NomUrovRazdela = aNomUrovRazdela;
}

// устанавливает параметры is_filter2, ColOnFilter2, is_filter для текущей строки
void TFormDopInp::SetFilter2OnRow(TFormLogicF1 *FormLogicF1, int Row, int is_filter2, int ColOnFilter2, int is_filter)
{
    PAtrRow AtrRow = GetListRowsItem(FormLogicF1, Row);
    AtrRow->is_filter2 = is_filter2;
    AtrRow->ColOnFilter2 = ColOnFilter2;
    AtrRow->is_filter = is_filter;
}

// устанавливает параметры tocnost для текущей строки
void TFormDopInp::SetTocnostOnRow(TFormLogicF1 *FormLogicF1, int Row, int tocnost)
{
    PAtrRow AtrRow = GetListRowsItem(FormLogicF1, Row);
    AtrRow->tocnost = tocnost;
}

// устанавливает Дополнительные атрибуты из DopAtrCol в атрибуты текущей строки
void TFormDopInp::SetDopAtrToAtrRow(TFormLogicF1 *FormLogicF1, int32_t Row, PDopAtrCol aDopAtrCol)
{
    PAtrRow AtrRow = GetListRowsItem(FormLogicF1, Row);
    if (aDopAtrCol->is_planId)
    {
        AtrRow->plan_id = aDopAtrCol->plan_id;
    }
    if (aDopAtrCol->is_UstGoda)
    {
        if (aDopAtrCol->is_set_god == 0)
        {
            AtrRow->god = FormLogicF1->CurrYear + aDopAtrCol->god_plus;
        }
        else
        {
            AtrRow->god = aDopAtrCol->god_ravno;
        }
    }
    if (aDopAtrCol->is_NameReplValue)
    {
        AtrRow->colReplValue = aDopAtrCol->colReplValue;
        AtrRow->NameReplValue = aDopAtrCol->NameReplValue;
    }
}

// устанавливает Дополнительные атрибуты в случае строк по связи 1:N
void TFormDopInp::SetDopAtrToAtrRow1n(TFormLogicF1 *FormLogicF1, int Row, PDopAtrCol aDopAtrCol)
{
    PAtrRow AtrRow = GetListRowsItem(FormLogicF1, Row);
    AtrRow->isRow1n = true;
    if (aDopAtrCol->readonly == 1)
    {
        AtrRow->isRow1nReadOnly = true;
    }
    if (aDopAtrCol->is_hidden == 1)
    {
        AtrRow->isRow1nhidden = true;
    }
}

void TFormDopInp::SetFormatStyle(TFormLogicF1 *FormLogicF1, int Row, TFormatStyle FormatStyle)
{
    PAtrRow AtrRow = GetListRowsItem(FormLogicF1, Row);
    AtrRow->FormatStyle = FormatStyle;
}

// возвращает OwnerId для заданной строки
int TFormDopInp::GetOwnerId(TFormLogicF1 *FormLogicF1, int Row)
{
    int Result = 0;
    PAtrRow AtrRow = GetListRowsItem(FormLogicF1, Row);
    if (AtrRow == nullptr)
    {
        return Result;
    }
    Result = AtrRow->OwnerId;
    return Result;
}

// возвращает ObjectId для заданной строки
int TFormDopInp::GetObjectId(TFormLogicF1 *FormLogicF1, int Row)
{
    int Result = 0;
    PAtrRow AtrRow = GetListRowsItem(FormLogicF1, Row);
    if (AtrRow == nullptr)
    {
        return Result;
    }
    Result = AtrRow->ObjectId;
    return Result;
}

// возвращает objekt_class_id для заданной строки
int TFormDopInp::GetObjekt_class_id(TFormLogicF1 *FormLogicF1, int Row)
{
    int Result = 0;
    PAtrRow AtrRow = GetListRowsItem(FormLogicF1, Row);
    if (AtrRow == nullptr)
    {
        return Result;
    }
    Result = AtrRow->objekt_class_id;
    return Result;
}

// возвращает plan_id для заданной строки
int TFormDopInp::GetPlanId(TFormLogicF1 *FormLogicF1, int Row)
{
    int Result = 0;
    PAtrRow AtrRow = GetListRowsItem(FormLogicF1, Row);
    if (AtrRow == nullptr)
    {
        return Result;
    }
    Result = AtrRow->plan_id;
    return Result;
}

// сделать отметку TAtrRow.IsUpd
void TFormDopInp::IsUpdAtrRow(TFormLogicF1 *FormLogicF1, int pokaz_meropr_id)
{
    int i, j;
    TwwStringList *p;
    PAtrRow AtrRow;

    for (i = 1; i <= FormLogicF1->FormsValuesArray->Size(); i++)
    {
        p = FormLogicF1->FormsValuesArray->GetItem(i - 1);
        if (p->work_with != 1)
            continue;
        for (j = p->ListRows->Count() - 1; j >= 0; j--)
        {
//            AtrRow = &(p->ListRows->items[j]);
            AtrRow = ((*p->ListRows)[j]);
            if (AtrRow == nullptr)
                continue;
            if (AtrRow->CurRowVid != 2)
                continue;
            if (AtrRow->ObjectId != pokaz_meropr_id)
                continue;
            AtrRow->IsUpd = true;
            goto LEnd;
        }
    }
    LEnd:;
}

// проверить, есть ли IsUpd в true
bool TFormDopInp::IsUpdinTrue(TFormLogicF1 *FormLogicF1)
{
    bool Result = false;
    TwwStringList * p = TFormDopInp::GetTww(FormLogicF1);
    if (!p->ListRows->IsEmpty())
    {
        int LastRow = FormLogicF1->f1Forms->LastRow_Get();
        for (int i = 1; i <= LastRow; i++)
        {
//            PAtrRow AtrRow = &(p->ListRows[i]);
            PAtrRow AtrRow = ((*p->ListRows)[i]);
            if (AtrRow != nullptr && AtrRow->CurRowVid != 2 && !AtrRow->IsUpd)
            {
                Result = true;
                return Result;
            }
        }
    }
    return Result;
}

// установить IsUpd в False - без обработки данных за месяц - они в FormGF.SetIsUpdToFalse
void TFormDopInp::SetIsUpdToFalse(TFormLogicF1 *FormLogicF1)
{
    TwwStringList * p = GetTww(FormLogicF1);
    int LastRow = FormLogicF1->f1Forms->LastRow_Get();
    for (int i = 1; i <= LastRow; i++)
    {
//        PAtrRow atrRow = &(p->ListRows[i]);
        PAtrRow atrRow = ((*p->ListRows)[i]);
        if (atrRow == nullptr)
        {
            continue;
        }
        if (atrRow->CurRowVid != 2)
        {
            continue;
        }
        if (!atrRow->IsUpd)
        {
            continue;
        }
        atrRow->IsUpd = false;
    }
}

bool TFormDopInp::getUpRowVTomCisle(TFormLogicF1 *FormLogicF1, int aRow, int &ObjectIdUp, int &nRowUp)
{
    TwwStringList * p = GetTww(FormLogicF1);
    for (int i = aRow - 1; i >= 1; i--)
    {
//        PAtrRow AtrRow = &(p->ListRows[i]);
        PAtrRow AtrRow = ((*p->ListRows)[i]);

        if (AtrRow == nullptr)
        {
            continue;
        }
        if (AtrRow->pokaz_vert == 1)
        {
            ObjectIdUp = AtrRow->ObjectId;
            nRowUp = i;
            return true;
        }
    }
    return false;
}

// создает список операторов столбцов формы
// ListStmnt определен как array of TAtrStmnt; // список операторов столбцов формы
void TFormDopInp::CreateListStmnt(TFormLogicF1 *FormLogicF1)
{
    TwwStringList * p = GetTww(FormLogicF1);
    p->nColForBP = 0;

    p->ListStmnt.resize(500); // резервирую на 1 больше, чтобы начинать с 1
    int wi = p->MaxHrCol;
    for (int n = 0; n <= p->MaxHrCol; n++) 
      p->ListStmnt[n].Stmnt = "";

    FormLogicF1->FormDM->quFormStr->First();
    while (!FormLogicF1->FormDM->quFormStr->EoF())
    {
        std::string Stmnt = FormLogicF1->FormDM->quFormStr->FieldByName("Stmnt")->AsString();
        int index = TDB_FormStr::GetIndexByStmnt(Stmnt);
        if (index >= 0)
        {
            int attr_1 = FormLogicF1->FormDM->quFormStr->FieldByName("attr_1")->AsInteger();
            PAtrStmnt AtrStmnt = &(p->ListStmnt[attr_1]);
            AtrStmnt->form_id = FormLogicF1->FormDM->quFormStr->FieldByName("form_id")->AsInteger();
            AtrStmnt->form_str_n = FormLogicF1->FormDM->quFormStr->FieldByName("form_str_n")->AsInteger();
            AtrStmnt->IsGroup = FormLogicF1->FormDM->quFormStr->FieldByName("is_group")->AsInteger();
            AtrStmnt->PeriodType = FormLogicF1->FormDM->quFormStr->FieldByName("period")->AsInteger();
            AtrStmnt->Stmnt = Stmnt;
            AtrStmnt->FormatStyle.align_vert = FormLogicF1->FormDM->quFormStr->FieldByName("align_vert")->AsInteger();
            AtrStmnt->FormatStyle.align_gor = FormLogicF1->FormDM->quFormStr->FieldByName("align_gor")->AsInteger();
            AtrStmnt->FormatStyle.font = FormLogicF1->FormDM->quFormStr->FieldByName("font")->AsString();
            AtrStmnt->FormatStyle.h = FormLogicF1->FormDM->quFormStr->FieldByName("h")->AsInteger();
            AtrStmnt->FormatStyle.is_bold = FormLogicF1->FormDM->quFormStr->FieldByName("is_bold")->AsInteger();
            AtrStmnt->FormatStyle.FontColor = FormLogicF1->FormDM->quFormStr->FieldByName("fontColor")->AsInteger();
            AtrStmnt->FormatStyle.fonColor = FormLogicF1->FormDM->quFormStr->FieldByName("patternBGcolor")->AsInteger();
            if (AtrStmnt->FormatStyle.fonColor == 0)
            {
                AtrStmnt->FormatStyle.fonColor = 16777215;
            }
            AtrStmnt->FormatStyle.borderStyle = FormLogicF1->FormDM->quFormStr->FieldByName("borderStyle")->AsInteger();
            AtrStmnt->FormatStyle.borderColor = FormLogicF1->FormDM->quFormStr->FieldByName("borderColor")->AsInteger();
            AtrStmnt->read_only = !FormLogicF1->CanEdit || (FormLogicF1->quForms->FieldByName("form_readonly")->AsInteger() == 1) || (FormLogicF1->quForms->FieldByName("form_is_readonly").AsInteger() == 1) || (FormLogicF1->FormDM->quFormStr->FieldByName("readonly").AsInteger() == 1);
            if (AtrStmnt->PeriodType == 7)
            {
                AtrStmnt->read_only = true;
            }
            AtrStmnt->readonly = FormLogicF1->FormDM->quFormStr->FieldByName("readonly")->AsInteger();
            AtrStmnt->is_hidden = FormLogicF1->FormDM->quFormStr->FieldByName("is_hidden")->AsInteger();
            AtrStmnt->vid_otch = FormLogicF1->FormDM->quFormStr->FieldByName("vid_otch")->AsInteger();
            AtrStmnt->is_ist_fin = FormLogicF1->FormDM->quFormStr->FieldByName("is_ist_fin")->AsInteger();
            AtrStmnt->ist_fin_grad_id = FormLogicF1->FormDM->quFormStr->FieldByName("ist_fin_grad_id")->AsInteger();
            AtrStmnt->natur_vid = FormLogicF1->FormDM->quFormStr->FieldByName("natur_vid")->AsInteger();
            AtrStmnt->ValuePeriod = FormLogicF1->FormDM->quFormStr->FieldByName("ValuePeriod")->AsInteger();
            if ((FormLogicF1->FormDM->quFormStr->FieldByName("period")->AsInteger() == 4) && (FormLogicF1->FormDM->quFormStr->FieldByName("ValuePeriod")->AsInteger() == 5))
            {
                if (FormLogicF1->CurrQuarter == 0)
                {
                    AtrStmnt->ValuePeriod = 1;
                }
                else
                {
                    AtrStmnt->ValuePeriod = FormLogicF1->CurrQuarter - 1;
                }
                if (FormLogicF1->CurrQuarter == 1)
                {
                    AtrStmnt->ValuePeriod = 10;
                }
            }
            if ((FormLogicF1->FormDM->quFormStr->FieldByName("period")->AsInteger() == 6) && (FormLogicF1->FormDM->quFormStr->FieldByName("ValuePeriod")->AsInteger() == 13))
            {
                if (FormLogicF1->CurrMonth == 0)
                {
                    AtrStmnt->ValuePeriod = 1;
                }
                else
                {
                    AtrStmnt->ValuePeriod = FormLogicF1->CurrMonth - 1;
                }
                if (FormLogicF1->CurrMonth == 1)
                {
                    AtrStmnt->ValuePeriod = 30;
                }
            }
            AtrStmnt->note1 = FormLogicF1->FormDM->quFormStr->FieldByName("note1")->AsString();
            AtrStmnt->note2 = FormLogicF1->FormDM->quFormStr->FieldByName("note2")->AsString();
            AtrStmnt->field_id = FormLogicF1->FormDM->quFormStr->FieldByName("field_id")->AsInteger();
            AtrStmnt->CoefPlus1 = FormLogicF1->FormDM->quFormStr->FieldByName("CoefPlus1")->AsInteger();
            AtrStmnt->is_Check = FormLogicF1->FormDM->quFormStr->FieldByName("is_Check")->AsInteger();
            AtrStmnt->ColOnFilter2 = FormLogicF1->FormDM->quFormStr->FieldByName("ColOnFilter2")->AsInteger();
            AtrStmnt->plan_id = FormLogicF1->FormDM->quFormStr->FieldByName("plan_id")->AsInteger();
            AtrStmnt->pokaz_id = FormLogicF1->FormDM->quFormStr->FieldByName("pokaz_id")->AsInteger();
            AtrStmnt->is_set_god = FormLogicF1->FormDM->quFormStr->FieldByName("is_set_god")->AsInteger();
            AtrStmnt->god_plus = FormLogicF1->FormDM->quFormStr->FieldByName("god_plus")->AsInteger();
            AtrStmnt->god_ravno = FormLogicF1->FormDM->quFormStr->FieldByName("god_ravno")->AsInteger();
            AtrStmnt->din_izm = FormLogicF1->FormDM->quFormStr->FieldByName("din_izm")->AsString();
            if (AtrStmnt->Stmnt == "ЗНАЧЕНИЕобщНП")
            {
                int comnp_id = FormLogicF1->FormDM->quFormStr->FieldByName("natur_vid")->AsInteger();
                TDB_ComonNPokaz::GetInfo(comnp_id, AtrStmnt->IsFieldsSprvcnik, AtrStmnt->toTblName, AtrStmnt->toColumnName1);
            }
            AtrStmnt->nValidator = FormLogicF1->FormDM->quFormStr->FieldByName("nValidator")->AsInteger();
            AtrStmnt->vid_uslovij = FormLogicF1->FormDM->quFormStr->FieldByName("vid_uslovij")->AsInteger();
            AtrStmnt->Value1 = FormLogicF1->FormDM->quFormStr->FieldByName("Value1")->AsFloat();
            AtrStmnt->Value2 = FormLogicF1->FormDM->quFormStr->FieldByName("Value2")->AsFloat();
            AtrStmnt->Shablon = FormLogicF1->FormDM->quFormStr->FieldByName("Shablon")->AsString();
            AtrStmnt->Msg = FormLogicF1->FormDM->quFormStr->FieldByName("Msg")->AsString();
            AtrStmnt->NumberPP = FormLogicF1->FormDM->quFormStr->FieldByName("attr_3")->AsInteger();
            AtrStmnt->attr_3 = FormLogicF1->FormDM->quFormStr->FieldByName("attr_3")->AsInteger();
            AtrStmnt->attr_4 = FormLogicF1->FormDM->quFormStr->FieldByName("attr_4")->AsInteger();
            AtrStmnt->tocnost = FormLogicF1->FormDM->quFormStr->FieldByName("tocnost")->AsInteger();
            AtrStmnt->isGroupSelect = FormLogicF1->FormDM->quFormStr->FieldByName("isGroupSelect")->AsInteger();
            if (AtrStmnt->Stmnt == "НАИМПОКАЗальтерн")
            {
                p->PokazColAlter = attr_1;
            }
            AtrStmnt->stmntPrisvoitLink = 0;
            std::string numberFormat = FormLogicF1->FormDM->quFormStr->FieldByName("numberFormat")->AsString();
            if (numberFormat != "")
            {
                AtrStmnt->CondFormats = json::parse(numberFormat);
            }
            if ((AtrStmnt->Stmnt == "ПлнОСТАТОК") || (AtrStmnt->Stmnt == "ФктОСТАТОК"))
            {
                p->isPlFctOstatok = true;
            }
            if (AtrStmnt->Stmnt == "ИСТЗНАЧВВОДПК")
            {
                p->isISTZNACHVVODPK = true;
            }
            AtrStmnt->ModulOfMetodsOnClick = FormLogicF1->FormDM->quFormStr->FieldByName("name_svod")->AsString();
            if (AtrStmnt->Stmnt == "TreeImgShow/Hide1")
            {
                p->colTree1 = attr_1;
            }
        }
        FormLogicF1->FormDM->quFormStr->Next();
    }
}

// возвращает атрибуты форматирования, объявленные для столбца
// aTypeData - тип значения по столбцу (1 - число, 2 - строка и т.д.)
// используется для вычисления Align_gor, если он не определен, т.е.0.
// Однако, если aTypeData = 0, то никаких добавочных вычислений, берется, что задано
void TFormDopInp::getColFrmtrAtr(TFormLogicF1 *FormLogicF1, int aTypeData, int aCol, TFormatStyle& FormatStyle)
{
    TwwStringList *p = GetTww(FormLogicF1);
    if (p == nullptr)
    {
        return;
    }
    if (p->ListStmnt.empty())
    {
        return;
    }

    FormatStyle = p->ListStmnt[aCol].FormatStyle;

    if (aTypeData == 0)
    {
        return;
    }

    switch (aTypeData)
    {
    case 1:
        if (FormatStyle.align_vert == 0)
        {
            FormatStyle.align_vert = 2;
        }
        if (FormatStyle.align_vert == 0)
        {
            FormatStyle.align_vert = 3;
        }
        break;
    case 2:
        if (FormatStyle.align_vert == 0)
        {
            FormatStyle.align_vert = 2;
        }
        if (FormatStyle.align_vert == 0)
        {
            FormatStyle.align_vert = 2;
        }
        break;
    }
}

void TFormDopInp::getColFrmtrAtr(PAtrStmnt AtrStmnt, int aTypeData, TFormatStyle &FormatStyle)
{
    FormatStyle = AtrStmnt->FormatStyle;

    if (aTypeData == 0)
    {
        return;
    }
    switch (aTypeData)
    {
    case 1:
        if (FormatStyle.align_vert == 0)
        {
            FormatStyle.align_vert = 2;
        }
        if (FormatStyle.align_vert == 0)
        {
            FormatStyle.align_vert = 3;
        }
        break;
    case 2:
        if (FormatStyle.align_vert == 0)
        {
            FormatStyle.align_vert = 2;
        }
        if (FormatStyle.align_vert == 0)
        {
            FormatStyle.align_vert = 2;
        }
        break;
    }
}

// возвращает атрибуты форматирования для столбца с Наименованием показателя
bool TFormDopInp::getColNameFrmtrAtr(TFormLogicF1 *FormLogicF1, int aCol, TFormatStyle &FormatStyle)
{
    bool Result = false;
    TwwStringList * p = TFormDopInp::GetTww(FormLogicF1);
    if (p == nullptr) return Result;
//mak???    if (p->ListStmnt == nullptr) return Result;
        
    if (p->ListStmnt[aCol].Stmnt != "НАИМЕНОВАНИЕ")
    {
        return Result;
    }
    FormatStyle = p->ListStmnt[aCol].FormatStyle;
    if (FormatStyle.align_vert == 0)
    {
        FormatStyle.align_vert = 1; // вверх
    }
    if (FormatStyle.align_gor == 0)
    {
        FormatStyle.align_gor = 1; // влево
    }
    Result = true;
    return Result;
}

// установить isCellSizeFixed = true - что размеры ячеек этого столбца фиксированы
void TFormDopInp::setCellSizeFixed(TFormLogicF1 *FormLogicF1, int aCol)
{
    TwwStringList * p = GetTww(FormLogicF1);
    if (p == nullptr) return;

    p->ListStmnt[aCol].isCellSizeFixed = true;    
}

// извлекает наименование без идентификации строки
std::string TFormDopInp::ExcIdRow(std::string aName, TFormLogicF1 *FormLogicF1, int nSheet, int Row, int PokazCol)
{
    std::string Result = aName;
    if (PokazCol != GetNameCol(FormLogicF1))
    {
        return Result;
    }
    TwwStringList * p = FormLogicF1->FormsValuesArray->GetItem(nSheet - 1);
    //    PAtrRow AtrRow = p.ListRows->items[Row];
    PAtrRow AtrRow = ((*p->ListRows)[Row]);
    if (AtrRow->Row > 0)
    {
        Result = Copy(aName, AtrRow->L + 1, size(aName));
    }
    return Result;
}

// заменяет #MnsNNN в строке NNN - цифры
std::string TFormDopInp::ReplaceMnsNNN(std::string aSS, int CurrInputRow, int CurrCol)
{
    std::string wSS = aSS;
    std::string Result = wSS;
    while (true)
    {
        int pos1 = Pos("#Mns", wSS);
        if (pos1 < 0)
            break;
        std::string s = "";
        for (int i = pos1 + 4; i < wSS.length(); i++)
        {
            if (!isdigit(wSS[i]))
                break;
            s += wSS[i];
        }
        if (s.empty())
        {
            throw std::runtime_error("После #Mns отсутствуют цифры");
        }
        int n = std::stoi(s);
        if (n >= CurrInputRow)
        {
            throw std::runtime_error("После #Mns задано слишком большое число");
        }
        // не менять s, далее используется его длина
        wSS = wSS.substr(0, pos1) + std::to_string(CurrInputRow - n) + wSS.substr(pos1 + 4 + s.length());
    }
    return wSS;
}

// заменяет #PlsNNN в строке NNN - цифры
std::string TFormDopInp::ReplacePlsNNN(const std::string aSS, int CurrInputRow, int CurrCol)
{
    std::string wSS = aSS;
    std::string Result = wSS;
    while (true)
    {
        int pos1 = Pos("#Pls", wSS);
        if (pos1 < 0)
            break;
        std::string s;
        for (int i = pos1 + 4; i < wSS.length(); ++i)
        {
            if (!isdigit(wSS[i]))
                break;
            s += wSS[i];
        }
        if (s.empty())
        {
            throw std::runtime_error("После #Pls отсутствуют цифры");
        }
        int n = std::stoi(s);
       // не менять s, далее используется его длина       
        wSS = wSS.substr(0, pos1) + std::to_string(CurrInputRow + n) + wSS.substr(pos1 + 4 + s.length());
    }
    return wSS;
}

bool isAlphanumeric(char16_t ch) {
    // Проверка на английские буквы и цифры
    if ((ch >= u'0' && ch <= u'9') || (ch >= u'A' && ch <= u'Z') || (ch >= u'a' && ch <= u'z')) {
        return true;
    }
    // Проверка на русские буквы (кириллица)
    if ((ch >= u'\u0400' && ch <= u'\u04FF') || (ch == u'\u0451') || (ch == u'\u0401')) { // включая Ё и ё
        return true;
    }
    return false;
}

// заменяет #StrLLL в строке LLL - цифры и/или буквы
std::string TFormDopInp::ReplaceStrNNN(std::string aSS, int CurrInputRow, int CurrCol)
{
//    std::cout << "ReplaceStrNNN" << endl;
    std::u16string wSS = utf8_to_utf16(aSS);
    std::u16string Result;
    Result.reserve(wSS.size()); // Резервируем память для результата

    size_t pos = 0;
    while (true) {
        size_t pos1 = wSS.find(u"#Str", pos);
        if (pos1 == std::u16string::npos) {
            Result += wSS.substr(pos); // Добавляем остаток строки
            break;
        }

        Result += wSS.substr(pos, pos1 - pos); // Добавляем часть строки до #Str
        pos = pos1 + 4;

        std::u16string s;
        for (size_t i = pos; i < wSS.length(); ++i) {
            if (!isAlphanumeric(wSS[i])) {
                break;
            }
            s += wSS[i];
            ++pos;
        }

        if (s.empty()) {
            throw std::runtime_error("После #Str отсутствуют символы");
        }

        int n;
        if (!TFormSsilkaNaStr::findSsilkaNaStr(utf16_to_utf8(s), n)) {
            throw std::runtime_error("После #Str задано условное обозначение " + utf16_to_utf8(s) + ", которое не определено");
        }

        Result += utf8_to_utf16(std::to_string(n));
    }

    return utf16_to_utf8(Result);
}

// заменяет #Row и #Col в строке
std::string TFormDopInp::ReplaceNRowNCol(int CurrInputRow, int CurrCol, const std::string note1)
{
    std::string s;
    std::string s1, s_;
   
    s = StringReplace(note1, "#Row", IntToStr(CurrInputRow));
    s = StringReplace(s, "#Prv", IntToStr(CurrInputRow - 1));
    
    s = TFormDopInp::ReplaceMnsNNN(s, CurrInputRow, CurrCol);
    s = TFormDopInp::ReplacePlsNNN(s, CurrInputRow, CurrCol);
    s = TFormDopInp::ReplaceStrNNN(s, CurrInputRow, CurrCol);

    s = StringReplace(s, "#Login", GetUserLogin());
    s = StringReplace(s, "#Date", FormatDateTime("dd.mm.yyyy", Date()));
    
    s1 = "";
    if (CurrCol > 26)
    {
        CurrCol -= 26;
        s1 = "A";
        if (CurrCol > 26)
        {
            CurrCol -= 26;
            s1 = "B";
        }
        if (CurrCol > 26)
        {
            CurrCol -= 26;
            s1 = "C";
        }
        if (CurrCol > 26)
        {
            CurrCol -= 26;
            s1 = "D";
        }
        if (CurrCol > 26)
        {
            CurrCol -= 26;
            s1 = "E";
        }
        if (CurrCol > 26)
        {
            CurrCol -= 26;
            s1 = "F";
        }
        if (CurrCol > 26)
        {
            CurrCol -= 26;
            s1 = "G";
        }
        if (CurrCol > 26)
        {
            CurrCol -= 26;
            s1 = "H";
        }
    }
    s1 += static_cast<char>(static_cast<int>('A') + CurrCol - 1);

    s_ = "";
    if (!s.empty() && s[0] != '^')
    {
        s_ = "^";
    }

    std::string result = s_ + StringReplace(s, "#Col", s1);
    return result;
}

std::string evalRef(const std::string &bSS) {
    std::stringstream ss(bSS);
    std::string s0, s1, s2;
    int nCol, nRow, dCol, dRow;

    std::getline(ss, s0, ',');
    s0.erase(std::remove(s0.begin(), s0.end(), ' '), s0.end()); // Убираем пробелы
    nCol = LetterToColumn(s0);
    nRow = LetterToRow(s0);

    std::getline(ss, s1, ',');
    s1.erase(std::remove(s1.begin(), s1.end(), ' '), s1.end());
    dRow = std::stoi(s1);

    std::getline(ss, s2);
    s2.erase(std::remove(s2.begin(), s2.end(), ' '), s2.end());
    dCol = std::stoi(s2);

    nCol += dCol;
    nRow += dRow;

    return getOboznColumn(nCol) + std::to_string(nRow);
}

std::string TFormDopInp::ReplaceOffSet(std::string aSS) {
    size_t Offset = 0;
    std::string sRes;
    size_t n = aSS.find("offset", Offset);

    if (n == std::string::npos) {
        return aSS;
    }

    while (n != std::string::npos) {
        sRes += aSS.substr(Offset, n - Offset);
        Offset = n + 6;

        size_t nB = aSS.find('(', Offset);
        if (nB == std::string::npos) {
            return aSS;
        }
        Offset = nB + 1;

        size_t nE = aSS.find(')', Offset);
        if (nE == std::string::npos) {
            return aSS;
        }

        sRes += evalRef(aSS.substr(nB + 1, nE - nB - 1));
        Offset = nE + 1;

        n = aSS.find("offset", Offset);
        if (n == std::string::npos) {
            sRes += aSS.substr(nE + 1);
        }
    }

    return sRes;
}


// возвращает TwwStringList для текущей формы
TwwStringList * TFormDopInp::GetTww(TFormLogicF1 *FormLogicF1)
{
    return FormLogicF1->GetTww();
}

// возвращает номер столбца с наименованием для текущей формы
int TFormDopInp::GetNameCol(TFormLogicF1 *FormLogicF1)
{
    TwwStringList * p = GetTww(FormLogicF1);
    return p->PokazCol;
}

// возвращает альтернативный номер столбца с наименованием для текущей формы
int TFormDopInp::GetNameColAlter(TFormLogicF1 *FormLogicF1)
{
    TwwStringList *p = GetTww(FormLogicF1);
    int Result = p->PokazColAlter;
    if (Result == 0)
    {
        Result = p->PokazColother;
    }
    return Result;
}

// возвращает номер столбца с наименованием для формы с указанным номером
int TFormDopInp::GetNameCol_byN(TFormLogicF1* FormLogicF1, int aNSheet) 
{
        TwwStringList* p = FormLogicF1->FormsValuesArray->GetItem(aNSheet);
        return p->PokazCol;
}

// возвращает FormId текущей формы
int TFormDopInp::GetFormId(TFormLogicF1* FormLogicF1) 
{
  if (FormLogicF1->FormsValuesArray->IsEmpty()) {
    return 0;
  } else {
    TwwStringList* p = TFormDopInp::GetTww(FormLogicF1);
    return p->FormId;
  }
}

// возвращает FormId формы с указанным номером
int TFormDopInp::GetFormId_byN(TFormLogicF1* FormLogicF1, int aNSheet) 
{
    if (FormLogicF1->FormsValuesArray->Size() == 0) {
        return 0;
    } else {
        TwwStringList * p = FormLogicF1->FormsValuesArray->GetItem(aNSheet);
        return p->FormId;
    }
}

// возвращает FormOrder текущей формы
int TFormDopInp::GetFormOrder(TFormLogicF1* FormLogicF1) 
{
  if (FormLogicF1->FormsValuesArray->IsEmpty()) {
    return 0;
  } else {
    TwwStringList* p = TFormDopInp::GetTww(FormLogicF1);
    return p->FormOrder;
  }
}

int TFormDopInp::GetFormOrder_byN(TFormLogicF1* FormLogicF1, int aNSheet) 
{
    if (FormLogicF1->FormsValuesArray->Size() == 0) {
        return 0;
    } else {
        TwwStringList * p = FormLogicF1->FormsValuesArray->GetItem(aNSheet);
        return p->FormOrder;
    }
}

// возвращает MaxHrCol текущей формы
int TFormDopInp::GetMaxHrCol(TFormLogicF1* FormLogicF1)
{
    if (FormLogicF1->FormsValuesArray->Size() == 0) {
        return 0;
    } else {
        TwwStringList * p = TFormDopInp::GetTww(FormLogicF1);
        return p->MaxHrCol;
    }
}

// возвращает MaxHrCol формы с указанным номером
int TFormDopInp::GetMaxHrCol_byN(TFormLogicF1* FormLogicF1, int aNSheet) 
{
    if (FormLogicF1->FormsValuesArray->Size() == 0) {
        return 0;
    } else {
        TwwStringList * p = FormLogicF1->FormsValuesArray->GetItem(aNSheet) ;
        return p->MaxHrCol;
    }
}

// найти столбец ФактВсего к началу периода месяцу
bool TFormDopInp::GetFactVsegoKNacPerid(TFormLogicF1* FormLogicF1, int nRow, TDateTime & dt)
{
    TwwStringList* p = TFormDopInp::GetTww(FormLogicF1);
    int wCol = 0;
    bool Result = false;
    for (int i = 1; i <= p->MaxHrCol; i++) {
        if (p->ListStmnt[i].Stmnt == "ФактВсегоВНачале" && p->ListStmnt[i].PeriodType != 7) {
            wCol = i;
            break;
        }
    }
    if (wCol > 0) {
        PCellValueAtr vd = TFormDopInp::GetCellValueAtrByRC(FormLogicF1, nRow, wCol);
        if (vd != nullptr) {
            dt = vd->ValueDate;
            Result = true;
        }
    }
    return Result;
}

// делает произвольный столбец невидимым
void TFormDopInp::SetCol1Hidden(TFormLogicF1* FormLogicF1, int BegRow, int nCol) 
{
    int i, n;
    n = nCol;
    int LastRow = FormLogicF1->f1Forms->LastRow_Get();
    for (i = BegRow; i <= LastRow; i++) {
        FormLogicF1->f1Forms->Selection_Set(i, n, i, n);
        FormLogicF1->f1Forms->WordWrap_Set(false);
    }
    FormLogicF1->f1Forms->ColHidden_Set(n, true);
}

int TFormDopInp::GetKolStrIn_ShapZag(TFormLogicF1* FormLogicF1) 
{
    TwwStringList* p = GetTww(FormLogicF1);
    return p->MaxHrRow;
}

// сделать текущей строку
void TFormDopInp::SetSelectionById(TFormLogicF1* FormLogicF1, int ObjectId)
{
    int i, j;
    PAtrRow AtrRow;
    int LastCol = FormLogicF1->f1Forms->LastCol_Get();
    int LastRow = FormLogicF1->f1Forms->LastRow_Get();
    for (i = 1; i <= LastRow; i++) {
        AtrRow = GetListRowsItem(FormLogicF1, i);
        if (AtrRow == nullptr)
            continue;
        if (AtrRow->ObjectId == ObjectId) {
            FormLogicF1->f1Forms->Selection_Set(i, 1, i, 256);
            for (j = 1; j <= LastCol; j++) {
                if (FormLogicF1->f1Forms->ColHidden_Get(j))
                    continue;
                FormLogicF1->f1Forms->ActiveCell_Set(i, j);
                break;
            }
            FormLogicF1->f1Forms->ShowActiveCell();
            break;
        }
    }
}

// вычислить атрибуты бюджетополучаетля и вставить необходимые в форму
void TFormDopInp::SetAttribByBP(TFormLogicF1* FormLogicF1, int nRow, int nCol) 
{
    // вычислить лицевой счет РАСЧСЧЕТ для бюджетополучаетля
    SetAttrByBP(FormLogicF1, nRow, nCol, "РАСЧСЧЕТ");
    SetAttrByBP(FormLogicF1, nRow, nCol, "ИНН");
    SetAttrByBP(FormLogicF1, nRow, nCol, "ЮрАдрес");
}

// вычислить лицевой счет РАСЧСЧЕТ и другие атрибуты после выбора бюджетополучаетля и вставляем в форму, если нужно
void TFormDopInp::SetAttrByBP(TFormLogicF1* FormLogicF1, int nRow, int nCol, std::string Stmnt) 
{
    int LicScetCol, i;
    TwwStringList* p = GetTww(FormLogicF1);
    TFormatStyle *FormatStyle = nullptr;
    LicScetCol = 0;
    for (i = 1; i <= p->MaxHrCol; i++) {
        if (p->ListStmnt[i].Stmnt == Stmnt) {
            LicScetCol = i;
            getColFrmtrAtr(FormLogicF1, 0, i, *FormatStyle);
            break;
        }
    }
    if (LicScetCol == 0)
        return;
    PCellValueAtr vd1 = GetCellValueAtrByRC(FormLogicF1, nRow, nCol);
    PCellValueAtr vd = GetCellValueAtrByRC(FormLogicF1, nRow, LicScetCol);
      // это Счет, ИНН, ЮрАдрес
    if (vd1 != nullptr) {
        int DataType = 0;
        std::string Value;
        if (Stmnt == "РАСЧСЧЕТ")
            Value = TDB_BPoluc::GetRascScetBP(vd1->BPId);
        else if (Stmnt == "ИНН")
            Value = TDB_BPoluc::GetINNBP(vd1->BPId);
        else if (Stmnt == "ЮрАдрес") {
            Value = TDB_BPoluc::GetJurAddresBP(vd1->BPId);
            DataType = 2;
        }
        TForm1::SetCellValue(FormLogicF1, FormLogicF1->f1Forms, p->MaxHrCol, vd->Row, vd->Col, vd->Row, vd->Col, Value, DataType, nullptr);
        FormLogicF1->f1Forms->Selection_Set(vd->Row, vd->Col, vd->Row, vd->Col);
        FormLogicF1->f1Forms->Protection_Set(true, true);
        FormLogicF1->f1Forms->Selection_Set(nRow, nCol, nRow, nCol);
    }
}

// посмотреть, есть ли в текущей строке столбец с БП
int TFormDopInp::isEstColumnforBP(TFormLogicF1* FormLogicF1, int nRow) 
{
    TwwStringList* p = GetTww(FormLogicF1);
    int BPCol = 0;
    for (int i = 1; i <= p->MaxHrCol; i++) {
        if ((p->ListStmnt[i-1].Stmnt == "БДЖПОЛУЧТЛЬ") || (p->ListStmnt[i-1].Stmnt == "БДЖПОЛУЧТЛЬ_нет()")) {
            PCellValueAtr vd = GetCellValueAtrByRC(FormLogicF1, nRow, i);
            if (vd != nullptr) {
                BPCol = i;
                break;
            }
        }
    }
    return BPCol;
}

// вычислить атрибуты мероприятия договора
void TFormDopInp::SetAttribByNMrpr(TFormLogicF1* FormLogicF1, int nRow, int nCol, int idMeropr) 
{
    TwwStringList* p = GetTww(FormLogicF1);
    TFormatStyle *FormatStyle = nullptr;
    int nmp;
    int wCol = 0;
    for (int i = 1; i <= p->MaxHrCol; i++) {
        if (p->ListStmnt[i-1].Stmnt == "НомерМрпр") {
            wCol = i;
            getColFrmtrAtr(FormLogicF1, 0, i, *FormatStyle);
            break;
        }
    }
    if (wCol != 0) {
        PCellValueAtr vd = GetCellValueAtrByRC(FormLogicF1, nRow, wCol);
        if (vd != nullptr) {
            std::string Value = TDB_NMrpr::PokazidToNMP(idMeropr, FormLogicF1->CurrYear, nmp);
            TForm1::SetCellValue(FormLogicF1, FormLogicF1->f1Forms, p->MaxHrCol, nRow, wCol, nRow, wCol, Value, 0, FormatStyle);
            FormLogicF1->f1Forms->Selection_Set(nRow, wCol, nRow, wCol);
            FormLogicF1->f1Forms->Protection_Set(true, true);
            FormLogicF1->f1Forms->Selection_Set(nRow, nCol, nRow, nCol);
        }
    }
}

// вернуть имя текущего рабочего листа
std::string TFormDopInp::GetSheetName(TFormLogicF1* FormLogicF1) 
{
    int nSheet = FormLogicF1->f1Forms->Sheet_Get();
    return char16ToUTF8(FormLogicF1->f1Forms->SheetName_Get(nSheet));
}

// взять из текущей форме имя формы и ид объекта владельца раскрытия
void TFormDopInp::GetForUnRaskrit(TFormLogicF1* FormLogicF1, std::string& FormName, int& ObjectId, bool& IsRefreshOwner) 
{
    TwwStringList* wp = GetTww(FormLogicF1);
    FormName = wp->NameOwnerRaskrit;
    ObjectId = wp->ObjectIdOwnerRaskrit;
    IsRefreshOwner = wp->IsRefreshOwner;
}

// сделать форму (по имени) текущей
bool TFormDopInp::SetCurSheet(TFormLogicF1* FormLogicF1, std::string FormName) 
{
    bool Result = false;
    if (FormLogicF1->FormsValuesArray->Size() == 0)
        return Result;
    for (int nSheet = 1; nSheet >= FormLogicF1->f1Forms->NumSheets_Get(); nSheet++) {
        if (char16ToUTF8(FormLogicF1->f1Forms->SheetName_Get(nSheet)) == FormName) {
            FormLogicF1->f1Forms->Sheet_Set(nSheet);
            Result = true;
            break;
        }
    }
    return Result;
}

// дать формулу под номером aN, заменив параметры
std::string TFormDopInp::GetSubFormulaByNpar(std::string aSS, int aN) 
{
    std::string wsN = TFormDopInpMF::GetSubFormulaByN(aSS, aN);
    std::string ws0 = TFormDopInpMF::GetSubFormulaByN(aSS, 0);
    std::string paramName, paramVal;
    if (ws0.empty())
        return wsN;
    if (ws0[0] == '^')
        ws0.erase(0, 1);
    TStringList * sl = new TStringList;
	sl->delimiter = ';';
	sl->DelimitedText() = ws0;
	for (int i = 0; i < sl->Count(); i++) {
	  paramName = sl->Names[i];
	  paramVal = sl->Values[paramName];
	  wsN = AnsiReplaceStr(wsN, paramName, paramVal);
	};       
    
    return wsN;
}

// для работы с формулой конкатенации строк - характеристик объекта
// возвращает номер символа [, или 0 (конец)
int TFormDopInp::GetNextFormulaConc(std::string aSS, int& aId, std::string& aRazd, int Offset) 
{
    int Result = 0;
    aId = 0;
    aRazd.clear();
    if (aSS.empty())
        return Result;
    int pos1 = PosEx("[", aSS, Offset);
    if (pos1 < 0)
        return Result;
    int pos2 = PosEx("]", aSS, pos1 + 1);
    if (pos2 < 0)
        return Result;
    if (pos1 == 0)
        aRazd.clear();
    else
        aRazd = aSS.substr(Offset, pos1 - Offset);
    std::string w = aSS.substr(pos1 + 1, pos2 - pos1 - 1);
    if (w.empty())
        return Result;
    try {
        aId = std::stoi(w);
    } catch (...) {
        return Result;
    }
    Result = static_cast<int>(pos1);
    return Result;
}



void TFormDopInp::GetFont(std::string& fName, short& fSize, bool& bBold, bool& bItalic, bool& bUnderline, bool& bStrikeout, int& fColor, bool& bOutline, bool& bShadow)
{
    fName = fName_;
    fSize = fSize_;
    bBold = false;
    bItalic = false;
    bUnderline = false;
    bStrikeout = false;
    fColor = 0;
    bOutline = false;
    bShadow = false;
}

// установить для текущей формы Шрифт по умолчанию
void TFormDopInp::SetFont(std::string aFName, short aFSize)
{
    if (aFName.empty() && aFSize == 0) {
        fName_ = fNameC;
        fSize_ = fSizeC;
    } else {
        fName_ = aFName;
        fSize_ = aFSize;
    }
}

void TFormDopInp::GetRootPokaztlForm(TFormLogicF1* FormLogicF1, int& wPokaz_id, std::string& wPokaz_name, int& wPokaz_is_razd, int& wPokaz_type, int& wPokaz_vert) 
{
    int aPokaz_id = 0;
    // вычисление aPokaz_id    
    TwwStringList* p = GetTww(FormLogicF1);
    for (int j = 1; j <= p->form_endBodyRow; j++) {
//        PAtrRow AtrRow = &p->ListRows[j];
        PAtrRow AtrRow = ((*p->ListRows)[j]);

        if (AtrRow == nullptr)
            continue;
        if (int N = AtrRow->CurRowVid; N == 1 || N == 2 || N == 3 || N == 4 || N == 5 || N == 12) {
            aPokaz_id = AtrRow->ObjectId; // это ид показателя
            break;
        }
    }
    if (aPokaz_id == 0) {
        wPokaz_id = 0;
        wPokaz_name = "";
        return;
    }
    TDB_Pokaztl::GetRootPokaztl(aPokaz_id, wPokaz_id, wPokaz_name, wPokaz_is_razd, wPokaz_type, wPokaz_vert);
}

// дать формулу для столбца aN, заменив параметры
//std::string TFormDopInp::GetFormulaForColpar(std::string aSS, int aNCol) {
//    std::string wsN = TFormDopInpMF::GetFormulaForCol(aSS, aNCol);
//    std::string ws0 = TFormDopInpMF::GetFormulaForCol(aSS, 0);
//
//    if (ws0.empty())
//        return wsN;
//    if (ws0[0] == '^')
//        ws0.erase(0, 1);
//    int Offset = 0;
//    while (true) {
//        int n = PosEx("{", ws0, Offset);
//        if (n < 0)
//            break;
//        Offset = n + 1;
//        int n2 = PosEx("}=", ws0, Offset);
//        if (n2 < 0)
//            break;
//        // std::string paramName = ws0.substr(n, n2 - n + 1); БЫЛО на delphi
//        std::string paramName = ws0.substr(n-1, n2 - n);
//        Offset = n2 + 2;
//        n = n2 + 2;
//        n2 = PosEx(";", ws0, Offset);
//        if (n2 < 0)
//            n2 = ws0.length();
//        string paramVal = Copy(ws0, n, n2 - n);
//        wsN = AnsiReplaceStr(wsN, paramName, paramVal);
//        Offset = n2 + 1;
//        if (Offset >= ws0.length())
//            break;
//    }
//    return wsN;
//}
std::string TFormDopInp::GetFormulaForColpar(std::string aSS, int aNCol) {
    std::string wsN = TFormDopInpMF::GetFormulaForCol(aSS, aNCol);
    std::string ws0 = TFormDopInpMF::GetFormulaForCol(aSS, 0);
    if (ws0.empty()) {
        return wsN;
    }
    if (ws0[0] == '^') {
        ws0.erase(0, 1);
    }

    size_t offset = 0;
    while (true) {
        size_t n = PosEx("{", ws0, offset);
        if (n == 0) {
            break;
        }
        offset = n;
        size_t n2 = PosEx("}=", ws0, offset);
        if (n2 == 0) {
            break;
        }
        std::string paramName = ws0.substr(n - 1, n2 - n + 1);
        offset = n2 + 1;
        n = n2 + 1;
        n2 = PosEx(";", ws0, offset);
        if (n2 == 0) {
            n2 = ws0.length() + 1;
        }
        std::string paramVal = ws0.substr(n, n2 - n - 1);
        wsN = AnsiReplaceStr(wsN, paramName, paramVal);
        offset = n2;
        if (offset > ws0.length()) {
            break;
        }
    }
    return wsN;
}

