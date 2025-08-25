#include "SystFuncsLogic.h"
#include "SystFuncsAltWin.h"
#include "unTF1CellFormat.h"
#include "DB_FormStr.h"
#include "DB_DenegnPotok.h"
#include "FormUpdF1Logic.h"
#include "FormEnd.h"

/////////////////////////////////////////////////////////////
// В этом модуле собраны функции, которые вызываются в конце формирования в F1Book
// Эти функции обращаются к значениям в ячейке
// и к пространственным характеристикам ячеек
// обращение к БД отсутствует
////////////////////////////////////////////////////////////

// переносим наименование в другую ячейку
// а также суррогатное видимое имя заменяется на ... (троеточние)
void TFormEnd::SetNameNewCol(int formId, TFormLogicF1 * formLogicF1, int begRow, int endRow) {
    int oldCol = 0;
    if (begRow == 0) return;

    int nameCol = TFormDopInp::GetNameCol(formLogicF1);
    bool isNameColHidden = formLogicF1->f1Forms->ColHidden_Get(nameCol);

    for (int i = begRow; i <= endRow; i++) {
        formLogicF1->f1Forms->Selection_Set(i, nameCol, i, nameCol);
        TF1CellFormat cf;
        cf.GetCellFormat(*formLogicF1->f1Forms);

        std::string s = char16ToUTF8(formLogicF1->f1Forms->Text_Get());
        if (s.empty()) continue;
        int nL = s.length();
        if (s[nL - 1] != '#') {
            if (!isNameColHidden && nL > 5 && Pos("_srgt", s) >= 0) {
                formLogicF1->f1Forms->Text_Set(utf8_to_utf16("...").c_str());
                formLogicF1->f1Forms->HAlign_Set(static_cast<int16_t>(F1HAlignConstants::F1HAlignCenter));// static_cast<int16_t>(F1HAlignConstants::F1HAlignCenter)
                formLogicF1->f1Forms->VAlign_Set(static_cast<int16_t>(F1VAlignConstants::F1VAlignCenter)); // static_cast<int16_t>(F1VAlignConstants::F1VAlignCenter)
            }
            continue;
        }

        int m = 0;
        if (s.empty() || nL == 0) continue;
        for (int j = nL - 1; j >= 0; j--) {
            if (s[j] == '#') {
            	// между двумя знаками '#' находится номер нового столбца Наименования
                std::string ws = s.substr(j + 1, nL - (j + 1));
                if (!ws.empty()) m = std::stoi(ws);
                break;
            }
        }
        if (m == 0) continue;

        formLogicF1->f1Forms->Text_Set(utf8_to_utf16("").c_str());
        formLogicF1->f1Forms->Selection_Set(i, m, i, m);
        if (oldCol != m) oldCol = m;

        int alignVert, alignHor;
        TDB_FormStr::GetAlignment(formId, nameCol + oldCol, alignVert, alignHor);

        std::string font;
        int height, isBold;
        TDB_FormStr::GetFontInfo(formId, nameCol + oldCol, font, height, isBold);

        bool isBoldBool = false, isItalicBool = false;
        switch (isBold) {
        case 0: isBoldBool = cf.FontBold_Get(); break;
        case 1: isBoldBool = true; break;
        case 2: isItalicBool = true; break;
        case 3: isBoldBool = true; isItalicBool = true; break;
        }

        cf.WordWrap_Set(true);
        switch (alignVert) {
        case 1: cf.AlignVertical_Set( 1); break; // static_cast<int16_t>(F1VAlignConstants::F1VAlignTop)
        case 2: cf.AlignVertical_Set( 2); break; // static_cast<int16_t>(F1VAlignConstants::F1VAlignCenter)
        case 3: cf.AlignVertical_Set( 3); break; // static_cast<int16_t>(F1VAlignConstants::F1VAlignBottom)
        }
        switch (alignHor) {
        case 1: cf.AlignHorizontal_Set( 1); break; // static_cast<int16_t>(F1HAlignConstants::F1HAlignLeft)
        case 2: cf.AlignHorizontal_Set( 2); break; // static_cast<int16_t>(F1HAlignConstants::F1HAlignCenter)
        case 3: cf.AlignHorizontal_Set( 3); break; // static_cast<int16_t>(F1HAlignConstants::F1HAlignRight)
        }

        if (!font.empty()) cf.FontName_Set(font);
        else font = "Arial";
        if (height > 0) cf.FontSize_Set(height);
        else height = 10;
        cf.FontBold_Set( isBoldBool);
        cf.FontItalic_Set( isItalicBool);

        cf.SetCellFormat(*formLogicF1->f1Forms);
        formLogicF1->f1Forms->Text_Set(utf8_to_utf16(s.substr(0, nL - 1)).c_str());
    }
}

// делает столбец с наименованием невидимым
void TFormEnd::SetNameHidden(TFormLogicF1* FormLogicF1, int BegRow) {
// isNameHidden - true, если скрывать столбец с наименованием показателя	
    int N = TFormDopInp::GetNameCol(FormLogicF1);
    TFormDopInp::SetCol1Hidden(FormLogicF1, BegRow, N);
}

// делаем невидимыми строки, не соответствующие второму фильтру
// так же обрабатывается оператор ПОЛОЖИТЬ
// так же обрабатывается окраска Locked ячеек
// так же обрабатывается окраска ячеек с Контексным меню (программируемым)
// так же обрбатывается явно заданная окраска столбца и строки


void TFormEnd::SetHiddenByFilter2(TFormLogicF1* FormLogicF1, int BegRow, int EndRow) {
    TwwStringList* p = TFormDopInp::GetTww(FormLogicF1);
    int R1, C1, R2, C2;
    FormLogicF1->f1Forms->Selection_Get(0, R1, C1, R2, C2);

    bool isRsdl = false;
    bool isFilter = false;
    int rsdlNRow = 0;

    auto doOpPologit_Pokrasit = [&](int aRow) {
        PAtrRow wAtrRow;
        bool bLocked, bHidden;
        PCellValueAtr vd, wvd;

        if (!(p->ColorLockedCell == 0 || p->ColorLockedCell == 16777215)) {
            for (int wCol = 1; wCol <= p->MaxHrCol; wCol++) {
                if (wCol != p->PokazCol && wCol != p->PokazColAlter && wCol != p->PokazColother && wCol > p->is_noout_nstr) {
                    FormLogicF1->f1Forms->Selection_Set(aRow, wCol, aRow, wCol);
                    FormLogicF1->f1Forms->Protection_Get(bLocked, bHidden);
                    if (!bLocked) continue;
                    vd = TFormDopInp::GetCellValueAtrByRC(FormLogicF1, aRow, wCol);
                    if (!vd) continue;
                    if (vd->EvalType == 0 && (vd->pokaz_type == 3 || vd->pokaz_type == 4 || vd->pokaz_type == 5)) continue;

                    if (p->form_readonly == 5) {
                        wAtrRow = TFormDopInp::GetListRowsItem(FormLogicF1, aRow);
                        if (!p->ListStmnt[wCol].read_only) {
                            wvd = TFormDopInp::GetCellValueAtrByRC(FormLogicF1, aRow - 1, wCol);
                            if (aRow == (p->form_k_zagtop + p->form_k1 + 1)) {
                                wvd = TFormDopInp::GetCellValueAtrByRC(FormLogicF1, aRow, wCol);
                            }
                            if (wvd && wvd->EvalType != -1 && !wAtrRow->isReadOnly && vd->EvalType != 1) continue;
                        }
                    }

                    FormLogicF1->f1Forms->BackgroundColor_Set(p->ColorLockedCell);
                }
            }
        }

        if (aRow <= BegRow) return;

        wAtrRow = TFormDopInp::GetListRowsItem(FormLogicF1, aRow - 1);
        if (!wAtrRow || wAtrRow->CurRowVid != 16) return;

        for (int wCol = 1; wCol <= p->MaxHrCol; wCol++) {
            FormLogicF1->f1Forms->Selection_Set(aRow - 1, wCol, aRow - 1, wCol);
            if (!(FormLogicF1->f1Forms->Type == -1 || FormLogicF1->f1Forms->Type == -2)) continue;
            FormLogicF1->f1Forms->FormulaRC(aRow, wCol) = utf8_to_utf16(TFormDopInpMF::ConvertColToSimvs(wCol) + std::to_string(aRow - 1)).c_str();
            FormLogicF1->f1Forms->Selection_Set(aRow, wCol, aRow, wCol);
            FormLogicF1->f1Forms->Protection_Set(true, true);
            if (!(p->ColorLockedCell == 0 || p->ColorLockedCell == 16777215)) {
                FormLogicF1->f1Forms->Pattern_Set(2, p->ColorLockedCell, 16777215);
            }
        }
    };

    // Определяем функцию заранее
    std::function<int(int, int)> doHiddenSubObjects;

    doHiddenSubObjects = [&](int aRow, int wOwnerObjectId) -> int {
        int ii = aRow + 1;
        while (ii <= EndRow) {
            if (ii > EndRow) return ii;
            PAtrRow wAtrRow = TFormDopInp::GetListRowsItem(FormLogicF1, ii);
            if (!wAtrRow) return ii;
            if (wAtrRow->OwnerId != wOwnerObjectId) return ii;
            FormLogicF1->f1Forms->RowHidden[ii] = true;
            ii = doHiddenSubObjects(ii, wAtrRow->ObjectId);
        }
        return ii;
    };

    std::function<void(int&, bool&)> doHiddenRsdl;
     doHiddenRsdl = [&](int& aRsdlNRow, bool& aIsRsdlHidden) {
        int i = aRsdlNRow + 1;
        PAtrRow wAtrRow = TFormDopInp::GetListRowsItem(FormLogicF1, aRsdlNRow);
        int rsdl_id = wAtrRow->ObjectId;

        while (i <= EndRow) {
            wAtrRow = TFormDopInp::GetListRowsItem(FormLogicF1, i);
            if (wAtrRow->CurRowVid == 2) {
                if (!FormLogicF1->f1Forms->RowHidden[i]) aIsRsdlHidden = false;
                i++;
            } else if (wAtrRow->CurRowVid == 1) {
                bool isRsdlHidden = true;
                int rsdlNRow = i;
                doHiddenRsdl(rsdlNRow, isRsdlHidden);
                if (!isRsdlHidden) aIsRsdlHidden = false;
                i = rsdlNRow + 1;
            } else if (wAtrRow->CurRowVid == 3) {
                if (aIsRsdlHidden) {
                    if (wAtrRow->is_filter != 15) {
                        FormLogicF1->f1Forms->RowHidden[aRsdlNRow] = true;
                        FormLogicF1->f1Forms->RowHidden[i] = true;
                    }
                }
                aRsdlNRow = i;
                return;
            } else if (wAtrRow->CurRowVid == 4) {
                if (!FormLogicF1->f1Forms->RowHidden[i]) aIsRsdlHidden = false;
                i++;
            } else {
                i++;
            }
        }
    };

    auto doApplyUslovFormatir = [&](int nRow) {
        for (int nCol = 1; nCol <= p->MaxHrCol; nCol++) {
            PAtrStmnt AtrStmnt = &(p->ListStmnt[nCol]);
             TF1CellFormat cf;
            if (AtrStmnt->CondFormats == nullptr) continue;

            for (const auto& condFormat : AtrStmnt->CondFormats) {
                double frmtdValue = (char16ToUTF8(FormLogicF1->f1Forms->TextRC(nRow,nCol)) == "") ? 0 : FormLogicF1->f1Forms->NumberRC(nRow,nCol);
                int uslovie = condFormat["uslovieType"];
                double value1 = condFormat["value1"];
                double value2 = condFormat["value2"];
                int backgColor = condFormat["backgColor"];
                int textColor = condFormat["textColor"];
                bool entireLine = condFormat.contains("colorEntireLine") && condFormat["colorEntireLine"].get<bool>();

                auto doApplyColors = [&](int nRow, int nCol, bool entireLine) {
                    if (!entireLine) {
                        FormLogicF1->f1Forms->Selection_Set(nRow, nCol, nRow, nCol);
                        cf.GetCellFormat(*FormLogicF1->f1Forms);
                        cf.FontColor_Set(SystFuncs::SystFuncsAltWin::GetColorFromRGB(SystFuncs::SystFuncsAltWin::GetColorR(textColor), SystFuncs::SystFuncsAltWin::GetColorG(textColor), SystFuncs::SystFuncsAltWin::GetColorB(textColor)));
                        cf.PatternFG_Set(SystFuncs::SystFuncsAltWin::GetColorFromRGB(SystFuncs::SystFuncsAltWin::GetColorR(backgColor), SystFuncs::SystFuncsAltWin::GetColorG(backgColor), SystFuncs::SystFuncsAltWin::GetColorB(backgColor)));
                        cf.SetCellFormat(*FormLogicF1->f1Forms);
                    } else {
                        for (int kCol = 1; kCol <= p->MaxHrCol; kCol++) {
                            FormLogicF1->f1Forms->Selection_Set(nRow, kCol, nRow, kCol);
                            cf.GetCellFormat(*FormLogicF1->f1Forms);
                            cf.FontColor_Set(SystFuncs::SystFuncsAltWin::GetColorFromRGB(SystFuncs::SystFuncsAltWin::GetColorR(textColor), SystFuncs::SystFuncsAltWin::GetColorG(textColor), SystFuncs::SystFuncsAltWin::GetColorB(textColor)));
                            cf.PatternFG_Set(SystFuncs::SystFuncsAltWin::GetColorFromRGB(SystFuncs::SystFuncsAltWin::GetColorR(backgColor), SystFuncs::SystFuncsAltWin::GetColorG(backgColor), SystFuncs::SystFuncsAltWin::GetColorB(backgColor)));
                            cf.SetCellFormat(*FormLogicF1->f1Forms);
                        }
                    }
                };

                switch (uslovie) {
                    case 0:
                        if (frmtdValue == value1) doApplyColors(nRow, nCol, entireLine);
                        break;
                    case 1:
                        if (frmtdValue != value1) doApplyColors(nRow, nCol, entireLine);
                        break;
                    case 2:
                        if (frmtdValue < value1) doApplyColors(nRow, nCol, entireLine);
                        break;
                    case 3:
                        if (frmtdValue > value1) doApplyColors(nRow, nCol, entireLine);
                        break;
                    case 4:
                        if (frmtdValue <= value1) doApplyColors(nRow, nCol, entireLine);
                        break;
                    case 5:
                        if (frmtdValue >= value1) doApplyColors(nRow, nCol, entireLine);
                        break;
                    case 6:
                        if (frmtdValue >= value1 && frmtdValue <= value2) doApplyColors(nRow, nCol, entireLine);
                        break;
                    case 7:
                        if (frmtdValue != 0) doApplyColors(nRow, nCol, entireLine);
                        break;
                    case 8:
                        if (frmtdValue == 0) doApplyColors(nRow, nCol, entireLine);
                        break;
                }
            }
        }
    };

    for (int i = EndRow; i >= BegRow; i--) {
        TF1CellFormat cf;
        doOpPologit_Pokrasit(i);
        doApplyUslovFormatir(i);
        if (FormLogicF1->f1Forms->RowHidden[i]) continue;

        PAtrRow AtrRow = TFormDopInp::GetListRowsItem(FormLogicF1, i);
        if (!AtrRow) continue;

        for (int j = 1; j <= p->MaxHrCol; j++) {
            FormLogicF1->f1Forms->Selection_Set(i, j, i, j);
            PAtrStmnt AtrStmnt = &(p->ListStmnt[j]);

            if (AtrStmnt->Stmnt == "ОткрытьФормуПодчинОб" || AtrStmnt->Stmnt == "ОткрытьФормуТекущОб") {
                cf.GetCellFormat(*FormLogicF1->f1Forms);
                cf.FontUnderline_Set(true);
                cf.SetCellFormat(*FormLogicF1->f1Forms);
            } else if (AtrStmnt->ModulOfMetodsOnClick.find("\"RClick\":true") != std::string::npos) {
                cf.GetCellFormat(*FormLogicF1->f1Forms);
                cf.FontBold_Set( true);
                cf.FontItalic_Set( true);
                cf.FontColor_Set(SystFuncs::SystFuncsAltWin::GetColorFromRGB(0, 0, 255));
                cf.SetCellFormat(*FormLogicF1->f1Forms);
            }

            if (!(AtrStmnt->FormatStyle.fonColor == 0 || AtrStmnt->FormatStyle.fonColor == 16777215)) {
                FormLogicF1->f1Forms->Pattern_Set(2, AtrStmnt->FormatStyle.fonColor, 16777215);
            }
            if (!(AtrRow->FormatStyle.fonColor == 0 || AtrRow->FormatStyle.fonColor == 16777215)) {
                FormLogicF1->f1Forms->Pattern_Set(2, AtrRow->FormatStyle.fonColor, 16777215);
            }

            TFormEnd::doCondFormatir(FormLogicF1, i, j, false);
        }

        if (!(AtrRow->is_filter2 > 0)) continue;
        int nCol = AtrRow->ColOnFilter2;
        if (nCol <= 0) continue;
        if (nCol > FormLogicF1->f1Forms->LastCol) continue;
        FormLogicF1->f1Forms->Selection_Set(i, nCol, i, nCol);
        if (AtrRow->CurRowVid == 1) {
            isRsdl = true;
            rsdlNRow = i;
            continue;
        }
        if (AtrRow->is_filter2 > 0) isFilter = true;

        switch (AtrRow->is_filter2) {
            case 1:
            case 5:
            case 6:
                if (FormLogicF1->f1Forms->Type == 0) FormLogicF1->f1Forms->RowHidden[i] = true;
                if (FormLogicF1->f1Forms->Type == 1 || FormLogicF1->f1Forms->Type == -1) {
                    if (IsZero(FormLogicF1->f1Forms->Number, 0.0001)) {
                        switch (AtrRow->is_filter2) {
                            case 1:
                            case 6:
                                FormLogicF1->f1Forms->RowHidden[i] = true;
                                break;
                            case 5:
                                TFormUpdF1Logic::deleteRowFromF1(FormLogicF1, i);
                                break;
                        }
                    }
                }
                break;
            case 2:
                if (FormLogicF1->f1Forms->Type == 1 || FormLogicF1->f1Forms->Type == -1) {
                    if (FormLogicF1->f1Forms->Number != 0) FormLogicF1->f1Forms->RowHidden[i] = true;
                }
                break;
            case 3:
                if (FormLogicF1->f1Forms->Type == 0) FormLogicF1->f1Forms->RowHidden[i] = true;
                break;
            case 4:
                if (FormLogicF1->f1Forms->Type != 0) FormLogicF1->f1Forms->RowHidden[i] = true;
                break;
        }

        if (FormLogicF1->f1Forms->RowHidden[i] && p->work_with == 4 && AtrRow->CurRowVid == 4) {
            doHiddenSubObjects(i, AtrRow->ObjectId);
        }
    }

    if (isRsdl && isFilter) {
        bool isRsdlHidden = true;
        doHiddenRsdl(rsdlNRow, isRsdlHidden);
    }

    if (R1 > p->form_endBodyRow) R1 = p->form_endBodyRow;
    if (R2 > p->form_endBodyRow) R2 = p->form_endBodyRow;
    FormLogicF1->f1Forms->Selection_Set(R1, C1, R2, C2);
}






void setCellsColor(TFormLogicF1* FormLogicF1, int wCol1, int wColN, int BackGroundColor, int aRow) {
    TF1CellFormat cf;
    for (int wCol = wCol1; wCol <= wColN; ++wCol) {
        FormLogicF1->f1Forms->Selection_Set(aRow, wCol, aRow, wCol);
        cf.GetCellFormat(*FormLogicF1->f1Forms);
        cf.PatternBG_Set(BackGroundColor);
        cf.PatternStyle_Set( 0);
        cf.PatternFG_Set(cf.PatternBG_Get());
        cf.SetCellFormat(*FormLogicF1->f1Forms);
    }
}

// обработка условного форматирования
void TFormEnd::doCondFormatir(TFormLogicF1* FormLogicF1, int aRow, int aCol, bool isEdit) {
    auto p = TFormDopInp::GetTww(FormLogicF1);
    auto AtrStmnt = &(p->ListStmnt[aCol]);

    if (AtrStmnt->CondFormats == nullptr) return;

    std::string strVarinat = char16ToUTF8(FormLogicF1->f1Forms->TextRC(aRow,aCol));

    auto cases = AtrStmnt->CondFormats.find("cases");
    if (cases == AtrStmnt->CondFormats.end()) return;

    auto vrnt = (*cases).find(strVarinat);
    if (vrnt == (*cases).end()) return;

    auto def = AtrStmnt->CondFormats.find("def");
    if (def == AtrStmnt->CondFormats.end()) return;

    int BackGroundColor;
    if (!(*vrnt)["bgColor"].is_null()) {
        BackGroundColor = (*vrnt)["bgColor"];
        if (isEdit) {
            if ((*def)["colorEntireLine"].is_null() || !(*def)["colorEntireLine"]) {
                FormLogicF1->f1Forms->Pattern_Set(0, BackGroundColor, 16777215);
            } else {
                for (int wCol = 1; wCol <= FormLogicF1->f1Forms->MaxCol; ++wCol) {
                    FormLogicF1->f1Forms->Selection_Set(aRow, wCol, aRow, wCol);

                    FormLogicF1->f1Forms->Pattern_Set(0, BackGroundColor, BackGroundColor);
                }
            }
        } else {
            FormLogicF1->f1Forms->Selection_Set(aRow, aCol, aRow, aCol);
            FormLogicF1->f1Forms->Pattern_Set(0, BackGroundColor, BackGroundColor);
            if ((*def)["colorEntireLine"].is_null() || !(*def)["colorEntireLine"]) return;
            for (int wCol = 1; wCol <= FormLogicF1->f1Forms->MaxCol; ++wCol) {
                FormLogicF1->f1Forms->Selection_Set(aRow, wCol, aRow, wCol);
                FormLogicF1->f1Forms->Pattern_Set(0, BackGroundColor, BackGroundColor);
            }
        }
    }
}

// объединяем ячейки
void TFormEnd::SetAlignmentObjedinit(int form_id, TFormLogicF1* FormLogicF1, int BegRow, int EndRow) {
    TwwStringList * wp = TFormDopInp::GetTww(FormLogicF1);
    int form_str_n = wp->form_str_n_AlignmentObjedinit;
    if (form_str_n == 0) return;
    int is_group;
    TDB_FormStr::GetObjedinitKolCell(form_id, form_str_n, is_group);
    int align_vert, align_hor;
    TDB_FormStr::GetAlignment(form_id, form_str_n, align_vert, align_hor);
    std::string font;
    int h, is_bold;
    TDB_FormStr::GetFontInfo(form_id, form_str_n, font, h, is_bold);
    int begCol = TFormDopInp::GetNameCol(FormLogicF1);
    int nameCol = begCol;
    int endCol = begCol + is_group - 1;
    for (int i = begCol; i <= endCol; i++) {
        if (FormLogicF1->f1Forms->ColHidden_Get(i)) continue;
        begCol = i;
        break;
    }
    if (endCol > FormLogicF1->f1Forms->LastCol_Get())
      endCol = FormLogicF1->f1Forms->LastCol_Get();
    int R1, C1, R2, C2;
    FormLogicF1->f1Forms->Selection_Get(0, R1, C1, R2, C2);
    for (int i = BegRow; i <= EndRow; i++) {
        if (FormLogicF1->f1Forms->RowHidden_Get(i)) continue;
        PAtrRow AtrRow = TFormDopInp::GetListRowsItem(FormLogicF1, i);
        if (AtrRow == nullptr || !(AtrRow->CurRowVid == 1 || AtrRow->CurRowVid == 10)) continue;
        bool isBold = (AtrRow->FormatStyle.is_bold & 0x01) != 0;
        bool isItalic = (AtrRow->FormatStyle.is_bold & 0x02) != 0;
        bool isUnderline = (AtrRow->FormatStyle.is_bold & 0x04) != 0;
        bool isStrikeout = (AtrRow->FormatStyle.is_bold & 0x08) != 0;
        std::string sValue = char16ToUTF8(FormLogicF1->f1Forms->TextRC_Get(i, nameCol));
        FormLogicF1->f1Forms->Selection_Set(i, begCol, i, endCol);
        FormLogicF1->f1Forms->MergeCells_Set(true);
        TF1CellFormat cf;
        cf.GetCellFormat(*FormLogicF1->f1Forms);
        cf.WordWrap_Set(true);
        cf.SetCellFormat(*FormLogicF1->f1Forms);
        FormLogicF1->f1Forms->RowHeightAuto_Set(i, begCol, i, endCol, true);
        cf.GetCellFormat(*FormLogicF1->f1Forms);
        cf.WordWrap_Set(true);
        switch (align_vert) {
        case 0:
        case 1: cf.AlignVertical_Set( static_cast<int16_t>(F1VAlignConstants::F1VAlignTop)); break;
        case 2: cf.AlignVertical_Set( static_cast<int16_t>(F1VAlignConstants::F1VAlignCenter)); break;
        case 3: cf.AlignVertical_Set( static_cast<int16_t>(F1VAlignConstants::F1VAlignBottom)); break;
        }
        switch (align_hor) {
        case 1: cf.AlignHorizontal_Set( static_cast<int16_t>(F1HAlignConstants::F1HAlignLeft)); break;
        case 0:
        case 2: cf.AlignHorizontal_Set( static_cast<int16_t>(F1HAlignConstants::F1HAlignCenter)); break;
        case 3: cf.AlignHorizontal_Set( static_cast<int16_t>(F1HAlignConstants::F1HAlignRight)); break;
        }
        if (font.empty()) font = "Arial";
        if (!AtrRow->FormatStyle.font.empty())
            font = AtrRow->FormatStyle.font;
        if (h > 0) cf.FontSize_Set(h);
        else h = 10;
        if (AtrRow->FormatStyle.h > 0)
            h = AtrRow->FormatStyle.h;
        cf.FontSize_Set(h);
        cf.FontName_Set(font);
        cf.FontBold_Set( isBold);
        cf.FontItalic_Set( isItalic);
        cf.SetCellFormat(*FormLogicF1->f1Forms);
        FormLogicF1->f1Forms->TextRC_Set(i, begCol, utf8_to_utf16(sValue).c_str());
    }
    FormLogicF1->f1Forms->Selection_Set(R1, C1, R2, C2);
}

// вычислить итоги по разделу за месяц и в том числе
void TFormEnd::doItogoZaMesjc_VTomCisle(TFormLogicF1* formLogicF1) {
    TwwStringList* p = TFormDopInp::GetTww(formLogicF1);
    if (!p->isPlFctOstatok) {
        return;
    }

    int wCol1 = 0;
    int wCol2 = 0;
//MAK    int cellValueAtrCount = TFormDopInp::GetCellValueAtrCount(formLogicF1);
//MAK    for (int i = 1; i < cellValueAtrCount; i++) {
//        PCellValueAtr vd = TFormDopInp::GetPCellValueAtrByIndex(formLogicF1, i);
//        if (vd->DataType == (21 + deltaDType) || vd->DataType == (22 + deltaDType)) { // 'ПлнОСТАТОК', 'ФктОСТАТОК'
//            if (vd->Col != wCol2) {
//                TFormEnd::ItogoZaMesjc(formLogicF1, vd->Col);
//                wCol2 = vd->Col;
//            }
//            TFormEnd::VTomCisle(formLogicF1, vd->Row, vd->Col);
//        }
//MAK    }
    for (auto it = TFormDopInp::GetTww(formLogicF1)->items.begin(); it != TFormDopInp::GetTww(formLogicF1)->items.end(); ++it) {
        auto vd = std::get<PCellValueAtr>(it->second);
        if (vd->DataType == (21 + deltaDType) || vd->DataType == (22 + deltaDType)) { // 'ПлнОСТАТОК', 'ФктОСТАТОК'
            if (vd->Col != wCol2) {
                TFormEnd::ItogoZaMesjc(formLogicF1, vd->Col);
                wCol2 = vd->Col;
            }
            TFormEnd::VTomCisle(formLogicF1, vd->Row, vd->Col);
        }
    }
}

void SetItogoUp(TFormLogicF1* FormLogicF1, int aRowItogo, int aOwnerNomRow, double aD, int nCol) {
    int j;
    PCellValueAtr vd1;
    int OwnerNomRow1, OwnerNomRow2;
    PAtrRow AtrRow = TFormDopInp::GetListRowsItem(FormLogicF1, aOwnerNomRow);
    if (AtrRow == nullptr)
        return;
    OwnerNomRow2 = AtrRow->OwnerNomRow;
    int LastRow = FormLogicF1->f1Forms->LastRow_Get();
    for (j = aRowItogo + 1; j <= LastRow; j++) {
        vd1 = TFormDopInp::GetCellValueAtrByRC(FormLogicF1, j, nCol);
        if (vd1 == nullptr)
            continue;
        switch (TFormDopInp::GetListRowsItem(FormLogicF1, vd1->Row)->CurRowVid) {
        case 3: // итого по разделу
            OwnerNomRow1 = TFormDopInp::GetListRowsItem(FormLogicF1, vd1->Row)->OwnerNomRow;
            if ((OwnerNomRow1 == OwnerNomRow2) && (TFormDopInp::GetListRowsItem(FormLogicF1, OwnerNomRow1) != nullptr)) {
                FormLogicF1->f1Forms->NumberRC_Set(j, nCol, aD + FormLogicF1->f1Forms->NumberRC_Get(j, nCol));
                SetItogoUp(FormLogicF1, j, OwnerNomRow1, aD, nCol);
            }
            break;
        }
    }
}

void DoGetValueItogoMesjc(TFormLogicF1* FormLogicF1, int aRowItogo, int aNCol) {
    PCellValueAtr vd1 = TFormDopInp::GetCellValueAtrByRC(FormLogicF1, aRowItogo, aNCol);
    if (vd1 == nullptr)
        return;
    TDateTime FromDate;
    TDateTime ToDate;
    int Year, Month, Day;

    ToDate = vd1->ValueDate; 
    DecodeDate(ToDate, Year, Month, Day);
    FromDate = EncodeDate(Year, Month, 1);
    
    double wd = TDB_DenegnPotok::GetValueItogoMesjc(vd1->PokazId, vd1->PlanId, FromDate, vd1->ValueDate, vd1->VidOtch, vd1->CellId);
    wd = PreobrValue(wd, vd1->DataType, vd1->coef_Pokaz, vd1->coef_Forma);
    FormLogicF1->f1Forms->NumberRC_Set(aRowItogo, aNCol, wd);
}

// подсчет итого по разделу за месяц
void TFormEnd::ItogoZaMesjc(TFormLogicF1* FormLogicF1, int nCol) {
    int i, OwnerNomRow, RowItogo;
    double d;
    PCellValueAtr vd;

    d = 0;
    RowItogo = 0;
    OwnerNomRow = 0;
    int LastRow = FormLogicF1->f1Forms->LastRow_Get();
    for (i = LastRow; i >= 1; i--) {
        PAtrRow AtrRow = TFormDopInp::GetListRowsItem(FormLogicF1, i);
        if (AtrRow == nullptr)
            continue;

        switch (AtrRow->CurRowVid) {
        case 1: // раздел
            if (RowItogo > 0) {
                if ((OwnerNomRow > 0) && (OwnerNomRow == i)) {
                    FormLogicF1->f1Forms->NumberRC_Set(RowItogo, nCol, d);
                    SetItogoUp(FormLogicF1, RowItogo, OwnerNomRow, d, nCol);
                    d = 0;
                    OwnerNomRow = AtrRow->OwnerNomRow;
                    RowItogo = 0;
                }
            }
            continue;
        case 2: // показатель
            vd = TFormDopInp::GetCellValueAtrByRC(FormLogicF1, i, nCol);
            if (vd == nullptr)
                continue;
            if ((OwnerNomRow > 0) && (OwnerNomRow == AtrRow->OwnerNomRow)) {
                d += FormLogicF1->f1Forms->NumberRC_Get(i, nCol);
                if (RowItogo == 0) {
                    SetItogoUp(FormLogicF1, i, i, d, nCol);
                    d = 0;
                }
            }
            break;
        case 3: // итого по разделу
            vd = TFormDopInp::GetCellValueAtrByRC(FormLogicF1, i, nCol);
            if (vd == nullptr)
                continue;
            if (RowItogo > 0) {
                FormLogicF1->f1Forms->NumberRC_Set(RowItogo, nCol, d);
                SetItogoUp(FormLogicF1, RowItogo, OwnerNomRow, d, nCol);
                if (TFormDopInp::GetListRowsItem(FormLogicF1, OwnerNomRow) == nullptr)
                    DoGetValueItogoMesjc(FormLogicF1, RowItogo, nCol);
            }
            RowItogo = i;
            OwnerNomRow = AtrRow->OwnerNomRow;
            d = 0;
            break;
        }
    }

    if (RowItogo > 0) {
        FormLogicF1->f1Forms->NumberRC_Set(RowItogo, nCol, d);
        SetItogoUp(FormLogicF1, RowItogo, OwnerNomRow, d, nCol);
        if (TFormDopInp::GetListRowsItem(FormLogicF1, OwnerNomRow) == nullptr)
            DoGetValueItogoMesjc(FormLogicF1, RowItogo, nCol);
    }
}

// подсчет в том числе за месяц
void TFormEnd::VTomCisle(TFormLogicF1* FormLogicF1, int nRow, int nCol) {
    int i;
    PCellValueAtr vd, vd1 = nullptr;
    double d = 0.0;

    if (!TFormDopInp::GetCellValueAtrByName(FormLogicF1, "R" + std::to_string(nRow) + "C" + std::to_string(nCol), *vd))
        return;
    if (!TFormDopInp::GetCellValueAtrByName(FormLogicF1,
        "R" + std::to_string(TFormDopInp::GetListRowsItem(FormLogicF1, vd->Row)->OwnerNomRow) +
        "C" + std::to_string(nCol), *vd1))
        return;
    if (vd1->EvalType != 1)
        return; // not in the same list

    for (i = TFormDopInp::GetListRowsItem(FormLogicF1, vd->Row)->OwnerNomRow + 1; i <= FormLogicF1->f1Forms->LastRow_Get(); i++) {
        if (!TFormDopInp::GetCellValueAtrByName(FormLogicF1, "R" + std::to_string(i) + "C" + std::to_string(nCol), *vd1))
            continue;
        if (TFormDopInp::GetListRowsItem(FormLogicF1, vd1->Row)->OwnerNomRow !=
            TFormDopInp::GetListRowsItem(FormLogicF1, vd->Row)->OwnerNomRow)
            break;
        switch (TFormDopInp::GetListRowsItem(FormLogicF1, vd->Row)->CurRowVid) {
        case 1:
            continue; // section
        case 2:
            d += FormLogicF1->f1Forms->NumberRC_Get(i, nCol); // показатель
        }
    }

    FormLogicF1->f1Forms->NumberRC_Set(TFormDopInp::GetListRowsItem(FormLogicF1, vd->Row)->OwnerNomRow, nCol, d);
    // since there may be nesting in the same list
    VTomCisle(FormLogicF1, TFormDopInp::GetListRowsItem(FormLogicF1, vd->Row)->OwnerNomRow, nCol);
}

// дублирует итого по разделу в Разделе
void TFormEnd::ExecIfDublirItogo(TFormLogicF1* FormLogicF1) {
    int R1, C1, R2, C2;
    TwwStringList* p;
    int i, j, n;
    PAtrRow AtrRow;
    PCellValueAtr vd;
    bool is0;

    FormLogicF1->f1Forms->Selection_Get(0, R1, C1, R2, C2);
    p = TFormDopInp::GetTww(FormLogicF1);
    int LastCol = FormLogicF1->f1Forms->LastCol_Get();
    for (i = p->form_endBodyRow; i >= p->form_begBodyRow; i--) {
        if (FormLogicF1->f1Forms->RowHidden_Get(i))
            continue;
        AtrRow = TFormDopInp::GetListRowsItem(FormLogicF1, i);
        if (AtrRow == nullptr)
            continue;
        if (AtrRow->CurRowVid != 3 && AtrRow->CurRowVid != 5)
            continue;
        is0 = true;
        for (j = 1; j <= LastCol; j++) {
            if (FormLogicF1->f1Forms->ColHidden_Get(j))
                continue;
            switch (FormLogicF1->f1Forms->TypeRC_Get(i, j)) {
            case 1:
            case -1:
                FormLogicF1->f1Forms->NumberRC_Set(AtrRow->OwnerNomRow, j, FormLogicF1->f1Forms->NumberRC_Get(i, j));
                if (!IsZero(FormLogicF1->f1Forms->NumberRC_Get(i, j), 0.00001))
                    is0 = false;
                FormLogicF1->f1Forms->Selection_Set(AtrRow->OwnerNomRow, j, AtrRow->OwnerNomRow, j);
                FormLogicF1->f1Forms->HAlign_Set(static_cast<int16_t>(F1HAlignConstants::F1HAlignRight));
                break;
            }
        }
        FormLogicF1->f1Forms->RowHidden_Set(i, true);
        if (is0)
            FormLogicF1->f1Forms->RowHidden_Set(AtrRow->OwnerNomRow, true);
    }
    FormLogicF1->f1Forms->Selection_Set(R1, C1, R2, C2);
}

// Осуществляет обработку оператора OdinakovZnac
// задает: как обработать соседние ячейки с одинаковыми значениями
void TFormEnd::execOdinakovZnac(TFormLogicF1* FormLogicF1) {
    int R1, C1, R2, C2;
    TwwStringList* p = TFormDopInp::GetTww(FormLogicF1);
    if (p->stmntODINAKOVZNAC == "") {
        return;
    }
    FormLogicF1->f1Forms->Selection_Get(0, R1, C1, R2, C2);
    int begRow = p->form_begBodyRow;
    int endRow = p->form_endBodyRow;
    std::string note2 = p->stmntODINAKOVZNAC;
    int LastCol = FormLogicF1->f1Forms->LastCol_Get();
    for (int i = endRow; i >= begRow + 1; i--) {
        bool isRavno = true;
        for (int j = 1; j <= LastCol; j++) {
            if (FormLogicF1->f1Forms->ColHidden_Get(j)) {
                continue;
            }
            int wn = TFormDopInpMF::GetPokazIdForCol(note2, j);
            if (wn == 0) {
                continue;
            }
            if (FormLogicF1->f1Forms->FormattedTextRC_Get(i, j) != FormLogicF1->f1Forms->FormattedTextRC_Get(i - 1, j)) {
                isRavno = false;
                break;
            }
            switch (wn) {
            case 1:
                FormLogicF1->f1Forms->TextRC_Set(i, j, utf8_to_utf16("").c_str());
                break;
            case 2:
                FormLogicF1->f1Forms->Selection_Set(i - 1, j, i, j);
                FormLogicF1->f1Forms->MergeCells_Set(true);
                break;
            case 3:
                if (char16ToUTF8(FormLogicF1->f1Forms->TextRC_Get(i, j)) == "") {
                    continue;
                }
                FormLogicF1->f1Forms->Selection_Set(i - 1, j, i, j);
                FormLogicF1->f1Forms->MergeCells_Set(true);
                break;
            }
        }
    }
    FormLogicF1->f1Forms->Selection_Set(R1, C1, R2, C2);
}

// обработка строкового оператора ::УдалитьСкрытые2 в конце формы
void TFormEnd::DeleteHiddenRows2(TFormLogicF1* FormLogicF1, int aRowOper, bool isHiddenOnlyFiltr)
{
    TwwStringList* p = TFormDopInp::GetTww(FormLogicF1);
    int begRow = p->form_begBodyRow;
    int endRow = p->form_endBodyRow;

  // во всех видимых формулах формулы заменяем на значения - Здесь этого НЕ делаем

  // проверяем, что нет формул в невидимых строках
  // Новое: это разрешаем, НО: в дальнейшем(ниже) НЕ удаляем скрытые строки-формулы

    if (aRowOper > 0)
        FormLogicF1->f1Forms->RowHidden_Set(aRowOper, true); // скрыли саму строку с оператором
  
  // удаляем скрытые строки
  // Новый эффективный алгоритм
    std::vector<int> A(endRow - begRow + 1);
    int dd = 0;

    for (int i = begRow; i <= endRow; i++) {
        if (FormLogicF1->f1Forms->RowHidden_Get(i)) {
            PAtrRow AtrRow = TFormDopInp::GetListRowsItem(FormLogicF1, i);
            if (AtrRow != nullptr && AtrRow->CurRowVid == 11)
                continue;
            if (isHiddenOnlyFiltr && AtrRow != nullptr && AtrRow->isRowHidden == 1)
                continue;
            A[i - begRow] = std::numeric_limits<int>::max();
            dd++;
        }
        else
            A[i - begRow] = dd;
    }

    FormLogicF1->f1Forms->EnableProtection = false;
    for (int i = endRow; i >= begRow; i--) {
        PAtrRow AtrRow = TFormDopInp::GetListRowsItem(FormLogicF1, i);
        if (A[i - begRow] == std::numeric_limits<int>::max()) {
            if (AtrRow != nullptr) {
                p->ListRows->Delete(i);
                delete AtrRow;
            }
            TF1toDE::DeleteRows(FormLogicF1, i, i);
            FormLogicF1->f1Forms->MaxRow-1;
            p->form_endBodyRow--;
        }
        else if (AtrRow != nullptr) {
            AtrRow->Row -= A[i - begRow];
            if (AtrRow->OwnerNomRow > i)
                AtrRow->OwnerNomRow -= A[i - begRow];
        }
    }
    FormLogicF1->f1Forms->EnableProtection = true;
    int LastCol = FormLogicF1->f1Forms->LastCol_Get();
    for (int i = begRow; i <= endRow; i++) {
        if (A[i - begRow] == std::numeric_limits<int>::max()) {
            for (int j = 1; j <= LastCol; j++) {
                int n = p->IndexOf("R" + std::to_string(i) + "C" + std::to_string(j));
                if (n < 0)
                    continue;
                PCellValueAtr vd = std::get<PCellValueAtr>(p->Objects[n]);
                p->Delete(n);
                delete vd;
            }
        }
        else {
            for (int j = 1; j <= LastCol; j++) {
                int n = p->IndexOf("R" + std::to_string(i) + "C" + std::to_string(j));
                if (n < 0)
                    continue;
                PCellValueAtr vd = std::get<PCellValueAtr>(p->Objects[n]);
                p->Delete(n);
                vd->Row -= A[i - begRow];
                p->AddObject("R" + std::to_string(vd->Row) + "C" + std::to_string(vd->Col), (TObject*)vd);
            }
        }
    }
    int LastRow = FormLogicF1->f1Forms->LastRow_Get();
    for (int i = 1; i <= LastRow; i++) {
        for (int j = 1; j <= LastCol; j++) {
            switch (FormLogicF1->f1Forms->TypeRC_Get(i, j)) {
            case -1:
                if (std::string(char16ToUTF8(FormLogicF1->f1Forms->FormulaRC_Get(i, j))).find("#REF!") != std::string::npos)
                    FormLogicF1->f1Forms->NumberRC_Set(i, j, 0);
                break;
            case -2:
                if (std::string(char16ToUTF8(FormLogicF1->f1Forms->FormulaRC_Get(i, j))).find("#REF!") != std::string::npos)
                    FormLogicF1->f1Forms->TextRC_Set(i, j, utf8_to_utf16("").c_str());
                break;
            }
        }
    }
}

// обработка строкового оператора ::УдалитьСкрытыеФильтром в конце формы
void TFormEnd::DeleteHiddenRowsByFiltr(TFormLogicF1* FormLogicF1, int aRowOper) {
    TFormEnd::DeleteHiddenRows2(FormLogicF1, aRowOper, true);
}

// обработка строкового оператора ::УдалитьСкрытые в конце формы
void TFormEnd::DeleteHiddenRows(TFormLogicF1* FormLogicF1, int aRowOper) {
    TwwStringList* p = TFormDopInp::GetTww(FormLogicF1);
    int begRow = p->form_begBodyRow;
    int endRow = p->form_endBodyRow;
    int LastCol = FormLogicF1->f1Forms->LastCol_Get();
    // во всех видимых формулах формулы заменяем на значения    
    for (int i = endRow; i >= begRow; i--) {
        if (FormLogicF1->f1Forms->RowHidden_Get(i)) continue;
        PAtrRow AtrRow = TFormDopInp::GetListRowsItem(FormLogicF1, i);
        if (AtrRow == nullptr) continue;
        if (AtrRow->CurRowVid != 11 /*formula*/) continue;
        for (int j = 1; j <= LastCol; j++) {
            switch (FormLogicF1->f1Forms->TypeRC_Get(i, j)) {
            case 0: continue;
            case -1: FormLogicF1->f1Forms->NumberRC_Set(i, j, FormLogicF1->f1Forms->NumberRC_Get(i, j)); break;
            case -2: FormLogicF1->f1Forms->TextRC_Set(i, j, FormLogicF1->f1Forms->TextRC_Get(i, j)); break;
            }
        }
    }

    FormLogicF1->f1Forms->RowHidden_Set(aRowOper, true); // скрыли саму строку с оператором

    // Новый эффективный алгоритм    
    std::vector<int> A(endRow - begRow + 1);
    int dd = 0;
    for (int i = begRow; i <= endRow; i++) {
        if (FormLogicF1->f1Forms->RowHidden_Get(i)) {
            A[i - begRow] = std::numeric_limits<int>::max();  // признак удаления стьроки
            dd++;
        } else {
            A[i - begRow] = dd; // на сколько надо уменьшать номер строки
        }
    }

    FormLogicF1->f1Forms->EnableProtection = false;
    for (int i = endRow; i >= begRow; i--) {
        PAtrRow AtrRow = TFormDopInp::GetListRowsItem(FormLogicF1, i);
        if (A[i - begRow] == std::numeric_limits<int>::max()) {
            if (AtrRow != nullptr) {
                p->ListRows->Delete(i);
                delete AtrRow;
            }
            TF1toDE::DeleteRows(FormLogicF1, i, i);
            FormLogicF1->f1Forms->MaxRow-1;
            p->form_endBodyRow--;
        } else {
            if (AtrRow == nullptr) continue;
            AtrRow->Row -= A[i - begRow];
            if (AtrRow->OwnerNomRow > i)
                AtrRow->OwnerNomRow -= A[i - begRow];
        }
    }
    FormLogicF1->f1Forms->EnableProtection = true;

    for (int i = begRow; i <= endRow; i++) {
        if (A[i - begRow] == std::numeric_limits<int>::max()) {
            for (int j = 1; j <= LastCol; j++) {
                int n = p->IndexOf("R" + std::to_string(i) + "C" + std::to_string(j));
                if (n < 0) continue;
                PCellValueAtr vd = std::get<PCellValueAtr>(p->Objects[n]);
                p->Delete(n);
                delete vd;
            }
        } else {
            for (int j = 1; j <= LastCol; j++) {
                int n = p->IndexOf("R" + std::to_string(i) + "C" + std::to_string(j));
                if (n < 0) continue;
                PCellValueAtr vd = std::get<PCellValueAtr>(p->Objects[n]);
                p->Delete(n);
                vd->Row -= A[i - begRow];
                p->AddObject("R" + std::to_string(vd->Row) + "C" + std::to_string(vd->Col), (TObject*)vd);
            }
        }
    }

    A.clear();
}

// раскрасить шапку формы в серый цвет
void TFormEnd::doShapkaColor(TFormLogicF1* FormLogicF1) {
    TwwStringList* p = TFormDopInp::GetTww(FormLogicF1);
    if (p->isShapkaColor == 0 || p->isShapkaColor == 2) return;
    
    int R1, C1, R2, C2;
    FormLogicF1->f1Forms->Selection_Get(0, R1, C1, R2, C2);
    
    int nRow1 = p->form_begBodyRow - p->form_k1;
    int nCol1 = 1;
    int nRow2 = p->form_begBodyRow - 1;
    int nCol2 = FormLogicF1->f1Forms->LastCol_Get();
    FormLogicF1->f1Forms->Selection_Set(nRow1, nCol1, nRow2, nCol2);
    
    // поэлементно, чтобы не закрасить заданный другой фон ячейки
    for (int nRow = nRow1; nRow <= nRow2; nRow++) {
        for (int nCol = nCol1; nCol <= nCol2; nCol++) {
            FormLogicF1->f1Forms->Selection_Set(nRow, nCol, nRow, nCol);
            int pPattern, pcrFG, pcrBG;
            FormLogicF1->f1Forms->Pattern_Get(pPattern, pcrFG, pcrBG);
            if (pcrBG == 16777215 || pcrBG == 0) {
                FormLogicF1->f1Forms->BackgroundColor_Set(SystFuncs::SystFuncsAltWin::GetColorFromRGB(210, 210, 210));
            }
        }
    }
        
    FormLogicF1->f1Forms->Selection_Set(R1, C1, R2, C2);
}

// чередовать раскрасить строк таблицы в разные цвета
void TFormEnd::doLinesColor(TFormLogicF1* FormLogicF1) {
    TwwStringList* p = TFormDopInp::GetTww(FormLogicF1);
    if (p->isLinesColor == 0) return;

    int nRow1 = p->form_begBodyRow;
    int nCol1 = 1;
    int nRow2 = p->form_endBodyRow;
    int nCol2 = FormLogicF1->f1Forms->LastCol_Get();

    int pcrFG, pcrBG;
    int pPattern;
    int I = nRow1 + 1;
    int R1, C1, R2, C2;
    FormLogicF1->f1Forms->Selection_Get(0, R1, C1, R2, C2);

    while (I <= nRow2) {
        for (int J = nCol1; J <= nCol2; J++) {
            FormLogicF1->f1Forms->Selection_Set(I, J, I, J);
            FormLogicF1->f1Forms->Pattern_Get(pPattern, pcrFG, pcrBG);
            if (pcrFG == 0) {
                #ifdef WEBMODE
                FormLogicF1->f1Forms->SetBackgroundColor(RGB(230, 230, 230));
                #else
                FormLogicF1->f1Forms->BackgroundColor_Set(SystFuncs::SystFuncsAltWin::GetColorFromRGB(230, 230, 230));
                #endif
            }
        }
        I += 2;
    }

    FormLogicF1->f1Forms->Selection_Set(nRow1, nCol1, nRow2, nCol2);
    FormLogicF1->f1Forms->Border_Set(1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0);
    FormLogicF1->f1Forms->Selection_Set(R1, C1, R2, C2);
}













