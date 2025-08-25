

//#include <boost/chrono/detail/inlined/win/chrono.hpp>
#include "FormDopUpdLogic.h"
#include "unSqlPackets.h"
#include "SystFuncsLogic.h"
#include "Form1.h"
#include "FormGFLogic.h"
#include "DB_CellValue.h"
#include "FormEnd.h"
#include "Form2Plus.h"
#include "Form2loadValue.h"
#include "UpdDBbyValue.h"
#include "DB_CNP_value.h"
#include "DB_LinksObjects.h"
#include "DB_ComonNPokaz.h"
#include "DB_Pokaztl.h"
#include "FunCell.h"
#include "Base91Converter.h"

void GetAllItems(TSelCells & sc, TFormLogicF1* FormLogicF1, bool & isUpd_isOnActivForm_RefreshROcells, TwwStringList* p)
{
	std::string OrdS;
	int OrdN;
	// цикл выборки записей
    sc.sp->First();
    while (!sc.sp->EoF()) {
        OrdS = sc.sp->FieldByName("ordern")->AsString();
        OrdS = Trim(OrdS);
        OrdS = "$" + OrdS;
        //MAK OrdN = HexStringToInt(OrdS);
        OrdN = Base91Converter::Base91StringToInt(OrdS);

//        PCellValueAtr vd = TFormDopInp::GetPCellValueAtrByIndex(FormLogicF1, OrdN);//СДЕЛАТЬ ПОИСК ПО ИТЕРАТОРУ итератор увеличиваем дельту

        union
        {
            struct
            {
                unsigned short nRow;
                unsigned short nCol;
            } Ref;
            int number;
        } RowCol;

        RowCol.number = OrdN;
        PCellValueAtr vd = TFormDopInp::GetCellValueAtrByRC(FormLogicF1, RowCol.Ref.nRow, RowCol.Ref.nCol);


        FormLogicF1->f1Forms->Selection_Set(vd->Row, vd->Col, vd->Row, vd->Col);
        double ValueD = sc.sp->FieldByName("value")->AsFloat();
        ValueD = PreobrValue(ValueD, vd->DataType, vd->coef_Pokaz, vd->coef_Forma);

        if (vd->EvalType == 0)
            if (!SameValue(FormLogicF1->f1Forms->NumberRC(vd->Row, vd->Col), ValueD, 0.000001))
                isUpd_isOnActivForm_RefreshROcells = true;


        if (!SameValue(FormLogicF1->f1Forms->NumberRC(vd->Row, vd->Col), ValueD, 0.000001))
            TForm1::SetCellValue_Obnovl(p, FormLogicF1, FloatToStr(ValueD), vd->DataType, vd->Row, vd->Col);
        sc.sp->Next();
    }
    // End of цикл выборки записей
}

void GetAllItems(TSelCells & sc, TFormLogicF1* FormLogicF1, bool & isUpd_isOnActivForm_RefreshROcells, TwwStringList* p, nlohmann::json& JSONresult)
{
    std::string OrdS;
    int OrdN;
    // цикл выборки записей
    sc.sp->First();
    while (!sc.sp->EoF()) {
        OrdS = sc.sp->FieldByName("ordern")->AsString();
        OrdS = Trim(OrdS);
        OrdS = "$" + OrdS;
        //MAK OrdN = HexStringToInt(OrdS);
        OrdN = Base91Converter::Base91StringToInt(OrdS);

//        PCellValueAtr vd = TFormDopInp::GetPCellValueAtrByIndex(FormLogicF1, OrdN);//СДЕЛАТЬ ПОИСК ПО ИТЕРАТОРУ итератор увеличиваем дельту

        union
        {
            struct
            {
                unsigned short nRow;
                unsigned short nCol;
            } Ref;
            int number;
        } RowCol;

        RowCol.number = OrdN;
        PCellValueAtr vd = TFormDopInp::GetCellValueAtrByRC(FormLogicF1, RowCol.Ref.nRow, RowCol.Ref.nCol);


        FormLogicF1->f1Forms->Selection_Set(vd->Row, vd->Col, vd->Row, vd->Col);
        double ValueD = sc.sp->FieldByName("value")->AsFloat();
        ValueD = PreobrValue(ValueD, vd->DataType, vd->coef_Pokaz, vd->coef_Forma);

        if (vd->EvalType == 0)
            if (!SameValue(FormLogicF1->f1Forms->NumberRC(vd->Row, vd->Col), ValueD, 0.000001))
                isUpd_isOnActivForm_RefreshROcells = true;


        if (!SameValue(FormLogicF1->f1Forms->NumberRC(vd->Row, vd->Col), ValueD, 0.000001))
            TForm1::SetCellValue_Obnovl(p, FormLogicF1, FloatToStr(ValueD), vd->DataType, vd->Row, vd->Col);

        JSONresult["Row"].push_back(vd->Row);
        JSONresult["Col"].push_back(vd->Col);
        JSONresult["Values"].push_back(utf16_to_utf8(FormLogicF1->f1Forms->TextRC_Get(vd->Row, vd->Col)));
        JSONresult["Styles"].push_back(utf16_to_utf8(FormLogicF1->f1Forms->StyleNameRC_Get(vd->Row, vd->Col)));

        sc.sp->Next();
    }
    // End of цикл выборки записей
}

// обновление вычисляемых ячеек, это делается после того,
//   - как текущей стала другая форма
//   - как завершилась операция ввода значения в ячейку, здесь vd задан
bool TFormDopUpdLogic::doUpdExecuteCells(int nSheet, TFormLogicF1* FormLogicF1, int aNRow, int aNCol, bool noSetActiveCell, nlohmann::json& JSONresult)
{
    TracerManager::Instance().StartFunctionTrace("TFormDopUpdLogic::doUpdExecuteCells");
    PCellValueAtr aVd;
    PCellValueAtr vd, vdUp;
    PAtrRow AtrRow, AtrRowUp;
    int i = 0, c, r, wi;
    std::string Value;
    double ValueD, ValueD2;
    TSelCells sc;
    TwwStringList* p;
    int Year, Month, Day;
    std::string OrdS;
    int OrdN;
    std::string Stmnt;
    TDateTime OsobDate;
    int wCol1, wCol2;
    int objectid;
    int CurrCellId;
    int CurrPlanId;
    int CurrVidOtch;
    TDateTime CurrValueDate;
    int CurrYear;
    bool isOtherForm = false;
    bool wB;
    bool pLocked, pHidden;
    bool isUpd_isOnActivForm_RefreshROcells; // Устновим true, если такие ячейки изменились
    bool Result = true;

    //{ Примечание: Здесь вначале НЕ устанавливается FormLogicF1.ActionView.doBeginSostSaveAllAction }
    
    r = FormLogicF1->f1Forms->Row;
    c = FormLogicF1->f1Forms->Col;

    isUpd_isOnActivForm_RefreshROcells = true;

    p = FormLogicF1->FormsValuesArray->GetItem(nSheet - 1);
    if (aNRow == 0 && aNCol == 0)
        aVd = nullptr;
    else
        aVd = TFormDopInp::GetCellValueAtrByRC_p(p, aNRow, aNCol);

    try {
        if (p != nullptr)
            if (p->is_sort > 0) {
                TracerManager::Instance().EndFunctionTrace();
                Result = false;
                return Result;
            }
    }
    catch (std::exception& e) {
        TracerManager::Instance().AddSpanLog("Exception.TFormDopUpdLogic::doUpdExecuteCells", {{"errordesr", e.what()}},
                                             TracerManager::LOG_LEVEL_ERROR);
    }

    // в форме будут обновлены только вычисляемые значения (все, никаких фильтров для
    //    эффективности не применяется
    // sc:=TSelCells.Create; Это здесь не нужно
    // ввод данных
    sc.BeginAdd();
    // цикл ввода данных
//MAK    for (i = 0; i < TFormDopInp::GetCellValueAtrCount(FormLogicF1); i++) {
//MAK        vd = TFormDopInp::GetPCellValueAtrByIndex(FormLogicF1, i);
    union
    {
        struct
        {
            unsigned short nRow;
            unsigned short nCol;
        } Ref;
        int number;
    } RowCol;
    for (auto it = TFormDopInp::GetTww(FormLogicF1)->items.begin(); it != TFormDopInp::GetTww(FormLogicF1)->items.end(); ++it) {
        auto vd = std::get<PCellValueAtr>(it->second);
        if (vd->ObjectId == 0 && vd->PokazId == 0 && vd->CellId == 0)
            continue;
        if (p == nullptr || vd->Col >= p->ListStmnt.size()) {
            continue;
        }
        if (p->ListStmnt[vd->Col].Stmnt == "СистХарПоказтля")
            continue;
        wB = (p->isOnActivForm_RefreshROcells) && (isOtherForm) && (vd->EvalType == 0) && (vd->DataType == 1);
        if (wB) {
            FormLogicF1->f1Forms->Selection_Set(vd->Row, vd->Col, vd->Row, vd->Col);
            FormLogicF1->f1Forms->Protection_Get(pLocked, pHidden);
            wB = pLocked;
        }
        else {
            if (aVd != NULL)
                if (vd != aVd)
                    if (vd->ObjectId == aVd->ObjectId && vd->CellId == aVd->CellId && vd->CellId > 0 && vd->PlanId == aVd->PlanId && vd->VidOtch == aVd->VidOtch && vd->ValueDate == aVd->ValueDate)
                        wB = true;
        }
        // Добавление для показателя "Из Них" - 08.10.2019
        if (!wB)
            if (vd->DataType == 1)
                if (TFormDopInp::GetListRowsItem(FormLogicF1, vd->Row)->pokaz_vert == 2)
                    wB = true;

        if ((vd->EvalType > 0) && (vd->DataType == 1) && (vd->BPId == 0) || (wB)) {
            PCellValueAtr vd = std::get<PCellValueAtr>(it->second);

            RowCol.Ref.nRow = vd->Row;
            RowCol.Ref.nCol = vd->Col;
            sc.AddCell(RowCol.number, vd);
            if (sc.CurrNumb >= 1000) {
                sc.Execute();
                sc.sp->Open();
                GetAllItems(sc, FormLogicF1, isUpd_isOnActivForm_RefreshROcells, p);
                sc.sp->Close();
                sc.BeginAdd();
            }
        }
    }
    // End of цикл ввода данных
    
  // если был ввод в характеристику иерархического класса
  if (aVd != NULL)
  {
    AtrRow = TFormDopInp::GetListRowsItem(FormLogicF1, aVd->Row);
    if (AtrRow->CurRowVid != 2)  // делаем если не мероприятие
    for (wCol1 = 1; wCol1 <= p->MaxHrCol; wCol1++)
    {
      if (aVd == NULL) break;  // 24/11/2020

      vd = TFormDopInp::GetCellValueAtrByRC_p(p, aVd->Row, wCol1);
      if (vd == NULL) continue;
      wB = false;
      if (vd->is_Prisv){
        wB = true;
        vd->is_Prisv = false;
      };
      if (! wB) 
        if (wCol1 != aVd->Col)
          continue;
        else wB = (aVd != NULL) && (vd->ObjectId != 0) && (vd->DataType = 1);
      if (wB) 
      while (true) {
        AtrRow = TFormDopInp::GetListRowsItem(FormLogicF1, aVd->Row);
        AtrRowUp = NULL;
        for (i = aVd->Row - 1;  i>= 0; i--)
        {
          AtrRowUp = TFormDopInp::GetListRowsItem(FormLogicF1, i);
          if (AtrRowUp == NULL) continue;
          if ((AtrRow->OwnerId == AtrRowUp->ObjectId) && 
              (AtrRow->objekt_class_id == AtrRowUp->objekt_class_id))
          {
            vd = TFormDopInp::GetCellValueAtrByRC_p(p, i, wCol1);
            wi = TFormDopInp::GetICellValueAtrByRC(FormLogicF1, i, wCol1);
            if (wi < 0) break;
            sc.AddCell(wi, vd);
            if (sc.CurrNumb >= 1000)
            {
              sc.Execute(); // получить данные
              sc.sp->Open(); // доступ к записям
              GetAllItems(sc, FormLogicF1, isUpd_isOnActivForm_RefreshROcells, p); // цикл выборки записей
              sc.sp->Close();
              sc.BeginAdd();
            };
            AtrRow = AtrRowUp;
            continue;
          };
        };
        if (AtrRowUp = NULL)
            break;
      };
    };
  };
  // End of если был ввод в характеристику иерархического класса
    
    if (sc.CurrNumb > 0) {
        sc.Execute();
        sc.sp->Open();
        GetAllItems(sc, FormLogicF1, isUpd_isOnActivForm_RefreshROcells, p, JSONresult);
    }
    //--sc.Free(); Это не нужно
    
    if (isUpd_isOnActivForm_RefreshROcells) {
        // чтобы выполнить имеющиеся операторы ПРИСВОИТЬ (только моментальные, реагирующие на ввод значения)
        // а обновление ячеек вида vd->EvalType = 0 равнозначно вводу значения
        if (!TFormDopUpdLogic::doForPrisvoitUpd(FormLogicF1, JSONresult)) {
            Result = false;
            return Result;
        };
        isUpd_isOnActivForm_RefreshROcells = false;
    }

    if (TFormDopInp::IsUpdinTrue(FormLogicF1)) { // это нужно здесь
        TFormGFLogic::UpdPlGrFin(FormLogicF1);
        TFormGFLogic::UpdFactGrFin(FormLogicF1);
        TFormGFLogic::UpdFactKv(FormLogicF1);
        TFormGFLogic::SetIsUpdToFalse(FormLogicF1);
    }

//MAK    for (i = 0; i < TFormDopInp::GetCellValueAtrCount(FormLogicF1); i++) {
//MAK        vd = TFormDopInp::GetPCellValueAtrByIndex(FormLogicF1, i);
    for (auto it = TFormDopInp::GetTww(FormLogicF1)->items.begin(); it != TFormDopInp::GetTww(FormLogicF1)->items.end(); ++it) {
        auto vd = std::get<PCellValueAtr>(it->second);
        FormLogicF1->f1Forms->Selection_Set(vd->Row, vd->Col, vd->Row, vd->Col);
        objectid = vd->ObjectId;
        CurrCellId = vd->CellId;
        CurrPlanId = vd->PlanId;
        CurrVidOtch = vd->VidOtch;
        CurrValueDate = vd->ValueDate;
        Stmnt = p->ListStmnt[vd->Col].Stmnt;
        if (vd->DataType == (30 + deltaDType))
            continue;
        if (vd->DataType > (10 + deltaDType)) {
            Value = "";
            ValueD = 0;
            ValueD2 = 0;
            switch (vd->DataType) {
                case 11 + deltaDType:
                    continue;
                case 12 + deltaDType:
                    continue;
                case 14 + deltaDType:
                case 17 + deltaDType://'ЗНАЧЕНИЕСВНИ','ЗНАЧЕНИЕНВНИ', 'ОТКЛОНЕНИЕСВНачКВ', 'ОТКЛОНЕНИЕНВНачКВ'
                    objectid = vd->ObjectId;
                    CurrCellId = vd->CellId;
                    CurrPlanId = vd->PlanId;
                    CurrValueDate = vd->ValueDate;
                    DecodeDate(vd->ValueDate, Year, Month, Day);
                    CurrYear = Year;
                    if (vd->DataType == (14 + deltaDType)) {//'ЗНАЧЕНИЕСВНИ', 'ЗНАЧЕНИЕНВНИ'
                        switch (vd->pokaz_type) {
                            case 1: // для числа
                                CurrVidOtch = vd->VidOtch;
                                ValueD = TDB_CellValue::GetValuePeriod(objectid, CurrCellId, CurrPlanId, CurrVidOtch, CurrYear, CurrValueDate);
                                break;
                            case 2: // для строки
                                Value = TDB_CellValue::GetNI_byStr(vd->ValueDate, objectid, CurrCellId, CurrPlanId, CurrVidOtch, Year);
                                break;
                        }
                    }
                    else {// 'ОТКЛОНЕНИЕСВНачКВ', 'ОТКЛОНЕНИЕНВНачКВ'
                        OsobDate = vd->ValueDate;
                        // здесь нужно работать только с OsobDate
                        if (vd->DataType == (17 + deltaDType)) {
                            DecodeDate(IncMonth(OsobDate, -2), Year, Month, Day);
                            OsobDate = EncodeDate(Year, Month, 1);
                        }
                        CurrVidOtch = 5;// Факт
                        ValueD = TDB_CellValue::GetValuePeriod(objectid, CurrCellId, CurrPlanId, CurrVidOtch, CurrYear, OsobDate);
                        CurrVidOtch = 4; // Ут. план
                        ValueD2 = TDB_CellValue::GetValuePeriod(objectid, CurrCellId, CurrPlanId, CurrVidOtch, CurrYear, OsobDate);
                        ValueD = ValueD - ValueD2;
                    }
                    break;
                case 16 + deltaDType:// КБК-Дгвр:Сумма№БО, 'КБК-Дгвр:СуммаБО'
                    continue;
                case 18 + deltaDType: // 'ЗНАЧЕНИЕСВИР','ЗНАЧЕНИЕНВИР'
                    continue;
                case 19 + deltaDType: // 'УТОЧНЕНИЕСВ', 'УТОЧНЕНИЕСВНИ',
                    continue;
                case 20 + deltaDType: // 'Мрпр-Дгвр:Сумма№БО' - раньше было 'УТОЧНЕНИЕНВ', 'Мрпр-Дгвр:СуммаБО' - раньше было 'УТОЧНЕНИЕНВНИ'
                    continue;
                case 21 + deltaDType: // 'ПлнОСТАТОК'
                    continue;
                case 22 + deltaDType: // 'ФктОСТАТОК'
                    continue;
                case 23 + deltaDType:// Агрегат для 'ЗНАЧЕНИЕСВ'
                    continue;
                case 24 + deltaDType: // 'ПлнОСТАТОКНИ'
                    continue;
                case 25 + deltaDType: // 'УТОЧНЕНИЕСВПосле'
                    continue;
                case 41 + deltaDType: // 'КБК-Дгвр:СуммаКЗ'
                    continue;
                case 44 + deltaDType:// 'ФактКвДату'{'УТОЧНЕНИЕСВдф'}
                    continue;
                case 45 + deltaDType:// 'СуммаПлГрНИВМесяце'{'УТОЧНЕНИЕСВНИдф'}
                    continue;
                case 26 + deltaDType:
                case 29 + deltaDType:
                case 31 + deltaDType:
                case 32 + deltaDType:
                case 40 + deltaDType:
                case 51 + deltaDType:
                case 53 + deltaDType:
                case 54 + deltaDType:
                case 55 + deltaDType:
                case 56 + deltaDType:
                case 57 + deltaDType:
                case 58 + deltaDType:
                case 59 + deltaDType:
                case 60 + deltaDType:
                case 61 + deltaDType:
                case 62 + deltaDType:
                case 63 + deltaDType:
                case 64 + deltaDType:
                // 'СуммаПлГрНИВНачале' 'ФактВсегоВНачале' 'ПоступлВсегоВНачале' 'УТВ_УТ_КассПл'
                // 'УточПланВклДату' 'СуммаПлГрНИГод' 'ХарактСВобъекта' 'КонкатСтрок' 'КонкатСтрокСВобъек'
                // 'Экономия/Резерв' 'отчетCO_RP_015' 'СведенияОбЭкономии'                               
                    continue;
            }
            if (ValueD != 0) {
                ValueD = ClearNolDbl(ValueD);
            }
            if (ValueD != 0) {
                Value = FloatToStr(PreobrValue(ValueD, vd->DataType, vd->coef_Pokaz, vd->coef_Forma));
            }
            TForm1::SetCellValue_Obnovl(p, FormLogicF1, Value, vd->DataType, vd->Row, vd->Col);
        }
        if (p->is_dublir_itogo == 1) {
            TFormEnd::ExecIfDublirItogo(FormLogicF1);
        }
    }

    wCol1 = 0;
    wCol2 = 0;
//MAK    for (i = 1; i < TFormDopInp::GetCellValueAtrCount(FormLogicF1); i++) {
//MAK        vd = TFormDopInp::GetPCellValueAtrByIndex(FormLogicF1, i);
    for (auto it = TFormDopInp::GetTww(FormLogicF1)->items.begin(); it != TFormDopInp::GetTww(FormLogicF1)->items.end(); ++it) {
        auto vd = std::get<PCellValueAtr>(it->second);
        if (vd->DataType == (11 + deltaDType))// фактические данные за месяц
            if (vd->Col != wCol1) {
                TFormEnd::ItogoZaMesjc(FormLogicF1, vd->Col);
                wCol1 = vd->Col;
            }
        if (vd->DataType == (11 + deltaDType))// фактические данные за месяц
            TFormEnd::VTomCisle(FormLogicF1, vd->Row, vd->Col);
        if (vd->DataType == (21 + deltaDType) || vd->DataType == (22 + deltaDType)) { // 'ПлнОСТАТОК', 'ФктОСТАТОК'
            if (vd->Col != wCol2) {
                TFormEnd::ItogoZaMesjc(FormLogicF1, vd->Col);
                wCol2 = vd->Col;
            }
            TFormEnd::VTomCisle(FormLogicF1, vd->Row, vd->Col);
        }
    }

    if (!noSetActiveCell)
        FormLogicF1->f1Forms->ActiveCell_Set(r, c);

    TForm2Plus::ExecEvalCells(FormLogicF1);
    TracerManager::Instance().EndFunctionTrace();
    return Result;
}

//bool TFormDopUpdLogic::doForPrisvoitUpd(TFormLogicF1* FormLogicF1) {
//    TracerManager::Instance().StartFunctionTrace("TFormDopUpdLogic::doForPrisvoitUpd_");
//    TwwStringList* p = TFormDopInp::GetTww(FormLogicF1);
//    if (p == nullptr) {TracerManager::Instance().EndFunctionTrace();return;};
//    if (!p->IsStmntPrisvoit) {TracerManager::Instance().EndFunctionTrace();return;}; // если в форме нет оператора Присвоить, то выход
//    if (p->ListRows->IsEmpty()) {TracerManager::Instance().EndFunctionTrace();return;};
//
//    int R1, C1, R2, C2;
//    FormLogicF1->f1Forms->Selection_Get(0, R1, C1, R2, C2);
//
//    // doPrisvoit;
//    TFormDopUpdLogic::doPrisvoitOnly(FormLogicF1, p);
//    TFormDopUpdLogic::doEvalCells(FormLogicF1, p);
//
//    while (true) {
//        // --if not doPrisvoit then break;
//        if (!TFormDopUpdLogic::doPrisvoitOnly(FormLogicF1, p)){
//            break;
//        }
//        TFormDopUpdLogic::doEvalCells(FormLogicF1, p);
//    }
//
//    if (p->form_is_main != 2 && p->isExecAllVspomogForms) { // не вспомогателная и задан признак - выполнить все вспомогательные формы
//        int wSheet = FormLogicF1->f1Forms->Sheet;
//
//        // открыть все вспомогательные формы, если еще не открыты
//        // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//        // ++++++++++++++++++++FormShowF1.openAllVspomogatForms;
//        //!!!!!!!!!!!!! ДОБАВИТЬ
//        // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
//        FormLogicF1->f1Forms->Sheet = wSheet;
//        // End of -> открыть все вспомогательные формы, если еще не открыты
//
//        for (int i = 1; i <= FormLogicF1->f1Forms->NumSheets; i++) {
//            TwwStringList* wp = FormLogicF1->FormsValuesArray->GetItem(i - 1);
//            if (wp->form_is_main == 2) {
//                FormLogicF1->isDoSelChangeVspomogat = true; // ++   03/12/2019
//                FormLogicF1->f1Forms->Sheet = i;
//                TForm2loadValue::SetInputValue(FormLogicF1, wp, false, 0); // isRowMode, aRow
//                TFormDopUpdLogic::doPrisvoitOnly(FormLogicF1, wp); // 03/12/2019
//                FormLogicF1->isDoSelChangeVspomogat = false; // ++ 03/12/2019
//            }
//        }
//        FormLogicF1->f1Forms->Sheet_Set(wSheet);
//    }
//    FormLogicF1->f1Forms->Selection_Set(R1, C1, R2, C2);
//    TracerManager::Instance().EndFunctionTrace();
//}

bool TFormDopUpdLogic::doForPrisvoitUpd(TFormLogicF1* FormLogicF1, nlohmann::json& JSONresult) {
    TracerManager::Instance().StartFunctionTrace("TFormDopUpdLogic::doForPrisvoitUpd");
    TwwStringList* p = TFormDopInp::GetTww(FormLogicF1);
    bool Result = true;
    if (p == nullptr) {TracerManager::Instance().EndFunctionTrace(); return Result;}
    if (!p->IsStmntPrisvoit) {TracerManager::Instance().EndFunctionTrace();return Result;}; // если в форме нет оператора Присвоить, то выход
    if (p->ListRows->IsEmpty()) {TracerManager::Instance().EndFunctionTrace();return Result;};

    int R1, C1, R2, C2;
    FormLogicF1->f1Forms->Selection_Get(0, R1, C1, R2, C2);

    // doPrisvoit;
    Result = TFormDopUpdLogic::doPrisvoitOnly(FormLogicF1, p, JSONresult);
    TFormDopUpdLogic::doEvalCells(FormLogicF1, p);
    TracerManager::Instance().EndFunctionTrace();
    ///!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    return Result;

    while (true) {
        // --if not doPrisvoit then break;
        if (!TFormDopUpdLogic::doPrisvoitOnly(FormLogicF1, p, JSONresult)){
            break;
        }
        TFormDopUpdLogic::doEvalCells(FormLogicF1, p);
    }

    if (p->form_is_main != 2 && p->isExecAllVspomogForms) { // не вспомогателная и задан признак - выполнить все вспомогательные формы
        int wSheet = FormLogicF1->f1Forms->Sheet;

        // открыть все вспомогательные формы, если еще не открыты
        // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
        // ++++++++++++++++++++FormShowF1.openAllVspomogatForms;
        //!!!!!!!!!!!!! ДОБАВИТЬ
        // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

        FormLogicF1->f1Forms->Sheet = wSheet;
        // End of -> открыть все вспомогательные формы, если еще не открыты

        for (int i = 1; i <= FormLogicF1->f1Forms->NumSheets; i++) {
            TwwStringList* wp = FormLogicF1->FormsValuesArray->GetItem(i - 1);
            if (wp->form_is_main == 2) {
                FormLogicF1->isDoSelChangeVspomogat = true; // ++   03/12/2019
                FormLogicF1->f1Forms->Sheet = i;
                TForm2loadValue::SetInputValue(FormLogicF1, wp, false, 0); // isRowMode, aRow
                TFormDopUpdLogic::doPrisvoitOnly(FormLogicF1, wp, JSONresult); // 03/12/2019
                FormLogicF1->isDoSelChangeVspomogat = false; // ++ 03/12/2019
            }
        }
        FormLogicF1->f1Forms->Sheet_Set(wSheet);
    }
    FormLogicF1->f1Forms->Selection_Set(R1, C1, R2, C2);
}

//bool TFormDopUpdLogic::doPrisvoitOnly(TFormLogicF1* FormLogicF1, TwwStringList* p) {
//    TracerManager::Instance().StartFunctionTrace("TFormDopUpdLogic::doPrisvoitOnly_");
//    int j, toCol;
//    double d0, d0edIzm, d1;
//    PCellValueAtr vd1;
//    bool Result = false;
//    int LastCol = FormLogicF1->f1Forms->LastCol_Get();
//    for (j = p->form_begBodyRow; j <= p->form_endBodyRow; j++) { // начинаем с начала формы
//
//        PAtrRow AtrRow = ((*p->ListRows)[j]);
//
//        if (AtrRow == nullptr) continue;
//        if ((AtrRow->CurRowVid == 14 /* ПРИСВОИТЬ */) && (AtrRow->IsUpd)) {
//            for (toCol = 1; toCol <= LastCol; toCol++) {
//                switch (FormLogicF1->f1Forms->TypeRC_Get(AtrRow->Row, toCol)) {
//                    case -1:
//                    case -2:
//                    case -3: { // в ячейке есть формула
//                        d0 = FormLogicF1->f1Forms->NumberRC_Get(AtrRow->Row, toCol);
//
//                        PAtrRow AtrRow1 = ((*p->ListRows)[j + 1]);
//                        if (vd1->EvalType != 0){
//                            Result = false;
//                            TracerManager::Instance().EndFunctionTrace();
//                            return Result;
//                        }
//                        if (AtrRow1 == nullptr) continue;
//                        vd1 = TFormDopInp::GetCellValueAtrByRC(FormLogicF1, AtrRow1->Row, toCol);
//                        if (vd1 == nullptr) continue;
//                        if (vd1->pokaz_type > 1) continue;
//                        d0edIzm = PreobrValue(d0, vd1->pokaz_type, vd1->coef_Forma, vd1->coef_Pokaz);
//                        d1 = FormLogicF1->f1Forms->NumberRC_Get(vd1->Row, vd1->Col);
//                        if (!SameValue(d0, d1, 0.000001)) {
//                            Result = true;
//                            FormLogicF1->f1Forms->NumberRC_Set(vd1->Row, vd1->Col, d0);
//                            bool msg=false;
//                            if (vd1->CellId > 0)
//                                TUpdDBbyValue::UpdDBbyValue1(vd1->CellId, vd1->PlanId, vd1->VidOtch, vd1->ValueDate, d0edIzm, vd1->ObjectId, msg);
//                            else
//                                TDB_CNP_value::addNumberItem(vd1->PokazId, vd1->comnp_id, vd1->cell_period, vd1->PlanId,
//                                                             vd1->VidOtch, vd1->god, vd1->kvrtl, vd1->mesjc, vd1->den, d0edIzm);
//
//                            // !!!!!!!!!!!!!!!!!  27.02.2021
//                            if (p->ListStmnt[vd1->Col].stmntPrisvoitLink > 0)
//                                TFormDopUpdLogic::doStmntAssignByVvodPokaztl(FormLogicF1, p, vd1->Row, p->ListStmnt[vd1->Col].stmntPrisvoitLink, vd1->Col);
//
//                            // ПРОШЕРСТИТЬ ВО ВСЕХ ОТКРЫТЫХ ФОРМАХ (НЕ ВСПОМОГАТЕЛЬНЫХ И НЕ ТЕКУЩИЕ)ЯЧЕЙКИ И НАЙТИ ТАКУЮ У КОТОРОЙ VD ИДЕНТИЧЕН VD1
//                            // И В ��АКУЮ ЯЧЕЙКУ ПОЛОЖ��ТЬ ЗНАЧЕНИЕ(НЕ В БАЗУ ДАННЫХ) С учетом еденици измерения формы
//                            TFormDopUpdLogic::doRefreshComputedCells((vd1->CellId == 0), FormLogicF1, p, vd1, d0edIzm);
//                        }
//                    }
//                }
//            }
//        }
//    }
//
//    TracerManager::Instance().EndFunctionTrace();
//    return Result;
//}

bool TFormDopUpdLogic::doPrisvoitOnly(TFormLogicF1* FormLogicF1, TwwStringList* p, nlohmann::json& JSONresult) {
    TracerManager::Instance().StartFunctionTrace("TFormDopUpdLogic::doPrisvoitOnly");
    int j, toCol;
    double d0, d0edIzm, d1;
    PCellValueAtr vd1;
    bool Result = false;
    int LastCol = FormLogicF1->f1Forms->LastCol_Get();
    for (j = p->form_begBodyRow; j <= p->form_endBodyRow; j++) { // начинаем с начала формы
        PAtrRow AtrRow = ((*p->ListRows)[j]);

        if (AtrRow == nullptr) continue;
        if ((AtrRow->CurRowVid == 14 /* ПРИСВОИТЬ */) && (AtrRow->IsUpd)) {
            for (toCol = 1; toCol <= LastCol; toCol++) {
                switch (FormLogicF1->f1Forms->TypeRC_Get(AtrRow->Row, toCol)) {
                    case -1:
                    case -2:
                    case -3: { // в ячейке есть формула
                        d0 = FormLogicF1->f1Forms->NumberRC_Get(AtrRow->Row, toCol);
                        PAtrRow AtrRow1 = ((*p->ListRows)[j + 1]);

                        if (AtrRow1 == nullptr) continue;
                        vd1 = TFormDopInp::GetCellValueAtrByRC(FormLogicF1, AtrRow1->Row, toCol);
                        if (vd1->EvalType != 0){
//                            JSONresult.clear();
//                            JSONresult["error"] = "Попытка выполнить присвоение в вычисляемою ячейку. Строка "+to_string(AtrRow1->Row);
//                            Result = false;
//                            TracerManager::Instance().EndFunctionTrace();
//                            return Result;
                            continue;
                        }
                        if (vd1 == nullptr) continue;
                        if (vd1->pokaz_type > 1) continue;
                        d0edIzm = PreobrValue(d0, vd1->pokaz_type, vd1->coef_Forma, vd1->coef_Pokaz);
                        d1 = FormLogicF1->f1Forms->NumberRC_Get(vd1->Row, vd1->Col);
                        if (!SameValue(d0, d1, 0.000001)) {
                            Result = true;
                            JSONresult["Row"].push_back(AtrRow->Row);
                            JSONresult["Col"].push_back(toCol);
                            JSONresult["Values"].push_back(d0);
                            JSONresult["Styles"].push_back(utf16_to_utf8(FormLogicF1->f1Forms->StyleNameRC_Get(AtrRow->Row, toCol)));

                            JSONresult["Row"].push_back(vd1->Row);
                            JSONresult["Col"].push_back(vd1->Col);
                            JSONresult["Values"].push_back(d0);
                            JSONresult["Styles"].push_back(utf16_to_utf8(FormLogicF1->f1Forms->StyleNameRC_Get(vd1->Row, vd1->Col)));

                            FormLogicF1->f1Forms->NumberRC_Set(vd1->Row, vd1->Col, d0);
                            bool msg = false;
                            if (vd1->CellId > 0) {
                                TUpdDBbyValue::UpdDBbyValue1(vd1->CellId, vd1->PlanId, vd1->VidOtch, vd1->ValueDate, d0edIzm, vd1->ObjectId, msg);
                            } else {
                                TDB_CNP_value::addNumberItem(vd1->PokazId, vd1->comnp_id, vd1->cell_period, vd1->PlanId,vd1->VidOtch, vd1->god, vd1->kvrtl, vd1->mesjc, vd1->den, d0edIzm);
                            }

                            // !!!!!!!!!!!!!!!!!  27.02.2021
                            if (p->ListStmnt[vd1->Col].stmntPrisvoitLink > 0)
                                TFormDopUpdLogic::doStmntAssignByVvodPokaztl(FormLogicF1, p, vd1->Row, p->ListStmnt[vd1->Col].stmntPrisvoitLink, vd1->Col);

                            // ПРОШЕРСТИТЬ ВО ВСЕХ ОТКРЫТЫХ ФОРМАХ (НЕ ВСПОМОГАТЕЛЬНЫХ И НЕ ТЕКУЩИЕ)ЯЧЕЙКИ И НАЙТИ ТАКУЮ У КОТОРОЙ VD ИДЕНТИЧЕН VD1
                            // И В ��АКУЮ ЯЧЕЙКУ ПОЛОЖ��ТЬ ЗНАЧЕНИЕ(НЕ В БАЗУ ДАННЫХ) С учетом еденици измерения формы
                            TFormDopUpdLogic::doRefreshComputedCells((vd1->CellId == 0), FormLogicF1, p, vd1, d0edIzm);
                        }
                    }
                }
            }
        }
    }

    TracerManager::Instance().EndFunctionTrace();
    return Result;
}

// обновление только вычисляемых значения
void TFormDopUpdLogic::doEvalCells(TFormLogicF1* FormLogicF1, TwwStringList* p) {
    TracerManager::Instance().StartFunctionTrace("TFormDopUpdLogic::doEvalCells");
    int i;
    TSelCells sc;
    std::string OrdS;
    int OrdN;
    double ValueD, ValueD2;
    PCellValueAtr vd;

    // перенесено выше
    /*auto GetAllItems = [&]() {
        // Record selection loop
        sc.sp->First();
        while (!sc.sp->EoF()) {
            OrdS = sc.sp->FieldByName("ordern")->AsString;
            OrdS = trim(OrdS);
            OrdS = "$" + OrdS;
            OrdN = std::stoi(OrdS);
            vd = TFormDopInp::GetPCellValueAtrByIndex(FormLogicF1, OrdN);
            FormLogicF1->f1Forms->Selection_Set(vd->Row, vd->Col, vd->Row, vd->Col);
            ValueD = sc.sp->FieldByName("value")->AsFloat;
            ValueD = PreobrValue(ValueD, vd->DataType, vd->coef_Pokaz, vd->coef_Forma);
            TForm1::SetCellValue_Obnovl(p, FormLogicF1, std::to_string(ValueD), vd->DataType, vd->Row, vd->Col);
            sc.sp->Next();
        }
        // End of record selection loop
    };*/

    // в форме будут обновлены только вычисляемые значения (все, никаких фильтров для
    //    эффективности не применяется
    // sc:=TSelCells.Create; Не нужно
    // ввод данных
    sc.BeginAdd();
    // Data entry loop
    bool isUpd_isOnActivForm_RefreshROcells;
    union
    {
        struct
        {
            unsigned short nRow;
            unsigned short nCol;
        } Ref;
        int number;
    } RowCol;

//MAK    for (int i = 0; i < TFormDopInp::GetCellValueAtrCount(FormLogicF1); i++) {
//        auto vd = TFormDopInp::GetPCellValueAtrByIndex(FormLogicF1, i);
//
//        RowCol.Ref.nRow = vd->Row;
//        RowCol.Ref.nCol = vd->Col;
//        //RowCol.number = i;
//
//        if (vd->EvalType > 0 && vd->DataType == 1 && vd->BPId == 0) {
//            sc.AddCell(RowCol.number, vd);
//            if (sc.CurrNumb >= 1000) {
//                sc.Execute(); // Получить данные
//                sc.sp->Open(); // Доступ к записям
//                GetAllItems(sc, FormLogicF1, isUpd_isOnActivForm_RefreshROcells, p); // Цикл выбора записей
//                sc.sp->Close();
//                sc.BeginAdd();
//            }
//        }
//MAK    }
    for (auto it = TFormDopInp::GetTww(FormLogicF1)->items.begin(); it != TFormDopInp::GetTww(FormLogicF1)->items.end(); ++it) {
        auto vd = std::get<TCellValueAtr*>(it->second);

        RowCol.Ref.nRow = vd->Row;
        RowCol.Ref.nCol = vd->Col;

        if (vd->EvalType > 0 && vd->DataType == 1 && vd->BPId == 0) {
            sc.AddCell(RowCol.number, vd);
            if (sc.CurrNumb >= 1000) {
                sc.Execute(); // Получить данные
                sc.sp->Open(); // Доступ к записям
                GetAllItems(sc, FormLogicF1, isUpd_isOnActivForm_RefreshROcells, p); // Цикл выбора записей
                sc.sp->Close();
                sc.BeginAdd();
            }
        }
    }
    // End of data entry loop
    sc.Execute(); // Get data
    sc.sp->Open(); // Access records
    GetAllItems(sc, FormLogicF1, isUpd_isOnActivForm_RefreshROcells, p);// ТУТ ВНУТРИ ТОЖЕ ГЛЯНУТЬ
    //--sc.Free(); Не нужно
    TracerManager::Instance().EndFunctionTrace();
}

    // вынесена локальная процедура
    string GetStrCellValue(TFormLogicF1* FormLogicF1, int aRow, int aCol){
    	/*TCellValue cv = FormLogicF1.f1Forms.Xls.GetCellValue(aRow, aCol);
        if (cv.IsString()) {
            return cv.AsString();
        } else if (cv.IsFormula()) {
            cv = FormLogicF1.f1Forms.xls.RecalcCell(FormLogicF1.f1Forms.Xls.ActiveSheet, aRow, aCol, false);
            return cv.ToStringInvariant();
        } else {
            return "";
        }*/
        std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> converter;
        return converter.to_bytes(FormLogicF1->f1Forms->TextRC_Get(aRow, aCol));
    };

    // вынесена локальная процедура
    // вычисляем для одного определения
    bool eval1(TFormLogicF1* FormLogicF1, TwwStringList * p, int aCol, int nRow)
    {
        int ColF, ColP;
        PCellValueAtr vd;
        double summa, saveSum, OldValue, vvodZnac;
        std::string sCellValue;

        ColF = p->ListStmnt[aCol].god_plus; // Column with formula
        ColP = p->ListStmnt[aCol].god_ravno; // Numeric indicator - receiver

        vd = TFormDopInp::GetCellValueAtrByRC(FormLogicF1, nRow, ColP);
        switch (vd->pokaz_type) {
            case 1: // Number
                summa = FormLogicF1->f1Forms->NumberRC_Get(nRow, ColF);
                saveSum = PreobrValue(summa, vd->DataType, vd->coef_Forma, vd->coef_Pokaz);

                // Read old value to restore if needed
                OldValue = TDB_CellValue::GetValue(vd->ObjectId, vd->CellId, vd->PlanId, vd->VidOtch, vd->ValueDate);

                if (TUpdDBbyValue::UpdDBbyValue1(vd->CellId, vd->PlanId, vd->VidOtch, vd->ValueDate, saveSum, vd->ObjectId, false)) {
                    FormLogicF1->f1Forms->NumberRC_Set(nRow, ColP, summa);
                    // 06.06.2022
                    if (vd->kakAgregirUpByField == 8) // If aggregating to the owner event
                        TFormDopUpdLogic::doAgregirUpByField(p, vd);

                    vd->is_Prisv = true;
                } else {
                    // Restore old value
                    TUpdDBbyValue::UpdDBbyValue1(vd->CellId, vd->PlanId, vd->VidOtch, vd->ValueDate, OldValue, vd->ObjectId, false);
                    return false;
                }
                break;
            case 2: // String
            case 5: // Date

                sCellValue = GetStrCellValue(FormLogicF1, nRow, ColF);
                FormLogicF1->f1Forms->TextRC_Set(nRow, ColP, utf8_to_utf16(sCellValue).c_str());
                if (p->ListStmnt[ColP].Stmnt == "ЗНАЧЕНИЕобщНП") {
                    TDB_CNP_value::addStringItem(vd->PokazId, vd->comnp_id, vd->cell_period, vd->PlanId,
                                                 vd->VidOtch, vd->god, vd->kvrtl, vd->mesjc, vd->den, sCellValue);
                } else {
                    TDB_CellValue::SetStrValue2(sCellValue, vd->ObjectId, vd->CellId, vd->PlanId, vd->VidOtch, vd->ValueDate);
                }
                break;
        }
        return true;
    };


// вычисления для оператора 'ОперПрисвоить', определенного в столбце
//   связанного с вводом показатещля
// nRow - строка, где введено значение
// nCol - столбец с оператором вертикального Присваивания
// nColVvod - столбец, куда введено значение
bool TFormDopUpdLogic::doStmntAssignByVvodPokaztl(TFormLogicF1* FormLogicF1, TwwStringList* p, int nRow, int nCol, int nColVvod) {
    TracerManager::Instance().StartFunctionTrace("TFormDopUpdLogic::doStmntAssignByVvodPokaztl");
    int i, colFrom, colTo;

    bool result = true;

    // Evaluate for the given column
    if (!eval1(FormLogicF1, p, nCol, nRow)) {
        TracerManager::Instance().EndFunctionTrace();
        return false;
    }
    int LastCol = FormLogicF1->f1Forms->LastCol_Get();
    // вычислим остальные, если они есть    
    for (i = nCol + 1; i <= LastCol; i++) {
        colFrom = TCol1_Col2::getCol1(p->ListStmnt[i].field_id);//{в этом поле запомнен столбец}
        colTo = TCol1_Col2::getCol2(p->ListStmnt[i].field_id);//{в этом поле запомнен столбец}
        if (p->ListStmnt[i].Stmnt == "ОперПрисвоить" && (((colFrom == nColVvod) && (colTo == 0)) || ((colFrom <= nColVvod) && (colTo >= nColVvod)))) {
            if (!eval1(FormLogicF1, p, i, nRow)) {
                TracerManager::Instance().EndFunctionTrace();
                return false;
            }
        }
    }
    TracerManager::Instance().EndFunctionTrace();
    return result;
}

// p - описание текущей формы vd - описание текущей яечйки  d0edIzm - значение исходной ячейки в минимальной еденице измерения
//isComNP - если true значит общ.нат.показ. иначе обыч.показатель.
void TFormDopUpdLogic::doRefreshComputedCells(bool isComNP, TFormLogicF1* FormLogicF1, TwwStringList* p, PCellValueAtr vd, double d0edIzm) {
    TracerManager::Instance().StartFunctionTrace("TFormDopUpdLogic::doRefreshComputedCells");
    int i, j, nRow, nCol;
    TwwStringList * wpNew;
    PCellValueAtr vdNew;
    double wd;

    for (i = 1; i <= FormLogicF1->f1Forms->NumSheets_Get(); i++) {
        wpNew = FormLogicF1->FormsValuesArray->GetItem(i - 1);
        if (wpNew->form_is_main == '2') { // вспомогательная форма
            continue;
        }
        if (wpNew->FormId == p->FormId) { // текущая форма
            continue;
        }
        for (j = 0; j < wpNew->Count(); j++) {
            vdNew = std::get<PCellValueAtr>(wpNew->Objects[j]);
            if (vdNew == nullptr) {
                continue;
            }
            if (vdNew->pokaz_type != 1) {
                continue;
            }
            if (!isComNP) {
                if (vd->CellId == vdNew->CellId &&
                    vd->PlanId == vdNew->PlanId &&
                    vd->VidOtch == vdNew->VidOtch &&
                    vd->ValueDate == vdNew->ValueDate &&
                    vd->ObjectId == vdNew->ObjectId) {
                    wd = PreobrValue(d0edIzm, vdNew->pokaz_type, vdNew->coef_Pokaz, vdNew->coef_Forma);
                    FormLogicF1->f1Forms->NumberSRC_Set(i, vdNew->Row, vdNew->Col, wd);
                }
            } else {
                if (vd->PokazId == vdNew->PokazId &&
                    vd->comnp_id == vdNew->comnp_id &&
                    vd->cell_period == vdNew->cell_period &&
                    vd->PlanId == vdNew->PlanId &&
                    vd->VidOtch == vdNew->VidOtch &&
                    vd->god == vdNew->god &&
                    vd->kvrtl == vdNew->kvrtl &&
                    vd->mesjc == vdNew->mesjc &&
                    vd->den == vdNew->den) {
                    wd = PreobrValue(d0edIzm, vdNew->pokaz_type, vdNew->coef_Pokaz, vdNew->coef_Forma);
                    FormLogicF1->f1Forms->NumberSRC_Set(i, vdNew->Row, vdNew->Col, wd);
                }
            }
        }
    }
    TracerManager::Instance().EndFunctionTrace();
}

// вычисл и сохр агрегированную сумму для мероприятия-владельца для подчиненных объектов
void TFormDopUpdLogic::doAgregirUpByField(TwwStringList* p, PCellValueAtr vd) {
    TDB_LinksObjects::doAgregirUpByField(*p, vd);
}


// возвращает атрибуты ячейуи
bool TFormDopUpdLogic::CellValueAtr(TFormLogicF1* FormLogicF1, int nSheet, int nLocRow, int nLocCol, json& JSONresult) {
    TwwStringList* p;
    PAtrStmnt atrStmnt;
    PCellValueAtr vd; 
    TStringList* enumNames;
    std::string enumName;
    std::string gradations; 
    std::string Stmnt;
    PAtrRow AtrRow;
    bool readOnly = false;
    bool bLocked = false;
    bool bHidden = false;
    TQueryUni *qTmp = new TQueryUni(DefDBconnect.Conn);
    try {
        p = TFormDopInp::GetTww(FormLogicF1);
        if (p && (nLocCol < p->ListStmnt.size())) {
            atrStmnt = &(p->ListStmnt[nLocCol]);
            vd = TFormDopInp::GetCellValueAtrByRC_p(p, nLocRow, nLocCol);


            PAtrRow AtrRow = ((*p->ListRows)[nLocRow]);
            FormLogicF1->f1Forms->Selection_Set(nLocRow, nLocCol, nLocRow, nLocCol);
            FormLogicF1->f1Forms->Protection_Get(bLocked, bHidden);
            if (bLocked) {
                readOnly = true;
            }


            Stmnt = atrStmnt->Stmnt;
            if (atrStmnt->readonly != 0) {
                readOnly = true;
            }
            if (vd->read_onlyProg) {
                readOnly = true;
            }
            if (AtrRow->isReadOnly) {
                readOnly = true;
            }

            if (Stmnt == "ЗНАЧЕНИЕобщНП") {
                vd->EnumId = TDB_ComonNPokaz::GetEnum_id(vd->comnp_id);
            } else {
                vd->EnumId = TDB_Pokaztl::GetEnumId(vd->PokazId);
            }
            JSONresult["TblSessionId"] = FormLogicF1->TblSessionId;
            JSONresult["nSheet"] = nSheet;
            JSONresult["nRow"] = nLocRow;
            JSONresult["nCol"] = nLocCol;
            JSONresult["readOnly"] = readOnly;
            JSONresult["cellType"] = vd->pokaz_type;

            JSONresult["ObjectId"] = vd->ObjectId;
            JSONresult["CellId"] = vd->CellId;
            JSONresult["PlanId"] = vd->PlanId;
            JSONresult["VidOtch"] = vd->VidOtch;
            JSONresult["ValueDate"] = DateTimeToStr(vd->ValueDate);
            JSONresult["PokazId"] = vd->PokazId;
            JSONresult["comnp_id"] = vd->comnp_id;
            JSONresult["cell_period"] = vd->cell_period;
            JSONresult["God"] = vd->god;
            JSONresult["Kvrtl"] = vd->kvrtl;
            JSONresult["Mesjc"] = vd->mesjc;
            JSONresult["Den"] = vd->den;


            // Получение и добавление ранее введённых значений
            json oldStringsArr;
            if (vd->CellId > 0) {
                qTmp->SQL->Clear();
                qTmp->SQL->Add("SELECT DISTINCT str_value FROM Cell_value");
                qTmp->SQL->Add("WHERE cell_id = :cell_id");
                qTmp->SQL->Add("AND plan_id = :plan_id");
                qTmp->SQL->Add("AND vid_otc = :vid_otc");
                qTmp->SQL->Add("ORDER BY str_value");
                qTmp->ParamByName("cell_id")->AsInteger() =  vd->CellId;
                qTmp->ParamByName("plan_id")->AsInteger() = vd->PlanId;
                qTmp->ParamByName("vid_otc")->AsInteger() = vd->VidOtch;
            } else {
                qTmp->SQL->Clear();
                qTmp->SQL->Add("SELECT DISTINCT str_value");
                qTmp->SQL->Add("FROM cnp_value");
                qTmp->SQL->Add("WHERE pokaz_id > 0");
                qTmp->SQL->Add("AND comnp_id = :comnp_id");
                qTmp->SQL->Add("AND vid_period = :vid_period");
                qTmp->SQL->Add("AND plan_id = :plan_id");
                qTmp->SQL->Add("AND vid_otc = :vid_otc");
                qTmp->ParamByName("comnp_id")->AsInteger() = vd->comnp_id;
                qTmp->ParamByName("vid_period")->AsInteger() = vd->cell_period;
                qTmp->ParamByName("plan_id")->AsInteger() =  vd->PlanId;
                qTmp->ParamByName("vid_otc")->AsInteger() =  vd->VidOtch;
            }
            qTmp->Open();
            while (!qTmp->EoF()) {
                oldStringsArr.push_back(qTmp->FieldByName("str_value").AsString());
                qTmp->Next();
            }

            if (oldStringsArr == nullptr){
                oldStringsArr.push_back("");
                JSONresult.emplace("stringOptions", oldStringsArr);
            } else {
                JSONresult.emplace("stringOptions", oldStringsArr);
            }


            if (vd->pokaz_type == 3) {// Проверяем, соответствует ли тип данных перечислению
                enumName = TDB_Enum::GetEnumName(vd->EnumId);// Получаем имя перечисления
                // Инициализация объекта enum и его градаций
                json enumObj;
                json itemsArr;

                TDB_Enum::GetEnumGradAll(qTmp, vd->EnumId); // Заполняем qTmp данными о градациях
                qTmp->First();// Убедимся, что начинаем чтение с первой записи
                while (!qTmp->EoF()) {
                    json itemObj;
                    itemObj.emplace("id", qTmp->FieldByName("enum_id").AsInteger());
                    itemObj.emplace("name", qTmp->FieldByName("enum_name").AsString());
                    itemsArr.push_back(itemObj); // Добавляем объект градации в массив
                    qTmp->Next();
                }
                // Добавляем собранную информацию в объект enum
                enumObj.emplace("id", vd->EnumId);
                enumObj.emplace("name", enumName);
                enumObj.emplace("items", itemsArr);
                JSONresult.emplace("enum", enumObj); // Добавляем enum в итоговый JSON объект

                json validatorObj ;
                validatorObj.emplace("nValidator", atrStmnt->nValidator);
                validatorObj.emplace("vid_uslovij", atrStmnt->vid_uslovij);
                validatorObj.emplace("Value1", atrStmnt->Value1);
                validatorObj.emplace("Value2", atrStmnt->Value2);
                validatorObj.emplace("Shablon", atrStmnt->Shablon);
                validatorObj.emplace("Msg", atrStmnt->Msg);
                JSONresult.emplace("validator", validatorObj);
            } else {
                JSONresult["enum"] = nullptr;
            }
        }
        delete qTmp;
        return true;
    } catch (std::exception& e) {
        TracerManager::Instance().AddSpanLog("Exception.TFormDopUpdLogic::CellValueAtr", {{"errordesr", e.what()}},
                                             TracerManager::LOG_LEVEL_ERROR);
        JSONresult.emplace("message", "Ячейка не найдена");
        delete qTmp;
        return false;
    }

}

bool TFormDopUpdLogic::CellEndingEdit(TFormLogicF1* FormLogicF1, int nSheet, int nRow, int nCol, std::string editValue, int valueID, nlohmann::json& JSONresult) {
//    if (editValue.empty()) return false;

    FormLogicF1->ActionView->doBeginSostSaveAllAction();
    PCellValueAtr vd = TFormDopInp::GetCellValueAtrByRC(FormLogicF1, nRow, nCol);
    TwwStringList * p = TFormDopInp::GetTww(FormLogicF1);
    std::string oldvalue = char16ToUTF8(FormLogicF1->f1Forms->TextRC_Get(nRow, nCol));
    int idItem;
    bool Result=false;
    if (vd->ObjectId==0) {
        idItem = vd->PokazId;
    }
    else {
        idItem = vd->ObjectId;
    }
    switch (vd->pokaz_type) {
        case 0: // Примечание, Ведомство
            break;

        case 1: { // число
            if (vd->cell_period == 0) {
                TDB_CellValue::SetValueUslPost(editValue, vd);
                return true;
            }

            double FloatValue = StrToFloat(editValue);
            FloatValue = PreobrValue(FloatValue, vd->DataType, vd->coef_Forma, vd->coef_Pokaz);

            if (vd->DataType == 1) {
                if (p->ListStmnt[nCol].vid_uslovij == 8) {
                    oldvalue = std::to_string(TDB_CellValue::GetValue(vd->ObjectId, vd->CellId, vd->PlanId, vd->VidOtch, vd->ValueDate));
                }

                if (TUpdDBbyValue::UpdDBbyValue1(vd->CellId, vd->PlanId, vd->VidOtch, vd->ValueDate, FloatValue, vd->ObjectId, false, JSONresult)) {
                    TDB_GurnProtokol::execProtokolAddItem(vd->CellId, vd->PlanId, vd->VidOtch, vd->ValueDate, FloatValue, vd->ObjectId, PreobrValue(FormLogicF1->f1Forms->NumberRC(vd->Row, vd->Col), vd->DataType, vd->coef_Forma, vd->coef_Pokaz));

                    if (editValue.empty())
                        TDB_CellValue::DelOnlyValue(vd->ObjectId, vd->CellId, vd->PlanId, vd->VidOtch, vd->ValueDate);

                    if (vd->kakAgregirUpByField == 8)
                        doAgregirUpByField(p, vd);

                    FormLogicF1->f1Forms->NumberRC_Set(nRow, nCol, StrToFloat(editValue));


                } else if (p->ListStmnt[nCol].vid_uslovij == 8) {
                    TUpdDBbyValue::UpdDBbyValue1(vd->CellId, vd->PlanId, vd->VidOtch, vd->ValueDate, std::stod(oldvalue), vd->ObjectId, false);
                }
            } else if (vd->DataType == 54 + deltaDType) {
                if (vd->ObjectId == 0){
                    idItem = vd->PokazId;
                } else {
                    idItem = vd->ObjectId;
                }
                TDB_CNP_value::addNumberItem(idItem, vd->comnp_id, vd->cell_period, vd->PlanId,
                                            vd->VidOtch, vd->god, vd->kvrtl, vd->mesjc, vd->den, FloatValue);
            }
            FormLogicF1->f1Forms->Recalc();
            Result = TFormDopUpdLogic::doForPrisvoitUpd(FormLogicF1, JSONresult);
            FormLogicF1->f1Forms->NumberRC_Set(nRow, nCol, StrToFloat(editValue));
            break;
        }
        case 2:
        case 6: { // текст
            std::string stmnt = p->ListStmnt[nCol].Stmnt;
            FormLogicF1->f1Forms->TextRC_Set(nRow, nCol, utf8_to_utf16(editValue).c_str());
            if (stmnt == "ЗНАЧЕНИЕобщНП") {
                TDB_CNP_value::addStringItem(idItem, vd->comnp_id, vd->cell_period, vd->PlanId,
                                             vd->VidOtch, vd->god, vd->kvrtl, vd->mesjc, vd->den, editValue);
            } else {
                TDB_CellValue::SetStrValue(vd, editValue, vd->ObjectId, vd->CellId, vd->PlanId, vd->VidOtch, vd->ValueDate);
            }
            break;
        }
        case 3: { // перечисление
            int enum_id = valueID;
            std::string stmnt = p->ListStmnt[nCol].Stmnt;
            if (stmnt == "ЗНАЧЕНИЕобщНП"){
                if (enum_id == 0) {
                    TDB_CNP_value::DelOnlyValue(idItem, vd->comnp_id, vd->cell_period, vd->PlanId,
                                                vd->VidOtch, vd->god, vd->kvrtl, vd->mesjc, vd->den);
                    JSONresult["Values"].push_back("");
                } else {
                    TDB_CNP_value::addNumberItem(idItem, vd->comnp_id, vd->cell_period, vd->PlanId,
                                                 vd->VidOtch, vd->god, vd->kvrtl, vd->mesjc, vd->den, enum_id);
                    JSONresult["Values"].push_back(editValue);
                }
            } else {
                if (enum_id == 0) {
                    TDB_CellValue::DelOnlyValue(vd->ObjectId, vd->CellId, vd->PlanId, vd->VidOtch, vd->ValueDate);
                    JSONresult["Values"].push_back("");
                } else {
                    TDB_CellValue::SetValue(enum_id, vd->ObjectId,
                                           vd->CellId, vd->PlanId, vd->VidOtch, vd->ValueDate);
                    JSONresult["Values"].push_back(editValue);
                }
            }
            JSONresult["Row"].push_back(vd->Row);
            JSONresult["Col"].push_back(vd->Col);

            JSONresult["Styles"].push_back(utf16_to_utf8(FormLogicF1->f1Forms->StyleNameRC_Get(vd->Row, vd->Col)));
            FormLogicF1->f1Forms->TextRC_Set(nRow, nCol, utf8_to_utf16(TDB_Enum::GetEnumName(enum_id)).c_str());
            break;
        }
        case 5:{
            std::string ws;
            std::string stmnt = p->ListStmnt[nCol].Stmnt;
            if (editValue == "") {
                ws = "";
            } else {
                double FloatValue = StrToFloat(editValue);
                TDateTime SelectDate = UnixTimestampToDateTime(FloatValue);
                ws = SelectDate.ToFormattedDate();
            }
            if (stmnt == "ЗНАЧЕНИЕобщНП"){
                TDB_CNP_value::addStringItem(idItem, vd->comnp_id, vd->cell_period, vd->PlanId,
                                            vd->VidOtch, vd->god, vd->kvrtl, vd->mesjc, vd->den, ws);
            } else {
                TDB_CellValue::SetStrValue(vd, ws, vd->ObjectId,
                                          vd->CellId, vd->PlanId, vd->VidOtch, vd->ValueDate);
            }
            FormLogicF1->f1Forms->TextRC_Set(nRow, nCol, utf8_to_utf16(ws).c_str());
        }
    }
    FormLogicF1->f1Forms->Recalc();
    //Result = TFormDopUpdLogic::doForPrisvoitUpd(FormLogicF1, JSONresult);
    TFormDopUpdLogic::RecalcFormulas(FormLogicF1, JSONresult);
    return true;
}

void TFormDopUpdLogic::AddDefMrprBack(int nSheet, TFormLogicF1 *FormLogicF1, int plan_id, bool isItogo, int pokaz_is_razd,
                                 int pokaz_owner_id, int in_cast, const string &pokaz_name, const string &Primecanie,
                                 int is_list, int level_sign, const string &level_text, int pokaz_vert,
                                 const string &vert_str, int pokaz_type, const string &pokaz_code,
                                 const string &pokaz_bk_code, const TDateTime &pokaz_beg_date,
                                 const TDateTime &pokaz_end_date, int enum_id, int edizm_id, int pokaz_is_const,
                                 int pokaz_is_god, int pokaz_is_kv, int pokaz_is_mes, int pokaz_is_ist_fin,
                                 int pokaz_is_din_izm, int &pokaz_id) {

    pokaz_id = TDB_Pokaztl::AddParam(isItogo, plan_id, pokaz_name, pokaz_is_razd, pokaz_owner_id,
                                     pokaz_vert, pokaz_code, pokaz_bk_code, pokaz_type, enum_id, edizm_id,
                                     pokaz_beg_date, pokaz_end_date, level_sign, vert_str, is_list, in_cast);

    TDB_Pokaztl::UpdOrtIzm(pokaz_id, pokaz_is_mes, pokaz_is_kv, pokaz_is_god,
                           pokaz_is_ist_fin, pokaz_is_din_izm, pokaz_is_const);

    FillCell(pokaz_id, 0);

    // полагаем, что новый показатель последний под CurOwnerId
    TDB_Pokaztl::Set_level_text(plan_id, pokaz_owner_id, pokaz_id, level_sign);
}

int TFormDopUpdLogic::findNRowIns(TFormLogicF1 *FormLogicF1, int nRowSel, bool isSub) {
    PAtrRow AtrRow = TFormDopInp::GetListRowsItem(FormLogicF1, nRowSel);
    TwwStringList* p = TFormDopInp::GetTww(FormLogicF1);

    int OwnerId;
    if (isSub) {
        OwnerId = AtrRow->ObjectId;
    } else {
        OwnerId = AtrRow->OwnerId;
    }

    // Цикл до конца строк - ищем самую последнюю строку, владелец которой есть OwnerId
    // и встанем за ним



    int nRowIns = nRowSel + 1; // это вариант по умолчанию
    for (int wRow = nRowIns; wRow <= p->form_endBodyRow; ++wRow) {
        AtrRow = TFormDopInp::GetListRowsItem(FormLogicF1, wRow);
        if (AtrRow == nullptr) {
            continue;
        }
        if (OwnerId == AtrRow->OwnerId) {
            nRowIns = wRow + 1;
        }
    }

    return nRowIns;
}

void TFormDopUpdLogic::insertRowEndDataToForm(TFormLogicF1 *FormLogicF1, int nSheet, TwwStringList *p, int nRowIns,
                                              int aColSelect, int pokaz_id, int cast_id) {
    int maxRow = FormLogicF1->f1Forms->MaxRow_Get();
    TFormDopUpdLogic::insertEmptyNewRow(FormLogicF1, nSheet, nRowIns, p, 0, cast_id,
                                        pokaz_id, 0, 0, 2);

    // сгенерировать описания ячеек и ввести данные в ячейки
    //ВЕРНУТЬ!!! TFormDopUpdLogic::CellValueAtrForRow(FormLogicF1, nRowIns, pokaz_id);

    // это нужно
    FormLogicF1->f1Forms->MaxRow_Set(maxRow + 1);
    // End: это нужно
    /*afmForm.f1Forms.SetSelection(nRowIns, aColSelect, nRowIns, aColSelect);
    afmForm.f1Forms.SetActiveCell(nRowIns, aColSelect);
    afmForm.f1Forms.ShowActiveCell();*/

}

void TFormDopUpdLogic::insertEmptyNewRow(TFormLogicF1 *FormLogicF1, int nSheet, int insertRow, TwwStringList *p,
                                         int OwnerNomRow, int Ownerid, int pokaz_id, int level_sign, int L,
                                         int CurRowVid) {
    TFormDopUpdLogic::insertEmptyRow(FormLogicF1, insertRow, p, OwnerNomRow, Ownerid, pokaz_id,
                                     level_sign, L, CurRowVid);
}

void TFormDopUpdLogic::insertEmptyRow(TFormLogicF1 *FormLogicF1, int insertRow, TwwStringList *p, int OwnerNomRow,
                                      int OwnerId, int pokaz_id, int level_sign, int L, int CurRowVid) {
    int i, j, n;
    PCellValueAtr* vd;
    std::list<PCellValueAtr*> wList;
    TAtrRow* AtrRow = new TAtrRow();

    // вставляем строку в ListRows
    AtrRow->Row = insertRow;
    AtrRow->IsTerminal = true;
    AtrRow->OwnerNomRow = OwnerNomRow;
    AtrRow->CurRowVid = CurRowVid;
    AtrRow->level_sign = level_sign;
    AtrRow->L = L;
    AtrRow->OwnerId = OwnerId;
    AtrRow->objekt_class_id = 0;
    AtrRow->ObjectId = pokaz_id;
    AtrRow->inSort = false;
    AtrRow->IsUpd = false;
    AtrRow->is_filter2 = 0;
    AtrRow->ColOnFilter2 = 0;
    AtrRow->FormatStyle.setToNull();


    p->ListRows->Insert(insertRow, AtrRow);

    // меняем номера строк
    for (i = insertRow + 1; i < p->ListRows->Count(); i++) {
        AtrRow = ((*p->ListRows)[i]);
        if (AtrRow == nullptr) continue;
        AtrRow->Row += 1;
        if (AtrRow->OwnerNomRow > insertRow) {
            AtrRow->OwnerNomRow += 1;
        }
    }

//    for (i = insertRow; i <= FormLogicF1->f1Forms->LastRow_Get(); i++) {
//        for (j = 1; j <= FormLogicF1->f1Forms->LastCol_Get(); j++) {
//            p->Find("R" + std::to_string(i) + "C" + std::to_string(j), n);
//            if (n < 0) continue;
//            vd = std::get_if<PCellValueAtr*>(p->Objects[n]);
//            p->getObjectByKey<PCellValueAtr>(CellName);
//            p->Delete(n);
//            wList.push_back(vd);
//        }
//    }
//
//    for (auto& item : wList) {
//        vd = item;
//        vd->Row += 1;
//        p->AddObject("R" + std::to_string(vd->Row) + "C" + std::to_string(vd->Col), reinterpret_cast<TObject*>(vd));
//    }
//
//    // вставляем пустую строку в F1
//    TF1toDE::InsertRowsCells(p, FormLogicF1, insertRow, insertRow);
//    FormLogicF1->f1Forms->RowHeight(insertRow) = 0xFFFFFFFF; // F1RowHeightAuto = $FFFFFFFF
//
//    // FormLogicF1->f1Forms.MaxRow := FormLogicF1->f1Forms.MaxRow + 1; // Это не нужно, поскольку в TF1toDE::InsertRowsCells уже увеличивается
//    p->form_endBodyRow += 1;
}

bool TFormDopUpdLogic::ClickButton(TFormLogicF1 *FormLogicF1, int nSheet, int nRow, json &JSONresult) {
    int j, toCol;
    PAtrRow AtrRow, AtrRow1;
    double d0, d0edIzm, d1;
    PCellValueAtr vd1;
    bool Result = false;
    TwwStringList* p = FormLogicF1->FormsValuesArray->GetItem(nSheet - 1);
    int LastCol = FormLogicF1->f1Forms->LastCol_Get();
    for (j = p->form_begBodyRow; j <= p->form_endBodyRow; j++) { // начинаем с начала формы

        PAtrRow AtrRow = ((*p->ListRows)[j]);

        if (AtrRow == nullptr) continue;
        if ((AtrRow->CurRowVid == 14 /* ПРИСВОИТЬ */) && !(AtrRow->IsUpd)) {
            for (toCol = 1; toCol <= LastCol; toCol++) {
                switch (FormLogicF1->f1Forms->TypeRC_Get(AtrRow->Row, toCol)) {
                    case -1:
                    case -2:
                    case -3: { // в ячейке есть формула
                        d0 = FormLogicF1->f1Forms->NumberRC_Get(AtrRow->Row, toCol);

                        PAtrRow AtrRow1 = ((*p->ListRows)[j + 1]);

                        if (AtrRow1 == nullptr) continue;
                        vd1 = TFormDopInp::GetCellValueAtrByRC(FormLogicF1, AtrRow1->Row, toCol);
                        if (vd1 == nullptr) continue;
                        if (vd1->pokaz_type > 1) continue;
                        d0edIzm = PreobrValue(d0, vd1->pokaz_type, vd1->coef_Forma, vd1->coef_Pokaz);
                        d1 = FormLogicF1->f1Forms->NumberRC_Get(vd1->Row, vd1->Col);
                        if (!SameValue(d0, d1, 0.000001)) {
                            Result = true;
                            JSONresult["Row"].push_back(AtrRow->Row);
                            JSONresult["Col"].push_back(toCol);
                            JSONresult["Values"].push_back(d0);
                            JSONresult["Styles"].push_back(utf16_to_utf8(FormLogicF1->f1Forms->StyleNameRC_Get(AtrRow->Row, toCol)));

                            JSONresult["Row"].push_back(vd1->Row);
                            JSONresult["Col"].push_back(vd1->Col);
                            JSONresult["Values"].push_back(d1);
                            JSONresult["Styles"].push_back(utf16_to_utf8(FormLogicF1->f1Forms->StyleNameRC_Get(vd1->Row, vd1->Col)));

                            FormLogicF1->f1Forms->NumberRC_Set(vd1->Row, vd1->Col, d0);
                            bool msg = false;
                            if (vd1->CellId > 0)
                                TUpdDBbyValue::UpdDBbyValue1(vd1->CellId, vd1->PlanId, vd1->VidOtch, vd1->ValueDate, d0edIzm, vd1->ObjectId, msg);
                            else
                                TDB_CNP_value::addNumberItem(vd1->PokazId, vd1->comnp_id, vd1->cell_period, vd1->PlanId,
                                                             vd1->VidOtch, vd1->god, vd1->kvrtl, vd1->mesjc, vd1->den, d0edIzm);

                            // !!!!!!!!!!!!!!!!!  27.02.2021
                            if (p->ListStmnt[vd1->Col].stmntPrisvoitLink > 0)
                                TFormDopUpdLogic::doStmntAssignByVvodPokaztl(FormLogicF1, p, vd1->Row, p->ListStmnt[vd1->Col].stmntPrisvoitLink, vd1->Col);

                            // ПРОШЕРСТИТЬ ВО ВСЕХ ОТКРЫТЫХ ФОРМАХ (НЕ ВСПОМОГАТЕЛЬНЫХ И НЕ ТЕКУЩИЕ)ЯЧЕЙКИ И НАЙТИ ТАКУЮ У КОТОРОЙ VD ИДЕНТИЧЕН VD1
                            // И В ��АКУЮ ЯЧЕЙКУ ПОЛОЖ��ТЬ ЗНАЧЕНИЕ(НЕ В БАЗУ ДАННЫХ) С учетом еденици измерения формы
                            TFormDopUpdLogic::doRefreshComputedCells((vd1->CellId == 0), FormLogicF1, p, vd1, d0edIzm);
                        }
                    }
                }
            }
        }
    }
    if (Result){
        JSONresult["Message"] = "Операция завершена";
    } else {
        PAtrRow AtrRow;
        AtrRow = ((*p->ListRows)[nRow]);
        JSONresult["Message"] = AtrRow->msg;
    }
    return Result;

}

void TFormDopUpdLogic::RecalcFormulas(TFormLogicF1 *FormLogicF1, json &JSONresult) {
    TracerManager::Instance().StartFunctionTrace("TFormDopUpdLogic::RecalcFormulas");
    int typeRC;
    int r, c;
    std::string s;
    std::string JSONString = to_string(JSONresult);
    int LastCol = FormLogicF1->f1Forms->LastCol_Get();
    int LastRow = FormLogicF1->f1Forms->LastRow_Get();
    for (r = 1; r <= LastRow; ++r) {
        for (c = 1; c <= LastCol; ++c) {
            typeRC = FormLogicF1->f1Forms->TypeRC(r, c);

            if (typeRC < 0) { // IsFormula
                if (typeRC == -3) { // FormulaResult.IsError
//                    s = "Строка " + std::to_string(fv->row) + ", столбец " + std::to_string(fv->col) +
//                        ". Некорректный результат формулы: " + _xlsF1.FormulaRC(fv->row, fv->col);
//                    throw std::runtime_error(s);
                }

                if (typeRC == -2) { // FormulaResult.IsString
                    JSONresult["Row"].push_back(r);
                    JSONresult["Col"].push_back(c);
                    JSONresult["Styles"].push_back(utf16_to_utf8(FormLogicF1->f1Forms->StyleNameRC_Get(r, c)));
                    JSONresult["Values"].push_back(utf16_to_utf8(FormLogicF1->f1Forms->TextRC_Get(r, c)));
                } else {
                    FormLogicF1->f1Forms->FormulaResultNumberRC(r, c);
                    double valueWI = FormLogicF1->f1Forms->NumberRC_Get(r, c);
                    JSONresult["Row"].push_back(r);
                    JSONresult["Col"].push_back(c);
                    JSONresult["Styles"].push_back(utf16_to_utf8(FormLogicF1->f1Forms->StyleNameRC_Get(r, c)));
                    JSONresult["Values"].push_back(FormLogicF1->f1Forms->FormulaResultNumberRC(r, c));
//                    if (valueWI != 0){
//                        JSONresult["Row"].push_back(r);
//                        JSONresult["Col"].push_back(c);
//                        JSONresult["Styles"].push_back(utf16_to_utf8(FormLogicF1->f1Forms->StyleNameRC_Get(r, c)));
//                        JSONresult["Values"].push_back(FormLogicF1->f1Forms->FormulaResultNumberRC(r, c));
////                        JSONresult["Values"].push_back(FormLogicF1->f1Forms->NumberRC_Get(r, c));
//                    }

                }

//            } else if (typeRC > 0) {
//                typeRC = FormLogicF1->f1Forms->TypeRC(r, c);
//                if (typeRC == 1) {
//                    double wi = FormLogicF1->f1Forms->NumberRC_Get(r, c);
//                    JSONresult["Row"].push_back(r);
//                    JSONresult["Col"].push_back(c);
//                    JSONresult["Styles"].push_back(utf16_to_utf8(FormLogicF1->f1Forms->StyleNameRC_Get(r, c)));
//                    JSONresult["Values"].push_back(FormLogicF1->f1Forms->FormulaResultNumberRC(r, c));
//                }
            }
        }
    }
    TracerManager::Instance().EndFunctionTrace();
}

bool TFormDopUpdLogic::CellsImport(TFormLogicF1* formLogic, const nlohmann::json& jsonObj, nlohmann::json& resultJson) {
    int nSheet = jsonObj["nSheet"];

    // Создаем структуру ответа
    resultJson = {
            {"nSheet", nSheet},
            {"Row", nlohmann::json::array()},
            {"Col", nlohmann::json::array()},
            {"Values", nlohmann::json::array()},
            {"Styles", nlohmann::json::array()}
    };

    // Проходимся по всем изменениям
    for (const auto& cellEdit : jsonObj["editValues"]) {
        int nRow = cellEdit["nRow"];
        int nCol = cellEdit["nCol"];
        std::string editValue = cellEdit["editValue"];
        int valueID = 0;

        // Определение типа ячейки
        PCellValueAtr vd = TFormDopInp::GetCellValueAtrByRC(formLogic, nRow, nCol);
        if (vd->pokaz_type == 3) { // Enum
            valueID = cellEdit["editValue"]["id"];
            editValue = cellEdit["editValue"]["name"];
        } else {
            if (editValue.empty()) {
                editValue = "0";
            }
        }

        // Начало редактирования ячейки
        formLogic->ActionView->doBeginSostSaveAllAction();
        TwwStringList *p = TFormDopInp::GetTww(formLogic);
        std::string oldvalue = char16ToUTF8(formLogic->f1Forms->TextRC_Get(nRow, nCol));
        int idItem = (vd->ObjectId == 0) ? vd->PokazId : vd->ObjectId;

        switch (vd->pokaz_type) {
            case 0: // Примечание, Ведомство
                break;

            case 1: { // число
                if (vd->cell_period == 0) {
                    TDB_CellValue::SetValueUslPost(editValue, vd);
                } else {
                    double FloatValue = StrToFloat(editValue);
                    FloatValue = PreobrValue(FloatValue, vd->DataType, vd->coef_Forma, vd->coef_Pokaz);

                    if (vd->DataType == 1) {
                        if (p->ListStmnt[nCol].vid_uslovij == 8) {
                            oldvalue = std::to_string(TDB_CellValue::GetValue(vd->ObjectId, vd->CellId, vd->PlanId, vd->VidOtch, vd->ValueDate));
                        }

                        if (TUpdDBbyValue::UpdDBbyValue1(vd->CellId, vd->PlanId, vd->VidOtch, vd->ValueDate, FloatValue, vd->ObjectId, false, resultJson)) {
                            TDB_GurnProtokol::execProtokolAddItem(vd->CellId, vd->PlanId, vd->VidOtch, vd->ValueDate, FloatValue, vd->ObjectId, PreobrValue(formLogic->f1Forms->NumberRC(vd->Row, vd->Col), vd->DataType, vd->coef_Forma, vd->coef_Pokaz));

                            if (editValue.empty())
                                TDB_CellValue::DelOnlyValue(vd->ObjectId, vd->CellId, vd->PlanId, vd->VidOtch, vd->ValueDate);

                            if (vd->kakAgregirUpByField == 8)
                                doAgregirUpByField(p, vd);

                            formLogic->f1Forms->NumberRC_Set(nRow, nCol, StrToFloat(editValue));
                        } else if (p->ListStmnt[nCol].vid_uslovij == 8) {
                            TUpdDBbyValue::UpdDBbyValue1(vd->CellId, vd->PlanId, vd->VidOtch, vd->ValueDate, std::stod(oldvalue), vd->ObjectId, false);
                        }
                    }
                    resultJson["Row"].push_back(nRow);
                    resultJson["Col"].push_back(nCol);
                    resultJson["Values"].push_back(utf16_to_utf8(formLogic->f1Forms->TextRC_Get(nRow, nCol)));
                    resultJson["Styles"].push_back(utf16_to_utf8(formLogic->f1Forms->StyleNameRC_Get(nRow, nCol)));
                    formLogic->f1Forms->Recalc();
                    formLogic->f1Forms->NumberRC_Set(nRow, nCol, StrToFloat(editValue));
                }
                break;
            }
            case 2:
            case 6: { // текст
                std::string stmnt = p->ListStmnt[nCol].Stmnt;
                formLogic->f1Forms->TextRC_Set(nRow, nCol, utf8_to_utf16(editValue).c_str());
                if (stmnt == "ЗНАЧЕНИЕобщНП") {
                    TDB_CNP_value::addStringItem(idItem, vd->comnp_id, vd->cell_period, vd->PlanId, vd->VidOtch, vd->god, vd->kvrtl, vd->mesjc, vd->den, editValue);
                } else {
                    TDB_CellValue::SetStrValue(vd, editValue, vd->ObjectId, vd->CellId, vd->PlanId, vd->VidOtch, vd->ValueDate);
                }
                break;
            }
            case 3: { // перечисление
                int enum_id = valueID;
                std::string stmnt = p->ListStmnt[nCol].Stmnt;
                if (stmnt == "ЗНАЧЕНИЕобщНП") {
                    if (enum_id == 0) {
                        TDB_CNP_value::DelOnlyValue(idItem, vd->comnp_id, vd->cell_period, vd->PlanId, vd->VidOtch, vd->god, vd->kvrtl, vd->mesjc, vd->den);
                        resultJson["Values"].push_back("");
                    } else {
                        TDB_CNP_value::addNumberItem(idItem, vd->comnp_id, vd->cell_period, vd->PlanId, vd->VidOtch, vd->god, vd->kvrtl, vd->mesjc, vd->den, enum_id);
                        resultJson["Values"].push_back(editValue);
                    }
                } else {
                    if (enum_id == 0) {
                        TDB_CellValue::DelOnlyValue(vd->ObjectId, vd->CellId, vd->PlanId, vd->VidOtch, vd->ValueDate);
                        resultJson["Values"].push_back("");
                    } else {
                        TDB_CellValue::SetValue(enum_id, vd->ObjectId, vd->CellId, vd->PlanId, vd->VidOtch, vd->ValueDate);
                        resultJson["Values"].push_back(editValue);
                    }
                }
                resultJson["Row"].push_back(vd->Row);
                resultJson["Col"].push_back(vd->Col);
                resultJson["Styles"].push_back(utf16_to_utf8(formLogic->f1Forms->StyleNameRC_Get(vd->Row, vd->Col)));
                formLogic->f1Forms->TextRC_Set(nRow, nCol, utf8_to_utf16(TDB_Enum::GetEnumName(enum_id)).c_str());
                break;
            }
            case 5: { // дата
                std::string stmnt = p->ListStmnt[nCol].Stmnt;
                double FloatValue = StrToFloat(editValue);
                TDateTime SelectDate = UnixTimestampToDateTime(FloatValue);
                std::string ws = SelectDate.ToFormattedDate();

                if (stmnt == "ЗНАЧЕНИЕобщНП") {
                    TDB_CNP_value::addStringItem(idItem, vd->comnp_id, vd->cell_period, vd->PlanId, vd->VidOtch, vd->god, vd->kvrtl, vd->mesjc, vd->den, ws);
                } else {
                    TDB_CellValue::SetStrValue(vd, ws, vd->ObjectId, vd->CellId, vd->PlanId, vd->VidOtch, vd->ValueDate);
                }
                formLogic->f1Forms->TextRC_Set(nRow, nCol, utf8_to_utf16(ws).c_str());
                break;
            }
        }
        std::string ws2 = to_string(resultJson);
        TFormDopUpdLogic::doForPrisvoitUpd(formLogic, resultJson);
        formLogic->f1Forms->Recalc();
        TFormDopUpdLogic::RecalcFormulas(formLogic, resultJson);
    }

    return true;
}





