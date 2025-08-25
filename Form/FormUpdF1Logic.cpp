#include "SystFuncsLogic.h"
#include "DB_Common.h"
#include "Form1.h"
#include "FormDopInp.h"
#include "Form2Plus.h"
#include "Form2loadValue.h"
#include "DB_Pokaztl.h"
#include "DB_CellValue.h"
#include "DB_PlanStr.h"
#include "DB_UserPlan.h"
#include "DB_FormStr.h"
#include "DB_CNP_value.h"
#include "DB_EdIzm.h"
#include "DB_Cell.h"
#include "DB_NMrpr.h"
#include "DB_bkMetaDef.h"
#include "FormUpdF1Logic.h"

// изменить наименование в F1 с учетом префикса
void TFormUpdF1Logic::updNameinF1(TFormLogicF1* FormLogicF1, int nRow, std::string newName) {
        PAtrRow AtrRow;
        TwwStringList* p = TFormDopInp::GetTww(FormLogicF1);
        int colName = TFormDopInp::GetNameCol(FormLogicF1);

        AtrRow = TFormDopInp::GetListRowsItem(FormLogicF1, nRow);
        if (AtrRow == nullptr) {
            FormLogicF1->f1Forms->TextRC_Set(nRow, colName, utf8_to_utf16(newName).c_str());
            return;
        }
        if (AtrRow->L > 0) {
            ;
            FormLogicF1->f1Forms->TextRC_Set(nRow, colName, utf8_to_utf16(Copy(char16ToUTF8(FormLogicF1->f1Forms->TextRC(nRow, colName)), 1, AtrRow->L)+newName).c_str());
        }
    }

// найти нужную ячейку и положить строку
// если field_id = 0, значит меняется ячейка с наименованием
void TFormUpdF1Logic::setInCellStr(TFormLogicF1* FormLogicF1, int nRow, int objectid, int field_id, std::string str_value) {
    int R1, C1, R2, C2;
    FormLogicF1->f1Forms->Selection_Get(0,R1,C1,R2,C2);

    if (field_id == 0) {
        TwwStringList* p = TFormDopInp::GetTww(FormLogicF1);

        ;
        FormLogicF1->f1Forms->TextRC_Set(nRow, p->PokazCol, utf8_to_utf16(str_value).c_str());
    } else {
        int LastCol = FormLogicF1->f1Forms->LastCol_Get();
        for (int i = 1; i <= LastCol; i++) {
            FormLogicF1->f1Forms->Selection_Set(nRow, i, nRow, i);
            PCellValueAtr vd = TFormDopInp::GetCellValueAtrByRC(FormLogicF1, nRow, i);
            if (vd == nullptr) continue;
            if (!(vd->ObjectId == objectid && vd->PokazId == field_id)) continue;

            ;
            FormLogicF1->f1Forms->Text_Set(utf8_to_utf16(str_value).c_str());
        }
    }
    FormLogicF1->f1Forms->Selection_Set(R1, C1, R2, C2);
}

// найти нужную ячейку с источником финансирования и положить число
void TFormUpdF1Logic::setInCellNmbr(TFormLogicF1* FormLogicF1, int nRow, int objectid, int field_id, int ist_fin_id, int VidOtch, double value) {
    int R1, C1, R2, C2;
    FormLogicF1->f1Forms->Selection_Get(0, R1, C1, R2, C2);
    int LastCol = FormLogicF1->f1Forms->LastCol_Get();
    for (int i = 1; i <= LastCol; i++) {
        FormLogicF1->f1Forms->Selection_Set(nRow, i, nRow, i);
        PCellValueAtr vd = TFormDopInp::GetCellValueAtrByRC(FormLogicF1, nRow, i);
        if (vd == nullptr) continue;
        if (!((vd->ObjectId == objectid) && (vd->PokazId == field_id) && (vd->ist_fin_id == ist_fin_id) && (vd->VidOtch == VidOtch))) continue;
        FormLogicF1->f1Forms->Number_Set(value);
        break;
    }
    FormLogicF1->f1Forms->Selection_Set(R1, C1, R2, C2);
}

// вывод строки в ячейку с форматированием
void TFormUpdF1Logic::SetCellValueStr(TFormLogicF1* FormLogicF1, int nRow, int nCol, std::string str_value) {
    short DataType = 2;
    TFormatStyle* FormatStyle = new TFormatStyle();
    TwwStringList* p = TFormDopInp::GetTww(FormLogicF1);

    TFormDopInp::getColFrmtrAtr(FormLogicF1, DataType, nCol, *FormatStyle);
    TForm1::SetCellValue(FormLogicF1, FormLogicF1->f1Forms, p->MaxHrCol, nRow, nCol, nRow, nCol,
                 str_value, DataType, FormatStyle);
}

// изменить ячейки с числами из БД в строке nRow для объекта objectid
void TFormUpdF1Logic::setInRowNmbr(TFormLogicF1* FormLogicF1, int nRow, int objectid) {
    int R1, C1, R2, C2;
    double value;

    FormLogicF1->f1Forms->Selection_Get(0, R1, C1, R2, C2);
    int LastCol = FormLogicF1->f1Forms->LastCol_Get();
    for (int i = 1; i <= LastCol; i++) {
        FormLogicF1->f1Forms->Selection_Set(nRow, i, nRow, i);
        PCellValueAtr vd = TFormDopInp::GetCellValueAtrByRC(FormLogicF1, nRow, i);
        if (vd == nullptr || vd->DataType != 1 || vd->ObjectId != objectid) {
            continue;
        }
        value = TDB_CellValue::GetValue(objectid, vd->CellId, vd->PlanId, vd->VidOtch, vd->ValueDate);
        FormLogicF1->f1Forms->Number_Set(value);
    }
    FormLogicF1->f1Forms->Selection_Set(R1, C1, R2, C2);
}

// обновить строку
void TFormUpdF1Logic::doReDraw(TFormLogicF1* FormLogicF1, TwwStringList* p, int nRow) {
        TForm2loadValue::SetInputValue(FormLogicF1, p, true, nRow);
        
        PAtrRow AtrRow = TFormDopInp::GetListRowsItem(FormLogicF1, nRow);

    ;
    FormLogicF1->f1Forms->TextRC_Set(nRow, p->PokazCol, utf8_to_utf16(TDB_Pokaztl::GetPokazName(AtrRow->ObjectId)).c_str());
        
        for (int n = 1; n <= FormLogicF1->f1Forms->MaxCol_Get(); n++) {
            FormLogicF1->f1Forms->DrawBorders(nRow, n, nRow, n, 1, 0, true);
        }
    }


// вставить в таблицу F1 строку - Объект
// результат = false при вставке первого подобъекта в раздел
//             в этом случае F1 следует перерисовать
bool TFormUpdF1Logic::insertObjectToF1(TFormLogicF1* FormLogicF1, int OwnerId, int object_id, const std::string object_name,
                                      int objekt_class_id, std::string KBK, int level_sign, int& aInsertRow) {
    bool Result = false;
    TwwStringList* p = TFormDopInp::GetTww(FormLogicF1);
    int colName = TFormDopInp::GetNameCol(FormLogicF1);
    int i = FormLogicF1->f1Forms->Row_Get(); // выбранная пользователем строка
    PAtrRow AtrRow = TFormDopInp::GetListRowsItem(FormLogicF1, i);
    if (AtrRow == nullptr) return false;

    // 19.11.2019
    if ((AtrRow->CurRowVid == 9 || AtrRow->CurRowVid == 14) && 
        i == FormLogicF1->f1Forms->LastRow_Get() && i > p->form_begBodyRow) {
        i = FormLogicF1->f1Forms->Row_Get() - 1;
        AtrRow = TFormDopInp::GetListRowsItem(FormLogicF1, i);
    }
    int OwnerNomRow = i;
    int LastRow = FormLogicF1->f1Forms->LastRow_Get();
    if (AtrRow->ObjectId == OwnerId) { // значит была операция добавление ПОДобъекта
        // ищем в F1 последнюю строку - объект класса objekt_class_id, подчиненную OwnerId
        bool isFind = false;

        for (int i1 = i + 1; i1 <= LastRow; i1++) {
            PAtrRow AtrRow1 = TFormDopInp::GetListRowsItem(FormLogicF1, i1);
            if (AtrRow1 == nullptr) continue;
            if (AtrRow1->OwnerNomRow < i) break; // не нашли
            if (AtrRow1->OwnerId != OwnerId) continue;
            if (AtrRow1->CurRowVid == 4) {
                if (AtrRow1->objekt_class_id != objekt_class_id) continue;
            } else if (AtrRow1->CurRowVid == 3 || AtrRow1->CurRowVid == 5) {
                break;
            } else {
                continue;
            }
            isFind = true;
            aInsertRow = i1 + 1;
            OwnerNomRow = AtrRow1->OwnerNomRow;
        }
        if (!isFind) aInsertRow = i + 1; // это случай, когда вставляем первый в раздел
    } else { // значит была операция добавление объекта
        // ищем в F1 последнюю строку - объект класса objekt_class_id, подчиненную OwnerId
        bool isFind = false;
        int OwnerNomRow = AtrRow->OwnerNomRow;
        for (int i1 = i; i1 <= LastRow; i1++) {
            PAtrRow AtrRow1 = TFormDopInp::GetListRowsItem(FormLogicF1, i1);
            if (AtrRow1 == nullptr) continue;
            if (AtrRow1->OwnerNomRow < AtrRow->OwnerNomRow) break; // не нашли
            if (AtrRow1->OwnerId != OwnerId) continue;
            if (AtrRow1->CurRowVid == 4) {
                if (AtrRow1->objekt_class_id != objekt_class_id) continue;
            } else if (AtrRow1->CurRowVid == 3 || AtrRow1->CurRowVid == 5) {
                break;
            } else {
                continue;
            }
            isFind = true;
            aInsertRow = i1 + 1;
            OwnerNomRow = AtrRow1->OwnerNomRow;
        }
        if (!isFind) aInsertRow = i + 1; // это случай, когда вставляем сразу за выбранным
    }

    // в результате нашли aInsertRow - номер вставляемой строки

    // теперь в etlnRow надо найти номер строки с эталон��ым объектом
    bool isFind = false;
    int etlnRow;
    for (int i = aInsertRow - 1; i >= p->form_begBodyRow; i--) {
        PAtrRow AtrRow1 = TFormDopInp::GetListRowsItem(FormLogicF1, i);
        if (AtrRow1->CurRowVid != 4) continue;
        if (AtrRow1->objekt_class_id != objekt_class_id) continue;
        isFind = true;
        etlnRow = i;
        break;
    }
    if (!isFind) return false; // нет эталлоного объекта - неукого скопировать нужные данные

    // вставить пустую строку с номером  aInsertRow
    TFormUpdF1Logic::insertEmptyRow(FormLogicF1, aInsertRow, p , OwnerNomRow, OwnerId, objekt_class_id, object_id,
                                   level_sign, 0, 4);
    TFormUpdF1Logic::DostupnCells(FormLogicF1, aInsertRow, etlnRow);

    // для вствленной строки отрабатываем ячейки
    int LastCol = FormLogicF1->f1Forms->LastCol_Get();
    for (int i = 1; i <= LastCol; i++) {
        FormLogicF1->f1Forms->Selection_Set(etlnRow, i, etlnRow, i);
        int cf = FormLogicF1->f1Forms->CellFormat_Get();
        FormLogicF1->f1Forms->Selection_Set(aInsertRow, i, aInsertRow, i);
        FormLogicF1->f1Forms->CellFormat_Set(cf);
        if (i == colName) {
            std::string ws = TFormUpdF1Logic::prefixObjectName(FormLogicF1, aInsertRow, p, colName);

            ;
            FormLogicF1->f1Forms->Text_Set(utf8_to_utf16(ws + object_name).c_str());
        }
        PCellValueAtr vd = TFormDopInp::GetCellValueAtrByRC(FormLogicF1, etlnRow, i);
        PCellValueAtr vd1 = nullptr;
        if (vd != nullptr) {
            vd1 = new TCellValueAtr();//MAK ДЛЯ ПРОВЕРКИ ГОСТЕХ
            TFormDopInp::CopyCellValueAtr(*vd1, vd);
            vd1->Row = aInsertRow;
            vd1->ObjectId = object_id;
            p->AddObject("R" + std::to_string(vd1->Row) + "C" + std::to_string(vd1->Col), vd1);
        }
        std::string Value;
        std::string stmnt = p->ListStmnt[i].Stmnt;
        if (stmnt == "КОДБК") {
            Value = KBK;
        } else if (stmnt == "Примечание") {
            Value = TDB_Pokaztl::GetPrimecanie(object_id);
        } else if (stmnt == "Ведомство") {
            if (vd1 != nullptr) {
                Value = TDB_PlanStr::GetVedomstvo(vd1->PlanId, vd1->ObjectId);
            }
        } else if (stmnt == "КОДБК1") {
            Value = SetCodeBK_rzdltl(KBK, FormLogicF1->CurrYear);
        } else if (stmnt == "КОДБКРздл" || stmnt == "КОДБКРздл1") {
            Value = getKBK_RZD(KBK, FormLogicF1->CurrYear);
            if (stmnt == "КОДБКРздл1") Value = SetCodeBK_RZD_rzdltl(Value, 0);
        } else if (stmnt == "КОДБКЦелСт" || stmnt == "КОДБКЦелСт1") {
            Value = getKBK_CS(KBK, FormLogicF1->CurrYear);
            if (stmnt == "КОДБКЦелСт1") Value = SetCodeBK_CST_rzdltl(Value, FormLogicF1->CurrYear, 0);
        } else if (stmnt == "КОДБКВидРасх") {
            Value = getKBK_VR(KBK, FormLogicF1->CurrYear);
        } else if (stmnt == "КОДБКЭкКл") {
            Value = getKBK_KOSGU(KBK, FormLogicF1->CurrYear);
        } else if (stmnt == "ФОРМУЛАF1") {
            Value = TFormDopInp::ReplaceNRowNCol(aInsertRow, i, TDB_FormStr::GetNote1(p->FormId, 1000 + i));
        } else if (stmnt == "НАИМПОКАЗ") {
            Value = TDB_Pokaztl::GetPokazName(object_id);
        }

        if (!Value.empty()) {
            if (Value[0] == '^') {
                FormLogicF1->f1Forms->Formula_Set(utf8_to_utf16(Value.substr(1, 500)).c_str());
            } else {
                FormLogicF1->f1Forms->Text_Set(utf8_to_utf16(Value).c_str());
            }
        }

        if (vd1 != nullptr) { // введем значения характристик
            if (stmnt == "ЗНАЧЕНИЕСВ") {
                if (vd1->DataType == 1) {
                    double dValue;
                    if (vd1->cell_period == 0) {
                        dValue = TDB_CellValue::GetNumValueUslPost(vd1->ObjectId, vd1->CellId, vd1->PlanId, vd1->VidOtch, vd1->ValueDate);
                    } else {
                        dValue = TDB_CellValue::GetValue(vd1->ObjectId, vd1->CellId, vd1->PlanId, vd1->VidOtch, vd1->ValueDate);
                    }
                    dValue = PreobrValue(dValue, 1, vd1->coef_Pokaz, vd1->coef_Forma);
                    FormLogicF1->f1Forms->Number_Set(dValue);
                } else if (vd1->DataType == 2) {
                    std::string Value;
                    if (vd1->cell_period == 0) {
                        Value = TDB_CellValue::GetStrValueUslPost(vd1->ObjectId, vd1->CellId, vd1->PlanId, vd1->VidOtch, vd1->ValueDate);
                    } else {
                        Value = TDB_CellValue::GetStrValue(vd1->ObjectId, vd1->CellId, vd1->PlanId, vd1->VidOtch, vd1->ValueDate);
                    }
                    FormLogicF1->f1Forms->Text_Set(utf8_to_utf16(Value).c_str());
                }
            } else if (stmnt == "ЗНАЧЕНИЕобщНП") {
                if (vd1->DataType == 1) {
                    double dValue;
                    if (TDB_CNP_value::getNumberItem(vd1->ObjectId, vd1->comnp_id, vd1->cell_period, vd1->PlanId,
                                                    vd1->VidOtch, vd1->god, vd1->kvrtl, vd1->mesjc, vd1->den, dValue)) {
                        dValue = PreobrValue(dValue, 1, vd1->coef_Pokaz, vd1->coef_Forma);
                        FormLogicF1->f1Forms->Number_Set(dValue);
                    }
                } else if (vd1->DataType == 2) {
                    std::string Value;
                    if (TDB_CNP_value::getStringItem(vd1->ObjectId, vd1->comnp_id, vd1->cell_period, vd1->PlanId,
                                                    vd1->VidOtch, vd1->god, vd1->kvrtl, vd1->mesjc, vd1->den, Value)) {
                        FormLogicF1->f1Forms->Text_Set(utf8_to_utf16(Value).c_str());
                    }
                }
            }
        }

        if (stmnt == "№п/п") {
            TForm2Plus::SetNumberPP(FormLogicF1, p->form_begBodyRow, FormLogicF1->f1Forms->LastRow); // Ставим № п/п
        }
    }
    TFormUpdF1Logic::toVisible(FormLogicF1, aInsertRow, colName);
    Result = true;
    return Result;
}


// вставить пустую строку с номером insertRow
void TFormUpdF1Logic::insertEmptyRow(TFormLogicF1* FormLogicF1, int insertRow, TwwStringList * p,
                                    int OwnerNomRow, int Ownerid, int objekt_class_id, int ObjectId,
                                    int level_sign, int L, int CurRowVid) {
    // вставляем строку в ListRows
//    PAtrRow AtrRow;
    PAtrRow AtrRow = new TAtrRow();
    AtrRow->Row = insertRow;
    AtrRow->IsTerminal = true;
    AtrRow->OwnerNomRow = OwnerNomRow;
    AtrRow->CurRowVid = CurRowVid;
    AtrRow->level_sign = level_sign;
    AtrRow->L = L;
    AtrRow->OwnerId = Ownerid;
    AtrRow->objekt_class_id = objekt_class_id;
    AtrRow->ObjectId = ObjectId;
    AtrRow->inSort = false;
    AtrRow->IsUpd = false;
    AtrRow->is_filter2 = 0;
    AtrRow->ColOnFilter2 = 0;
    AtrRow->FormatStyle.setToNull();

    p->ListRows->Insert(insertRow, AtrRow);

    // меняем номера строк
    for (int i = insertRow + 1; i < p->ListRows->Count(); ++i) {
//        AtrRow* atrRow = p->ListRows->Items[i];
        PAtrRow AtrRow = ((*p->ListRows)[i]);

        if (AtrRow == nullptr) {
            continue;
        }
        AtrRow->Row = AtrRow->Row + 1;
        if (AtrRow->OwnerNomRow > insertRow) {
            AtrRow->OwnerNomRow = AtrRow->OwnerNomRow + 1;
        }
    }


// Создаем ваш список для хранения объектов
    TList<PCellValueAtr> wList;

// Перебираем ячейки от insertRow до последней строки и колонки
    for (int i = insertRow; i <= FormLogicF1->f1Forms->LastRow; ++i) {
        for (int j = 1; j <= FormLogicF1->f1Forms->LastCol; ++j) {
            std::string key = "R" + std::to_string(i) + "C" + std::to_string(j);
            int n = p->IndexOf(key.c_str());
            if (n < 0) {
                continue;
            }
            PCellValueAtr vd = std::get<PCellValueAtr>(p->Objects[n]);
            p->Delete(n);
            wList.Add(vd);
        }
    }

// Увеличиваем значение Row каждого объекта в списке
    for (int i = 0; i < wList.Count(); ++i) {
        PCellValueAtr vd = reinterpret_cast<PCellValueAtr>(wList[i]);
        vd->Row = vd->Row + 1;
        p->AddObject(("R" + std::to_string(vd->Row) + "C" + std::to_string(vd->Col)).c_str(), reinterpret_cast<TObject*>(vd));
    }

// Освобождаем память, выделенную для объектов в списке
    for (int i = 0; i < wList.Count(); ++i) {
        delete reinterpret_cast<PCellValueAtr>(wList[i]);
    }
    wList.Clear(); // Очищаем список


    // вставляем пустую строку в F1    
    //mak??? FormLogicF1->f1Forms->InsertRowsCells(p, insertRow, insertRow);
    FormLogicF1->f1Forms->RowHeight_Set(insertRow, 0xFFFFFFFF); // F1RowHeightAuto = 0xFFFFFFFF

    p->form_endBodyRow++;
}

// делаем доступность ячеек вставленной строки для ввода такой же, что и у эталонной
void TFormUpdF1Logic::DostupnCells(TFormLogicF1 * FormLogicF1, int insertRow, int etlnRow) {
        int R1, C1, R2, C2;
        int nCol;
        bool etlnLocked, etlnHidden;
        bool inrtLocked, inrtHidden;

        FormLogicF1->f1Forms->Selection_Get(0, R1, C1, R2, C2);

        try {
            int LastCol = FormLogicF1->f1Forms->LastCol_Get();
            for (nCol = 1; nCol <= LastCol; nCol++) {
                FormLogicF1->f1Forms->Selection_Set(etlnRow, nCol, etlnRow, nCol);
                FormLogicF1->f1Forms->Protection_Get(etlnLocked, etlnHidden);
                FormLogicF1->f1Forms->Selection_Set(insertRow, nCol, insertRow, nCol);
                FormLogicF1->f1Forms->Protection_Get(inrtLocked, inrtHidden);

                if ((etlnLocked == inrtLocked) && (etlnHidden == inrtHidden)) {
                    continue;
                }

                FormLogicF1->f1Forms->Protection_Set(etlnLocked, etlnHidden);
            }
        } catch (...) {
            // Handle any exceptions
        }

        FormLogicF1->f1Forms->Selection_Set(R1, C1, R2, C2);
    }

// попытка поставить впереди наименования нумерацию по местоположению (по level_sign)
std::string TFormUpdF1Logic::prefixObjectName(TFormLogicF1* FormLogicF1, int insertRow, TwwStringList* p, int ColName) {
    std::string result;
    std::string letter = "а)б)в)г)д)е)ж)з)и)к)л)м)н)о)п)р)с)т)у)ф)х)ц)ч)ш)";
//    PAtrRow AtrRow = &(p->ListRows->items[insertRow]);
    PAtrRow AtrRow = ((*p->ListRows)[insertRow]);

    if (AtrRow->level_sign == 4) return result;
    if (AtrRow->level_sign == 3) {
        result = " - ";
        AtrRow->L = result.length();
        return result;
    }
//    PAtrRow AtrRow1 = &(p->ListRows[insertRow - 1]);
    PAtrRow AtrRow1 = ((*p->ListRows)[insertRow - 1]);

    std::string predS = char16ToUTF8(FormLogicF1->f1Forms->TextRC_Get(insertRow - 1, ColName));
    predS = predS.substr(0, AtrRow1->L);
    predS = Trim(predS);
    if (AtrRow1->CurRowVid == AtrRow->CurRowVid) {
        if (AtrRow1->level_sign != AtrRow->level_sign) return result;
        switch (AtrRow->level_sign) {
        case 1: {
            std::string s;
            predS.pop_back(); // remove last dot
            for (int i = predS.length() - 1; i >= 0; i--) {
                if (predS[i] == '.') break;
                s = predS[i] + s;
                predS.pop_back();
            }
            int n;
            try {
                n = std::stoi(s);
            }
            catch (...) {
                n = 1;
            }
            n++;
            result = predS + std::to_string(n) + ". ";
            AtrRow->L = result.length();
            return result;
        }
        case 2: {
            int n = letter.find(predS);
            if (n < 0) return result;
            result = letter.substr(n + 2, 2) + " ";
            AtrRow->L = result.length();
            return result;
        }
        }
    }
    else {
        if (AtrRow1->ObjectId != AtrRow->OwnerId) return result;
        switch (AtrRow->level_sign) {
        case 1: {
            std::string s = "1. ";
            if (AtrRow1->level_sign != 1)
                result = s;
            else
                result = predS + s;
            AtrRow->L = result.length();
            return result;
        }
        case 2: {
            result = letter.substr(0, 2) + " ";
            AtrRow->L = result.length();
            return result;
        }
        }
    }
    return result;
}

// сделать добавленную строку в области видимости
void TFormUpdF1Logic::toVisible(TFormLogicF1* FormLogicF1, int insertRow, int colName) {
    if (FormLogicF1->f1Forms->ColHidden[colName]) return;
    FormLogicF1->f1Forms->Selection_Set(insertRow, colName, insertRow, colName);
    FormLogicF1->f1Forms->ActiveCell_Set(insertRow, colName);
    FormLogicF1->f1Forms->ShowActiveCell();
}

// вставить в таблицу F1 строку - показатель
// результат = false при вставке первого подобъекта в раздел
//             в этом случае F1 следует перерисовать
bool TFormUpdF1Logic::insertPokaztlToF1(TFormLogicF1* FormLogicF1, int OwnerId, int pokaz_id, string pokaz_name, string KBK,
                                        int pokaz_type, int enum_id, int edizm_id, int level_sign, int & aInsertRow) {
    bool Result = false;
    int idCast = TDB_Pokaztl::GetPokazIdInCast1(pokaz_id);
    TwwStringList* p = TFormDopInp::GetTww(FormLogicF1);
    int colName = TFormDopInp::GetNameCol(FormLogicF1);
    int i = FormLogicF1->f1Forms->Row_Get(); // выбранная пользователем строка
    PAtrRow AtrRow = TFormDopInp::GetListRowsItem(FormLogicF1, i);
    if (AtrRow == nullptr) return false;
    int wplan_id = AtrRow->plan_id; // чтобы использовать во вставленной строке
    int insertRow = 0;
    int OwnerNomRow = i;
    int LastRow = FormLogicF1->f1Forms->LastRow_Get();
    if (AtrRow->ObjectId == OwnerId) { // значит была операция добавление ПОДпоказателя
        // ищем в F1 последнюю строку - показатель, подчиненный OwnerId
        bool isFind = false;

        for (int i1 = i + 1; i1 <= LastRow; i1++) {
            PAtrRow AtrRow1 = TFormDopInp::GetListRowsItem(FormLogicF1, i1);
            if (AtrRow1 == nullptr) continue;
            if (AtrRow1->OwnerNomRow < i) break; // не нашли
            if (AtrRow1->OwnerId != OwnerId) continue;
            if (AtrRow1->CurRowVid == 3) break;
            if (AtrRow1->CurRowVid != 2) continue;
            isFind = true;
            int insertRow = i1 + 1;
            OwnerNomRow = AtrRow1->OwnerNomRow;
            // продолжаем искать, мы не знаем, где последний нужный
        }
        if (!isFind)
            int insertRow = i + 1; // это случай, когда вставляем первый в раздел
    } else { // значит была операция добавление показателя
        // ищем в F1 последнюю строку - показатель, подчиненный OwnerId
        bool isFind = false;
        int OwnerNomRow = AtrRow->OwnerNomRow;
        for (int i1 = i; i1 <= LastRow; i1++) {
            PAtrRow AtrRow1 = TFormDopInp::GetListRowsItem(FormLogicF1, i1);
            if (AtrRow1 == nullptr) continue;
            if (AtrRow1->OwnerNomRow < AtrRow->OwnerNomRow) break; // не нашли
            if (AtrRow1->OwnerId != OwnerId) continue;
            if (AtrRow1->CurRowVid == 3) break;
            if (AtrRow1->CurRowVid != 2) continue;
            isFind = true;
            int insertRow = i1 + 1;
            OwnerNomRow = AtrRow1->OwnerNomRow;
            // продолжаем искать, мы не знаем, где последний нужный
        }
        if (!isFind)
            int insertRow = i + 1; // это случай, когда вставляем сразу за выбранным
    }
    // в результате нашли insertRow - номер вставляемой строки

    // теперь в etlnRow надо найти номер строки с эталонным объектом
    bool isFind = false;
    int etlnRow;
    for (int i = insertRow - 1; i >= p->form_begBodyRow; i--) {
        PAtrRow AtrRow1 = TFormDopInp::GetListRowsItem(FormLogicF1, i);
        if (AtrRow1->CurRowVid != 2) continue;
        if (idCast != TDB_Pokaztl::GetPokazIdInCast1(AtrRow1->ObjectId)) continue;
        isFind = true;
        etlnRow = i;
        break;
    }
    if (!isFind) return false; // нет эталлоного объекта - неукого скопировать нужные данные
    PAtrRow AtrRow1 = nullptr;
    if (TDB_Pokaztl::GetTypePokaz(AtrRow1->ObjectId) != pokaz_type) return false;

    // вставить пустую строку с номером  insertRow
    TFormUpdF1Logic::insertEmptyRow(FormLogicF1, insertRow, p, OwnerNomRow, OwnerId, 0, pokaz_id,
                                    level_sign, 0, 2);
    TFormUpdF1Logic::DostupnCells(FormLogicF1, insertRow, etlnRow);
    AtrRow = TFormDopInp::GetListRowsItem(FormLogicF1, insertRow);
    if (wplan_id > 0)
        AtrRow->plan_id = wplan_id;

    // для вствленной строки отрабатываем ячейки
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    int LastCol = FormLogicF1->f1Forms->LastCol_Get();
    for (int i = 1; i <= LastCol; i++) {
        FormLogicF1->f1Forms->Selection_Set(etlnRow, i, etlnRow, i);
        int cf = FormLogicF1->f1Forms->CellFormat_Get();
        FormLogicF1->f1Forms->Selection_Set(insertRow, i, insertRow, i);
        FormLogicF1->f1Forms->CellFormat_Set(cf);
        if (i == colName) {
            std::string ws = TFormUpdF1Logic::prefixObjectName(FormLogicF1, insertRow, p, colName);
            FormLogicF1->f1Forms->Text_Set(utf8_to_utf16(ws + pokaz_name).c_str());
            if (p->is_edIzm == 1 || p->is_edIzm == 3)
                FormLogicF1->f1Forms->Text_Set(utf8_to_utf16(char16ToUTF8(FormLogicF1->f1Forms->Text_Get()) + ", " + TDB_EdIzm::GetEdIzmName(edizm_id)).c_str());
        }
        PCellValueAtr vd = TFormDopInp::GetCellValueAtrByRC(FormLogicF1, etlnRow, i);
        std::string stmnt = p->ListStmnt[i].Stmnt;
        PCellValueAtr vd1 = nullptr;
        vd1 = new TCellValueAtr();
        if (vd != nullptr) {

            TFormDopInp::CopyCellValueAtr(*vd1, vd);
            vd1->Row = insertRow;
            vd1->PokazId = pokaz_id;
            if (vd->CellId > 0) {
                if (stmnt != "ЕДИЗМ" && stmnt != "ЕДИЗМобщНП") {
                    TDB_Cell::GetCell_id(q, vd->PokazId, vd->CellId);
                    vd1->CellId = TDB_Cell::GetCellId2(vd1->PokazId, q->FieldByName("cell_period")->AsInteger(),
                                                       q->FieldByName("ist_fin_id")->AsInteger(), q->FieldByName("cell_din_izm")->AsString());
                }
            }
            p->AddObject("R" + std::to_string(vd1->Row) + "C" + std::to_string(vd1->Col), vd1);
        }
        std::string Value = "";
        int Id_Znac;
        if (stmnt == "КОДБК")
            Value = KBK;
        else if (stmnt == "НомерМрпр")
            Value = TDB_NMrpr::PokazidToNMP(pokaz_id, FormLogicF1->CurrYear, Id_Znac);
        else if (stmnt == "Примечание")
            Value = TDB_Pokaztl::GetPrimecanie(pokaz_id);
        else if (stmnt == "Ведомство")
            Value = TDB_PlanStr::GetVedomstvo(vd1->PlanId, vd1->PokazId);
        else if (stmnt == "КОДБК1")
            Value = SetCodeBK_rzdltl(KBK, FormLogicF1->CurrYear);
        else if (stmnt == "КОДБКРздл" || stmnt == "КОДБКРздл1") {
            Value = TDB_bkMetaDef::GetSubKBKByVid(KBK, 0, FormLogicF1->CurrYear);
            if (stmnt == "КОДБКРздл1")
                Value = SetCodeBK_RZD_rzdltl(Value, 0);
        } else if (stmnt == "КОДБКЦелСт" || stmnt == "КОДБКЦелСт1") {
            Value = TDB_bkMetaDef::GetSubKBKByVid(KBK, 1, FormLogicF1->CurrYear);
            if (stmnt == "КОДБКЦелСт1")
                Value = SetCodeBK_CST_rzdltl(Value, FormLogicF1->CurrYear, 0);
        } else if (stmnt == "КОДБКВидРасх")
            Value = TDB_bkMetaDef::GetSubKBKByVid(KBK, 2, FormLogicF1->CurrYear);
        else if (stmnt == "КОДБКЭкКл")
            Value = TDB_bkMetaDef::GetSubKBKByVid(KBK, 3, FormLogicF1->CurrYear);
        else if (stmnt == "ЕДИЗМ") {
            vd1->CellId = edizm_id;
            Value = TDB_EdIzm::GetEdIzmName(vd1->CellId);
        } else if (stmnt == "ЕДИЗМобщНП")
            Value = TDB_EdIzm::GetEdIzmName(vd1->CellId);
        else if (stmnt == "FORMULAF1")
            Value = TFormDopInp::ReplaceNRowNCol(insertRow, i, TDB_FormStr::GetNote1(p->FormId, 1000 + i));
        if (!Value.empty()) {
            if (Value[0] == '^')
                FormLogicF1->f1Forms->Formula_Set(utf8_to_utf16(Copy(Value, 2, 500)).c_str());
            else
                FormLogicF1->f1Forms->Text_Set(utf8_to_utf16(Value).c_str());
        }
        if (stmnt == "№п/п")
            TForm2Plus::SetNumberPP(FormLogicF1, p->form_begBodyRow, FormLogicF1->f1Forms->LastRow); // Ставим № п/п
    }
    TFormUpdF1Logic::toVisible(FormLogicF1, insertRow, colName);
    aInsertRow = insertRow;
    Result = true;
    return Result;
}


// удалить из названий строк префиксы
void TFormUpdF1Logic::delPrefixFromNames(TFormLogicF1* FormLogicF1) {
        TwwStringList* p = TFormDopInp::GetTww(FormLogicF1);
        int colName = TFormDopInp::GetNameCol(FormLogicF1);
        for (int i = p->form_begBodyRow; i <= p->form_endBodyRow; i++) {
            PAtrRow AtrRow = TFormDopInp::GetListRowsItem(FormLogicF1, i);
            if (AtrRow != nullptr) {
                if (AtrRow->L > 0) {
                    FormLogicF1->f1Forms->TextRC_Set(i, colName, utf8_to_utf16(Copy(char16ToUTF8(FormLogicF1->f1Forms->TextRC(i, colName)),AtrRow->L+1, 5000)).c_str());
                }
            }
        }
    }

// удалить строку из F1 с пересчетом префиксов наименований, с корректировкой ListRows и списка TValueDates
void TFormUpdF1Logic::deleteRowFromF1(TFormLogicF1* FormLogicF1, int nRow) {
    int i, j, n;
    PAtrRow AtrRow, AtrRow1;
    TwwStringList* p;
    PCellValueAtr vd;
    int L;

    p = TFormDopInp::GetTww(FormLogicF1);
    AtrRow = TFormDopInp::GetListRowsItem(FormLogicF1, nRow);
    L = AtrRow->L;
    TFormUpdF1Logic::deleteRowFromF1_0(FormLogicF1, nRow);
    if (L > 0) {
        TFormUpdF1Logic::delPrefixFromNames(FormLogicF1);
        TForm2Plus::SetIdRow(FormLogicF1, TFormDopInp::GetNameCol(FormLogicF1), p->form_begBodyRow - 1);
    }
    TForm2Plus::SetNumberPP(FormLogicF1, p->form_begBodyRow, FormLogicF1->f1Forms->LastRow);
}

// удалить строку из F1 с корректировкой ListRows и списка TValueDates, но БЕЗ пересчета префиксов наименований
void TFormUpdF1Logic::deleteRowFromF1_0(TFormLogicF1* FormLogicF1, int nRow) {
    TwwStringList* p = TFormDopInp::GetTww(FormLogicF1);
    PAtrRow AtrRow = TFormDopInp::GetListRowsItem(FormLogicF1, nRow);
    p->Delete(nRow);
    delete AtrRow;

    // Change row numbers
    for (int i = nRow; i < p->ListRows->Count(); i++) {
//        PAtrRow AtrRow = &(p->ListRows[i]);

        PAtrRow AtrRow = ((*p->ListRows)[i]);

        if (AtrRow) {
            AtrRow->Row--;
            if (AtrRow->OwnerNomRow > nRow)
                AtrRow->OwnerNomRow--;
        }
    }

    // Delete cells in the deleted row
    for (int i = 1; i <= FormLogicF1->f1Forms->LastCol; i++) {
        int n = p->IndexOf("R" + std::to_string(nRow) + "C" + std::to_string(i));
        if (n >= 0) {

            PCellValueAtr vd = std::get<PCellValueAtr>(p->Objects[n]);
            p->Delete(n);
            delete vd;
        }
    }

    // Adjust cell row numbers for rows below the deleted row
    for (int i = nRow + 1; i <= FormLogicF1->f1Forms->LastRow; i++) {
        for (int j = 1; j <= FormLogicF1->f1Forms->LastCol; j++) {
            int n = p->IndexOf("R" + std::to_string(i) + "C" + std::to_string(j));
            if (n >= 0) {
                PCellValueAtr vd = std::get<PCellValueAtr>(p->Objects[n]);
                p->Delete(n);
                vd->Row--;
                p->AddObject("R" + std::to_string(vd->Row) + "C" + std::to_string(vd->Col), vd);
            }
        }
    }

    FormLogicF1->f1Forms->EnableProtection = false;
    TF1toDE::DeleteRows(FormLogicF1, nRow, nRow);
    FormLogicF1->f1Forms->EnableProtection = true;

    FormLogicF1->CurrInputRow--;
    p->form_endBodyRow--;
}

// Удалить строку с показателем, объектом
void TFormUpdF1Logic::delRowByPokaz(TFormLogicF1* FormLogicF1, int nRow, int nCol) {
        TwwStringList* wp = TFormDopInp::GetTww(FormLogicF1);
        PAtrRow AtrRow;
        int R1, C1, R2, C2;
        bool IsNextHidden= false;

        if (FormLogicF1->f1Forms->LastRow_Get() > nRow)
            IsNextHidden = FormLogicF1->f1Forms->RowHidden_Get(nRow + 1);

        TFormUpdF1Logic::deleteRowFromF1(FormLogicF1, nRow);

        if (FormLogicF1->f1Forms->LastRow_Get() >= nRow) {
            FormLogicF1->f1Forms->RowHeight_Set(nRow, 0xFFFFFFFF);
            FormLogicF1->f1Forms->Selection_Get(0, R1, C1, R2, C2);
            if (IsNextHidden)
                FormLogicF1->f1Forms->RowHidden_Set(nRow, true);
            FormLogicF1->f1Forms->Selection_Set(R1, C1, R2, C2);
            AtrRow = TFormDopInp::GetListRowsItem(FormLogicF1, nRow);
            if (AtrRow != nullptr)
                TFormDopInp::SetSelectionById(FormLogicF1, AtrRow->ObjectId);
        }

        FormLogicF1->f1Forms->Col = nCol;
    }

bool TFormUpdF1Logic::delDefMrpr(TFormLogicF1 *FormLogicF1, int nRow, int pokaz_Id, string &msgError) {
    PAtrRow AtrRow;
    TwwStringList* p;
    int wplan;

    AtrRow = TFormDopInp::GetListRowsItem(FormLogicF1, nRow);
    p = TFormDopInp::GetTww(FormLogicF1);

    // проверка возможности удаления
    if (p->is_add != 1) {
        msgError = "Для данной формы эта операция не разрешена";
        return false;
    }

    wplan = FormLogicF1->CurrPlanId;
    if (wplan == 0) {
        wplan = AtrRow->plan_id;
    }
    if (wplan == 0) {
        wplan = p->plan_id;
    }

    if (TDB_UserPlan::GetUserPlan_Perm(wplan) != 2) { // 2 - На запись
        msgError = "Текущему пользователю для данного субъекта недоступны операции изменения";
        return false;
    }

    // тут должны быть еще проверки на возможность удаления

    return TDB_Pokaztl::DeletePokaztl(pokaz_Id, AtrRow->OwnerId, true);
}

bool TFormUpdF1Logic::UpdPokazMrprObj(int pokaz_id, std::string pokaz_name, std::string primecanie) {
    TDB_Pokaztl::UpdPokazName(pokaz_id, pokaz_name);
    TDB_Pokaztl::SetPrimecanie(pokaz_id, primecanie);
    return true;
}





























