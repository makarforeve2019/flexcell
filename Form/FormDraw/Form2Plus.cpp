#include "../SystFuncs/SystFuncsLogic.h"
#include "../DB/DB_FormStr.h"
#include "../DB/DB_ComonNPokaz.h"
#include "../DB/DB_DateUtvPlans.h"
#include "../DB/DB_BPolucRaj.h"
#include "../DB/DB_BPoluc.h"
#include "../DB/DB_Pokaztl.h"
#include "../DB/DB_TmpPokaztl.h"
#include "../DB/DB_Plan.h"
#include "../DB/DB_CellValue.h"
#include "../DB/DB_Dgvr.h"
#include "../DB/DB_Cell.h"
#include "../DB/DB_Enum.h"
#include "../DB/DB_EdIzm.h"
#include "../DB/DB_DenegnPotok.h"
#include "../DB/DB_KeyOfObjects.h"
#include "../DB/DB_bkKBK.h"
#include "../DB/DB_bkMetaDef.h"
#include "../DB/unDefDBconnect.h"
#include "unTF1CellFormat.h"
#include "Form1.h"
#include "Form2.h"
#include "FormSsilkaNaStr.h"
#include "unDocumentValueLogic.h"
#include "Form2Plus.h"

int LUnc = 0;

std::string TForm2Plus::DefaultFormat(int EdIzmInColumn) {
    switch (EdIzmInColumn) {
        case 0:
            return TForm2Plus::CreateNumberFormat(3, 0); // до копеек
        case 1:
            return TForm2Plus::CreateNumberFormat(3, 0); // до копеек
        case 2:
            return TForm2Plus::CreateNumberFormat(2, 0); // до тысяча
        case 3:
            return TForm2Plus::CreateNumberFormat(2, 0); // до миллион
        case 4:
            return TForm2Plus::CreateNumberFormat(2, 0); // до миллиарда
        default:
            return "";
    }
};

std::string TForm2Plus::CreateNumberFormat(int TocnostInForm, int EdIzmInColumn) {
// TocnostInForm = 0 - по умолчанию, 1 - целое, 2 - с точночтью до ,1 и т.д.
// EdIzmInColumn = 0 - как у показателя, 1 - в минимальных единицах, 2 - тысяча, 3 - миллион и т.д.
    const std::string shablon0 = "# ### ### ##0";

    const std::string toRed = ";[Red]-";
    std::string resFormat;
    std::string drop;

    switch (TocnostInForm) {
        case 0:
            resFormat = DefaultFormat(EdIzmInColumn);
            break;
        case 1:
            resFormat = shablon0 + toRed + shablon0;
            break;
        default:
            drop = ".0";
            if (TocnostInForm > 2 && TocnostInForm <= 10) {
                drop += DupeString("#", TocnostInForm - 2);
            } else if (TocnostInForm > 10) {
                drop += DupeString("0", TocnostInForm - 10);
            }
            resFormat = shablon0 + drop + toRed + shablon0 + drop;
            break;
    }

    return resFormat;
}

// установка "не чтения" для утвержд фактич данных
void TForm2Plus::SetFactNoVvod(TFormLogicF1 *FormLogicF1) {
    std::vector<bool> aa(50, false);
    int i, dup_id;
    TwwStringList *p = TFormDopInp::GetTww(FormLogicF1);
    bool isB = false;
    for (i = 1; i <= p->MaxHrCol; i++) {
        if (i > 50) break;
        if ((p->ListStmnt[i].vid_otch == 3) || ((p->ListStmnt[i].vid_otch == 0) && (FormLogicF1->CurrVidOtch == 3))) {
            if (p->ListStmnt[i].read_only) continue;
            if (p->ListStmnt[i].is_hidden == 1) continue;
            if (!((p->ListStmnt[i].Stmnt ==
                   "ЗНАЧЕНИЕСВ") /* (p->ListStmnt[i].Stmnt == 'ЗНАЧЕНИЕНВ') был заменен */))
                continue;
            aa[i - 1] = true; // кандитат на проверку
            isB = true;
        }
    }
    if (!isB /* нет ни одного кaндидата */) return;
    // есть хотя бы один кaндидат
    int root_Pokaz_id, root_Pokaz_is_razd, root_Pokaz_type, root_Pokaz_vert;
    std::string root_Pokaz_name;
    TFormDopInp::GetRootPokaztlForm(FormLogicF1, root_Pokaz_id, root_Pokaz_name, root_Pokaz_is_razd, root_Pokaz_type,
                                    root_Pokaz_vert);
    if (root_Pokaz_id == 0) return;
    dup_id = TDB_DateUtvPlans::FindItem(FormLogicF1->CurrPlanId, root_Pokaz_id, FormLogicF1->CurrYear);
    if (dup_id == 0) return;
    int pRow1, pCol1, pRow2, pCol2;
    FormLogicF1->f1Forms->Selection_Get(0, pRow1, pCol1, pRow2, pCol2);
    for (i = 0; i < TFormDopInp::GetCellValueAtrCount(FormLogicF1); i++) {
        PCellValueAtr vd = std::get<TCellValueAtr *>(p->Objects[i]);
        if (vd == nullptr) continue;
        if (vd->Col > 50) continue;
        if (YearOf(vd->ValueDate) != FormLogicF1->CurrYear) continue;
        if (aa[vd->Col - 1]) {
            FormLogicF1->f1Forms->Selection_Set(vd->Row, vd->Col, vd->Row, vd->Col);
            FormLogicF1->f1Forms->Protection_Set(true, true);
            p->ListStmnt[vd->Col].read_only = true;
        }
    }
    FormLogicF1->f1Forms->Selection_Set(pRow1, pCol1, pRow2, pCol2);
}

// установка "только чтение" для формы "только чтение по условию"
void TForm2Plus::SetReadOnlyByCondition(TFormLogicF1 *FormLogicF1) {
    int i, j, dup_id;
    PAtrRow AtrRow;
    TwwStringList *p = TFormDopInp::GetTww(FormLogicF1);
    PCellValueAtr vd;
    int root_Pokaz_id, root_Pokaz_is_razd, root_Pokaz_type, root_Pokaz_vert;
    std::string root_Pokaz_name;
    int pRow1, pCol1, pRow2, pCol2;
    bool pLocked, pHidden;
    int plan_id, god, month;
    TDateTime date_utv;

    FormLogicF1->f1Forms->Selection_Get(0, pRow1, pCol1, pRow2, pCol2);
    int LastRow = FormLogicF1->f1Forms->LastRow_Get();
    for (i = 1; i <= LastRow; i++) {
        if (FormLogicF1->f1Forms->RowHidden_Get(i)) continue;
        AtrRow = TFormDopInp::GetListRowsItem(FormLogicF1, i);
        if (AtrRow == nullptr) continue;
        // 12{агрегат}, 13{кортеж}
        if (int N = AtrRow->CurRowVid; !(N == 2 || N == 4 || N == 12 || N == 13)) continue;

        for (j = 1; j <= p->MaxHrCol; j++) {
            if (!(p->ListStmnt[j].Stmnt == "ЗНАЧЕНИЕСВ")) continue;
            if (p->ListStmnt[j].read_only) continue;
            if (FormLogicF1->f1Forms->ColHidden_Get(j)) continue;
            vd = TFormDopInp::GetCellValueAtrByRC(FormLogicF1, i, j);
            if (vd == nullptr) continue;
            FormLogicF1->f1Forms->Selection_Set(vd->Row, vd->Col, vd->Row, vd->Col);
            FormLogicF1->f1Forms->Protection_Get(pLocked, pHidden);
            if (pLocked) continue;
            if (p->ListStmnt[j].PeriodType != 6) continue;

            TDB_Pokaztl::GetRootPokaztl(vd->PokazId, root_Pokaz_id, root_Pokaz_name, root_Pokaz_is_razd,
                                        root_Pokaz_type, root_Pokaz_vert);
            if (root_Pokaz_id == 0) continue;

            plan_id = p->ListStmnt[j].plan_id;
            if (plan_id == 0) plan_id = FormLogicF1->CurrPlanId;
            god = p->ListStmnt[j].god_ravno;
            if (god == 0) god = FormLogicF1->CurrYear;
            month = p->ListStmnt[j].ValuePeriod;
            if (month == 0) month = FormLogicF1->CurrMonth;

            dup_id = TDB_DateUtvPlans::FindItem1(plan_id, root_Pokaz_id, god, month, date_utv);
            if (dup_id == 0) continue;

            if (Date() > date_utv) FormLogicF1->f1Forms->Protection_Set(true, true);
        }
    }

    FormLogicF1->f1Forms->Selection_Set(pRow1, pCol1, pRow2, pCol2);
}

// установка "только чтение" для формы оператором формулой ::Тест на "Только чтение"
void TForm2Plus::SetReadOnlyByStmntTest(TFormLogicF1 *FormLogicF1) {
    TwwStringList *p = TFormDopInp::GetTww(FormLogicF1);
    int i, j, wCol;
    PAtrRow AtrRow;
    PCellValueAtr vd;
    bool pLocked, pHidden;
    int pRow1, pCol1, pRow2, pCol2;

    if (p->nRowOfStmntTestReadOnly == 0)
        return;

    for (wCol = 1; wCol <= FormLogicF1->f1Forms->LastCol; wCol++) {
        switch (FormLogicF1->f1Forms->TypeRC_Get(p->nRowOfStmntTestReadOnly, wCol)) {
            case -1:
            case -2:
            case -3: // в ячейке есть формула
                if (FormLogicF1->f1Forms->NumberRC_Get(p->nRowOfStmntTestReadOnly, wCol) == 0) {
                    p->isStmntTestReadOnly = false;
                    return;
                }
                break;
        }
    }
    p->isStmntTestReadOnly = true;
    FormLogicF1->f1Forms->Selection_Get(0, pRow1, pCol1, pRow2, pCol2);
    int LastRow = FormLogicF1->f1Forms->LastRow_Get();
    for (i = 1; i <= LastRow; i++) {
        if (FormLogicF1->f1Forms->RowHidden_Get(i)) continue;
        AtrRow = TFormDopInp::GetListRowsItem(FormLogicF1, i);
        if (AtrRow == nullptr) continue;
        if (int N = AtrRow->CurRowVid; !(N == 2 || N == 4 || N == 12 || N == 13)) continue;

        for (j = 1; j <= FormLogicF1->f1Forms->LastCol; j++) {
            if (p->ListStmnt[j].read_only) continue;
            if (FormLogicF1->f1Forms->ColHidden_Get(j)) continue;
            vd = TFormDopInp::GetCellValueAtrByRC(FormLogicF1, i, j);
            if (vd == nullptr) continue;
            FormLogicF1->f1Forms->Selection_Set(vd->Row, vd->Col, vd->Row, vd->Col);
            FormLogicF1->f1Forms->Protection_Get(pLocked, pHidden);
            if (pLocked) continue;
            FormLogicF1->f1Forms->Protection_Set(true, true);
        }
    }

    for (wCol = 1; wCol <= FormLogicF1->f1Forms->LastCol; wCol++)
        p->ListStmnt[wCol].read_only = true;

    FormLogicF1->f1Forms->Selection_Set(pRow1, pCol1, pRow2, pCol2);
}

// установка "заблокировано" для ячеек, куда можно вводить для формы с сохранением в Log истории изменений
void TForm2Plus::SetReadOnlyForLog(TFormLogicF1 *FormLogicF1) {
    TwwStringList *p = TFormDopInp::GetTww(FormLogicF1);
    int pRow1, pCol1, pRow2, pCol2;

    FormLogicF1->f1Forms->Selection_Get(0, pRow1, pCol1, pRow2, pCol2);
    int LastRow = FormLogicF1->f1Forms->LastRow_Get();
    for (int i = 1; i <= LastRow; i++) {
        if (FormLogicF1->f1Forms->RowHidden_Get(i)) continue;
        PAtrRow AtrRow = TFormDopInp::GetListRowsItem(FormLogicF1, i);
        if (AtrRow == nullptr) continue;
        if (int N = AtrRow->CurRowVid; !(N == 2 || N == 4 || N == 12 || N == 13))
            continue;

        for (int j = 1; j <= FormLogicF1->f1Forms->LastCol; j++) {
            if (p->ListStmnt[j].read_only) continue;
            if (FormLogicF1->f1Forms->ColHidden_Get(j)) continue;
            PCellValueAtr vd = TFormDopInp::GetCellValueAtrByRC(FormLogicF1, i, j);
            if (vd == nullptr) continue;
            FormLogicF1->f1Forms->Selection_Set(vd->Row, vd->Col, vd->Row, vd->Col);
            bool pLocked, pHidden;
            FormLogicF1->f1Forms->Protection_Get(pLocked, pHidden);
            if (pLocked) continue;
            FormLogicF1->f1Forms->Protection_Set(true, true);
        }
    }

    // Этот фрагмент исключил. Нужно ли? Проверить потом!
    /*for (int wCol = 1; wCol <= FormLogicF1.f1Forms.LastCol; wCol++)
        p[wCol - 1].read_only = true;*/

    FormLogicF1->f1Forms->Selection_Set(pRow1, pCol1, pRow2, pCol2);
}

// устанавливает формат одного столбца
void TForm2Plus::SetNumberFormats1(TFormLogicF1 *FormLogicF1, int aRow, int aCol, TwwStringList *p) {


    int tocnost = 0;
//    TwwStringList * p = TFormDopInp::GetTww(FormLogicF1);

    PAtrRow AtrRow = TFormDopInp::GetListRowsItem(FormLogicF1, aRow);
    if (AtrRow != nullptr) {
        tocnost = AtrRow->tocnost;
    }
    if (tocnost == 0) {
        tocnost = p->ListStmnt[aCol].tocnost;
    }
    if (tocnost == 0) {
        tocnost = p->tocnost;
    }
    std::string nf = TForm2Plus::CreateNumberFormat(tocnost, p->ListStmnt[aCol].CoefPlus1); // какая ед измерения
    try {
        FormLogicF1->f1Forms->Selection_Set(aRow, aCol, aRow, aCol);
        FormLogicF1->f1Forms->NumberFormat = utf8_to_utf16(nf).c_str();
    } catch (exception &e) {
        // Ignore exception
        TracerManager::Instance().AddSpanLog("Exception.TForm2Plus::SetNumberFormats1", {{"errordesr", e.what()}},
                                             TracerManager::LOG_LEVEL_ERROR);
    }
}

void TForm2Plus::SetNumberFormats_ReadOnlyCond(TFormLogicF1 *FormLogicF1) {
    TwwStringList *p = TFormDopInp::GetTww(FormLogicF1);
    int BgnRow = p->form_k_zagtop + p->form_k1;
    int R1, C1, R2, C2;
    int tocnost;

    /*procedure doEvalCellReadOnly;
     var
       wRow, wn: integer;
     begin
       for wRow := p->form_begBodyRow to p->form_endBodyRow do begin
         wn:= p->ListStmnt[n].ColOnFilter2 {n+1};
         if wn <= FormLogicF1.f1Forms.LastCol then
           if FormLogicF1.f1Forms.NumberRC_Get(wRow, wn) > 0
           then begin
             FormLogicF1.f1Forms.Selection_Set(wRow,n,wRow,n);
             FormLogicF1.f1Forms.Protection_Set(true, true);
           end;
       end;
     end;*/

    FormLogicF1->f1Forms->Selection_Get(0, R1, C1, R2, C2);
    int LastRow = FormLogicF1->f1Forms->LastRow_Get();
    for (int n = 1; n <= FormLogicF1->f1Forms->LastCol; n++) {
        if (n == TFormDopInp::GetNameCol(FormLogicF1)) continue;

        if (p->ListStmnt[n].readonly == 2) {
            // doEvalCellReadOnly
            // подставляем ниже doEvalCellReadOnly
            // установить блокировку ячеек по условию
            for (int wRow = p->form_begBodyRow; wRow <= p->form_endBodyRow; wRow++) {
                int wn = p->ListStmnt[n].ColOnFilter2;
                if (wn <= FormLogicF1->f1Forms->LastCol && FormLogicF1->f1Forms->NumberRC_Get(wRow, wn) > 0) {
                    FormLogicF1->f1Forms->Selection_Set(wRow, n, wRow, n);
                    FormLogicF1->f1Forms->Protection_Set(true, true);
                }
            }
        }
        if (p->ListStmnt[n].ist_fin_grad_id > 0) {
            tocnost = p->ListStmnt[n].tocnost;
            if (tocnost == 0) tocnost = p->tocnost;
            std::string nf = TForm2Plus::CreateNumberFormat(tocnost, p->ListStmnt[n].CoefPlus1);
            try {
                FormLogicF1->f1Forms->Selection_Set(BgnRow + 1, n, LastRow, n);
                FormLogicF1->f1Forms->NumberFormat_Set(utf8_to_utf16(nf).c_str());
            } catch (exception &e) {
                TracerManager::Instance().AddSpanLog("Exception.TForm2Plus::SetNumberFormats_ReadOnlyCond", {{"errordesr", e.what()}},
                                                     TracerManager::LOG_LEVEL_ERROR);
            }
        } else if (p->ListStmnt[n].Stmnt == "ФОРМУЛАF1" || p->ListStmnt[n].Stmnt == "ПРОИЗВСТОЛБ" ||
                   p->ListStmnt[n].Stmnt == "ЗНАЧЕНИЕобщНП" || p->ListStmnt[n].Stmnt == "ЗНАЧЕНИЕСВ" ||
                   p->ListStmnt[n].Stmnt == "ЗНАЧЕНИЕСВНИ" || p->ListStmnt[n].Stmnt == "ЗНАЧЕНИЕСВИР" ||
                   p->ListStmnt[n].Stmnt == "СВОДпоСУБЪвАльб" || p->ListStmnt[n].Stmnt == "СВОДпоСУБЪвА/n" ||
                   p->ListStmnt[n].Stmnt == "СВОДобщНПпоСУБЪвАльб" || p->ListStmnt[n].Stmnt == "СВОДобщНПпоСУБЪвА/n") {
            if (p->ListStmnt[n].Stmnt == "ЗНАЧЕНИЕСВ") {
                PCellValueAtr vd = TFormDopInp::GetCellValueAtrByRC(FormLogicF1, BgnRow + 1, n);
                if (vd != nullptr && vd->pokaz_type == 2) continue;
            }
            tocnost = p->ListStmnt[n].tocnost;
            if (tocnost == 0) tocnost = p->tocnost;
            if (tocnost == 10) continue; // целое число без форматирования 29.04.2022
            std::string nf = TForm2Plus::CreateNumberFormat(tocnost, p->ListStmnt[n].CoefPlus1);
            try {
                FormLogicF1->f1Forms->Selection_Set(BgnRow + 1, n, LastRow, n);
                FormLogicF1->f1Forms->NumberFormat_Set(utf8_to_utf16(nf).c_str());
            } catch (exception &e) {
                TracerManager::Instance().AddSpanLog("Exception.TForm2Plus::SetNumberFormats_ReadOnlyCond", {{"errordesr", e.what()}},
                                                     TracerManager::LOG_LEVEL_ERROR);
            }
        }
    }

    try {
        TForm2Plus::SetNumberFormatsToStr(FormLogicF1);
    } catch (exception &e) {
        TracerManager::Instance().AddSpanLog("Exception.TForm2Plus::SetNumberFormats_ReadOnlyCond", {{"errordesr", e.what()}},
                                             TracerManager::LOG_LEVEL_ERROR);
    }

    FormLogicF1->f1Forms->Selection_Set(R1, C1, R2, C2);
}

// устанавливает формат строк, когда это задано явно
void TForm2Plus::SetNumberFormatsToStr(TFormLogicF1 *FormLogicF1) {
    int i, j;
    PAtrRow AtrRow;
    std::string nf;
    int LastRow = FormLogicF1->f1Forms->LastRow_Get();
    for (i = 1; i <= LastRow; i++) {
        if (FormLogicF1->f1Forms->RowHidden_Get(i)) continue;
        AtrRow = TFormDopInp::GetListRowsItem(FormLogicF1, i);
        if (AtrRow == nullptr) continue;
        if (AtrRow->tocnost == 0) continue;
        nf = TForm2Plus::CreateNumberFormat(AtrRow->tocnost, 0); // какая ед измерения

        for (j = 1; j <= FormLogicF1->f1Forms->LastCol; j++) {
            FormLogicF1->f1Forms->Selection_Set(i, j, i, j);
            if (FormLogicF1->f1Forms->Type_Get() == -1) { // формула
                try {
                    FormLogicF1->f1Forms->NumberFormat_Set(utf8_to_utf16(nf).c_str());
                } catch (exception &e) {
                    // Handle exception
                    TracerManager::Instance().AddSpanLog("Exception.TForm2Plus::SetNumberFormatsToStr", {{"errordesr", e.what()}},
                                                         TracerManager::LOG_LEVEL_ERROR);
                }
            }
        }
    }
}

// скрыть пустые разделы
void TForm2Plus::HiddenEmptyRazdel(TFormLogicF1 *FormLogicF1, int is_dublir_itogo) {
    PAtrRow AtrRow;
    bool isRasdel = false;
    for (int i = 1; i <= FormLogicF1->f1Forms->MaxCol; i++) {
        if (FormLogicF1->f1Forms->RowHidden_Get(i)) continue;
        AtrRow = TFormDopInp::GetListRowsItem(FormLogicF1, i);
        if (AtrRow == nullptr) continue;
        if (AtrRow->CurRowVid == 3 && isRasdel) {
            FormLogicF1->f1Forms->RowHidden_Set(i, true);
            FormLogicF1->f1Forms->RowHidden_Set(AtrRow->OwnerNomRow, true);
        }
        if (AtrRow->CurRowVid == 1) {
            isRasdel = true;
        } else {
            isRasdel = false;
        };
    }
    if (is_dublir_itogo == 3) {
        // просто скрыть все итого по разделу
        for (int i = 1; i <= FormLogicF1->f1Forms->MaxCol; i++) {
            if (FormLogicF1->f1Forms->RowHidden_Get(i)) continue;
            AtrRow = TFormDopInp::GetListRowsItem(FormLogicF1, i);
            if (AtrRow == nullptr) continue;
            if (AtrRow->CurRowVid == 3)
                FormLogicF1->f1Forms->RowHidden_Set(i, true);
        }
    }
}

// делает столбцы невидимыми
void TForm2Plus::SetColHidden(TFormLogicF1 *FormLogicF1) {
    TwwStringList *p = TFormDopInp::GetTww(FormLogicF1);
    for (int i = 1; i <= p->MaxHrCol; i++) {// 01.04.2019 - еще не вычислен MaxCol
        if (p->ListStmnt[i].is_hidden == 1)
            FormLogicF1->f1Forms->ColHidden_Set(i, true);
    }
}

// в таблицу f1Forms перед наименованием прописывает идентификацию строки
void TForm2Plus::SetIdRow(TFormLogicF1 *FormLogicF1, int PokazCol, int BeginRow) {
    std::string Letter = "а)б)в)г)д)е)ж)з)и)к)л)м)н)о)п)р)с)т)у)ф)х)ц)ч)ш)";
    int NameCol = PokazCol;
    int CurRow = BeginRow + 1;

    std::function<void(int, std::string)> ReDraw = [&](int OwnerNomRow, std::string SNum) {
        int inum = 1;
        std::string S;
        std::string s_;
        while (CurRow <= FormLogicF1->f1Forms->MaxCol) {
            if (FormLogicF1->f1Forms->RowHidden_Get(CurRow)) {
                CurRow++;
                continue;
            }
            PAtrRow AtrRow = TFormDopInp::GetListRowsItem(FormLogicF1, CurRow);
            if (AtrRow == nullptr) {
                CurRow++;
                continue;
            }
            if (AtrRow->CurRowVid == 3 || AtrRow->CurRowVid == 5) {
                CurRow++;
                continue;
            }
            if (AtrRow->OwnerNomRow == OwnerNomRow) {
                S = "";
                s_ = "";
                switch (AtrRow->level_sign) {
                    case 1:
                        S = std::to_string(inum);
                        S = SNum + S + ".";
                        s_ = " ";
                        inum++;
                        break;
                    case 2:
                        S = Letter.substr(2 * inum - 1, 2);
                        s_ = " ";
                        inum++;
                        break;
                    case 3:
                        S = " - ";
                        break;
                    case 4:
                        S = "";
                        if (inum > 1)
                            inum = 1;
                        break;
                    case 5:
                        S = TDB_Pokaztl::GetLevelText(AtrRow->ObjectId);
                        if (S != "") {
                            S = S + ".";
                        }

                        s_ = " ";
                        break;
                }
                std::string ws2 = S + s_ + char16ToUTF8(FormLogicF1->f1Forms->TextRC_Get(CurRow, NameCol));
                FormLogicF1->f1Forms->TextRC_Set(CurRow, NameCol, utf8_to_utf16(ws2).c_str());
                AtrRow->L = S.length() + s_.length();
                CurRow++;
                continue;
            }
            if (AtrRow->OwnerNomRow > OwnerNomRow) {
                ReDraw(AtrRow->OwnerNomRow, S);
                continue;
            }
            if (AtrRow->OwnerNomRow < OwnerNomRow) return;
        }
    };
    ReDraw(BeginRow, "");
}

// сводная строка
void TForm2Plus::SvodForm(TFormLogicF1 *FormLogicF1) {
    TwwStringList *p = TFormDopInp::GetTww(FormLogicF1);
    FormLogicF1->CurrInputRow = TF1toDE::CreateNewRowCells(p, FormLogicF1);
    TFormDopInp::ListRowsSetAtrRow(FormLogicF1, FormLogicF1->CurrInputRow, true, 0, 0, 0);
    TFormDopInp::SetListRowsItemAtr(FormLogicF1, FormLogicF1->CurrInputRow, 9, 0, 0, 0, 0, 0, "");
    int Is_bold = 0x01;
    TFormDopInp::SetFormatStyle(FormLogicF1, FormLogicF1->CurrInputRow, TFormatStyle("", 0, Is_bold, 0, 0));
    TForm2::SetTextNameCol(FormLogicF1, FormLogicF1->CurrInputRow,
                           FormLogicF1->quForms->FieldByName("name_svod")->AsString(), 0);
    std::string s = FormLogicF1->quForms->FieldByName("formula_svod")->AsString();
    int NCol = 0;
    std::string ColFormula;

    std::function<void()> SetFormula = [&]() {
        TFormatStyle *FormatStyle = new TFormatStyle();
        TFormDopInp::getColFrmtrAtr(FormLogicF1, 1, NCol, *FormatStyle);
        std::string sF = TFormDopInp::ReplaceNRowNCol(FormLogicF1->CurrInputRow, NCol, ColFormula);
        TForm1::SetCellValue(FormLogicF1, FormLogicF1->f1Forms, p->MaxHrCol, FormLogicF1->CurrInputRow, NCol,
                             FormLogicF1->CurrInputRow, NCol, sF, 1, FormatStyle);
    };

    s = TFormDopInpMF::GetNextFormula(s, NCol, ColFormula);
    while (s != "") {
        if (NCol > 0 && NCol <= FormLogicF1->f1Forms->MaxCol) {
            SetFormula();
        }
        s = TFormDopInpMF::GetNextFormula(s, NCol, ColFormula);
    }
    if (NCol > 0 && NCol <= FormLogicF1->f1Forms->MaxCol) {
        SetFormula();
    }
    switch (FormLogicF1->quForms->FieldByName("work_with_gf")->AsInteger()) {
        case 10: //{составление графика финансирования}
        case 11: //{фактическое финансирование}
        case 12:
        case 13:
        case 14:
            if (NCol > p->InitKolCol) {
                for (NCol = NCol + 1; NCol <= FormLogicF1->f1Forms->MaxCol; NCol++) {
                    SetFormula();
                }
            }
            break;
    }
}


void TForm2Plus::SetNumberPP(TFormLogicF1 *FormLogicF1, int BegRow, int EndRow) {
    TwwStringList *p = TFormDopInp::GetTww(FormLogicF1);
    int ColNPP = 0;
    for (int i = 1; i <= FormLogicF1->f1Forms->LastCol; i++) {
        if (p->ListStmnt[i].Stmnt == "№п/п") {
            ColNPP = i;
            break;
        }
    }
    if (ColNPP == 0) return;
    int NPP = 0;
    TFormatStyle FormatStyle;
    TFormDopInp::getColFrmtrAtr(FormLogicF1, 0, ColNPP, FormatStyle);
    int NumberPP = p->ListStmnt[ColNPP].NumberPP;
    for (int i = BegRow; i <= EndRow; i++) {
        if (FormLogicF1->f1Forms->RowHidden_Get(i)) continue;
        PAtrRow AtrRow = TFormDopInp::GetListRowsItem(FormLogicF1, i);
        if (AtrRow == nullptr) continue;
        switch (AtrRow->CurRowVid) {
            case 1:
            case 10: // раздел
                NPP = 0;
                continue; // в случае традиционного бюджета отсутствуют явные разделы
            case 2:
            case 4:
            case 12:
            case 13: // показатель, кортеж
                NPP++;
                std::string sNPP = std::to_string(NPP);
                if (NumberPP == 2) {
                    sNPP += ".";
                }
                TForm1::SetCellValue(FormLogicF1, FormLogicF1->f1Forms, p->MaxHrCol, i, ColNPP, i, ColNPP, sNPP, 0,
                                     &FormatStyle);
                FormLogicF1->f1Forms->Selection_Set(i, ColNPP, i, ColNPP);
                FormLogicF1->f1Forms->Protection_Set(true, true);
                break;
        }
    }
}

// заменяет вычисляемый год в строке (все)
// формат такой {Г} или {Г+nn} {Г-nn}
// формат такой {Г2} - последние 2 цифры года
// формат такой {Р} - имя района
// формат такой {Рк} - краткое имя района
// формат такой {Роив} - наименование органа исполнительной власти района
// формат такой {РС1} - расчетный(получательный) счет района
// формат такой {РС2} - расчетный(распределительный) счет района
// формат такой {РС3} - номер получательного счета района для ТБ
// формат такой {РС4} - номер распределительного счета района для ТБ
// формат такой {БП} - краткое наименование бюджетополучателя для района
// формат такой {БПп} - полное наименование бюджетополучателя для района
std::string TForm2Plus::ZamenitGod(TFormLogicF1 *FormLogicF1, const std::string CurrPlanName, int CurrYear,
                                   const std::string stroka) {
    u16string s0, s1;
    s1 = utf8_to_utf16(stroka);
    while (true) {
        s0 = s1;
        try {
            s1 = TForm2Plus::ZamenitGod1(FormLogicF1, utf8_to_utf16(CurrPlanName), CurrYear, s0);
        }
        catch (const std::exception &e) {
            //throw std::runtime_error("Ошибка при преобразовании параметров в строке " + s1 + "\n" + "По-видимому не выполнено надлежащее сохранение описания формы");
            TracerManager::Instance().AddSpanLog("Exception.TForm2Plus::ZamenitGod.По-видимому не выполнено надлежащее сохранение описания формы", {{"errordesr", e.what()}},
                                                 TracerManager::LOG_LEVEL_ERROR);
        }
        if (s0 == s1) {
            return utf16_to_utf8(s1);
        }
    }
}

// заменяет вычисляемый год, квартал, месяц в строке
// формат такой {Г} или {Г+nn} {Г-nn}
// формат такой {Г2} - последние 2 цифры года
// формат такой {К} или {К+nn} {К-nn} - квартал
// формат такой {М} или {М+nn} {М-nn} - месяц
// формат такой {НМ} - начало месяца
// формат такой {КМ} - конец месяца
// формат такой {И} - итоговый месяц, квартал, полугодие
// формат такой {Д} - текущая дата
//std::string TForm2Plus::ZamenitGod1(TFormLogicF1 * FormLogicF1, std::string CurrPlanName, int CurrYear, std::string stroka) {
//    int pos1, pos2, d;
//    std::string strgod, strkv, strmes, stri, ws;
//    int kv, mes, god;
//    TDateTime ToDate;
//    std::string bduserName;
//    std::string bduserINN, bduserKPP;
//    std::string bduserScet;
//
//    auto GetD = [&](int aOffset, int aPos1, int& aPos2, int& aD) -> bool {
//        aPos2 = stroka.find('}', aPos1);
//        if (aPos2 == std::string::npos) return false;
//        if (aPos2 < aPos1) return false;
//        if (stroka[aPos1 + aOffset] == '}') {
//            aD = 0;
//        } else {
//            if (stroka[aPos1 + aOffset] != '+' && stroka[aPos1 + aOffset] != '-') return false;
//            aD = std::stoi(stroka.substr(aPos1 + aOffset + 1, aPos2 - aPos1 - aOffset - 1));
//            if (stroka[aPos1 + aOffset] == '+') {
//                aD = aD;
//            } else {
//                aD = -aD;
//            }
//        }
//        return true;
//    };
//
//    auto GetStrKv = [](int aKv) -> std::string {
//        if (aKv < 1) aKv = 1;
//        if (aKv > 4) aKv = 4;
//        switch (aKv) {
//        case 1: return "I";
//        case 2: return "II";
//        case 3: return "III";
//        case 4: return "IV";
//        default: return "";
//        }
//    };
//    string searchString = "{Р}";
//    pos1 = stroka.find("{Р}"); // имя района
//    size_t startPos = stroka.find(searchString);
//    if (pos1 != std::string::npos) {
////        stroka = Copy(stroka,1,pos1-1)+CurrPlanName+Copy(stroka,pos1+3, stroka.length());
//        size_t endPos = startPos + searchString.length();
//        stroka.replace(startPos, endPos - startPos, CurrPlanName);
//
//    }
//
//    pos1 = stroka.find("{Рк}"); // краткое имя самого текущего района
//    if (pos1 != std::string::npos) {
//        ws = TDB_Plan::GetPlanShortName(FormLogicF1->CurrPlanId);
//        stroka = stroka.substr(0, pos1) + ws + stroka.substr(pos1 + 4);
//    }
//
//
//    pos1 = stroka.find("{Роив}"); // наименование органа исполнительной власти района
//    if (pos1 != std::string::npos) {
//        for (int i = 0; i < stroka.length(); ++i) {
//            unsigned char cr = stroka[i];
//            if (cr == 0){
//                std::cout << '!';
//            }
//            if (cr == '0'){
//                std::cout << '!';
//            }
//                std::cout << cr;
//        }
//        ws = TDB_Plan::getNameOIV(FormLogicF1->CurrPlanId);
//        std::string sub1 = stroka.substr(0, pos1);
//        int len = stroka.length();
//        std::string sub2 = stroka.substr(pos1 + 5, stroka.length());
//        std::string sub3 = stroka.substr(pos1, stroka.length());
//        stroka = sub1 + ws + sub2;
////        stroka = stroka.substr(0, pos1) + ws + stroka.substr(pos1 + 6);
////        if (stroka == "{Роив}") {
////            stroka = ws;
////        } else {
////            stroka = stroka.substr(0, pos1) + ws + stroka.substr(pos1 + 6);
////        }
//
//    }
//
//    pos1 = stroka.find("{БП}");  // наименование бюджетополучателя самого текущего района
//    if (pos1 != std::string::npos) {
//        TDB_BPolucRaj::getBP1forTB(FormLogicF1->CurrYear, FormLogicF1->CurrPlanId, bduserScet, bduserName, bduserINN);
//        bduserName = DelSkobka(bduserName);
//        stroka = stroka.substr(0, pos1) + bduserName + stroka.substr(pos1 + 4);
//    }
//
//    pos1 = stroka.find("{БПп}");  // полное наименование бюджетополучателя самого текущего района
//    if (pos1 != std::string::npos) {
//        TDB_BPolucRaj::getBP1forTBpolnoe(FormLogicF1->CurrYear, FormLogicF1->CurrPlanId, bduserScet, bduserName, bduserINN, bduserKPP);
//        bduserName = DelSkobka(bduserName);
//        stroka = stroka.substr(0, pos1) + bduserName + stroka.substr(pos1 + 5);
//    }
//
//    pos1 = stroka.find("{РС4}");  // номер распределительного счета района для ТБ
//    if (pos1 != std::string::npos) {
//        ws = TDB_BPolucRaj::RSRsp_GetBPbyPlanIdTB(FormLogicF1->CurrPlanId);
//        stroka = stroka.substr(0, pos1) + ws + stroka.substr(pos1 + 5);
//    }
//
//    pos1 = stroka.find("{РС3}");  // номер получательного счета района для ТБ
//    if (pos1 != std::string::npos) {
//    	ws = TDB_BPolucRaj::RSPlc_GetBPbyPlanIdTB(FormLogicF1->CurrYear, FormLogicF1->CurrPlanId);
//        stroka = stroka.substr(0, pos1) + ws + stroka.substr(pos1 + 5);
//    }
//
//    pos1 = stroka.find("{РС2}");  // номер счета района
//    if (pos1 != std::string::npos) {
//    	ws = TDB_BPoluc::GetRascScet2(FormLogicF1->CurrPlanId);
//        stroka = stroka.substr(0, pos1) + ws + stroka.substr(pos1 + 5);
//    }
//
//    pos1 = stroka.find("{РС1}");  // номер счета района
//    if (pos1 != std::string::npos) {
//    	ws = TDB_BPoluc::GetRascScet1(FormLogicF1->CurrPlanId);
//        stroka = stroka.substr(0, pos1) + ws + stroka.substr(pos1 + 5);
//    }
//
//    pos1 = stroka.find("{Д}");
//    if (pos1 != std::string::npos) {
//	    ToDate = FormLogicF1->CurrDate;
//	    if (FormLogicF1->CurrYear < YearOf(ToDate))
//	    ToDate = EncodeDate(FormLogicF1->CurrYear, 12, 31);
//	    ws =FormatDateTime("dd.mm.yy", ToDate);
//        stroka = stroka.substr(0, pos1) + ws + stroka.substr(pos1 + 3);
//    }
//
//    pos1 = stroka.find("{К");
//    if (pos1 != std::string::npos) {
//        if (GetD(2, pos1, pos2, d)) {// если ошибка, то пропускаем
//            kv = FormLogicF1->CurrQuarter + d;
//            strkv = GetStrKv(kv);
//            stroka = stroka.substr(0, pos1) + strkv + stroka.substr(pos2 + 1);
//        }
//    }
//
//    pos1 = stroka.find("{И}");
//    if (pos1 != std::string::npos) {
//        switch (FormLogicF1->CurrQuarter) {
//        case 1: stri = "I квартал"; break;
//        case 2: stri = "1-ое полугодие"; break;
//        case 3: stri = "9 месяцев"; break;
//        case 4: stri = "год"; break;
//        }
//        stroka = stroka.substr(0, pos1) + stri + stroka.substr(pos1 + 3);
//    }
//
//    pos1 = stroka.find("{НМ}");
//    if (pos1 != std::string::npos) {
//        ToDate = EncodeDate(FormLogicF1->CurrYear, FormLogicF1->CurrMonth, 01);
//        stri = FormatDateTime("dd.mm.yyyy", ToDate);
//        stroka = stroka.substr(0, pos1) + stri + stroka.substr(pos1 + 4);
//    }
//
//    pos1 = stroka.find("{КМ}");
//    if (pos1 != std::string::npos) {
//       ToDate = GetLastDayDate(FormLogicF1->CurrYear, 0, FormLogicF1->CurrMonth);
//       stri = FormatDateTime("dd.mm.yyyy", ToDate);
//        stroka = stroka.substr(0, pos1) + stri + stroka.substr(pos1 + 4);
//    }
//
//    pos1 = stroka.find("{М");
//    if (pos1 != std::string::npos) {
//        if (GetD(2, pos1, pos2, d)) {// если ошибка, то пропускаем
//            mes = FormLogicF1->CurrMonth + d;
//            if (mes < 1) mes = 1;
//            if (mes > 12) mes = 12;
//            switch (mes) {
//            case 1: strmes = "январь"; break;
//            case 2: strmes = "февраль"; break;
//            case 3: strmes = "март"; break;
//            case 4: strmes = "апрель"; break;
//            case 5: strmes = "май"; break;
//            case 6: strmes = "июнь"; break;
//            case 7: strmes = "июль"; break;
//            case 8: strmes = "август"; break;
//            case 9: strmes = "сентябрь"; break;
//            case 10: strmes = "октябрь"; break;
//            case 11: strmes = "ноябрь"; break;
//            case 12: strmes = "декабрь"; break;
//            }
//            stroka = stroka.substr(0, pos1) + strmes + stroka.substr(pos2 + 1);
//        }
//    }
//
//    pos1 = stroka.find("{Г");
//    if (pos1 != std::string::npos) {
//        if (GetD(2, pos1, pos2, d)) {// если ошибка, то пропускаем
//            god = CurrYear + d;
//            strgod = std::to_string(god);
//            stroka = stroka.substr(0, pos1) + strgod + stroka.substr(pos2 + 1);
//        }
//    }
//
//    pos1 = stroka.find("{Г2}");
//    if (pos1 != std::string::npos) {
//        strgod = std::to_string(CurrYear).substr(2, 2);
//        stroka = stroka.substr(0, pos1) + strgod + stroka.substr(pos1 + 4);
//    }
//
//    return stroka;
//}

u16string TForm2Plus::ZamenitGod1(TFormLogicF1 *FormLogicF1, u16string CurrPlanName, int CurrYear, u16string stroka) {
    int pos1, pos2, d;
    u16string strgod, strkv, strmes, stri, ws;
    int kv, mes, god;
    TDateTime ToDate;
    u16string bduserName;
    u16string bduserINN, bduserKPP;
    u16string bduserScet;

    auto GetD = [&](int aOffset, int aPos1, int &aPos2, int &aD) -> bool {
        aPos2 = stroka.find('}', aPos1);
        if (aPos2 == u16string::npos) return false;
        if (aPos2 < aPos1) return false;
        if (stroka[aPos1 + aOffset] == '}') {
            aD = 0;
        } else {
            if (stroka[aPos1 + aOffset] != '+' && stroka[aPos1 + aOffset] != '-') return false;
            aD = u16StringToInt(stroka.substr(aPos1 + aOffset + 1, aPos2 - aPos1 - aOffset - 1));
            if (stroka[aPos1 + aOffset] == '+') {
                aD = aD;
            } else {
                aD = -aD;
            }
        }
        return true;
    };

    auto GetStrKv = [](int aKv) -> u16string {
        if (aKv < 1) aKv = 1;
        if (aKv > 4) aKv = 4;
        switch (aKv) {
            case 1:
                return u"I";
            case 2:
                return u"II";
            case 3:
                return u"III";
            case 4:
                return u"IV";
            default:
                return u"";
        }
    };
    u16string searchString = u"{Р}";
    pos1 = stroka.find(u"{Р}"); // имя района
    size_t startPos = stroka.find(searchString);
    if (pos1 != u16string::npos) {
//        stroka = Copy(stroka,1,pos1-1)+CurrPlanName+Copy(stroka,pos1+3, stroka.length());
        size_t endPos = startPos + searchString.length();
        stroka.replace(startPos, endPos - startPos, CurrPlanName);

    }

    pos1 = stroka.find(u"{Рк}"); // краткое имя самого текущего района
    if (pos1 != u16string::npos) {
        ws = utf8_to_utf16(TDB_Plan::GetPlanShortName(FormLogicF1->CurrPlanId));
        stroka = stroka.substr(0, pos1) + ws + stroka.substr(pos1 + 4);
    }


    pos1 = stroka.find(u"{Роив}"); // наименование органа исполнительной власти района
    if (pos1 != u16string::npos) {
        ws = utf8_to_utf16(TDB_Plan::getNameOIV(FormLogicF1->CurrPlanId));
        u16string sub1 = stroka.substr(0, pos1);
        int len = stroka.length();
        u16string sub2 = stroka.substr(pos1 + 6, stroka.length());
        u16string sub3 = stroka.substr(pos1, stroka.length());
        stroka = sub1 + ws + sub2;
//        stroka = stroka.substr(0, pos1) + ws + stroka.substr(pos1 + 6);
//        if (stroka == "{Роив}") {
//            stroka = ws;
//        } else {
//            stroka = stroka.substr(0, pos1) + ws + stroka.substr(pos1 + 6);
//        }

    }

    pos1 = stroka.find(u"{БП}");  // наименование бюджетополучателя самого текущего района
    if (pos1 != u16string::npos) {
        std::string bduserScet8;
        std::string bduserName8;
        std::string bduserINN8;
        TDB_BPolucRaj::getBP1forTB(FormLogicF1->CurrYear, FormLogicF1->CurrPlanId, bduserScet8, bduserName8,
                                   bduserINN8);

        bduserScet = utf8_to_utf16(bduserScet8);
        bduserName = utf8_to_utf16(bduserName8);
        bduserINN = utf8_to_utf16(bduserINN8);
        bduserName = utf8_to_utf16(DelSkobka(utf16_to_utf8(bduserName)));
        stroka = stroka.substr(0, pos1) + bduserName + stroka.substr(pos1 + 4);
    }

    pos1 = stroka.find(u"{БПп}");  // полное наименование бюджетополучателя самого текущего района
    if (pos1 != u16string::npos) {
        std::string bduserScet8;
        std::string bduserName8;
        std::string bduserINN8;
        std::string bduserKPP8;
        TDB_BPolucRaj::getBP1forTBpolnoe(FormLogicF1->CurrYear, FormLogicF1->CurrPlanId, bduserScet8, bduserName8,
                                         bduserINN8, bduserKPP8);
        bduserScet = utf8_to_utf16(bduserScet8);
        bduserName = utf8_to_utf16(bduserName8);
        bduserINN = utf8_to_utf16(bduserINN8);
        bduserKPP = utf8_to_utf16(bduserKPP8);
        bduserName = utf8_to_utf16(DelSkobka(utf16_to_utf8(bduserName)));
        stroka = stroka.substr(0, pos1) + bduserName + stroka.substr(pos1 + 5);
    }

    pos1 = stroka.find(u"{РС4}");  // номер распределительного счета района для ТБ
    if (pos1 != u16string::npos) {
        ws = utf8_to_utf16(TDB_BPolucRaj::RSRsp_GetBPbyPlanIdTB(FormLogicF1->CurrPlanId));
        stroka = stroka.substr(0, pos1) + ws + stroka.substr(pos1 + 5);
    }

    pos1 = stroka.find(u"{РС3}");  // номер получательного счета района для ТБ
    if (pos1 != u16string::npos) {
        ws = utf8_to_utf16(TDB_BPolucRaj::RSPlc_GetBPbyPlanIdTB(FormLogicF1->CurrYear, FormLogicF1->CurrPlanId));
        stroka = stroka.substr(0, pos1) + ws + stroka.substr(pos1 + 5);
    }

    pos1 = stroka.find(u"{РС2}");  // номер счета района
    if (pos1 != u16string::npos) {
        ws = utf8_to_utf16(TDB_BPoluc::GetRascScet2(FormLogicF1->CurrPlanId));
        stroka = stroka.substr(0, pos1) + ws + stroka.substr(pos1 + 5);
    }

    pos1 = stroka.find(u"{РС1}");  // номер счета района
    if (pos1 != u16string::npos) {
        ws = utf8_to_utf16(TDB_BPoluc::GetRascScet1(FormLogicF1->CurrPlanId));
        stroka = stroka.substr(0, pos1) + ws + stroka.substr(pos1 + 5);
    }

    pos1 = stroka.find(u"{Д}");
    if (pos1 != u16string::npos) {
        ToDate = FormLogicF1->CurrDate;
        string wsDate = DateTimeToStr(ToDate);
        if (FormLogicF1->CurrYear < YearOf(ToDate))
            ToDate = EncodeDate(FormLogicF1->CurrYear, 12, 31);
        ws = utf8_to_utf16(FormatDateTime("%d.%m.%Y", ToDate));
        stroka = stroka.substr(0, pos1) + ws + stroka.substr(pos1 + 3);
    }

    pos1 = stroka.find(u"{К");
    if (pos1 != u16string::npos) {
        if (GetD(2, pos1, pos2, d)) {// если ошибка, то пропускаем
            kv = FormLogicF1->CurrQuarter + d;
            strkv = GetStrKv(kv);
            stroka = stroka.substr(0, pos1) + strkv + stroka.substr(pos2 + 1);
        }
    }

    pos1 = stroka.find(u"{И}");
    if (pos1 != u16string::npos) {
        switch (FormLogicF1->CurrQuarter) {
            case 1:
                stri = u"I квартал";
                break;
            case 2:
                stri = u"1-ое полугодие";
                break;
            case 3:
                stri = u"9 месяцев";
                break;
            case 4:
                stri = u"год";
                break;
        }
        stroka = stroka.substr(0, pos1) + stri + stroka.substr(pos1 + 3);
    }

    pos1 = stroka.find(u"{НМ}");
    if (pos1 != u16string::npos) {
        ToDate = EncodeDate(FormLogicF1->CurrYear, FormLogicF1->CurrMonth, 01);
        stri = utf8_to_utf16(FormatDateTime("dd.mm.yyyy", ToDate));
        stroka = stroka.substr(0, pos1) + stri + stroka.substr(pos1 + 4);
    }

    pos1 = stroka.find(u"{КМ}");
    if (pos1 != u16string::npos) {
        ToDate = GetLastDayDate(FormLogicF1->CurrYear, 0, FormLogicF1->CurrMonth);
        stri = utf8_to_utf16(FormatDateTime("dd.mm.yyyy", ToDate));
        stroka = stroka.substr(0, pos1) + stri + stroka.substr(pos1 + 4);
    }

    pos1 = stroka.find(u"{М");
    if (pos1 != u16string::npos) {
        if (GetD(2, pos1, pos2, d)) {// если ошибка, то пропускаем
            mes = FormLogicF1->CurrMonth + d;
            if (mes < 1) mes = 1;
            if (mes > 12) mes = 12;
            switch (mes) {
                case 1:
                    strmes = u"январь";
                    break;
                case 2:
                    strmes = u"февраль";
                    break;
                case 3:
                    strmes = u"март";
                    break;
                case 4:
                    strmes = u"апрель";
                    break;
                case 5:
                    strmes = u"май";
                    break;
                case 6:
                    strmes = u"июнь";
                    break;
                case 7:
                    strmes = u"июль";
                    break;
                case 8:
                    strmes = u"август";
                    break;
                case 9:
                    strmes = u"сентябрь";
                    break;
                case 10:
                    strmes = u"октябрь";
                    break;
                case 11:
                    strmes = u"ноябрь";
                    break;
                case 12:
                    strmes = u"декабрь";
                    break;
            }
            stroka = stroka.substr(0, pos1) + strmes + stroka.substr(pos2 + 1);
        }
    }

    pos1 = stroka.find(u"{Г");
    if (pos1 != u16string::npos) {
        if (GetD(2, pos1, pos2, d)) {// если ошибка, то пропускаем
            god = CurrYear + d;
            strgod = intToU16String(god);
            stroka = stroka.substr(0, pos1) + strgod + stroka.substr(pos2 + 1);
        }
    }

    pos1 = stroka.find(u"{Г2}");
    if (pos1 != u16string::npos) {
        strgod = intToU16String(CurrYear).substr(2, 2);
        stroka = stroka.substr(0, pos1) + strgod + stroka.substr(pos1 + 4);
    }

    return stroka;
}

// заменить фильтр
void TForm2Plus::ZamenitKBKFiltr(TFormLogicF1 *FormLogicF1, TwwStringList *p, const std::string KBKshablon) {
    int pos1;
    std::string stroka;

    if (p->KBKfiltrRow == 0 && p->KBKfiltrCol == 0)
        return;

    stroka = char16ToUTF8(FormLogicF1->f1Forms->TextRC_Get(p->KBKfiltrRow, p->KBKfiltrCol));
    pos1 = stroka.find("{КБКфильтр}");
    if (pos1 != std::string::npos) {
        stroka = stroka.substr(0, pos1) + KBKshablon + stroka.substr(pos1 + 11);
        FormLogicF1->f1Forms->TextRC_Set(p->KBKfiltrRow, p->KBKfiltrCol, utf8_to_utf16(stroka).c_str());
    }
}

// заменить фильтр по Дате
void TForm2Plus::ZamenitDateFiltr(TFormLogicF1 *FormLogicF1, TwwStringList *p, TDateTime DateDiapazonFrom,
                                  TDateTime DateDiapazonTo) {
    std::string stroka, ws;
    size_t pos1;

    if (!(p->DatefiltrFromRow == 0 && p->DatefiltrFromCol == 0)) {
        stroka = char16ToUTF8(FormLogicF1->f1Forms->TextRC_Get(p->DatefiltrFromRow, p->DatefiltrFromCol));
        pos1 = stroka.find("{Сфильтр}");
        if (pos1 != std::string::npos) {
            ws = FormatDateTime("dd.mm.yyyy", DateDiapazonFrom);
            stroka = stroka.substr(0, pos1) + ws + stroka.substr(pos1 + 9);
            FormLogicF1->f1Forms->TextRC_Set(p->DatefiltrFromRow, p->DatefiltrFromCol, utf8_to_utf16(stroka).c_str());
        }
    }

    if (!(p->DatefiltrToRow == 0 && p->DatefiltrToCol == 0)) {
        stroka = char16ToUTF8(FormLogicF1->f1Forms->TextRC_Get(p->DatefiltrToRow, p->DatefiltrToCol));
        pos1 = stroka.find("{ПОфильтр}");
        if (pos1 != std::string::npos) {
            ws = FormatDateTime("dd.mm.yyyy", DateDiapazonTo);
            stroka = stroka.substr(0, pos1) + ws + stroka.substr(pos1 + 10);
            FormLogicF1->f1Forms->TextRC_Set(p->DatefiltrToRow, p->DatefiltrToCol, utf8_to_utf16(stroka).c_str());
        }
    }
}

// заменяет наименование объекта в строке
// формат такой {ON}
std::string TForm2Plus::ZamenitNameObject(std::string NameObject, std::string stroka) {
    int pos1;
    std::string Result;

    pos1 = stroka.find("{OK}");
    if (pos1 > 0) {
        stroka = stroka.substr(0, pos1) + GetKeyObject() + stroka.substr(pos1 + 4);
    }
    Result = stroka;

    pos1 = stroka.find("{ON}");
    if (pos1 == std::string::npos) return Result;

    Result = stroka.substr(0, pos1) + NameObject + stroka.substr(pos1 + 4);
    return Result;
}

// создает список ForTopBottom
void TForm2Plus::CreateForTopBottom(TwwStringList *p) {
    p->ForTopBottom = new TStringList();
    // p->ForTopBottom->sorted = true;
}

// первоначальная обработка вычисляемых ячеек
void TForm2Plus::SetEvalCell(int dd, TFormLogicF1 *FormLogicF1, int nRow, int nCol, int nRow2, int nCol2,
                             TQueryUni *quFormStr) {
    std::string note2, fs;
    int PlanId, VidOtch, PokazType, is_list, coef_Pokaz, coef_Forma, PeriodType, period = 0;
    int ValuePeriod, CurrPokazId, Kv, Mes, IsIstFin, GradId, DataType;
    TDateTime PokazDate;
    int Year, Month, Day;
    TwwStringList *p;
    TFormatStyle *FormatStyle = new TFormatStyle();

    p = TFormDopInp::GetTww(FormLogicF1);
    note2 = quFormStr->FieldByName("note2")->AsString();
    if (note2 == "СУММАПОСТОЛБЦУ") {
        FormatStyle->setToNull();
        FormatStyle->align_vert = quFormStr->FieldByName("align_vert")->AsInteger();
        FormatStyle->align_gor = quFormStr->FieldByName("align_vert")->AsInteger();
        FormatStyle->font = quFormStr->FieldByName("font")->AsString();
        FormatStyle->h = quFormStr->FieldByName("h")->AsInteger();
        FormatStyle->is_bold = quFormStr->FieldByName("is_bold")->AsInteger();
        FormatStyle->isOrientation = 0;
        FormatStyle->FontColor = 0;

        TForm1::SetCellValue(FormLogicF1, FormLogicF1->f1Forms, p->MaxHrCol, nRow, nCol, nRow2, nCol2,
                             " ", dd + 1, FormatStyle);
        FormLogicF1->f1Forms->Protection_Set(true, true);
        TForm2Plus::AddForTopBottomItem(FormLogicF1, nRow, nCol, 0, 0, 0,
                                        0, 0, 0, 0, 0, 0,
                                        0, quFormStr->FieldByName("note2").AsString(),
                                        quFormStr->FieldByName("god_ravno").AsInteger(),
                                        quFormStr->FieldByName("note1").AsString());
    }
    if (note2 == "КОЛОБЪЕКТОВ") {
        FormatStyle->setToNull();
        FormatStyle->align_vert = quFormStr->FieldByName("align_vert")->AsInteger();
        FormatStyle->align_gor = quFormStr->FieldByName("align_gor")->AsInteger();
        FormatStyle->font = quFormStr->FieldByName("font")->AsString();
        FormatStyle->h = quFormStr->FieldByName("h")->AsInteger();
        FormatStyle->is_bold = quFormStr->FieldByName("is_bold")->AsInteger();
        FormatStyle->isOrientation = 0;
        FormatStyle->FontColor = 0;

        TForm1::SetCellValue(FormLogicF1, FormLogicF1->f1Forms, p->MaxHrCol, nRow, nCol, nRow2, nCol2,
                             " ", dd + 1, FormatStyle);
        FormLogicF1->f1Forms->Protection_Set(true, true);
        TForm2Plus::AddForTopBottomItem(FormLogicF1, nRow, nCol, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                        quFormStr->FieldByName("note2")->AsString(), 0,
                                        quFormStr->FieldByName("note1")->AsString());
    }
    if (note2 == "ФОРМУЛАF1") {
        fs = TFormDopInp::ReplaceNRowNCol(nRow, nCol, quFormStr->FieldByName("note1")->AsString());
        FormatStyle->setToNull();
        FormatStyle->align_vert = quFormStr->FieldByName("align_vert")->AsInteger();
        FormatStyle->align_gor = quFormStr->FieldByName("align_gor")->AsInteger();
        FormatStyle->font = quFormStr->FieldByName("font")->AsString();
        FormatStyle->h = quFormStr->FieldByName("h")->AsInteger();
        FormatStyle->is_bold = quFormStr->FieldByName("is_bold")->AsInteger();
        FormatStyle->NumberFormat = "# ### ### ##0.0;[Red]-# ### ### ##0.0";
        FormatStyle->isOrientation = 0;
        FormatStyle->FontColor = 0;

        TForm1::SetCellValue(FormLogicF1, FormLogicF1->f1Forms, p->MaxHrCol, nRow, nCol, nRow2, nCol2,
                             fs, dd + 4, FormatStyle);
        FormLogicF1->f1Forms->Protection_Set(true, true);
        TForm2Plus::AddForTopBottomItem(FormLogicF1, nRow, nCol, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                        quFormStr->FieldByName("note2")->AsString(), 0,
                                        quFormStr->FieldByName("note1")->AsString());
    }
    if (note2 == "ЗНАЧЕНИЕСВ") {
        FormatStyle->setToNull();
        FormatStyle->align_vert = quFormStr->FieldByName("align_vert")->AsInteger();
        FormatStyle->align_gor = quFormStr->FieldByName("align_gor")->AsInteger();
        FormatStyle->font = quFormStr->FieldByName("font")->AsString();
        FormatStyle->h = quFormStr->FieldByName("h")->AsInteger();
        FormatStyle->is_bold = quFormStr->FieldByName("is_bold")->AsInteger();
        FormatStyle->isOrientation = 0;
        FormatStyle->FontColor = 0;

        TForm1::SetCellValue(FormLogicF1, FormLogicF1->f1Forms, p->MaxHrCol, nRow, nCol, nRow2, nCol2,
                             " ", dd + 1, FormatStyle);
        FormLogicF1->f1Forms->Protection_Set(true, true);
        PlanId = quFormStr->FieldByName("plan_id")->AsInteger();
        if (PlanId == 0)
            PlanId = FormLogicF1->CurrPlanId;
        VidOtch = quFormStr->FieldByName("vid_otch")->AsInteger();
        if (VidOtch == 0)
            VidOtch = FormLogicF1->CurrVidOtch;
        TQueryUni *qPokaz = new TQueryUni(DefDBconnect.Conn);
        CurrPokazId = quFormStr->FieldByName("pokaz_id")->AsInteger();
        TDB_Pokaztl::GetPokaz(qPokaz, CurrPokazId);
        PokazType = qPokaz->FieldByName("pokaz_type")->AsInteger();
        is_list = qPokaz->FieldByName("is_list")->AsInteger();
        coef_Pokaz = TDB_EdIzm::GetCoef_PokazOpt(qPokaz->FieldByName("edizm_id")->AsInteger());
        coef_Forma = quFormStr->FieldByName("CoefPlus1")->AsInteger() - 1;
        if (coef_Forma < 0)
            coef_Forma = coef_Pokaz;
        else
            coef_Forma = coef_Forma * 3;
        PeriodType = quFormStr->FieldByName("period")->AsInteger();
        ValuePeriod = quFormStr->FieldByName("ValuePeriod")->AsInteger();
        FormLogicF1->quCell->Params0 = CurrPokazId;
        FormLogicF1->quCell->Params3 = FormLogicF1->FormDM->quFormStr->FieldByName("din_izm")->AsString();
        switch (PeriodType) {
            case 1:
            case 2:
            case 3:
                FormLogicF1->quCell->Params1 = 1;
                Kv = 0;
                Mes = 0;
                break;
            case 4:
            case 5:
                FormLogicF1->quCell->Params1 = 2;
                Kv = FormLogicF1->CurrQuarter;
                if (ValuePeriod > 0)
                    Kv = ValuePeriod;
                Mes = 0;
                break;
            case 6:
                FormLogicF1->quCell->Params1 = 3;
                Kv = 0;
                Mes = FormLogicF1->CurrMonth;
                if (ValuePeriod > 0)
                    Mes = ValuePeriod;
                break;
            default:
                FormLogicF1->quCell->Params1 = 0;
                Kv = 0;
                Mes = 0;
                break;
        }
        IsIstFin = FormLogicF1->FormDM->quFormStr->FieldByName("is_ist_fin")->AsInteger();
        if (IsIstFin == 1) {
            GradId = FormLogicF1->FormDM->quFormStr->FieldByName("ist_fin_grad_id")->AsInteger();
            if (qPokaz->FieldByName("pokaz_is_ist_fin")->AsInteger() == 0) {
                GradId = 0;
                IsIstFin = 0;
            }
        } else
            GradId = 0;
        FormLogicF1->quCell->Params2 = GradId;
        FormLogicF1->quCell->Open();
        if (!FormLogicF1->quCell->EoF()) {
            DataType = PokazType;
            PokazDate = EncodeDate(FormLogicF1->CurrYear, 1, 31);
            if (Kv > 0)
                PokazDate = IncMonth(PokazDate, Kv * 3 - 1);
            else if (Mes > 0) {
                if (Mes > 1)
                    PokazDate = IncMonth(PokazDate, Mes - 1);
            } else
                PokazDate = EncodeDate(FormLogicF1->CurrYear, 12, 31);
            // добавление для столбцов с заданной датой
            switch (period) {
                case 4:// {квартал}
                    if (ValuePeriod > 0)
                        PokazDate = GetLastDayDate(FormLogicF1->CurrYear, ValuePeriod, 0);
                    break;
                case 6://{месяц}
                    if (ValuePeriod > 0)
                        PokazDate = GetLastDayDate(FormLogicF1->CurrYear, 0, ValuePeriod);
                    break;
            }
            // обработка установки года
            switch (FormLogicF1->FormDM->quFormStr->FieldByName("is_set_god")->AsInteger()) {
                case 0://{наследуется}
                    if (FormLogicF1->FormDM->quFormStr->FieldByName("god_plus")->AsInteger() > 0) {
                        DecodeDate(PokazDate, Year, Month, Day);
                        PokazDate = EncodeDate(FormLogicF1->CurrYear +
                                               FormLogicF1->FormDM->quFormStr->FieldByName("god_plus")->AsInteger(),
                                               Month, Day);
                    }
                    break;
                case 1://{задается}
                    DecodeDate(PokazDate, Year, Month, Day);
                    PokazDate = EncodeDate(FormLogicF1->FormDM->quFormStr->FieldByName("god_ravno")->AsInteger(), Month,
                                           Day);
                    break;
            }


            TForm2Plus::AddForTopBottomItem(FormLogicF1, nRow, nCol, FormLogicF1->quCell->cell_id,
                                            PokazDate, DataType, VidOtch, PlanId, coef_Pokaz, coef_Forma, 0,
                                            CurrPokazId,
                                            PokazType, quFormStr->FieldByName("note2")->AsString(), 0,
                                            quFormStr->FieldByName("note1")->AsString());
        }
        delete qPokaz;
        FormLogicF1->quCell->Close();
    }
    if (note2 == "ХАРАКТЕРИСТИКА") {
        FormatStyle->setToNull();
        FormatStyle->align_vert = quFormStr->FieldByName("align_vert")->AsInteger();
        FormatStyle->align_gor = quFormStr->FieldByName("align_gor")->AsInteger();
        FormatStyle->font = quFormStr->FieldByName("font")->AsString();
        FormatStyle->h = quFormStr->FieldByName("h")->AsInteger();
        FormatStyle->is_bold = quFormStr->FieldByName("is_bold")->AsInteger();
        FormatStyle->isOrientation = 0;
        FormatStyle->FontColor = 0;

        TForm1::SetCellValue(FormLogicF1, FormLogicF1->f1Forms, p->MaxHrCol, nRow, nCol, nRow2, nCol2,
                             " ", dd, FormatStyle);
        FormLogicF1->f1Forms->Protection_Set(true, true);
        PlanId = quFormStr->FieldByName("plan_id")->AsInteger();
        if (PlanId == 0)
            PlanId = FormLogicF1->CurrPlanId;
        VidOtch = quFormStr->FieldByName("vid_otch")->AsInteger();
        if (VidOtch == 0)
            VidOtch = FormLogicF1->CurrVidOtch;
        TQueryUni *qPokaz = new TQueryUni(DefDBconnect.Conn);
        CurrPokazId = quFormStr->FieldByName("pokaz_id")->AsInteger();
        TDB_Pokaztl::GetPokaz(qPokaz, CurrPokazId);
        PokazType = qPokaz->FieldByName("pokaz_type")->AsInteger();
        is_list = qPokaz->FieldByName("is_list")->AsInteger();
        coef_Pokaz = TDB_EdIzm::GetCoef_PokazOpt(qPokaz->FieldByName("edizm_id")->AsInteger());
        coef_Forma = quFormStr->FieldByName("CoefPlus1")->AsInteger() - 1;
        if (coef_Forma < 0) // вариант Как у показателя
            coef_Forma = coef_Pokaz;
        else
            coef_Forma = coef_Forma * 3;
        PeriodType = quFormStr->FieldByName("period")->AsInteger();
        ValuePeriod = quFormStr->FieldByName("ValuePeriod")->AsInteger();
        FormLogicF1->quCell->Params0 = CurrPokazId;
        FormLogicF1->quCell->Params3 = FormLogicF1->FormDM->quFormStr->FieldByName("din_izm")->AsString();
        switch (PeriodType) {
            case 1:
            case 2:
            case 3:
                FormLogicF1->quCell->Params1 = 1;
                Kv = 0;
                Mes = 0;
                break;
            case 4:
            case 5:
                FormLogicF1->quCell->Params1 = 2;
                Kv = FormLogicF1->CurrQuarter;
                Mes = 0;
                break;
            case 6:
                FormLogicF1->quCell->Params1 = 3;
                Kv = 0;
                Mes = FormLogicF1->CurrMonth;
                break;
            default:
                FormLogicF1->quCell->Params1 = 0;
                Kv = 0;
                Mes = 0;
                break;
        }
        IsIstFin = FormLogicF1->FormDM->quFormStr->FieldByName("is_ist_fin")->AsInteger();
        if (IsIstFin == 1) {
            GradId = FormLogicF1->FormDM->quFormStr->FieldByName("ist_fin_grad_id")->AsInteger();
            if (qPokaz->FieldByName("pokaz_is_ist_fin")->AsInteger() == 0) {
                GradId = 0;
                IsIstFin = 0;
            }
        } else
            GradId = 0;
        FormLogicF1->quCell->Params2 = GradId;
        FormLogicF1->quCell->Open();
        if (!FormLogicF1->quCell->EoF()) {
            DataType = PokazType;
            PokazDate = EncodeDate(FormLogicF1->CurrYear, 1, 31);
            if (Kv > 0)
                PokazDate = IncMonth(PokazDate, Kv * 3 - 1);
            else if (Mes > 0) {
                if (Mes > 1)
                    PokazDate = IncMonth(PokazDate, Mes - 1);
            } else
                PokazDate = EncodeDate(FormLogicF1->CurrYear, 12, 31);
            // добавление для столбцов с заданной датой
            switch (period) {
                case 4://{квартал}
                    if (ValuePeriod > 0)
                        PokazDate = GetLastDayDate(FormLogicF1->CurrYear, ValuePeriod, 0);
                    break;
                case 6://{месяц}
                    if (ValuePeriod > 0)
                        PokazDate = GetLastDayDate(FormLogicF1->CurrYear, 0, ValuePeriod);
                    break;
            }
            // обработка установки года
            switch (FormLogicF1->FormDM->quFormStr->FieldByName("is_set_god")->AsInteger()) {
                case 0://{наследуется}
                    if (FormLogicF1->FormDM->quFormStr->FieldByName("god_plus")->AsInteger() > 0) {
                        DecodeDate(PokazDate, Year, Month, Day);
                        PokazDate = EncodeDate(FormLogicF1->CurrYear +
                                               FormLogicF1->FormDM->quFormStr->FieldByName("god_plus")->AsInteger(),
                                               Month, Day);
                    }
                    break;
                case 1://{задается}
                    DecodeDate(PokazDate, Year, Month, Day);
                    PokazDate = EncodeDate(FormLogicF1->FormDM->quFormStr->FieldByName("god_ravno")->AsInteger(), Month,
                                           Day);
                    break;
            }
            TForm2Plus::AddForTopBottomItem(FormLogicF1, nRow, nCol, FormLogicF1->quCell->cell_id,
                                            PokazDate, DataType, VidOtch, PlanId, coef_Pokaz, coef_Forma, 0,
                                            CurrPokazId,
                                            PokazType, quFormStr->FieldByName("note2")->AsString(), 0,
                                            quFormStr->FieldByName("note1")->AsString());
        }
        delete qPokaz;
        FormLogicF1->quCell->Close();
    }
    if (note2 == "ВЛОРГПОДЧ_НАИМ") {
        FormatStyle->setToNull();
        FormatStyle->align_vert = quFormStr->FieldByName("align_vert")->AsInteger();
        FormatStyle->align_gor = quFormStr->FieldByName("align_gor")->AsInteger();
        FormatStyle->font = quFormStr->FieldByName("font")->AsString();
        FormatStyle->h = quFormStr->FieldByName("h")->AsInteger();
        FormatStyle->is_bold = quFormStr->FieldByName("is_bold")->AsInteger();
        FormatStyle->isOrientation = 0;
        FormatStyle->FontColor = 0;

        TForm1::SetCellValue(FormLogicF1, FormLogicF1->f1Forms, p->MaxHrCol, nRow, nCol, nRow2, nCol2,
                             " ", dd, FormatStyle);
        FormLogicF1->f1Forms->Protection_Set(true, true);
        TForm2Plus::AddForTopBottomItem(FormLogicF1, nRow, nCol, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                        quFormStr->FieldByName("note2")->AsString(), 0,
                                        quFormStr->FieldByName("note1")->AsString());
    }
    if (note2 == "ВЛОРГПОДЧ_ЛСЧ") {
        FormatStyle->setToNull();
        FormatStyle->align_vert = quFormStr->FieldByName("align_vert")->AsInteger();
        FormatStyle->align_gor = quFormStr->FieldByName("align_gor")->AsInteger();
        FormatStyle->font = quFormStr->FieldByName("font")->AsString();
        FormatStyle->h = quFormStr->FieldByName("h")->AsInteger();
        FormatStyle->is_bold = quFormStr->FieldByName("is_bold")->AsInteger();
        FormatStyle->isOrientation = 0;
        FormatStyle->FontColor = 0;

        TForm1::SetCellValue(FormLogicF1, FormLogicF1->f1Forms, p->MaxHrCol, nRow, nCol, nRow2, nCol2,
                             " ", dd, FormatStyle);
        FormLogicF1->f1Forms->Protection_Set(true, true);
        TForm2Plus::AddForTopBottomItem(FormLogicF1, nRow, nCol, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                        quFormStr->FieldByName("note2")->AsString(), 0,
                                        quFormStr->FieldByName("note1")->AsString());
    }
    delete FormatStyle;
}

// добавляет элемент в список ForTopBottom
void TForm2Plus::AddForTopBottomItem(TFormLogicF1 *FormLogicF1, int Row, int Col, int CellId, TDateTime ValueDate,
                                     int DataType, int VidOtch, int PlanId, short coef_Pokaz, short coef_Forma,
                                     int ObjectId, int PokazId, unsigned char pokaz_type, std::string note2, int atr1,
                                     std::string note1) {
    TwwStringList *p = TFormDopInp::GetTww(FormLogicF1);
    Pftb newFtb = new Tftb;
    newFtb->Row = Row;
    newFtb->Col = Col;
    newFtb->CellId = CellId;
    newFtb->PokazId = PokazId;
    newFtb->ObjectId = ObjectId;
    newFtb->ValueDate = ValueDate;
    newFtb->DataType = DataType;
    newFtb->pokaz_type = pokaz_type;
    newFtb->VidOtch = VidOtch;
    newFtb->PlanId = PlanId;
    newFtb->coef_Pokaz = coef_Pokaz;
    newFtb->coef_Forma = coef_Forma;
    newFtb->note2 = note2;
    newFtb->atr1 = atr1;
    newFtb->note1 = note1;
    std::string cellName = "R" + std::to_string(newFtb->Row) + "C" + std::to_string(newFtb->Col);
    p->ForTopBottom->AddObject(cellName, newFtb);
}

// вычислить значения вычисляемых ячеек
void TForm2Plus::ExecEvalCells(TFormLogicF1 *FormLogicF1) {
    TwwStringList *p = TFormDopInp::GetTww(FormLogicF1);
    Pftb ftb;
    if (p->ForTopBottom != nullptr) {
        for (int i = 0; i < p->ForTopBottom->Count(); i++) {
            //ftb = Pftb(p->ForTopBottom->Objects[i]); ЗАМЕНИЛ
//            MyVariantSL& variant = std::get<Pftb>(p->ForTopBottom->Objects[i]);
//
//            // Используем std::get_if для безопасного получения указателя на Pftb
//            Pftb* ptrToPftb = std::get_if<Pftb>(&variant);
//            ftb = *ptrToPftb;
            ftb = std::get<Pftb>(p->ForTopBottom->Objects[i]);

            if (ftb->note2 == "СУММАПОСТОЛБЦУ") {
#ifndef WEBSERVICE
                EvalSummaPoStolbcu(FormLogicF1, ftb);
#else
                throw std::runtime_error("В описании формы F1Book, вычисляемой в Web-сервисе, не допускается использование функции \"СУММАПОСТОЛБЦУ\"");
#endif
            }
            if (ftb->note2 == "КОЛОБЪЕКТОВ") {
                TForm2Plus::EvalKolObjectov(FormLogicF1, ftb);
            }
            if (ftb->note2 == "ЗНАЧЕНИЕСВ") {
                TForm2Plus::EvalZnacSV(FormLogicF1, ftb);
            }
            if (ftb->note2 == "ХАРАКТЕРИСТИКА") {
                TForm2Plus::EvalField(FormLogicF1, ftb);
            }
            if (ftb->note2 == "ВЛОРГПОДЧ_НАИМ" || ftb->note2 == "ВЛОРГПОДЧ_ЛСЧ") {
                int plan_id_GUIS = FormLogicF1->CurrPlanId;
                int plan_rjn = TDB_Plan::GetParentIid(plan_id_GUIS, true);
                if (plan_rjn == 0) {
                    return;
                }
                TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
                TDB_BPolucRaj::GetBPbyPlanIdTBRsp(q, plan_rjn);
                if (ftb->note2 == "ВЛОРГПОДЧ_НАИМ") {
                    FormLogicF1->f1Forms->TextRC_Set(ftb->Row, ftb->Col, utf8_to_utf16(
                            DelSkobka(q->FieldByName("short_name")->AsString())).c_str());
                }
                if (ftb->note2 == "ВЛОРГПОДЧ_ЛСЧ") {
                    FormLogicF1->f1Forms->TextRC_Set(ftb->Row, ftb->Col,
                                                     utf8_to_utf16(q->FieldByName("rasc_scet")->AsString()).c_str());
                }
            }
        }
    }
}

void
TForm2Plus::ReSumma(TFormLogicF1 *FormLogicF1, int OwnerNomRow, bool IsSumma, int CurRow, double &summa, int SummaCol) {
    bool b;
    PAtrRow AtrRow;
    PAtrRow AtrOwner;
    while (CurRow <= FormLogicF1->f1Forms->LastRow) {
        AtrRow = TFormDopInp::GetListRowsItem(FormLogicF1, CurRow);
        if (AtrRow == nullptr) {
            CurRow++;
            continue;
        }
        if (AtrRow->OwnerNomRow == OwnerNomRow) {
            switch (AtrRow->CurRowVid) {
                case 1: // раздел
                    break;
                case 2: // показатель
                    if (IsSumma)
                        summa += FormLogicF1->f1Forms->NumberRC_Get(CurRow, SummaCol);
                    break;
                case 3: // итого по разделу
                    break;
                case 4: // объект
                    if (IsSumma)
                        summa += FormLogicF1->f1Forms->NumberRC_Get(CurRow, SummaCol);
                    break;
            }
            CurRow++;
            continue;
        }
        if (AtrRow->OwnerNomRow > OwnerNomRow) {
            AtrOwner = TFormDopInp::GetListRowsItem(FormLogicF1, AtrRow->OwnerNomRow);
            switch (AtrOwner->CurRowVid) {
                case 1:
                    b = true; // раздел
                    break;
                case 2:
                    b = false; // показатель
                    break;
                case 3:
                    b = false; // итого по разделу
                    break;
                case 4:
                    b = false; // объект
                    break;
            }
            ReSumma(FormLogicF1, AtrRow->OwnerNomRow, IsSumma, CurRow, summa, SummaCol);
            continue;
        }
        if (AtrRow->OwnerNomRow < OwnerNomRow)
            return;
    }
}

// вычисление СУММАПОСТОЛБЦУ
void TForm2Plus::EvalSummaPoStolbcu(TFormLogicF1 *FormLogicF1, Pftb ftb) {
    std::string nf;
    double summa = 0;
    int SummaCol = ftb->atr1;
    int CurRow = TFormDopInp::GetKolStrIn_ShapZag(FormLogicF1) + 1;
    ReSumma(FormLogicF1, CurRow - 1, true, CurRow, summa, SummaCol);
    nf = "# ### ##0.0#;-# ### ##0.0#";
    FormLogicF1->f1Forms->Selection_Set(ftb->Row, ftb->Col, ftb->Row, ftb->Col);
    FormLogicF1->f1Forms->NumberFormat = utf8_to_utf16(nf).c_str();
    FormLogicF1->f1Forms->NumberRC_Set(ftb->Row, ftb->Col, summa);
}

// вычисление КОЛОБЪЕКТОВ
void TForm2Plus::EvalKolObjectov(TFormLogicF1 *FormLogicF1, Pftb ftb) {
    double summa = 0;
    int CurRow = TFormDopInp::GetKolStrIn_ShapZag(FormLogicF1) + 1;
    int LastRow = FormLogicF1->f1Forms->LastRow_Get();
    while (CurRow <= LastRow) {
        PAtrRow AtrRow = TFormDopInp::GetListRowsItem(FormLogicF1, CurRow);
        if (AtrRow == nullptr) {
            CurRow++;
            continue;
        }
        switch (AtrRow->CurRowVid) {
            case 1: // раздел
                break;
            case 2: // показатель
                break;
            case 3: // итого по разделу
                break;
            case 4: // объект
                summa++;
                break;
        }
        CurRow++;
    }
    std::string nf = "# ### ##0.0#;-# ### ##0.0#";
    FormLogicF1->f1Forms->Selection_Set(ftb->Row, ftb->Col, ftb->Row, ftb->Col);
    FormLogicF1->f1Forms->NumberFormat_Set(utf8_to_utf16(nf).c_str());
    FormLogicF1->f1Forms->Number_Set(summa);
}

// вычисление ЗНАЧЕНИЕСВ
void TForm2Plus::EvalZnacSV(TFormLogicF1 *FormLogicF1, Pftb ftb) {
    double d;
    std::string ws, nf;

    switch (ftb->pokaz_type) {
        case 1:
            nf = "# ### ##0.0#;-# ### ##0.0#";
            d = TDB_CellValue::GetValue(0, ftb->CellId, ftb->PlanId, ftb->VidOtch, ftb->ValueDate);
            d = PreobrValue(d, ftb->DataType, ftb->coef_Pokaz, ftb->coef_Forma);
            FormLogicF1->f1Forms->Selection_Set(ftb->Row, ftb->Col, ftb->Row, ftb->Col);
            FormLogicF1->f1Forms->NumberFormat_Set(utf8_to_utf16(nf).c_str());
            FormLogicF1->f1Forms->Text_Set(utf8_to_utf16(ftb->note1 + std::to_string(d)).c_str()); // 25 авг 2005
            break;
        case 2:
            string wsdate = DateTimeToStr(ftb->ValueDate);
            ws = TDB_CellValue::GetStrValue(0, ftb->CellId, ftb->PlanId, ftb->VidOtch, ftb->ValueDate);
            FormLogicF1->f1Forms->TextRC_Set(ftb->Row, ftb->Col, utf8_to_utf16(ftb->note1 + ws).c_str()); // 25 авг 2005
            break;
    }
}

// вычисление ХАРАКТЕРИСТИКИ
void TForm2Plus::EvalField(TFormLogicF1 *FormLogicF1, Pftb ftb) {
    double d = 0.0;
    int objectid;
    int i;
    PAtrRow AtrRow;

    // выбор объекта и его владельца
    int LastRow = FormLogicF1->f1Forms->LastRow_Get();
    for (i = 1; i <= LastRow; i++) {
        AtrRow = TFormDopInp::GetListRowsItem(FormLogicF1, i);
        if (AtrRow == nullptr) {
            continue;
        }
        if (AtrRow->OwnerId == 0) {
            continue;
        }
        objectid = AtrRow->OwnerId;
        break;
    }

    d = PreobrValue(d, ftb->DataType, ftb->coef_Pokaz, ftb->coef_Forma);
    FormLogicF1->f1Forms->NumberRC_Set(ftb->Row, ftb->Col, d);
}

double TForm2Plus::GetSumPrixodVozvrat(TFormLogicF1 *FormLogicF1, bool isPrixod, PCellValueAtr vd) {
    TDateTime BegDate;
    int Year, Month, Day;
    DecodeDate(vd->ValueDate, Year, Month, Day);

    switch (vd->cell_period) {
        case 1:
            BegDate = EncodeDate(Year, 1, 1);
            break;
        case 2:
            BegDate = EncodeDate(Year, Month - 2, 1);
            break;
        case 3:
            BegDate = EncodeDate(Year, Month, 1);;
            break;
    }

    int link_pokaz_id = TDB_Dgvr::GetIdFieldMeropr(); // поле Договора - мероприятие
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    q->SQL->Clear();
    q->SQL->Add("SELECT SUM(value) as value");
    q->SQL->Add("FROM Denegn_potok");
    q->SQL->Add("WHERE plan_id = :plan_id");
    q->SQL->Add("AND Pokaz_id = :pokaz_id");
    q->SQL->Add("AND objectid = 0");
    q->SQL->Add("AND vid_otc = 5");
    q->SQL->Add("AND value_date BETWEEN :s AND :po");
    if (isPrixod) q->SQL->Add("AND value > 0");
    else q->SQL->Add("AND value < 0");
    q->SQL->Add("AND reg_nomer LIKE :patern");

    q->SQL->Add("UNION");
    q->SQL->Add("SELECT SUM(value) as value");
    q->SQL->Add("FROM Denegn_potok a, LinksObjects d");
    q->SQL->Add("WHERE a.plan_id = :plan_id1");
    q->SQL->Add("AND a.objectid <> 0");
    q->SQL->Add("AND a.vid_otc = 5");
    if (isPrixod) q->SQL->Add("AND a.value > 0");
    else q->SQL->Add("AND a.value < 0");
    q->SQL->Add("AND a.value_date BETWEEN :s1 AND :po1");
    q->SQL->Add("AND d.plan_id = a.plan_id");
    q->SQL->Add("AND d.objectid = a.objectid");
    q->SQL->Add("AND d.link_pokaz_id = :link_pokaz_id");
    q->SQL->Add("AND d.Pokaz_id = :pokaz_id1");
    q->SQL->Add("AND reg_nomer LIKE :patern1");

    q->ParamByName("plan_id")->AsInteger() = vd->PlanId;
    q->ParamByName("plan_id1")->AsInteger() = vd->PlanId;
    q->ParamByName("pokaz_id")->AsInteger() = vd->PokazId;
    q->ParamByName("pokaz_id1")->AsInteger() = vd->PokazId;
    q->ParamByName("s")->AsDateTime() = BegDate;
    q->ParamByName("s1")->AsDateTime() = BegDate;
    q->ParamByName("po")->AsDateTime() = vd->ValueDate;
    q->ParamByName("po1")->AsDateTime() = vd->ValueDate;
    q->ParamByName("link_pokaz_id")->AsInteger() = link_pokaz_id;
    q->ParamByName("patern")->AsString() = "[0-9][0-9]%";
    q->ParamByName("patern1")->AsString() = "[0-9][0-9]%";
    q->Open();

    double Result = 0.0;
    while (!q->EoF()) {
        Result += q->FieldByName("value")->AsFloat();
        q->Next();
    }

    delete q;
    return Result;
}

double TForm2Plus::SummaUtoc(TFormLogicF1 *FormLogicF1, bool IsItog, int PlanId, TDateTime PokazDate, int cellId) {
    int kv;
    int Year, Month, Day;
    DecodeDate(PokazDate, Year, Month, Day);
    kv = GetNKv(Month);

    FormLogicF1->FormDM->quSumPotok->ParamByName("PlanId")->AsInteger() = PlanId;
    FormLogicF1->FormDM->quSumPotok->ParamByName("God")->AsInteger() = Year;
    if (IsItog)
        FormLogicF1->FormDM->quSumPotok->ParamByName("kv0")->AsInteger() = 1;
    else
        FormLogicF1->FormDM->quSumPotok->ParamByName("kv0")->AsInteger() = kv;
    FormLogicF1->FormDM->quSumPotok->ParamByName("kv")->AsInteger() = kv;
    FormLogicF1->FormDM->quSumPotok->ParamByName("cellId")->AsInteger() = cellId;
    FormLogicF1->FormDM->quSumPotok->Open();
    FormLogicF1->FormDM->quSumPotok->First();
    double Result = 0;
    while (!FormLogicF1->FormDM->quSumPotok->EoF()) {
        Result += FormLogicF1->FormDM->quSumPotok->FieldByName("sumval")->AsFloat();
        FormLogicF1->FormDM->quSumPotok->Next();
    }
    FormLogicF1->FormDM->quSumPotok->Close();
    return Result;
}

double
TForm2Plus::Ostatok(TFormLogicF1 *FormLogicF1, int CurrPlanId, int CellIdKv, int CellIdMes, int CurrPokazId, int VidOtc,
                    TDateTime EndMes, int objectid) {
    int kv;
    int Year, Month, Day;
    TDateTime BegDate, ValueDateKv;

    DecodeDate(EndMes, Year, Month, Day);
    kv = GetNKv(Month);
    ValueDateKv = EncodeDate(Year, 1, 31);
    BegDate = IncMonth(ValueDateKv, kv * 3 - 3);
    DecodeDate(BegDate, Year, Month, Day);
    ValueDateKv = IncMonth(ValueDateKv, kv * 3 - 1);

    FormLogicF1->FormDM->quOstatok->ParamByName("objectid")->AsInteger() = objectid;
    FormLogicF1->FormDM->quOstatok->ParamByName("CurrPlanId")->AsInteger() = CurrPlanId;
    FormLogicF1->FormDM->quOstatok->ParamByName("CellIdKv")->AsInteger() = CellIdKv;
    FormLogicF1->FormDM->quOstatok->ParamByName("ValueDateKv")->AsDateTime() = ValueDateKv;
    FormLogicF1->FormDM->quOstatok->ParamByName("CellIdMes")->AsInteger() = CellIdMes;
    FormLogicF1->FormDM->quOstatok->ParamByName("CurrPokazId")->AsInteger() = CurrPokazId;
    FormLogicF1->FormDM->quOstatok->ParamByName("VidOtc")->AsInteger() = VidOtc;
    FormLogicF1->FormDM->quOstatok->ParamByName("BegDate")->AsDateTime() = BegDate;
    FormLogicF1->FormDM->quOstatok->ParamByName("EndDate")->AsDateTime() = EndMes;
    FormLogicF1->FormDM->quOstatok->Open();

    double value = 0, sumValue = 0;
    while (!FormLogicF1->FormDM->quOstatok->EoF()) {
        if (FormLogicF1->FormDM->quOstatok->FieldByName("vid")->AsInteger() == 1)
            value = FormLogicF1->FormDM->quOstatok->FieldByName("value")->AsFloat();
        if (FormLogicF1->FormDM->quOstatok->FieldByName("vid")->AsInteger() == 2)
            sumValue = FormLogicF1->FormDM->quOstatok->FieldByName("value")->AsFloat();
        FormLogicF1->FormDM->quOstatok->Next();
    }

    FormLogicF1->FormDM->quOstatok->Close();
    return value - sumValue;
}

double TForm2Plus::OstatokNI(TFormLogicF1 *FormLogicF1, int CurrPlanId, int CellIdKv, int CellIdMes, int CurrPokazId,
                             int VidOtc, TDateTime EndMes, int objectid) {
    int kv;
    int Year, Month, Day;
    TDateTime BegDate, ValueDateKv;
    double value, sumValue;

    DecodeDate(EndMes, Year, Month, Day);
    kv = GetNKv(Month);
    ValueDateKv = EncodeDate(Year, 1, 31);
    BegDate = IncMonth(ValueDateKv, kv * 3 - 3);
    DecodeDate(BegDate, Year, Month, Day);
    BegDate = EncodeDate(Year, 1, 1);
    ValueDateKv = IncMonth(ValueDateKv, kv * 3 - 1);

    FormLogicF1->FormDM->quOstatok->ParamByName("objectid")->AsInteger() = objectid;
    FormLogicF1->FormDM->quOstatok->ParamByName("CurrPlanId")->AsInteger() = CurrPlanId;
    FormLogicF1->FormDM->quOstatok->ParamByName("CellIdKv")->AsInteger() = CellIdKv;
    FormLogicF1->FormDM->quOstatok->ParamByName("ValueDateKv")->AsDateTime() = ValueDateKv;
    FormLogicF1->FormDM->quOstatok->ParamByName("CellIdMes")->AsInteger() = CellIdMes;
    FormLogicF1->FormDM->quOstatok->ParamByName("CurrPokazId")->AsInteger() = CurrPokazId;
    FormLogicF1->FormDM->quOstatok->ParamByName("VidOtc")->AsInteger() = VidOtc;
    FormLogicF1->FormDM->quOstatok->ParamByName("BegDate")->AsDateTime() = BegDate; // с первого января
    FormLogicF1->FormDM->quOstatok->ParamByName("EndDate")->AsDateTime() = EndMes;
    FormLogicF1->FormDM->quOstatok->Open();
    value = 0;
    sumValue = 0;
    while (!FormLogicF1->FormDM->quOstatok->EoF()) {
        if (FormLogicF1->FormDM->quOstatok->FieldByName("vid")->AsInteger() == 2)
            sumValue = FormLogicF1->FormDM->quOstatok->FieldByName("value")->AsFloat();
        FormLogicF1->FormDM->quOstatok->Next();
    }
    FormLogicF1->FormDM->quOstatok->Close();

    FormLogicF1->FormDM->quValuePeriod->ParamByName("objectid")->AsInteger() = objectid;
    FormLogicF1->FormDM->quValuePeriod->ParamByName("CurrCellId")->AsInteger() = CellIdKv;
    FormLogicF1->FormDM->quValuePeriod->ParamByName("CurrPlanId")->AsInteger() = CurrPlanId;
    FormLogicF1->FormDM->quValuePeriod->ParamByName("CurrYear")->AsInteger() = Year;
    FormLogicF1->FormDM->quValuePeriod->ParamByName("CurrVidOtch")->AsInteger() = 4;
    FormLogicF1->FormDM->quValuePeriod->Open();
    while (!FormLogicF1->FormDM->quValuePeriod->EoF()) {
        if (FormLogicF1->FormDM->quValuePeriod->FieldByName("value_date")->AsDateTime() > ValueDateKv)
            break;
        value += FormLogicF1->FormDM->quValuePeriod->FieldByName("value")->AsFloat();
        FormLogicF1->FormDM->quValuePeriod->Next();
    }
    FormLogicF1->FormDM->quValuePeriod->Close();

    return value - sumValue;
}

// вычислить значение формулы конкатенации строк - характеристик объекта
std::string TForm2Plus::evalKonkatStr(TFormLogicF1 *FormLogicF1, PCellValueAtr vd, TwwStringList *p) {
    std::string note1, ws, prefix, sIn, wsIn;
    int wCol, field_id, cell_id, offset, nameCol;
    int cell_period;
    int ist_fin_id;
    char cc;
    PAtrRow AtrRow, AtrRow1;

    AtrRow = TFormDopInp::GetListRowsItem(FormLogicF1, vd->Row);
    if (AtrRow == nullptr) return "";
    if (AtrRow->CurRowVid == 5) { // МультиИтого
        nameCol = TFormDopInp::GetNameCol(FormLogicF1);
        if (int N = FormLogicF1->quForms->FieldByName("is_edIzm")->AsInteger(); N == 2 || N ==
                                                                                          3) { // столбец с Имя объекта скрыто
            note1 = p->ListStmnt[vd->Col].note1;
            if (note1.length() > 1 && note1[0] == '+') {
                ws = "Итого"; // Итого кладем только в том случае, когда в конкатенацию входит Имя объекта
                AtrRow1 = TFormDopInp::GetListRowsItem(FormLogicF1, vd->Row - 1);
                if (AtrRow1 == nullptr) return "";
                if (AtrRow1->CurRowVid == 5) { // МультиИтого
                    ws = "Всего";
                }
            } else {
                ws = "";
            }
        }
        return ws;
    }
    if (AtrRow->CurRowVid != 4) return "";

    wCol = vd->Col;
    note1 = p->ListStmnt[wCol].note1;
    cc = ' ';
    if (note1.length() > 1 && note1[0] == '+') { // символ + говорит, что надо впереди положить Имя объекта
        note1.erase(0, 1);
        cc = '+';
    }

    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    TDB_Cell::GetInfCell_id(q, vd->CellId);
    cell_period = q->FieldByName("cell_period")->AsInteger();
    ist_fin_id = q->FieldByName("ist_fin_id")->AsInteger();
    delete q;

    ws = "";
    wsIn = "";
    offset = TFormDopInp::GetNextFormulaConc(note1, field_id, prefix, 1);
    cell_id = TDB_Cell::GetCellId(field_id, cell_period, ist_fin_id);
    sIn = TDB_CellValue::GetStrValue(vd->ObjectId, cell_id, vd->PlanId, vd->VidOtch, vd->ValueDate);
    sIn = Trim(sIn);
    ws += prefix + sIn;
    wsIn += sIn;

    while (true) {
        offset = note1.find(']', offset + 1);
        offset = TFormDopInp::GetNextFormulaConc(note1, field_id, prefix, offset + 1);
        if (offset == 0) break;
        cell_id = TDB_Cell::GetCellId(field_id, cell_period, ist_fin_id);
        sIn = TDB_CellValue::GetStrValue(vd->ObjectId, cell_id, vd->PlanId, vd->VidOtch, vd->ValueDate);
        sIn = Trim(sIn);
        ws += prefix + sIn;
        wsIn += sIn;
    }

    if (wsIn == "") ws = ""; // значит нет данных
    if (cc == '+') ws = TDB_Pokaztl::GetPokazName(vd->ObjectId) + ws;

    return ws;
}

// вычислить Характеристики связанного объекта и вставить необходимые в форму
// эта процедура работает и на 'КонкатСтрокСВобъек'
void TForm2Plus::SetXarakterSvjzObjecta(TFormLogicF1 *FormLogicF1, int aRow, int id_SvjzClassa, int id_SvjzObjecta) {
    TwwStringList *p = TFormDopInp::GetTww(FormLogicF1);
    for (int i = 1; i <= p->MaxHrCol; i++) {
        std::string stmnt = p->ListStmnt[i].Stmnt;
        if (stmnt != "ХарактСВобъекта" && stmnt != "КонкатСтрокСВобъек") continue;
        PCellValueAtr vd = TFormDopInp::GetCellValueAtrByRC(FormLogicF1, aRow, i);
        if (vd == nullptr || vd->DPId != id_SvjzClassa) continue;
        TFormatStyle *FormatStyle = nullptr;
        TFormDopInp::getColFrmtrAtr(FormLogicF1, 0, i, *FormatStyle);
        int PeriodType = p->ListStmnt[i].PeriodType;
        int objectidOld = vd->ObjectId;
        vd->ObjectId = id_SvjzObjecta;
        std::string Value;
        if (id_SvjzObjecta == 0) {
            Value = "";
        } else {
            if (stmnt == "КонкатСтрокСВобъек") {
                Value = TForm2Plus::evalKonkatStrSvObj(FormLogicF1, vd, p);
            } else {
                if (PeriodType == 0) {
                    int Id_Znac;
                    TDateTime ValueDate;
                    Value = TDB_CellValue::GetValueUslPost(vd, Id_Znac, ValueDate);
                } else {
                    switch (vd->pokaz_type) {
                        case 1: { // число
                            double ValueD = TDB_CellValue::GetValue(vd->ObjectId, vd->CellId, vd->PlanId, vd->VidOtch,
                                                                    vd->ValueDate);
                            Value = std::to_string(ValueD);
                            break;
                        }
                        case 2: // строка
                            Value = TDB_CellValue::GetStrValue(vd->ObjectId, vd->CellId, vd->PlanId, vd->VidOtch,
                                                               vd->ValueDate);
                            break;
                        case 3: { // перечисление
                            double ValueD = TDB_CellValue::GetValue(vd->ObjectId, vd->CellId, vd->PlanId, vd->VidOtch,
                                                                    vd->ValueDate);
                            Value = TDB_Enum::GetEnumName(static_cast<int>(ValueD));
                            break;
                        }
                        case 4: // из множества
                            break;
                        case 5: // дата
                            Value = TDB_CellValue::GetStrValue(vd->ObjectId, vd->CellId, vd->PlanId, vd->VidOtch,
                                                               vd->ValueDate);
                            break;
                    }
                }
            }
        }
        TForm1::SetCellValue(FormLogicF1, FormLogicF1->f1Forms, p->MaxHrCol,
                             vd->Row, vd->Col, vd->Row, vd->Col, Value, vd->pokaz_type, FormatStyle);
        FormLogicF1->f1Forms->Selection_Set(vd->Row, vd->Col, vd->Row, vd->Col);
        FormLogicF1->f1Forms->Protection_Set(true, true);
        vd->ObjectId = objectidOld;
    }
}

// вычислить значение формулы конкатенации строк - характеристик Связанного объекта
// эта функция вызывается только тогда, когда зафиксирован связанный объект: при выборе или чтении
// причем вызывается через TFormDopInp.SetXarakterSvjzObjecta
std::string TForm2Plus::evalKonkatStrSvObj(TFormLogicF1 *FormLogicF1, PCellValueAtr vd, TwwStringList *p) {
    // id_SvjzClassa находится в vd.DPId
    // id_SvjzObjecta находится в vd.ObjectId
    return TForm2Plus::evalKonkatStr(FormLogicF1, vd, p);
}


// вычисление необходимых значений СУММАКОНТРАКТ прямо через 15 отчет - для всех субъектов
void
TForm2Plus::SummaKONTRAKTeval15all(bool isBO15all, bool isKassRasxBO15all, double &dBO15all, double &dKassRasxBO15all,
                                   int god, int is_filter, std::string shablon, TDateTime lastdate_15fcu) {
    std::string KBK;
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    std::string KBKpred; // КБК предыдущей строки - на случай выборки записей с одинаковыми КБК
    std::string uncPred;
    int WispKBK_id;

    if (!isBO15all && !isKassRasxBO15all) {
        dBO15all = 0;
        dKassRasxBO15all = 0;
        return;
    }

    KBK = shablon;
    if (is_filter == 1)
        KBK = "";

    dBO15all = 0;
    dKassRasxBO15all = 0;
    KBKpred = "";
    uncPred = "";

    q->SQL->Add(
            "SELECT unc, KBK, summaNaGod, summaByBO, oplacenoToBO, ostatokFree, ispolnKBKistor.ispKBK_id as ispKBK_id");
    q->SQL->Add("FROM ispolnKBK, ispolnKBKistor");
    q->SQL->Add("WHERE god = :god");
    q->SQL->Add("AND ispolnKBK.KBK LIKE :KBK");
    q->SQL->Add("AND Len(ispolnKBK.KBK) = :Len");
    q->SQL->Add("AND ispolnKBK.ispKBK_id = ispolnKBKistor.ispKBK_id");
    q->SQL->Add("AND CAST(FLOOR(CAST(dateImpFCU AS FLOAT)) AS DATETIME) <= :dateImpFCU");
    q->SQL->Add("ORDER BY unc, KBK, dateImpFCU DESC");
    q->ParamByName("god")->AsInteger() = god;
    q->ParamByName("dateImpFCU")->AsDateTime() = lastdate_15fcu;
    q->ParamByName("KBK")->AsString() = KBK + "%";
    q->ParamByName("Len")->AsInteger() = TDB_bkMetaDef::GetDl_All(god);

    q->Open();
    WispKBK_id = 0;
    while (!q->EoF()) {
        if (WispKBK_id == q->FieldByName("ispKBK_id")->AsInteger()) goto PNext;
        WispKBK_id = q->FieldByName("ispKBK_id")->AsInteger();

        if (uncPred == q->FieldByName("unc")->AsString() && KBKpred == q->FieldByName("KBK")->AsString()) goto PNext;
        KBKpred = q->FieldByName("KBK")->AsString();
        uncPred = q->FieldByName("unc")->AsString();

        dBO15all += q->FieldByName("summaByBO")->AsFloat();
        dKassRasxBO15all += q->FieldByName("oplacenoToBO")->AsFloat();
        PNext:
        q->Next();
    }

    //MAK q->Free();
    delete q;
}

// оператор СУММАшаблонКБК
void TForm2Plus::SummaPatternKBK(TFormLogicF1 *FormLogicF1, int aI, TwwStringList *p, int pokaz_idRoot) {
    ;
}

// вычислить сводные данные по Целевым программам и подпрограммам
void TForm2Plus::doSvodForCelPrgrm(TFormLogicF1 *FormLogicF1) {
    TwwStringList *p = TFormDopInp::GetTww(FormLogicF1);
    TStringList nl;
    TList<int> tl;
    int cKBK = 0, cKBKrd = 0, cKBKcl = 0, cKBKvr = 0, cKBKku = 0;
    TFormatStyle *FormatStyle = new TFormatStyle();

    auto isVR = [&](int aI) -> bool {
        std::string ws;
        if (cKBK > 0) {
            ws = char16ToUTF8(FormLogicF1->f1Forms->TextRC_Get(aI, cKBK));
        } else {
            ws = char16ToUTF8(FormLogicF1->f1Forms->TextRC_Get(aI, cKBKrd)) +
                 char16ToUTF8(FormLogicF1->f1Forms->TextRC_Get(aI, cKBKcl)) +
                 char16ToUTF8(FormLogicF1->f1Forms->TextRC_Get(aI, cKBKvr));
            if (cKBKku > 0) {
                ws += char16ToUTF8(FormLogicF1->f1Forms->TextRC_Get(aI, cKBKku));
            }
        }
        ws = AnsiReplaceStr(ws, " ", "");
        return ws.length() == TDB_bkMetaDef::GetDl_R_CS_VR(FormLogicF1->CurrYear);
    };

    for (int i = 1; i <= FormLogicF1->f1Forms->LastCol; i++) {
        if (i == TFormDopInp::GetNameCol(FormLogicF1)) continue;
        std::string stmnt = p->ListStmnt[i].Stmnt;
        if (stmnt == "ФОРМУЛАF1" || stmnt == "ЗНАЧЕНИЕСВ" || stmnt == "ЗНАЧЕНИЕСВНИ" ||
            stmnt == "ЗНАЧЕНИЕСВИР" || stmnt == "ФактВсегоВНачале" || stmnt == "УточПланВклДату") {
            tl.Add(i);
        }
    }

    for (int i = 1; i <= FormLogicF1->f1Forms->LastCol; i++) {
        if (i == TFormDopInp::GetNameCol(FormLogicF1)) continue;
        std::string stmnt = p->ListStmnt[i].Stmnt;
        if (stmnt == "КОДБК" || stmnt == "КОДБК1") cKBK = i;
        if (stmnt == "КОДБКРздл" || stmnt == "КОДБКРздл1") cKBKrd = i;
        if (stmnt == "КОДБКЦелСт" || stmnt == "КОДБКЦелСт1") cKBKcl = i;
        if (stmnt == "КОДБКВидРасх") cKBKvr = i;
        if (stmnt == "КОДБКЭкКл") cKBKku = i;
    }

    if ((cKBK == 0) && (cKBKcl == 0)) {
        throw std::runtime_error("В форме отсутствует столбец К��К или целевой статьи");
    }
    if ((cKBK == 0) && (cKBKcl != 0) && (cKBKrd == 0)) {
        throw std::runtime_error("В форме отсутствует столбец КБК или раздела");
    }
    if ((cKBK == 0) && (cKBKcl != 0) && (cKBKrd != 0) && (cKBKvr == 0)) {
        throw std::runtime_error("В форме отсутствует столбец КБК или вида расходов");
    }
    int LastRow = FormLogicF1->f1Forms->LastRow_Get();
    for (int i = 1; i <= LastRow; i++) {
        PAtrRow AtrRow = TFormDopInp::GetListRowsItem(FormLogicF1, i);
        if (AtrRow == nullptr) continue;
        int CurRowVid = AtrRow->CurRowVid;
        switch (CurRowVid) {
            case 18:
            case 19:
            case 20:
            case 21: {
                TForm2Plus::ClearNList(FormLogicF1, tl, CurRowVid, i, nl, cKBK, cKBKrd, cKBKcl, cKBKvr, cKBKku);
                nl.AddObject(IntToStr(CurRowVid), i);
                for (int j = 0; j < tl.Count() - 1; j++) {
                    int wn = j;
                    TFormDopInp::getColFrmtrAtr(FormLogicF1, 0, wn, *FormatStyle);
                    if (p->ListStmnt[wn].Stmnt != "ФОРМУЛАF1") {
                        TForm1::SetCellValue(FormLogicF1, FormLogicF1->f1Forms, p->MaxHrCol, i, wn, i, wn, "0", 1,
                                             FormatStyle);
                    }
                    FormLogicF1->f1Forms->Selection_Set(i, wn, i, wn);
                    FormLogicF1->f1Forms->Protection_Set(true, true);
                }
                break;
            }
            case 2:
            case 13: {
                if (AtrRow->pokaz_vert > 0) {
                    if (isVR(i)) {
                        for (int j = 0; j < tl.Count() - 1; j++) {
                            int wn = j;
                            if (p->ListStmnt[wn].Stmnt != "ФОРМУЛАF1") {
                                for (int k = nl.Count() - 1; k >= 0; k--) {
                                    int wk = std::stoi(nl[k]);
                                    FormLogicF1->f1Forms->NumberRC_Set(wk, wn,
                                                                       FormLogicF1->f1Forms->NumberRC_Get(wk, wn) +
                                                                       FormLogicF1->f1Forms->NumberRC_Get(i, wn));
                                }
                            }
                        }
                    }
                }
                break;
            }
        }
    }
}

// чистка списка nl
void TForm2Plus::ClearNList(TFormLogicF1 *FormLogicF1, TList<int> tl, int aRowVid, int aN, TStringList nl, int cKBK,
                            int cKBKrd, int cKBKcl, int cKBKvr, int cKBKku) {
    int i;
    std::string csb_, csb;

    auto getCSfromStr = [&](int aI) -> std::string {
        std::string ws;
        if (cKBK > 0)
            ws = char16ToUTF8(FormLogicF1->f1Forms->TextRC_Get(aI, cKBK));
        else
            ws = char16ToUTF8(FormLogicF1->f1Forms->TextRC_Get(aI, cKBKcl));
        ws = AnsiReplaceStr(ws, " ", "");
        return getKBK_CS(ws, FormLogicF1->CurrYear);
    };

    switch (aRowVid) {
        case 18:
            nl.Clear();
            break;
        case 19:
            for (i = nl.Count() - 1; i >= 0; i--) {
                if (StrToInt(nl[i]) >= 19)
                    nl.Delete(i);
            }
            break;
        case 20:
            for (i = nl.Count() - 1; i >= 0; i--) {
                if (StrToInt(nl[i]) > 19)
                    nl.Delete(i);
            }
            break;
        case 21:
            csb_ = getCSfromStr(aN);
            csb_ = csb_.substr(0, 5);
            for (i = nl.Count() - 1; i >= 0; i--) {
                switch (StrToInt(nl[i])) {
                    case 18:
                    case 19:
                        break;
                    case 20:
                        csb = getCSfromStr(std::get<int>(nl.Objects[i]));
                        csb = csb.substr(0, 5);
                        if (csb_ != csb)
                            nl.Delete(i);
                        break;
                    case 21:
                        nl.Delete(i);
                        break;
                }
            }
            break;
    }
}

// вычисление различных значений для месяца
void TForm2Plus::ExecForMonth(TFormLogicF1 *FormLogicF1, int nRow) {
    PCellValueAtr vd;
    int i;
    std::string Value;
    double ValueD;
    std::string stmnt;
    TwwStringList *p = TFormDopInp::GetTww(FormLogicF1);
    int objectid;
    int CellId;
    int PlanId;
    int VidOtc;
    int Year, Month, Day;

//MAK    for (i = 0; i < TFormDopInp::GetCellValueAtrCount(FormLogicF1); i++) {
//MAK        vd = TFormDopInp::GetPCellValueAtrByIndex(FormLogicF1, i);
    for (auto it = TFormDopInp::GetTww(FormLogicF1)->items.begin();
         it != TFormDopInp::GetTww(FormLogicF1)->items.end(); ++it) {
        auto vd = std::get<PCellValueAtr>(it->second);
        stmnt = p->ListStmnt[vd->Col].Stmnt;
        if (vd->DataType == (30 + deltaDType)) continue;
        if (vd->Row != nRow) continue;
        if (vd->DataType > (10 + deltaDType)) {
            Value = "";
            ValueD = 0;
            switch (vd->DataType) {
                case 11 + deltaDType:  // расчет за месяц
                    objectid = vd->ObjectId;
                    CellId = vd->CellId;
                    PlanId = vd->PlanId;
                    VidOtc = vd->VidOtch;
                    DecodeDate(vd->ValueDate, Year, Month, Day);
                    ValueD = TDB_DenegnPotok::GetValueMonth(objectid, CellId, PlanId, VidOtc, Year, Month);
                    FormLogicF1->f1Forms->NumberRC_Set(vd->Row, vd->Col, ValueD);
                    break;
                case 21 + deltaDType:  // 'ПлнОСТАТОК'
                    ValueD = TForm2Plus::Ostatok(FormLogicF1, vd->PlanId, vd->CellId - 1, vd->CellId, vd->PokazId, 4,
                                                 vd->ValueDate, vd->ObjectId);
                    FormLogicF1->f1Forms->NumberRC_Set(vd->Row, vd->Col, ValueD);
                    break;
                case 22 + deltaDType:  // 'ФктОСТАТОК'
                    ValueD = TForm2Plus::Ostatok(FormLogicF1, vd->PlanId, vd->CellId - 1, vd->CellId, vd->PokazId, 5,
                                                 vd->ValueDate, vd->ObjectId);
                    FormLogicF1->f1Forms->NumberRC_Set(vd->Row, vd->Col, ValueD);
                    break;
                case 24 + deltaDType:  // 'ПлнОСТАТОКНИ'
                    ValueD = TForm2Plus::OstatokNI(FormLogicF1, vd->PlanId, vd->CellId - 1, vd->CellId, vd->PokazId, 4,
                                                   vd->ValueDate, vd->ObjectId);
                    if (ValueD != 0)
                        Value = std::to_string(PreobrValue(ValueD, vd->DataType, vd->coef_Pokaz, vd->coef_Forma));
                    FormLogicF1->f1Forms->NumberRC_Set(vd->Row, vd->Col, ValueD);
                    break;
            }
        }
    }
}

// распростанить служебную строку вида В том числе на другие столбцы с наименованием
void TForm2Plus::doOutputVTomCisle(TFormLogicF1 *FormLogicF1, int nRow, std::string VertStr) {
    int i;
    string stmnt;
    TwwStringList *p;
    TFormatStyle *FormatStyle = new TFormatStyle();

    p = TFormDopInp::GetTww(FormLogicF1);
    for (i = 1; i <= FormLogicF1->f1Forms->LastCol; i++) {
        stmnt = p->ListStmnt[i].Stmnt;
        if (stmnt == "НАИМПОКАЗ") {
            if (!TFormDopInp::getColNameFrmtrAtr(FormLogicF1, i, *FormatStyle)) {
                FormatStyle->setToNull();
                FormatStyle->align_vert = 1;
                FormatStyle->align_gor = 1;
                FormatStyle->font = "";
                FormatStyle->h = 0;
                FormatStyle->is_bold = 0;
                FormatStyle->isOrientation = 0;
                FormatStyle->FontColor = 0;
            }
            TForm1::SetCellValue(FormLogicF1, FormLogicF1->f1Forms, p->MaxHrCol, nRow, i, nRow, i, VertStr, 200,
                                 FormatStyle);
        }
    }
}

void TForm2Plus::doZamenaInForm2(TwwStringList *p, TFormLogicF1 *FormLogicF1, std::string note1, std::string note2) {
    int nRow;
    int i;
    std::string Value, sFrml, s;

    auto setAlignment = [&](int aRow, int aCol) {
        TFormatStyle *FormatStyle = new TFormatStyle();
        TF1CellFormat cf;
        TFormDopInp::getColFrmtrAtr(FormLogicF1, 1, aCol, *FormatStyle);
        FormLogicF1->f1Forms->Selection_Set(aRow, aCol, aRow, aCol);
        cf.GetCellFormat(*FormLogicF1->f1Forms);
        switch (FormatStyle->align_gor) {
            case 1:
                cf.AlignHorizontal_Set(static_cast<int16_t>(F1HAlignConstants::F1HAlignLeft));
                break;
            case 0:
                break; // по умолчанию
            case 2:
                cf.AlignHorizontal_Set(static_cast<int16_t>(F1HAlignConstants::F1HAlignCenter));
                break;
            case 3:
                cf.AlignHorizontal_Set(static_cast<int16_t>(F1HAlignConstants::F1HAlignRight));
                break;
        }
        cf.SetCellFormat(*FormLogicF1->f1Forms);
    };

    if (!TFormSsilkaNaStr::findSsilkaNaStr(note1, nRow)) {
        throw std::runtime_error("При обработке оператора Замена обнаружено, что метка " + note1 + " не определена");
    }

    for (i = 1; i <= p->MaxHrCol; i++) {
        if (i == TFormDopInp::GetNameCol(FormLogicF1)) continue;

        Value = TFormDopInp::GetFormulaForColpar(note2, i);
        sFrml = TFormDopInp::ReplaceNRowNCol(FormLogicF1->CurrInputRow, i, Value);

        if (sFrml.empty()) continue;
        if (sFrml[0] == '^') sFrml.erase(0, 1);

        try {
            FormLogicF1->f1Forms->FormulaRC_Set(nRow, i, utf8_to_utf16(sFrml).c_str());
            setAlignment(nRow, i);
        } catch (...) {
            s = "Строка " + std::to_string(FormLogicF1->CurrInputRow) +
                " Содержимое ячейки с ошибочной формулой при замене = " + sFrml;
            TracerManager::Instance().AddSpanLog("Exception.TForm2Plus::doZamenaInForm2", {{"errordesr", s}},
                                                 TracerManager::LOG_LEVEL_ERROR);
            throw std::runtime_error(s + ". Неверная формула");
        }
    }
}

bool TForm2Plus::getDocumFilesInfo(const std::string &Docum_id, TStringList &fileFullNamesAndNotes) {

    TQueryUni *q = new TQueryUni(DefDBconnect.DocumDBConn);

    q->SQL->Clear();
    q->SQL->Add("SELECT filename, note, ordern FROM docum_files");
    q->SQL->Add("WHERE docum_id = :docum_id");
    q->SQL->Add("ORDER BY ordern ASC");
    q->ParamByName("docum_id")->AsInteger() = 0;
    //mak ??? TAPIDocum.GetInt64(Docum_id);
    q->Open();

    if (q->EoF()) {
        return false;
    } else {
        while (!q->EoF()) {
            std::string sNote = q->FieldByName("note").AsString();
            if (sNote.empty()) sNote = "{}"; // If note is empty, set it to "{}"
            fileFullNamesAndNotes.Values[q->FieldByName("filename").AsString()] = sNote;
            q->Next();
        }
        return true;
    }

    // Free the query object
    //MAK q->Free();
    delete q;
}

std::string TForm2Plus::getFileDatesFromJSON(const std::string &JSONString) {
    nlohmann::json j = nlohmann::json::parse(JSONString);

    std::string dAdd, dUpd;

    if (j.contains("dAdd")) {
        dAdd = "Доб. " + j["dAdd"].get<std::string>();
    }

    if (j.contains("dUpd")) {
        dUpd = "Обн. " + j["dUpd"].get<std::string>();
    }

    if (dUpd.empty()) {
        return "(" + dAdd + ")";
    } else {
        return "(" + dAdd + ", " + dUpd + ")";
    }
}

// вывод стандартного текста для blob value - тип Документ
void TForm2Plus::SetTextDocument(TFormLogicF1 *FormLogicF1, int nRow, int nCol, bool isValue) {
    TwwStringList *p = TFormDopInp::GetTww(FormLogicF1);
    FormLogicF1->f1Forms->Selection_Set(nRow, nCol, nRow, nCol);
    FormLogicF1->f1Forms->Protection_Set(true, true);
    FormLogicF1->f1Forms->HAlign = static_cast<int16_t>(F1HAlignConstants::F1HAlignCenter);
    FormLogicF1->f1Forms->VAlign = static_cast<int16_t>(F1VAlignConstants::F1VAlignCenter);
    FormLogicF1->f1Forms->WordWrap = true;
    if (!isValue) {
        FormLogicF1->f1Forms->Text_Set(utf8_to_utf16("").c_str());
        return;
    }

    int formatType = p->ListStmnt[nCol].tocnost;

    switch (formatType) {
        case 1:
        case 2: {
            PCellValueAtr vd = TFormDopInp::GetCellValueAtrByRC(FormLogicF1, nRow, nCol);
            string stmnt = p->ListStmnt[nCol].Stmnt;
            string Docum_id;
            bool rtn;
            if (stmnt == "ЗНАЧЕНИЕобщНП")
                rtn = TDocumentValueLogic::GetDocumid(Docum_id, vd->PokazId, vd->comnp_id, vd->cell_period, vd->god,
                                                      vd->kvrtl, vd->mesjc, vd->den, vd->PlanId, vd->VidOtch);
            else
                rtn = TDocumentValueLogic::GetDocumid(vd->ObjectId, vd->CellId, vd->PlanId, vd->VidOtch, vd->ValueDate,
                                                      Docum_id);
            if (!rtn)
                return;
            break;
        }
    }

    if (formatType == 0) {
        string ws = "Документ";
        FormLogicF1->f1Forms->Text_Set(utf8_to_utf16(ws).c_str());
        FormLogicF1->f1Forms->FontUnderline_Set(true);
        FormLogicF1->f1Forms->FontColor_Set(SystFuncs::SystFuncsAltWin::GetColorFromRGB(0, 0, 250));
        return;
    }
    std::string Docum_id;
    TStringList fileFullNamesAndNotes;
    bool rtn = getDocumFilesInfo(Docum_id, fileFullNamesAndNotes);
    string ws = "";

    if (rtn) {
        switch (formatType) {
            case 1:
                for (int i = 0; i < fileFullNamesAndNotes.Count() - 1; i++)
                    ws = ws + fileFullNamesAndNotes.Names[i] + "\r\n\r\n";
                break;
            case 2:
                for (int i = 0; i < fileFullNamesAndNotes.Count() - 1; i++)
                    ws = ws + fileFullNamesAndNotes.Names[i] + ' ' +
                         getFileDatesFromJSON(fileFullNamesAndNotes.Values[fileFullNamesAndNotes.Names[i]]) +
                         "\r\n\r\n";
                break;
        }
        FormLogicF1->f1Forms->Text_Set(utf8_to_utf16(ws).c_str());
        FormLogicF1->f1Forms->FontUnderline_Set(true);
        FormLogicF1->f1Forms->FontColor_Set(SystFuncs::SystFuncsAltWin::GetColorFromRGB(0, 0, 250));
    }
}









