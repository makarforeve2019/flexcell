#include "SystFuncsLogic.h"
#include "unTF1CellFormat.h"
#include "DB_Pokaztl.h"
#include "FormSorted.h"


void TFormSorted::SortedForm(TFormLogicF1* FormLogicF1, int BegRow, int ncol1, int EndRow, int ncol2, bool isKusocno) {
    TFormSorted* vrFS = new TFormSorted();
    vrFS->FormLogicF1 = FormLogicF1;
    vrFS->BegRow = BegRow;
    vrFS->ncol1 = ncol1;
    vrFS->EndRow = EndRow;
    vrFS->ncol2 = ncol2;
    vrFS->isKusocno = isKusocno;

    for (int i = 1; i <= n10; i++) {
        vrFS->DefColSorts[i].NCol = 0;
        vrFS->DefColSorts[i].is_sort = 0;
    }

    TwwStringList* wp = TFormDopInp::GetTww(FormLogicF1);

    FormLogicF1->quFormStr->First();

    vrFS->MaxUS = 0;
    vrFS->gde_svod = 0;
    vrFS->isFormReadonly = false;
    vrFS->isSortSvod = false;
    while (!FormLogicF1->quFormStr->EoF()) {
        vrFS->form_str_n = FormLogicF1->quFormStr->FieldByName("form_str_n")->AsInteger();
        if (vrFS->form_str_n >= 1000 && vrFS->form_str_n < 1499) {
            vrFS->uroven_sort = FormLogicF1->quFormStr->FieldByName("uroven_sort")->AsInteger();
            vrFS->is_sort = FormLogicF1->quFormStr->FieldByName("is_sort")->AsInteger();
            if (vrFS->uroven_sort == 0 && vrFS->is_sort == 5) {
                vrFS->DefColSum[FormLogicF1->quFormStr->FieldByName("attr_1")->AsInteger()].is_sum = true;
            }
            if (vrFS->is_sort > 0 && vrFS->uroven_sort >= 1 && vrFS->uroven_sort <= n10) {
                if (vrFS->MaxUS < vrFS->uroven_sort) {
                    vrFS->MaxUS = vrFS->uroven_sort;
                }
                vrFS->DefColSorts[vrFS->uroven_sort].uroven_sort = vrFS->uroven_sort;
                vrFS->DefColSorts[vrFS->uroven_sort].NCol = FormLogicF1->quFormStr->FieldByName("attr_1")->AsInteger();
                vrFS->DefColSorts[vrFS->uroven_sort].is_sort = vrFS->is_sort;
                if (vrFS->is_sort == 2) {
                    vrFS->DefColSorts[vrFS->uroven_sort].NCol = -vrFS->DefColSorts[vrFS->uroven_sort].NCol;
                }
                vrFS->DefColSorts[vrFS->uroven_sort].is_svod = FormLogicF1->quFormStr->FieldByName("is_svod")->AsInteger();
                if (vrFS->DefColSorts[vrFS->uroven_sort].is_svod == 1) {
                    vrFS->gde_svod = 1;
                }
                if (vrFS->DefColSorts[vrFS->uroven_sort].is_svod >= 2) {
                    vrFS->gde_svod = 2;
                }
                if (vrFS->DefColSorts[vrFS->uroven_sort].is_svod >= 3) {
                    vrFS->isSortSvod = true;
                }
                vrFS->DefColSorts[vrFS->uroven_sort].name_svod = FormLogicF1->quFormStr->FieldByName("name_svod")->AsString();
            }
        }
        FormLogicF1->quFormStr->Next();
    }
    if (FormLogicF1->quForms->FieldByName("form_readonly")->AsInteger() == 1) {
        vrFS->isFormReadonly = true;
    }
    if (vrFS->isFormReadonly && vrFS->isSortSvod) {
        vrFS->isFormReadonlySortSvod = true;
    }
    // сохраняем старую нумерацию строк в наименовании показателя в форме в виде nnn.
    // в коллекции номера в ключах увеличим на 10000
    vrFS->p = TFormDopInp::GetTww(FormLogicF1);
    vrFS->NameCol = TFormDopInp::GetNameCol(FormLogicF1);
    for (int i = BegRow; i <= EndRow; i++) {
        string ws = IntToStr(i) + ".";
        string ws16 = char16ToUTF8(FormLogicF1->f1Forms->TextRC(i,vrFS->NameCol));
        string ws162 = ws + ws162;
        FormLogicF1->f1Forms->TextRC(i,vrFS->NameCol) = utf8_to_utf16(ws162).c_str();
        if (!vrFS->isFormReadonlySortSvod) {
            for (int j = ncol1; j <= ncol2; j++) {
                int nn = vrFS->p->IndexOf("R" + std::to_string(i) + "C" + std::to_string(j));
                if (nn < 0) {
                    continue;
                }
                PCellValueAtr vd = std::get<PCellValueAtr>(vrFS->p->Objects[nn]);
                vrFS->p->Delete(nn);
                vrFS->p->AddObject("R" + std::to_string(i + 10000) + "C" + std::to_string(j), vd);
            }
        }
    }
    vrFS->ListRows = TList<PAtrRow>();
//    vrFS->ListRows.Count() = 10000;
//    vrFS->ListRows.assign(vrFS->p.ListRows, laCopy);
    TDictionary<int, std::string>* OldValuesDict = new TDictionary<int, std::string>();
    TDictionary<int, std::string>* OldValuesParentDict = new TDictionary<int, std::string>();
    //ДЕЛАЕМ ИЗМЕНЕНИЯ ДЛЯ ИЕРАРХИЧНОЙ ФОРМЫ
    if (wp->work_with == 4 && vrFS->DefColSorts[2].NCol == 0) {

        int k = 1;
        for (int i = 1; i <= n10; i++) {
            int nColSort = vrFS->DefColSorts[i].NCol;
            if (nColSort == 0) {
                break;
            }
            for (int nRow = wp->form_begBodyRow; nRow <= wp->form_endBodyRow; nRow++) {
//                PAtrRow AtrRow = wp->ListRows->Items[nRow];

                PAtrRow AtrRow = ((*wp->ListRows)[i]);

                int objIdI = AtrRow->ObjectId;
                int classIdI = AtrRow->objekt_class_id;
                string wKeyOwnerStr;
                if (TDB_Pokaztl::Get_PokazBaseId(classIdI) != 0) {
                    std::string oldZnach = char16ToUTF8(FormLogicF1->f1Forms->TextRC(nRow,nColSort));
                    OldValuesDict->Add(objIdI, oldZnach);
                    FormLogicF1->f1Forms->TextRC(nRow,nColSort) = utf8_to_utf16(wKeyOwnerStr + oldZnach + std::to_string(k)).c_str();
                }
                else {
                    if (!OldValuesParentDict->ContainsValue(char16ToUTF8(FormLogicF1->f1Forms->TextRC(nRow,nColSort))) && OldValuesParentDict->ContainsKey(objIdI)) {
                        k = 0;
                        std::string oldZnach = char16ToUTF8(FormLogicF1->f1Forms->TextRC(nRow,nColSort));
                        wKeyOwnerStr = char16ToUTF8(FormLogicF1->f1Forms->TextRC(nRow,nColSort));
                        OldValuesParentDict->Add(objIdI, oldZnach);
                        FormLogicF1->f1Forms->TextRC(nRow,nColSort) = utf8_to_utf16(wKeyOwnerStr + oldZnach).c_str();
                    }
                    else {
                        std::string oldZnach = char16ToUTF8(FormLogicF1->f1Forms->TextRC(nRow,nColSort));
                        wKeyOwnerStr = char16ToUTF8(FormLogicF1->f1Forms->TextRC(nRow,nColSort)) + std::to_string(k);
                        OldValuesParentDict->Add(objIdI, oldZnach);
                        FormLogicF1->f1Forms->TextRC(nRow,nColSort) = utf8_to_utf16(wKeyOwnerStr + oldZnach).c_str();
                        k = k + 1;
                    }
                }
            }
        }
    }

  // сама сортировка
  //************* ncol1 должен быть всегда 1 ****************************
 
    //mak ??? V := VarArrayCreate([1, vrFS.MaxUS], varSmallint);
    //mak ??? for (int i = 1; i<= vrFS->MaxUS; i++)  V[i] =  vrFS->DefColSorts[i].NCol;

    int wBegRow = 0;
    int wEndRow = 0;
    for (int i = BegRow; i <= EndRow; i++) {
//        PAtrRow AtrRow = vrFS->p->ListRows->Items[i];

        PAtrRow AtrRow = ((vrFS->ListRows)[i]);

        if (AtrRow == NULL) {
            continue;
        }
        if (AtrRow->inSort) {
            if (wBegRow == 0) {
                wBegRow = i;
                wEndRow = i;
            }
            else {
                wEndRow = i;
            }
        }
        else {
            if (wBegRow == 0) {
                continue;
            }
            try {
                //mak ??? FormLogicF1->f1Forms->Sort(wBegRow, ncol1, wEndRow, ncol2, true, V);
            }
            catch (exception& E) {
                // showmessage(E.Message);
            }
            wBegRow = 0;
            wEndRow = 0;
        }
    }
    if (wBegRow > 0) {
        try {
            //mak ??? FormLogicF1->f1Forms->Sort(wBegRow, ncol1, wEndRow, ncol2, true, V);
        }
        catch (exception& E) {
            // showmessage(E.Message);
        }
    }

    vrFS->IsInsert = true;
    if (vrFS->gde_svod == 1) {
        for (int nUS = 1; nUS <= vrFS->MaxUS; nUS++) {
            EndRow = FormLogicF1->f1Forms->LastRow;
            if (vrFS->DefColSorts[nUS].is_sort == 0 && vrFS->DefColSorts[nUS].is_svod == 0) {
                continue;
            }
            if (vrFS->DefColSorts[nUS].is_sort == 1 && vrFS->DefColSorts[nUS].is_svod == 0) {
                continue;
            }
            for (int i = EndRow; i >= BegRow; i--) {
                if (vrFS->wGetListRowsItem(i)) {
                    if (vrFS->IsInsert) {
                        vrFS->Sumiruem(nUS, i + 1);
                        vrFS->InsertRow(nUS, i);
                        vrFS->IsInsert = false;
                        FormLogicF1->f1Forms->RowHidden[i] = false;
                    }
                    if (i == BegRow) {
                        vrFS->Sumiruem(nUS, BegRow);
                        break;
                    }
                    if (vrFS->wGetListRowsItem(i - 1)) {
                        if (!vrFS->Sravnim(nUS, i)) {
                            vrFS->IsInsert = true;
                        }
                    }
                }
                else {
                    vrFS->IsInsert = true;
                    vrFS->Sumiruem(nUS, i + 1);
                }
            }
        }
    }
    if (vrFS->gde_svod == 2) {
        for (int nUS = 1; nUS <= vrFS->MaxUS; nUS++) {
            EndRow = FormLogicF1->f1Forms->LastRow;
            if (vrFS->DefColSorts[nUS].is_svod == 0) {
                continue;
            }
            for (int i = EndRow; i >= BegRow; i--) {
                vrFS->IsInsert = false;
                if (vrFS->wGetListRowsItem(i)) {
                    if (i == BegRow) {
                        vrFS->IsInsert = true;
                    }
                    if (vrFS->wGetListRowsItem(i - 1)) {
                        if (!vrFS->Sravnim(nUS, i)) {
                            vrFS->IsInsert = true;
                        }
                    }
                    else {
                        vrFS->IsInsert = true;
                    }
                    if (vrFS->IsInsert) {
                        vrFS->InsertIntoRow(nUS, i);
                        vrFS->Sumiruem2(nUS, i);
                        FormLogicF1->f1Forms->RowHidden[i] = false;
                    }
                }
            }
        }
    }

    /*mak vrFS->p->ListRows->Clear();
    vrFS->p->ListRows->Count() = 10000;*/
    EndRow = FormLogicF1->f1Forms->LastRow;
    for (int i = BegRow; i <= EndRow; i++) {
        int nn = Pos(".", char16ToUTF8(FormLogicF1->f1Forms->TextRC(i,vrFS->NameCol)));
        if (nn == 0) {
            continue;
        }
        int nOld = StrToInt(Copy(char16ToUTF8(FormLogicF1->f1Forms->TextRC(i, vrFS->NameCol)), 1, nn-1));
        FormLogicF1->f1Forms->TextRC(i,vrFS->NameCol) = utf8_to_utf16(Copy(char16ToUTF8(FormLogicF1->f1Forms->TextRC(i, vrFS->NameCol)), nn+1, 1000)).c_str();;
        //mak ??? vrFS->p->ListRows->Insert(i, vrFS->ListRows->items[nOld]);
//        PAtrRow AtrRow = (PAtrRow)vrFS->p->ListRows->Items[i];
        PAtrRow AtrRow = ((vrFS->ListRows)[i]);

        AtrRow->Row = i;
        if (!vrFS->isFormReadonlySortSvod) {
            for (int j = ncol1; j <= ncol2; j++) {
                int nn = vrFS->p->IndexOf("R" + std::to_string(nOld + 10000) + "C" + std::to_string(j));
                if (nn < 0) {
                    continue;
                }
                PCellValueAtr vd = std::get<PCellValueAtr>(vrFS->p->Objects[nn]);
                vd->Row = i;
                vrFS->p->Delete(nn);
                vrFS->p->AddObject("R" + std::to_string(i) + "C" + std::to_string(j), (TObject*)vd);
            }
        }
    }
    vrFS->ListRows.Clear();

    if (wp->work_with == 4 && vrFS->DefColSorts[2].NCol == 0) {
        for (int i = 1; i <= n10; i++) {
            int nColSort = vrFS->DefColSorts[i].NCol;
            if (nColSort == 0) {
                break;
            }
            for (int nRow = wp->form_begBodyRow; nRow <= wp->form_endBodyRow; nRow++) {
//                PAtrRow AtrRow = wp->ListRows->Items[nRow];

                PAtrRow AtrRow = ((*wp->ListRows)[nRow]);
                
                int objIdI = AtrRow->ObjectId;
                std::string oldZnach;
                if (OldValuesDict->TryGetValue(objIdI, oldZnach)) {
                    FormLogicF1->f1Forms->TextRC(nRow,nColSort) = utf8_to_utf16(oldZnach).c_str();
                }
                if (OldValuesParentDict->TryGetValue(objIdI, oldZnach)) {
                    FormLogicF1->f1Forms->TextRC(nRow,nColSort) = utf8_to_utf16(oldZnach).c_str();
                }
            }
        }
    }

    delete vrFS;
}

void TFormSorted::SetBold(bool aBold) {
    FormLogicF1->f1Forms->FontItalic = false;
    FormLogicF1->f1Forms->FontBold = aBold;
    if (FormLogicF1->f1Forms->ColHidden[FormLogicF1->f1Forms->Col])
        FormLogicF1->f1Forms->WordWrap = false;
    else
        FormLogicF1->f1Forms->WordWrap = true;
}

bool TFormSorted::IsGoodStmnt(std::string aStmnt) {
    return ((aStmnt == "ЗНАЧЕНИЕСВ") || 
            (aStmnt == "ЗНАЧЕНИЕСВНИ") || (aStmnt == "ЗНАЧЕНИЕНВНИ") ||
            (aStmnt == "КБК-Дгвр:Сумма№БО") || (aStmnt == "КБК-Дгвр:СуммаБО") ||
            (aStmnt == "ЗНАЧЕНИЕСВИР") || (aStmnt == "ЗНАЧЕНИЕНВИР") ||
            (aStmnt == "ОТКЛОНЕНИЕСВНачКВ") || (aStmnt == "ОТКЛОНЕНИЕНВНачКВ") ||
            (aStmnt == "УТОЧНЕНИЕСВ") || (aStmnt == "УТОЧНЕНИЕСВНИ") ||
            (aStmnt == "Мрпр-Дгвр:Сумма№БО") || (aStmnt == "Мрпр-Дгвр:СуммаБО") ||
            (aStmnt == "ФактКвДату") || (aStmnt == "СуммаПлГрНИВМесяце") ||
            (aStmnt == "ПлнОСТАТОК") || (aStmnt == "ФктОСТАТОК") ||
            (aStmnt == "ФОРМУЛАF1") || (aStmnt == "ФактВсегоВНачале") || (aStmnt == "ПоступлВсегоВНачале") ||
            (aStmnt == "ЗНАЧЕНИЕобщНП") || (aStmnt == "СуммаПриход") || (aStmnt == "СуммаВозврат") ||
            (aStmnt == "СуммаПлГрНИВНачале"));
}

std::string TFormSorted::GetLicScet(int anUS, int aRow, int& aCol) {
    std::string Result = "";
    if ((aRow + 1) > FormLogicF1->f1Forms->LastRow) {
        return Result;
    }
    if (DefColSorts[anUS].NCol == p->nColForBP) {
        for (int i = 1; i <= FormLogicF1->f1Forms->LastCol; i++) {
            std::string stmnt = p->ListStmnt[i].Stmnt;
            if (stmnt != "РАСЧСЧЕТ") {
                continue;
            }
            Result = char16ToUTF8(FormLogicF1->f1Forms->TextRC(aRow + 1,i));
            if (Result == "0") {
                Result = "";
            }
            aCol = i;
            return Result;
        }
    }
    return Result;
}

std::string TFormSorted::GetBKCode(int anUS, int aRow, int& aCol) {
    std::string Result = "";
    if ((aRow + 1) > FormLogicF1->f1Forms->LastRow) {
        return Result;
    }
    std::string stmnt = p->ListStmnt[DefColSorts[anUS].NCol].Stmnt;
    if (stmnt == "КОДБК") {
        for (int i = 1; i <= FormLogicF1->f1Forms->LastCol; i++) {
            stmnt = p->ListStmnt[i].Stmnt;
            if (stmnt != "КОДБК") {
                continue;
            }
            Result = char16ToUTF8(FormLogicF1->f1Forms->TextRC(aRow + 1,i));
            aCol = i;
            return Result;
        }
    }
    return Result;
}

void TFormSorted::SetFormula(int anUS, int toRow, int toCol, double aD) {
    std::string Value, s;
    Value = TFormDopInp::ReplaceNRowNCol(toRow, toCol, p->ListStmnt[toCol].note1);
    try {
        FormLogicF1->f1Forms->Selection_Set(toRow, toCol, toRow, toCol);
        if (DefColSum[toCol].is_sum) // 26.08.2021
            FormLogicF1->f1Forms->Number = aD; // 16 августа 2009
        else
            FormLogicF1->f1Forms->Formula = utf8_to_utf16(Value.substr(2, 10000)).c_str();
        SetBold(true);
    } catch (...) {
        s = "Содержимое ячейки с ошибочной формулой = " + Value.substr(2, 1000);
        throw std::runtime_error("Неверная формула");
    }
}

void TFormSorted::Sumiruem(int anUS, int aRow) {
    if (aRow > FormLogicF1->f1Forms->LastRow) return;
    if (TFormDopInp::GetListRowsItem(FormLogicF1, aRow) == nullptr) return;

    for (int j = 1; j <= FormLogicF1->f1Forms->LastCol; j++) {
        std::string stmnt = p->ListStmnt[j].Stmnt;
        if (!IsGoodStmnt(stmnt)) continue;

        if (FormLogicF1->f1Forms->ColHidden[j]) continue; // если столбец скрыт, то не суммируем - 21 янв 2015
        FormLogicF1->f1Forms->Selection_Set(aRow, j, aRow, j);
        if ((FormLogicF1->f1Forms->Type == 2) || (FormLogicF1->f1Forms->Type == -2)) continue;

        double d = 0.0;
        int i;
        for (i = aRow; i <= FormLogicF1->f1Forms->LastRow; i++) {
            if (TFormDopInp::GetListRowsItem(FormLogicF1, i) == nullptr) break;
            d += FormLogicF1->f1Forms->NumberRC(i,j);
        }
        if (stmnt == "ФОРМУЛАF1") {
            SetFormula(anUS, i, j, d);
            continue;
        }

        if (d != 0.0) {
            FormLogicF1->f1Forms->Selection_Set(i, j, i, j);
            FormLogicF1->f1Forms->Number = d;
            SetBold(true);
        }
    }
}

bool TFormSorted::wGetListRowsItem(int aRow) {
    bool Result = false;
    PAtrRow AtrRow = TFormDopInp::GetListRowsItem(FormLogicF1, aRow);
    if (AtrRow == nullptr) {
        return Result;
    }
    if (!AtrRow->inSort) {
        return Result;
    }
    Result = true;
    return Result;
}

void TFormSorted::Sumiruem2(int anUS, int aRow) {
    int i, j;
    std::string stmnt;
    double d;

    for (j = 1; j <= FormLogicF1->f1Forms->LastCol; j++) {
        stmnt = p->ListStmnt[j].Stmnt;
        if (!IsGoodStmnt(stmnt)) continue;
        d = 0;
        for (i = aRow + 1; i <= FormLogicF1->f1Forms->LastRow; i++) {
            if (TFormDopInp::GetListRowsItem(FormLogicF1, i) == nullptr) break;
            if (!wGetListRowsItem(i)) continue; // элемент НЕ в сортировке
            d += FormLogicF1->f1Forms->NumberRC(i,j);
            if (DefColSorts[anUS].is_svod == 3 && wGetListRowsItem(i)) // элемент в сортировке
                FormLogicF1->f1Forms->RowHidden[i] = true;
        }
        if (stmnt == "ФОРМУЛАF1") {
            SetFormula(anUS, aRow, j, d);
            continue;
        }
        if (d != 0) {
            FormLogicF1->f1Forms->Selection_Set(aRow, j, aRow, j);
            FormLogicF1->f1Forms->Number = d;
            SetBold(DefColSorts[anUS].is_svod <= 2);
        }
    }
}

void TFormSorted::Perenumerovati(int nBegRow, int nMaxRow) {
    // перенумеруем ListRow
    for (int i = nMaxRow; i >= nBegRow; i--) {
            if (TFormDopInp::GetListRowsItem(FormLogicF1, i) != nullptr) { // нашли элемент
                p->ListRows->Exchange(i, i+1);
        }
    }
}

void TFormSorted::InsertRow(int anUS, int aRow) {
    int NCol, wnUS, isType;
    std::string font;
    byte Align_vert, Align_gor, h, is_bold;
    TwwStringList* p = TFormDopInp::GetTww(FormLogicF1);
    FormLogicF1->CurrInputRow++;
    TF1toDE::InsertRowsCells(p, FormLogicF1, aRow + 1, aRow + 1);
    FormLogicF1->f1Forms->RowHeight[aRow + 1] = 0xFFFFFFFF;
    FormLogicF1->f1Forms->Selection_Set(aRow + 1, NameCol, aRow + 1, NameCol);
    FormLogicF1->f1Forms->Text = utf8_to_utf16(DefColSorts[anUS].name_svod).c_str();
    SetBold(true);
    NCol = DefColSorts[anUS].NCol;
    FormLogicF1->f1Forms->Selection_Set(aRow, NCol, aRow, NCol);
    isType = FormLogicF1->f1Forms->Type;
    FormLogicF1->f1Forms->Selection_Set(aRow + 1, NCol, aRow + 1, NCol);
    switch (isType) {
    case 1:
        FormLogicF1->f1Forms->Number = FormLogicF1->f1Forms->NumberRC(aRow, NCol);
        break;
    case 2:
        FormLogicF1->f1Forms->Text = FormLogicF1->f1Forms->TextRC(aRow, NCol);
        break;
    default:
        FormLogicF1->f1Forms->Text = FormLogicF1->f1Forms->TextRC(aRow, NCol);
        break;
    }
    SetBold(true);
    for (wnUS = anUS - 1; wnUS >= 1; wnUS--) {
        if ((DefColSorts[wnUS].is_sort == 1) && (DefColSorts[wnUS].is_svod != 0))
            break;
        if (DefColSorts[wnUS].is_sort == 0)
            continue;
        NCol = DefColSorts[wnUS].NCol;
        FormLogicF1->f1Forms->Selection_Set(aRow, NCol, aRow, NCol);
        isType = FormLogicF1->f1Forms->Type;
        FormLogicF1->f1Forms->Selection_Set(aRow + 1, NCol, aRow + 1, NCol);
        switch (isType) {
        case 1:
            FormLogicF1->f1Forms->Number = FormLogicF1->f1Forms->NumberRC(aRow, NCol);
            break;
        case 2:
            FormLogicF1->f1Forms->Text = FormLogicF1->f1Forms->TextRC(aRow, NCol);
            break;
        default:
            FormLogicF1->f1Forms->Text = FormLogicF1->f1Forms->TextRC(aRow, NCol);
            break;
        }
        SetBold(true);
    }
    Perenumerovati(aRow + 1, FormLogicF1->f1Forms->LastRow);
}

void TFormSorted::InsertIntoRow(int anUS, int aRow) {
    int NCol, wnUS, i, wCol;
    TF1CellFormat cf;
    int pRow1, pCol1, pRow2, pCol2, isType;
    TwwStringList * p = TFormDopInp::GetTww(FormLogicF1);
    FormLogicF1->CurrInputRow++;
    TF1toDE::InsertRowsCells(p, FormLogicF1, aRow, aRow);

    for (i = 1; i <= FormLogicF1->f1Forms->LastCol; i++) {
        FormLogicF1->f1Forms->Selection_Set(aRow + 1, i, aRow + 1, i);
        cf.GetCellFormat(*FormLogicF1->f1Forms);
        FormLogicF1->f1Forms->Selection_Set(aRow, i, aRow, i);
        cf.SetCellFormat(*FormLogicF1->f1Forms);
    }

    FormLogicF1->f1Forms->RowHeight[aRow] = 0xFFFFFFFF;
    FormLogicF1->f1Forms->Selection_Set(aRow, NameCol, aRow, NameCol);
    FormLogicF1->f1Forms->Text = utf8_to_utf16(DefColSorts[anUS].name_svod).c_str();
    SetBold(DefColSorts[anUS].is_svod <= 2);

    NCol = std::abs(DefColSorts[anUS].NCol);
    FormLogicF1->f1Forms->Selection_Set(aRow + 1, NCol, aRow + 1, NCol);
    isType = FormLogicF1->f1Forms->Type;
    FormLogicF1->f1Forms->Selection_Set(aRow, NCol, aRow, NCol);

    switch (isType) {
    case 1:
        FormLogicF1->f1Forms->Number = FormLogicF1->f1Forms->NumberRC(aRow + 1, NCol);
        break;
    case 2:
        FormLogicF1->f1Forms->Text = FormLogicF1->f1Forms->TextRC(aRow + 1, NCol);
        break;
    default:
        FormLogicF1->f1Forms->Text = FormLogicF1->f1Forms->TextRC(aRow + 1, NCol);
        break;
    }

    SetBold(DefColSorts[anUS].is_svod <= 2);

    for (wnUS = anUS - 1; wnUS >= 1; wnUS--) {
        if ((DefColSorts[wnUS].is_sort == 1) && (DefColSorts[wnUS].is_svod != 0)) break;
        if (DefColSorts[wnUS].is_sort == 0) continue;

        NCol = DefColSorts[wnUS].NCol;
        FormLogicF1->f1Forms->Selection_Set(aRow + 1, NCol, aRow + 1, NCol);
        isType = FormLogicF1->f1Forms->Type;
        FormLogicF1->f1Forms->Selection_Set(aRow, NCol, aRow, NCol);

        switch (isType) {
        case 1:
            FormLogicF1->f1Forms->Number = FormLogicF1->f1Forms->NumberRC(aRow + 1, NCol);
            break;
        case 2:
            FormLogicF1->f1Forms->Text = FormLogicF1->f1Forms->TextRC(aRow + 1, NCol);
            break;
        default:
            FormLogicF1->f1Forms->Text = FormLogicF1->f1Forms->TextRC(aRow + 1, NCol);
            break;
        }

        SetBold(DefColSorts[anUS].is_svod <= 2);
    }

    if (DefColSorts[anUS].is_svod >= 3) {
        if (GetLicScet(anUS, aRow, wCol) != "") {
            FormLogicF1->f1Forms->Selection_Get(0, pRow1, pCol1, pRow2, pCol2);
            FormLogicF1->f1Forms->Selection_Set(aRow + 1, wCol, aRow + 1, wCol);
            cf.GetCellFormat(*FormLogicF1->f1Forms);
            FormLogicF1->f1Forms->Selection_Set(aRow, wCol, aRow, wCol);
            cf.SetCellFormat(*FormLogicF1->f1Forms);
            FormLogicF1->f1Forms->TextRC(aRow,wCol) = FormLogicF1->f1Forms->TextRC(aRow + 1,wCol);
            FormLogicF1->f1Forms->Selection_Set(pRow1, pCol1, pRow2, pCol2);
        }
        if (GetBKCode(anUS, aRow, wCol) != "")
            FormLogicF1->f1Forms->TextRC(aRow,wCol) = FormLogicF1->f1Forms->TextRC(aRow + 1,wCol);
    }

    Perenumerovati(aRow, FormLogicF1->f1Forms->LastRow);
}

bool TFormSorted::Sravnim(int anUS, int aRow) {
    std::string s0, s1;
    int nCol, wnUS;

    // сравниваем по всем ключам, начиная с начала
    for (wnUS = 1; wnUS <= anUS; wnUS++) {
        if ((DefColSorts[wnUS-1].is_sort == 0) && (DefColSorts[wnUS-1].is_svod == 0))
            continue; // ???
        //??if ((DefColSorts[wnUS-1].is_sort == 1) && (DefColSorts[wnUS-1].is_svod == 0))
        //    continue;
        // если строка в сортировке, независимо от свода она влияет на смену группы
        nCol = std::abs(DefColSorts[wnUS-1].NCol);
        s0 = char16ToUTF8(FormLogicF1->f1Forms->TextRC(aRow,nCol-1));
        s1 = char16ToUTF8(FormLogicF1->f1Forms->TextRC(aRow-1,nCol-1));
        if (s0 != s1) {
            return false;
        }
    }
    return true;
}
















































