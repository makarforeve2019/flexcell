#include "SystFuncsLogic.h"
#include "unDefDBconnect.h"
#include "Form1.h"
#include "Form2Plus.h"
#include "FormEnd.h"
#include "FormGFLogic.h"
#include "FormDopInp.h"
#include "FormSorted.h"
#include "unTF1CellFormat.h"
#include "DB_PlanStr.h"
#include "DB_Forms.h"
#include "DB_Plan.h"
#include "DB_FormStr.h"
#include "DB_CellValue.h"
#include "DB_Dgvr.h"
#include "unSqlPackets.h"
#include "unFormEventsExec.h"
#include "unXlsToStrJSON.h"
#include "unProgress.h"

#include "FormUpdF1Logic.h"
#include "FormDopUpdLogic.h"
#include "unFormLogicF1.h"
#include "DB_Pokaztl.h"

//void TFormLogicF1::Destroy()
//{
//    for (int i = FormsValuesArray->Size() - 1; i >= 0; i--)
//    {
//        ClearListsForForm(i + 1);
//    }
//    FormsValuesArray->clear();
//
//    delete FormDM;
//    delete quCell;
//
//    if (quForms != nullptr)
//    {
//        delete quForms;
//    }
//    quForms = nullptr;
//
//    delete ParamFormObozns;
//    delete ParamFormObjNames;
//    delete f1Forms;
//    delete ActionView;
//
//
//
//
//    delete GlobalVars;
//
//
//}

void TFormLogicF1::OpenForm(bool isCurForm, int CurrAlbumId, TInfoAlbumLogic InfoAlbumLogic, TInfoFormLogic InfoFormLogic, unsigned char IsParamForm, int ParamObjectId, std::string ParamObjectName, int FormId, int FormOrder,
                            int ownerFormPlanId, bool loadContent) {

    this->CurrAlbumId = CurrAlbumId;
    InfoAlbumLogicGet(InfoAlbumLogic);
    TDB_Forms::Get1FormAlbum(quForms, CurrAlbumId, FormId, EncodeDate(CurrYear, 6, 15));
    this->ActionView->doBeginSost(this->FormsValuesArray->Size() == 0);
    this->f1Forms->AutoRecalc = false;
    doOpenForm(&InfoFormLogic, IsParamForm, ParamObjectId, ParamObjectName, FormId, FormOrder, ownerFormPlanId, loadContent);
    quForms->Close();
    this->f1Forms->AutoRecalc = true;

    if (isCurForm) {
        this->CurrSheet = this->f1Forms->Sheet;
    }
}

void TFormLogicF1::CloseAlbum() {
    ActionView->doBeginSost(false);
    for (int i = FormsValuesArray->Size() - 1; i >= 0; i--) {
        doCloseForm(i + 1);
    }
}

void TFormLogicF1::CloseForm(int NSheet) {
  this->ActionView->doBeginSost(false);
  this->doCloseForm(NSheet);
}

void TFormLogicF1::doCloseForm(int NSheet) {
    try {
        f1Forms->DeleteSheets(NSheet, 1);
        ClearListsForForm(NSheet);
    } catch (...) {
        // In F1, the last remaining sheet cannot be deleted, so we need to catch the exception
    }
    ActionView->DeleteSheet(NSheet);
}

void TFormLogicF1::UpdExecuteCellsAnotherSheet(int nSheet, bool noSetActiveCell, nlohmann::json& JSONresult) {
    this->f1Forms->Sheet = nSheet;
    this->ActionView->doBeginSostSaveAllAction();
    TFormDopUpdLogic::doUpdExecuteCells(nSheet, this, 0, 0, noSetActiveCell, JSONresult);
}

bool TFormLogicF1::UpdExecuteCellsEndInput(int nSheet, int aNRow, int aNCol, bool noSetActiveCell, nlohmann::json& JSONresult) {
    bool Result = true;
    TracerManager::Instance().StartFunctionTrace("TFormLogicF1::UpdExecuteCellsEndInput");
    this->f1Forms->Sheet = nSheet;
    this->ActionView->doBeginSostSaveAllAction();
    Result = TFormDopUpdLogic::doUpdExecuteCells(nSheet, this, aNRow, aNCol, noSetActiveCell, JSONresult);
    TracerManager::Instance().EndFunctionTrace();
    return Result;
}

void TFormLogicF1::updNameinF1(int nSheet, int nRow, std::string newName) {
    this->f1Forms->Sheet = nSheet;
    this->ActionView->doBeginSostSaveAllAction();
    TFormUpdF1Logic::updNameinF1(this, nRow, newName);
}

void TFormLogicF1::setInCellStr(int nSheet, int nRow, int objectid, int field_id, std::string str_value) {
    f1Forms->Sheet = nSheet;
    ActionView->doBeginSostSaveAllAction();
    TFormUpdF1Logic::setInCellStr(this, nRow, objectid, field_id, str_value);
}

void TFormLogicF1::setInCellNmbr(int nSheet, int nRow, int objectid, int field_id, int ist_fin_id, int VidOtch, double value) {
    f1Forms->Sheet = nSheet;
    ActionView->doBeginSostSaveAllAction();
    TFormUpdF1Logic::setInCellNmbr(this, nRow, objectid, field_id, ist_fin_id, VidOtch, value);
}

void TFormLogicF1::SetCellValueStr(int nSheet, int nRow, int nCol, const std::string str_value) {
    f1Forms->Sheet = nSheet;
    ActionView->doBeginSostSaveAllAction();
    TFormUpdF1Logic::SetCellValueStr(this, nRow, nCol, str_value);
}

void TFormLogicF1::setInRowNmbr(int nSheet, int nRow, int objectid) {
    f1Forms->Sheet = nSheet;
    ActionView->doBeginSostSaveAllAction();
    TFormUpdF1Logic::setInRowNmbr(this, nRow, objectid);
}

void TFormLogicF1::reDraw(int nSheet, int nRow) {
    f1Forms->Sheet = nSheet;
    ActionView->doBeginSostSaveAllAction();
    auto p = TFormDopInp::GetTww(this);
    TFormUpdF1Logic::doReDraw(this, p, nRow);
}

bool TFormLogicF1::insertObjectToF1(int nSheet, int OwnerId, int object_id, std::string object_name, int objekt_class_id, std::string KBK, int level_sign, int& aInsertRow) {
    f1Forms->Sheet = nSheet;
    ActionView->doBeginSostSaveAllAction();
    return TFormUpdF1Logic::insertObjectToF1(this, OwnerId, object_id, object_name, objekt_class_id, KBK, level_sign, aInsertRow);
}

bool TFormLogicF1::insertPokaztlToF1(int nSheet, int OwnerId, int pokaz_id, std::string pokaz_name, std::string KBK, int pokaz_type, int enum_id, int edizm_id, int level_sign, int& aInsertRow) {
    f1Forms->Sheet = nSheet;
    ActionView->doBeginSostSaveAllAction();
    return TFormUpdF1Logic::insertPokaztlToF1(this, OwnerId, pokaz_id, pokaz_name, KBK, pokaz_type, enum_id, edizm_id, level_sign, aInsertRow);
}

void TFormLogicF1::delRowByPokaz(int nSheet, int nRow, int nCol) {
    f1Forms->Sheet = nSheet;
    ActionView->doBeginSostSaveAllAction();
    TFormUpdF1Logic::delRowByPokaz(this, nRow, nCol);
}

bool TFormLogicF1::XlsToStrJSON(int nSheet, nlohmann::json& wsXlsToStrJSON) {
    bool result = TXlsToStrJSON::doXlsToStrJSON(this, nSheet, wsXlsToStrJSON);
    return result;
}

bool TFormLogicF1::doOpenForm(TInfoFormLogic* InfoFormLogic, unsigned char IsParamForm, int ParamObjectId, string ParamObjectName, int FormId, int FormOrder, int ownerFormPlanId, bool loadContent) {
    TracerManager::Instance().StartFunctionTrace("TFormLogicF1::doOpenForm");
    string form_obozn, s, SheetObozn;
    int i, n, kol, InsPos, wFixedCols;
    TwwStringList* p;
    bool resultNew;
    json curElement;

    if (FormId == 0) {TracerManager::Instance().EndFunctionTrace(); return false;};
    if (FormOrder < 0) {TracerManager::Instance().EndFunctionTrace(); return false;};
    quForms->Locate("form_id", FormId);
    this->CurrSheet = 0;
    form_obozn = quForms->FieldByName("form_obozn")->AsString();
    try {
        SheetObozn = quForms->FieldByName("SheetObozn")->AsString();
    } catch (...) {
        SheetObozn = "";
    }
    if (SheetObozn != "") {
        form_obozn = SheetObozn;
    }
    form_obozn = TForm2Plus::ZamenitGod(this, this->CurrPlanName, this->CurrYear, form_obozn);
    if (FormsValuesArray->Size() == 0) {
        InsPos = 1;
        f1Forms->InsertSheets(2, 1);
        f1Forms->DeleteSheets(1, 1);
    } else {
        InsPos = 0;
        if (IsParamForm == 0) {
            for (i = 1; i <= f1Forms->NumSheets; i++) {
                
                
                if ((FormsValuesArray->GetItem(i - 1)->FormObozn == form_obozn) || (FormsValuesArray->GetItem(i - 1)->FormObozn == ("{" + form_obozn + "}"))) {
                    if (TFormDopInp::GetFormOrder_byN(this, i - 1) == FormOrder) {
                        this->CurrSheet = f1Forms->Sheet;
                        f1Forms->Sheet = i;
                        TracerManager::Instance().EndFunctionTrace();
                        return true;
                    } else {
                        TracerManager::Instance().AddSpanLog("Ошибка.TFormLogicF1::doOpenForm", {{"errordesr", "Обозначения форм или их порядок были изменены. Форма открыта не будет"}},
                                                             TracerManager::LOG_LEVEL_ERROR);
                        //throw Exception("Обозначения форм или их порядок были изменены. Форма открыта не будет");
                    }
                }
                if (TFormDopInp::GetFormOrder_byN(this, i - 1) <= FormOrder) {
                    if (TFormDopInp::GetFormOrder_byN(this, i - 1) == FormOrder) {
                        TracerManager::Instance().AddSpanLog("Ошибка.TFormLogicF1::doOpenForm", {{"errordesr", "Обозначения форм или их порядок были изменены. Форма открыта не будет"}},
                                                             TracerManager::LOG_LEVEL_ERROR);
                        //throw Exception("Обозначения форм или их порядок были изменены. Форма открыта не будет");
                    }
                    InsPos = i;
                }
            }
        }
        if (IsParamForm == 1) {
            n = ParamFormObozns->IndexOf(form_obozn);
            if (n == -1) {
                s = "";
                kol = 0;
                ParamFormObozns->AddObject(form_obozn, kol);
            } else {
                kol = *std::get<std::shared_ptr<int>>(ParamFormObozns->Objects[n]);
                kol++;
                s = "_" + IntToStr(kol);
                ParamFormObozns->SetObject(IntToStr(n), kol);
            }
            form_obozn = form_obozn + s;
            InsPos = f1Forms->NumSheets;
        }
        if (IsParamForm == 3) {
            n = ParamFormObozns->IndexOf(form_obozn);
            if (n == -1) {
                kol = 1;
                ParamFormObozns->AddObject(form_obozn, kol);
            } else {
                kol = *std::get<std::shared_ptr<int>>(ParamFormObozns->Objects[n]);
                kol++;
                ParamFormObozns->SetObject(IntToStr(n), kol);
            }
            s = "_" + IntToStr(kol);
            if ((n == -1) && (FormsValuesArray->GetItem(1 - 1)->FormObozn != form_obozn)) s = "";
            form_obozn = form_obozn + s;
            InsPos = f1Forms->NumSheets;
        }
        InsPos++;
        f1Forms->InsertSheets(InsPos, 1);
    }
    f1Forms->Sheet = InsPos;
    //++f1Forms->SheetName[InsPos] = utf8_to_utf16(form_obozn).c_str();
//ТУТ ЗАКОНЧИЛИ
// ddd ТУТ
//dddТУТ
    std::u16string ws = utf8_to_utf16(form_obozn);
    //const char16_t* ptr = ws.c_str();
    f1Forms->SheetName[InsPos] = utf8_to_utf16(form_obozn).c_str();
    f1Forms->ShowSelections = 1;
    f1Forms->HdrWidth = 26;
    f1Forms->AllowDelete = false;
    f1Forms->EnableProtection = true;
    p = new TwwStringList();
    p->isDoOkraska = false;
    FormsValuesArray->Insert(InsPos - 1, p);
    p->FormId = FormId;
    p->FormOrder = FormOrder;
    p->FormObozn = form_obozn;
    //p->Sorted = true;
    p->is_dublir_itogo = quForms->FieldByName("is_dublir_itogo")->AsInteger();
    p->IsParamForm = IsParamForm;
    p->ParamObjectId = ParamObjectId;
    p->ParamObjectName = ParamObjectName;
    p->plan_id = quForms->FieldByName("plan_id")->AsInteger();
    if ((p->plan_id == 0 || p->plan_id == 1) && (IsParamForm == 1) && (ParamObjectId > 0)) {
        TDB_PlanStr::getPlanNameByPokazId(ParamObjectId, p->plan_id);
    }
    p->plan_name = TDB_Plan::GetPlanName(p->plan_id);
    p->work_with = quForms->FieldByName("work_with")->AsInteger();
    p->work_with_gf = quForms->FieldByName("work_with_gf")->AsInteger();
    p->form_is_main = quForms->FieldByName("form_is_main")->AsInteger();
    if (p->form_is_main == 2) {
        f1Forms->SheetName[InsPos] = utf8_to_utf16("{" + form_obozn + "}").c_str();
        p->FormObozn = "{" + form_obozn + "}";
    }
    p->form_readonly = quForms->FieldByName("form_readonly")->AsInteger();
    p->is_add = quForms->FieldByName("is_add")->AsInteger();
    p->is_copy = quForms->FieldByName("is_copy")->AsInteger();
    p->is_sort = quForms->FieldByName("is_sort")->AsInteger();
    p->is_edIzm = quForms->FieldByName("is_edIzm")->AsInteger();
    p->tocnost = quForms->FieldByName("tocnost")->AsInteger();
    p->vid_form = quForms->FieldByName("vid_form")->AsInteger();
    p->PokazCol = quForms->FieldByName("col_n")->AsInteger();
    if (quForms->FieldByName("form_k_zagtop")->AsInteger() > 0) {
        p->DeltaRow = quForms->FieldByName("form_k_zagtop")->AsInteger();
    } else {
        if (p->work_with != 2) {
            p->DeltaRow = 3;
        }
    }
    p->MaxHrRow = quForms->FieldByName("form_k1")->AsInteger() + p->DeltaRow;
    p->MaxHrCol = quForms->FieldByName("form_k2")->AsInteger();
    p->form_k_zagtop = quForms->FieldByName("form_k_zagtop")->AsInteger();
    p->form_k_zagbottom = quForms->FieldByName("form_k_zagbottom")->AsInteger();
    p->form_k1 = quForms->FieldByName("form_k1")->AsInteger();
    p->isLandscape = quForms->FieldByName("isLandscape")->AsInteger();
    p->isShowZero = quForms->FieldByName("isShowZero")->AsInteger();
    p->OgrUserOprcii = quForms->FieldByName("OgrUserOprcii")->AsString();
    p->ogr_class_id = quForms->FieldByName("ogr_class_id")->AsInteger();
    p->ComplexForm = false;
    p->nModulOfMetods = quForms->FieldByName("nModulOfMetods")->AsInteger();
    p->isShapkaColor = quForms->FieldByName("isShapkaColor")->AsInteger();
    p->isLinesColor = quForms->FieldByName("isLinesColor")->AsInteger();
    p->isPanelSearch = quForms->FieldByName("isPanelSearch")->AsInteger();
    p->ColorLockedCell = quForms->FieldByName("ColorLockedCell")->AsInteger();
    p->is_noout_nstr = quForms->FieldByName("is_noout_nstr")->AsInteger();
    if (p->work_with == 2) {
        p->PokazCol = 0;
        p->MaxHrCol = 0;
        p->MaxHrRow = 0;
        p->DeltaRow = 0;
        p->form_k_zagtop = 0;
        p->form_k_zagbottom = 0;
        p->form_k1 = 0;
    }
    p->OwnerFormPlanId = ownerFormPlanId;
    wFixedCols = p->PokazCol;
    if (p->is_noout_nstr > p->PokazCol) {
        wFixedCols = p->is_noout_nstr;
    }
    if (p->is_noout_nstr > 0) {
        wFixedCols = p->is_noout_nstr;
    }
    if (p->is_noout_nstr == 0) {
        wFixedCols = 0;
    }
    p->isISTZNACHVVODPK = InfoFormLogic->isISTZNACHVVODPK;
    p->modeNumber = InfoFormLogic->modeNumber;
    p->selectedDate = InfoFormLogic->selectedDate;
    curElement = nullptr;
    CurrElementStr = "";
    curElement = ActionView->InsertSheet(InsPos, p->form_is_main, p->FormObozn, p->vid_form, p->DeltaRow, p->form_k1, p->MaxHrCol, wFixedCols);
    CurrElementStr = to_string(curElement);
    f1Forms->DefaultFontName = utf8_to_utf16(FontArial).c_str();
    f1Forms->DefaultFontSize = 10;
    if (loadContent) {
        if (p->vid_form == 1) {
            resultNew = InitFormChart(IsParamForm, ParamObjectId, ParamObjectName, FormId, p);
            TracerManager::Instance().EndFunctionTrace();
            return true;
        } else if ((p->vid_form == 0) && (p->work_with == 2)) {
            resultNew = InitFormObject(InfoFormLogic, IsParamForm, ParamObjectId, ParamObjectName, FormId, p);
        } else {
            resultNew = NewForm1(InfoFormLogic, IsParamForm, ParamObjectId, ParamObjectName, FormId, FormOrder, p);
        }
        if ((p->work_with != 2) && (curElement != nullptr)) {
            int wi = f1Forms->LastCol;
            for (i = 1; i <= f1Forms->LastCol; i++) {
                if (p->ListStmnt[i].isCellSizeFixed) {
                    ActionView->ModifInsertSheet(curElement, i);
                }
            }
        }
        TFormEventsExec::LoadFormEvents(*p);
        p->isGroupSelect = 0;
        if (p->work_with != 2) {
            for (i = 1; i <= f1Forms->LastCol; i++) {
                if (p->ListStmnt[i].isGroupSelect == 1) {
                    p->isGroupSelect = 1;
                }
            }
        }
        try {
            this->CurrSheet = f1Forms->Sheet;
        } catch (...) { ;
        }
        if ((p->work_with != 2) && (p->vid_form == 2)) {
            this->doSetColWidthAuto();
        }
        f1Forms->AllowInCellEditing = true;
        f1Forms->ShowLockedCellsError = false;
        f1Forms->ShowGridLines = false;
        f1Forms->ShowZeroValues = false;
        if (p->vid_form == 2) {
            f1Forms->ShowZeroValues = true;
        }
        if (p->work_with != 2) {
            TFormEnd::doShapkaColor(this);
            TFormEnd::doLinesColor(this);
            p->isDoOkraska = true;
        }
    }
    TracerManager::Instance().EndFunctionTrace();
    return true;
}

bool TFormLogicF1::NewForm1(TInfoFormLogic* InfoFormLogic, unsigned char IsParamForm, int ParamObjectId, std::string ParamObjectName, int FormId, int FormOrder, TwwStringList* p) {
//    std::unique_ptr<TwwStringList> p(TFormDopInp::GetTww(this));
    TracerManager::Instance().StartFunctionTrace("TFormLogicF1::NewForm1");
    std::string ErrorMsg1, ErrorMsg2;
    int newEndRow, wRow, wCol, nCol, lastGroupCol, lastGroupRow;
    PAtrRow AtrRow, AtrRow1;
    bool isItogoStyleOforml, isInBorderGroup;
    TFormatStyle *FormatStyle, *wFormatStyle;
    bool Result = false;

    if (quForms->FieldByName("pokaz_id")->IsNull()) Result = false;

    TunProgress::doShow(this, "<" + quForms->FieldByName("form_obozn")->AsString() + ">");
    TunProgress::ShowText(this, "Начальные действия");

    Result = TForm1::FormInit(InfoFormLogic, this, IsParamForm, ParamObjectId, ParamObjectName, FormId, FormOrder, quForms, ErrorMsg1, ErrorMsg2, p);

    if (!Result) {
        this->CloseForm(f1Forms->Sheet);
        throw std::runtime_error(ErrorMsg1 + " " + ErrorMsg2);
    }
    TQueryUni *quFormStr = new TQueryUni(DefDBconnect.Conn);
    TDB_FormStr::GetItemsN1N2(quFormStr, p->FormId, 500, 1499);

    if (p->form_k_zagtop == 0)
        this->doEvalHRow2(2, 1, p->MaxHrCol);
    TFormEnd::doItogoZaMesjc_VTomCisle(this);
    if (p->is_dublir_itogo == 1)
        TFormEnd::ExecIfDublirItogo(this);
    TFormEnd::SetHiddenByFilter2(this, p->form_begBodyRow , p->form_endBodyRow );

    if (InfoFormLogic->isCallBackMethod) {
        #ifndef RestServer
        InfoFormLogic->CallBackMethod;
        #endif
    }

    newEndRow =  p->form_endBodyRow;
    AtrRow = TFormDopInp::GetListRowsItem(this, newEndRow);
    if (AtrRow != nullptr) {
        if (AtrRow->CurRowVid == 9)
            newEndRow--;
        AtrRow1 = TFormDopInp::GetListRowsItem(this, newEndRow);
        if ((AtrRow1 != nullptr) && (AtrRow1->CurRowVid == 10))
            ErrorMsg1 = char16ToUTF8(f1Forms->TextRC(newEndRow,p->PokazCol));
        if (char16ToUTF8(f1Forms->TextRC(newEndRow,p->PokazCol)) == "::УдалитьСкрытые")
            TFormEnd::DeleteHiddenRows(this, newEndRow);
        else if (char16ToUTF8(f1Forms->TextRC(newEndRow,p->PokazCol)) == "::УдалитьСкрытые2")
            TFormEnd::DeleteHiddenRows2(this, newEndRow);
        else if (char16ToUTF8(f1Forms->TextRC(newEndRow,p->PokazCol)) == "::УдалитьСкрытыеФильтром")
            TFormEnd::DeleteHiddenRowsByFiltr(this, newEndRow);
        else if (char16ToUTF8(f1Forms->TextRC(newEndRow,p->PokazCol)) == "::Загрузка данных из xls-файла")
        {
            #ifndef RestServer
            InfoFormLogic->doLoadFromXLS;
            #endif
            f1Forms->RowHidden[newEndRow] = true;
        }
    }

    if (quForms->FieldByName("is_sort")->AsInteger() > 0)
    {
        TFormSorted::SortedForm(this, p->form_begBodyRow, 1, p->form_endBodyRow, f1Forms->LastCol, quForms->FieldByName("is_sort")->AsInteger() == 2);
        p->form_endBodyRow = f1Forms->LastRow;
    }

    TFormEnd::SetNameNewCol(FormId, this, p->form_begBodyRow, p->form_endBodyRow);

    if (quForms->FieldByName("is_edIzm")->AsInteger() == 2 || quForms->FieldByName("is_edIzm")->AsInteger() == 3)
        TFormEnd::SetNameHidden(this, p->form_begBodyRow);
    TFormEnd::SetAlignmentObjedinit(p->FormId, this, p->form_begBodyRow, p->form_endBodyRow);
    TFormEnd::execOdinakovZnac(this);

    if (this->CurrInputRow == 0) Result =  true;
    f1Forms->MaxRow = this->CurrInputRow;
    if (this->CurrInputRow == p->MaxHrRow)
        f1Forms->MaxRow=f1Forms->MaxRow+1;
    f1Forms->MaxCol =p->MaxHrCol;
    delete quFormStr;
    // некоторые установки для f1Forms (F1Book)
    // ...ТУТ НЕ ХВАТАЕТ КОДА ИЗ ОСНОВНОГО ПРОЕКТА!!!
    // End of -> некоторые установки для f1Forms (F1Book)
    int GetLastVisibleCol = this->GetLastVisibleCol();
    // создание границ в таблице F1Book
    for (int wRow = p->form_begBodyRow; wRow <= p->form_endBodyRow; ++wRow) {
        bool isItogoStyleOforml = false;
        if (p->StyleOforml != nullptr) {
            auto AtrRow = TFormDopInp::GetListRowsItem(this, wRow);
            if (AtrRow != nullptr && (AtrRow->CurRowVid == 3 || AtrRow->CurRowVid == 5)) {
                isItogoStyleOforml = true;
            }
        }

        for (int wCol = 1; wCol <= GetLastVisibleCol; ++wCol) {
            f1Forms->Selection_Set(wRow, wCol, wRow, wCol);
            f1Forms->Border_Set(-1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0);
            if (isItogoStyleOforml) {
                this->doItogoStyleOforml(p, wRow, wCol);
            }
        }
    }
    if (p->form_k1 > 0) {
        for (int wRow = p->DeltaRow + 1; wRow <= p->MaxHrRow; ++wRow) {
            for (int wCol = 1; wCol <= p->MaxHrCol; ++wCol) {
                f1Forms->Selection_Set(wRow, wCol, wRow, wCol);
                if (p->isShapkaColor == 0 || p->isShapkaColor == 1) {
                    f1Forms->Border_Set(-1, 2, 2, 2, 2, 0, 0, 0, 0, 0, 0);
                } else {
                    f1Forms->Border_Set(-1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0);
                }
            }
        }
    }

    TracerManager::Instance().EndFunctionTrace();
    return Result;
}

bool TFormLogicF1::InitFormChart(int IsParamForm, int ParamObjectId, std::string ParamObjectName, int FormId, TwwStringList* p) {
    // начальные значения - для Совместимостисти c InitForm
    bool Result = true;
    TForm2Plus::CreateForTopBottom(p);
    TFormDopInp::CreateListRows(this);
    p->ComplexForm = false;
    p->PokazCol = 1;
    p->MaxHrRow = 1;
    p->DeltaRow = 3;
    p->MaxHrCol = 1;
    this->CurrInputRow = 1;
    p->RootPokazId = 0;
    // End Of ->начальные значения - для Совместимостис InitForm

    // отрисовку перенес в FormShowF1
    return Result;
}

bool TFormLogicF1::InitFormObject(TInfoFormLogic *InfoFormLogic, unsigned char IsParamForm, int ParamObjectId, string ParamObjectName, int FormId, TwwStringList* p) {
    bool Result = true;
    int FormOrder;
    string ErrorMsg1, ErrorMsg2;
    int wRow, wCol;

    Result = true;
    TForm2Plus::CreateForTopBottom(p);
    TFormDopInp::CreateListRows(this);

    FormOrder = 0;
    Result = TForm1::FormInit(InfoFormLogic, this, IsParamForm, ParamObjectId, ParamObjectName, FormId, FormOrder, quForms /*.ToTDataset*/, ErrorMsg1, ErrorMsg2, p);

    return Result;
}


bool TFormLogicF1::doStmntAssignByVvodPokaztl(int nSheet, int nRow, int nCol, int nColVvod) {
    TwwStringList* p = this->FormsValuesArray->GetItem(nSheet - 1);
    this->ActionView->doBeginSostSaveAllAction();
    TFormDopUpdLogic logic;
    return logic.doStmntAssignByVvodPokaztl(this, p, nRow, nCol, nColVvod);
}

void TFormLogicF1::ClearListsForForm(int aNSheetDelete) {
    TwwStringList* wp = this->FormsValuesArray->GetItem(aNSheetDelete - 1);
    wp = nullptr;
    this->FormsValuesArray->SetItem(aNSheetDelete - 1, nullptr);
    this->FormsValuesArray->Delete(aNSheetDelete - 1);
}

void TFormLogicF1::doSetColWidthAuto() {
    TwwStringList* p = TFormDopInp::GetTww(this);
    if (p->vid_form != 2) return;
    int br = p->form_begBodyRow;
    int w = this->f1Forms->ColWidth[p->InitKolCol + 1];
    for (int i = p->InitKolCol + 1; i <= p->MaxHrCol; i++) {
        //mak ???this->f1Forms->Xls->AutofitRow(br, i, this->f1Forms_LastRow, i, false);
        if (this->f1Forms->ColWidth[i] < w)
            this->f1Forms->ColWidth[i] = w;
    }
}

int TFormLogicF1::GetLastVisibleCol() {
    int result = 1;
    for (int i = this->f1Forms->LastCol; i >= 1; i--) {
        if (!this->f1Forms->ColHidden[i]) {
            result = i;
            return result;
        }
    }
    return result;
}

TGlobalVars* GlobalVars = nullptr;
void TFormLogicF1::CreateGlobalVars(){
    GlobalVars = new TGlobalVars();
    GlobalVars->DBType = TDBType::Postgre;
    GlobalVars->UserId = this->user_id;
}

void TFormLogicF1::DeleteGlobalVars(){
    delete GlobalVars;
}

void TFormLogicF1::doEvalHRow2(int r, int c1, int c2) {
    //mak ??? this->f1Forms->Xls->AutofitRow(r, true, 1);
}
void TFormLogicF1::UpdFormatStyle(std::string aRzd1font, int aRzd1h, int aRzd1is_bold, int aRzd1fontColor, int aRzd1FonColor) {
    TF1CellFormat cf;
    cf.GetCellFormat(reinterpret_cast<TF1FlexCel &>(f1Forms));
    if (aRzd1font != "") cf.FontName_Set(aRzd1font);
    if (aRzd1h > 0) cf.FontSize_Set(aRzd1h);
    if (aRzd1is_bold > 0) {
        cf.FontBold_Set((aRzd1is_bold & 0x01) != 0);
        cf.FontItalic_Set((aRzd1is_bold & 0x02) != 0);
        cf.FontUnderline_Set((aRzd1is_bold & 0x04) != 0);
        cf.FontStrikeout_Set((aRzd1is_bold & 0x08) != 0);
    }
    if (aRzd1fontColor != 0) cf.FontColor_Set(aRzd1fontColor);
    if (aRzd1FonColor != 0) {
        cf.PatternBG_Set(aRzd1FonColor);
        cf.PatternStyle_Set(0); // приходится ставить
        cf.PatternFG_Set(cf.PatternBG_Get());
    }
    cf.SetCellFormat(reinterpret_cast<TF1FlexCel &>(f1Forms));
}
void TFormLogicF1::doItogoStyleOforml(TwwStringList* p, int aRow, int aCol) {


    if (aCol == p->PokazCol) return;
    if (f1Forms->ColHidden[aCol]) return;
    int nomUrovRazdela = TFormDopInp::GetListRowsItem(this, aRow)->NomUrovRazdela;
    switch (nomUrovRazdela) {
    case 1:
        //mak ???UpdFormatStyle(Itg1font,Itg1h,Itg1is_bold,Itg1fontColor,Itg1FonColor);
        break;
    case 2:
        //mak ???UpdFormatStyle(Itg2font,Itg2h,Itg2is_bold,Itg2fontColor,Itg2FonColor);
        break;
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
    case 9:
    case 10:
        //mak ???TFormLogicF1::UpdFormatStyle(p->Itg3font, p->Itg3h, p->Itg3is_bold, p->Itg3fontColor, p->Itg3FonColor);
        break;
    }
}

void TFormLogicF1::InfoAlbumLogicGet(TInfoAlbumLogic& InfoAlbumLogic) {
        CurrAlbumName = InfoAlbumLogic.CurrAlbumName;
        accessalbum_idSelBlockSubj = InfoAlbumLogic.accessalbum_idSelBlockSubj;
        CurrPlanId = InfoAlbumLogic.CurrPlanId;
        CurrPlanName = InfoAlbumLogic.CurrPlanName;
        CanEdit = InfoAlbumLogic.CanEdit;
        CurrVidOtch = InfoAlbumLogic.CurrVidOtch;
        CurrYear = InfoAlbumLogic.CurrYear;
        CurrQuarter = InfoAlbumLogic.CurrQuarter;
        CurrMonth = InfoAlbumLogic.CurrMonth;
        CurrDate = InfoAlbumLogic.CurrDate;
        aShablonKBK = InfoAlbumLogic.aShablonKBK;
        aId_Owner_link1n = InfoAlbumLogic.aId_Owner_link1n;
}

TwwStringList* TFormLogicF1::GetTww() {
    try {
        int wSheet = f1Forms->Sheet;
        return FormsValuesArray->GetItem(wSheet - 1);
    } catch (exception &e) {
        TracerManager::Instance().AddSpanLog("Exception.TFormLogicF1::GetTww", {{"erordescr", e.what()}},
                                             TracerManager::LOG_LEVEL_ERROR);
        return nullptr;
    }
}

TwwStringList* TFormLogicF1::GetTwwByIndex(int index) {
    try {
        int wSheet = f1Forms->Sheet;
        return FormsValuesArray->GetItem(index);
    } catch (exception &e) {
        TracerManager::Instance().AddSpanLog("Exception.TFormLogicF1::GetTww", {{"erordescr", e.what()}},
                                             TracerManager::LOG_LEVEL_ERROR);
        return nullptr;
    }
}

bool TFormLogicF1::CellValueAtr(int nSheet, int nRow, int nCol, nlohmann::json& JSONresult) {
    std::string ws = to_string(JSONresult);
    return TFormDopUpdLogic::CellValueAtr(this, nSheet, nRow, nCol, JSONresult);
}

bool TFormLogicF1::CellEndingEdit(int nSheet, int nRow, int nCol, const std::string editValue, int valueID, nlohmann::json& JSONresult) {
    bool result = TFormDopUpdLogic::CellEndingEdit(this, nSheet, nRow, nCol, editValue, valueID, JSONresult);
    return result;
}

int TFormLogicF1::insertPokaztlToF1Back(int nSheet, int nRowSel, bool isSub, int plan_id, bool isItogo,
                           int pokaz_is_razd, int pokaz_owner_id, int in_cast, const std::string& pokaz_name,
                           const std::string& Primecanie, int is_list, int level_sign, const std::string& level_text,
                           int pokaz_vert, const std::string& vert_str, int pokaz_type, const std::string& pokaz_code,
                           const std::string& pokaz_bk_code, const TDateTime& pokaz_beg_date, const TDateTime& pokaz_end_date,
                           int enum_id, int edizm_id, int pokaz_is_const, int pokaz_is_god, int pokaz_is_kv,
                           int pokaz_is_mes, int pokaz_is_ist_fin, int pokaz_is_din_izm) {
    int pokaz_id;
    int nRowIns;

    f1Forms->Sheet = nSheet;
    this->ActionView->doBeginSostSaveAllAction();

    TFormDopUpdLogic::AddDefMrprBack(nSheet, this, plan_id, isItogo, pokaz_is_razd, pokaz_owner_id, in_cast, pokaz_name,
                                     Primecanie, is_list, level_sign, level_text, pokaz_vert, vert_str, pokaz_type,
                                     pokaz_code, pokaz_bk_code, pokaz_beg_date, pokaz_end_date, enum_id, edizm_id,
                                     pokaz_is_const, pokaz_is_god, pokaz_is_kv, pokaz_is_mes, pokaz_is_ist_fin,
                                     pokaz_is_din_izm, pokaz_id);

    // Вычислить nRowIns - номер строки, куда вставляем
//    nRowIns = TFormDopUpdLogic::findNRowIns(this, nRowSel, isSub);
//    TwwStringList* p = TFormDopInp::GetTww(this);

//    TFormDopUpdLogic::insertRowEndDataToForm(this, InfoFormLogic, nSheet,
//                                            p ,nRowIns, pokaz_id, pokaz_owner_id);
    return pokaz_id;
}

bool TFormLogicF1::delDefRowByPokaz(int nSheet, int nRow, int nCol, std::string& msgError) {
    PAtrRow AtrRow;
    int pokaz_Id;

    f1Forms->Sheet = nSheet;
    AtrRow = TFormDopInp::GetListRowsItem(this, nRow);
    if (AtrRow == nullptr) {
        return false;
    }
    pokaz_Id = AtrRow->ObjectId;
    TDB_Pokaztl::ProcessPokazId(pokaz_Id, GlobalVars->idSelPlan);
//    return false;
    // пытаемся удалить описание мероприятия
    // если нет никаких противопоказаний, то описание удаляется из БД
    if (!TFormUpdF1Logic::delDefMrpr(this, nRow, pokaz_Id, msgError)) {
        return false;
    }

    this->ActionView->doBeginSostSaveAllAction();
//    TFormUpdF1Logic::deleteRowFromF1(this, nRow);

    return true;
}

bool TFormLogicF1::UpdPokazMrprObj(int pokaz_id, std::string pokaz_name, std::string primecanie) {
    TFormUpdF1Logic::UpdPokazMrprObj(pokaz_id, pokaz_name, primecanie);
    return true;
}







































