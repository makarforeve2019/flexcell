//
// Created by Alex on 26.06.2024.
//

#include "UpdBudgPro2.h"
#include "FormDopInp.h"
#include "SystFuncsLogic.h"
#include "DB_Pokaztl.h"
#include "DB_EdIzm.h"

nlohmann::json TUpdBudgPro2::F1Book1DblClickEvent(TFormLogicF1 *formLogicF1, int nSheet, int nRow, int nCol) {
    PCellValueAtr vd;
    PAtrRow AtrRow;
    nlohmann::json resJson;
    vd = TFormDopInp::GetCellValueAtrByRC(formLogicF1, nRow, nCol);
    if (vd == nullptr) {
        resJson["isChangeValue"] = false;
        resJson["isChangeCost"] = false;
        resJson["msg"] = "Выбранная ячейка не подлежит изменению";
        return resJson;
    }
    if (vd->EvalType != 0) {
        resJson["isChangeValue"] = false;
        resJson["isChangeCost"] = false;
        resJson["msg"] = "Выбранная ячейка не подлежит изменению";
        return resJson;
    }
    AtrRow = TFormDopInp::GetListRowsItem(formLogicF1, nRow);
    if (AtrRow->isReadOnly) {
        resJson["isChangeValue"] = false;
        resJson["isChangeCost"] = false;
        resJson["msg"] = "Выбранная ячейка не подлежит изменению";
        return resJson;
    }
    TwwStringList* p = TFormDopInp::GetTww(formLogicF1);
    //////////////////////poiskInfoIstFin//////////////////////////
    int n, j;
    int nRowIstFinans = 0;
    int nRowNatur = 0;
    int LastRow = formLogicF1->f1Forms->LastRow_Get();
    for (n = 1; n <= LastRow; ++n) {
        PAtrRow AtrRow2 = TFormDopInp::GetListRowsItem(formLogicF1, n);
        if (AtrRow2 == nullptr) continue;
        if (AtrRow2->CurRowVid != 11) continue; // не формула
        std::string istfinUtf8 = char16ToUTF8(formLogicF1->f1Forms->TextRC_Get(n, p->PokazCol));
        if (istfinUtf8 == "::Источники финансирования") {
            nRowIstFinans = n;
        }
        std::string naturUtf8 = char16ToUTF8(formLogicF1->f1Forms->TextRC_Get(n, p->PokazCol));
        if (naturUtf8 == "::Зависит от графы \"Объем\"") {
            nRowNatur = n;
        }
    }

    if (nRowIstFinans == 0) {
        resJson["isChangeValue"] = false;
        resJson["isChangeCost"] = false;
        resJson["msg"] = "Форма не содержит строку с указанием источников финансирования";
        return resJson;
    }
    if (formLogicF1->f1Forms->TextRC_Get(nRowIstFinans, nCol) == utf8_to_utf16("")) {
        resJson["isChangeValue"] = false;
        resJson["isChangeCost"] = false;
        resJson["msg"] = "Необходимо выбрать ячейку с источником финансирования";
        return resJson;
    }
    std::string  istFinansName = char16ToUTF8(formLogicF1->f1Forms->TextRC_Get(nRowIstFinans, nCol));
    int nColNatur = 0;

    resJson["isChangeValue"] = true;

    if (nRowNatur > 0) {
        if (formLogicF1->f1Forms->TextRC_Get(nRowNatur, nCol) == utf8_to_utf16("")) {
            nRowNatur = 0; // нет значения в натуре
        } else {
            try {
                nColNatur = StrToInt(char16ToUTF8(formLogicF1->f1Forms->TextRC_Get(nRowNatur, nCol)));
                resJson["isChangeCost"] = true;
            } catch (const std::exception&) {
                resJson["isChangeValue"] = false;
                resJson["isChangeCost"] = false;
                resJson["msg"] = "Столбец с натуральным значением указан некорректно";
                return resJson;
            }
        }
    }
    resJson["form_id"] = p->FormId;
    resJson["nRow"] = nRow;
    resJson["nCol"] = nCol;
    resJson["ObjectId"] = vd->ObjectId;
    resJson["pokaz_id"] = vd->PokazId;
    resJson["cell_id"] = vd->CellId;
    resJson["god"] = YearOf(vd->ValueDate);;
    resJson["kvrtl"] = GetNKv(MonthOf(vd->ValueDate));
    resJson["cellValueStatus"] = istFinansName;
    double d = formLogicF1->f1Forms->NumberRC_Get(vd->Row, vd->Col);
    d = PreobrValue(d, vd->DataType, vd->coef_Forma, vd->coef_Pokaz);
    resJson["cellValueStoim"] = d;
    resJson["namePokazForm"] = TDB_Pokaztl::GetPokazName(AtrRow->ObjectId) + "("+p->FormObozn+")" ;
    resJson["number"] = "";
    resJson["KBK"] = "";
    resJson["rashBefore"] = formLogicF1->f1Forms->NumberRC_Get(nRow, nCol);
    if (nRowNatur != 0)
        resJson["obmBefore"] = formLogicF1->f1Forms->NumberRC_Get(nRow, nColNatur);
    int edizm_id;
    std::string EdizmName = TDB_Pokaztl::GetPokazName_EdIzm(AtrRow->ObjectId, edizm_id);

    resJson["EdizmName"] = TDB_EdIzm::GetEdIzmName(edizm_id);
    resJson["album_id"] = formLogicF1->CurrAlbumId;

    resJson["ncolnatur"] = nColNatur;
    if (nRowNatur!=0 && nColNatur != 0) {
        PCellValueAtr vdNatur = TFormDopInp::GetCellValueAtrByRC(formLogicF1, nRowNatur, nColNatur);
        resJson["pokaz_idnatur"] = vdNatur->PokazId;
        resJson["cell_idnatur"] = vdNatur->CellId;
    }
    return resJson;
}
