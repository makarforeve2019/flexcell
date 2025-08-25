#include "../SystFuncs/SystFuncsLogic.h"
#include "../DB/unDefDBconnect.h"
#include "../Form/FormDraw/FormDopInp.h"
#include "../Form/FormDraw/Form1.h"
#include "../DB/DB_CNP_value.h"
#include "../DB/DB_Enum.h"
//#include "../DB/DB_VidProdukcii.h"
#include "../DB/DB_OKDP.h"
#include "../DB/DB_BPoluc.h"
#include "../DB/DB_OKVED.h"
#include "evalComonNPokaz_Logic.h"

void TevalComonNPokaz_Logic::showZnacenijCNP(TwwStringList* p, TFormLogicF1* FormLogicF1, PCellValueAtr vd, int nRow, int nCol){
    std::string Value;
    double ValueD;
    int ValueI;
    bool rtn;
    TFormatStyle* FormatStyle = new TFormatStyle();
    int DataType;
    std::string toTblName;
    int idItem;

    if (vd->ObjectId == 0)
        idItem = vd->PokazId;
    else
        idItem = vd->ObjectId;

    TFormDopInp::getColFrmtrAtr(FormLogicF1, 0, vd->Col, *FormatStyle);
    toTblName = p->ListStmnt[vd->Col].toTblName;

    switch (vd->pokaz_type) {
    case 1:
        rtn = TDB_CNP_value::getNumberItem(idItem, vd->comnp_id, vd->cell_period, vd->PlanId, vd->VidOtch, vd->god, vd->kvrtl, vd->mesjc, vd->den, ValueD);
        if (!rtn) return;
        ValueD = PreobrValue(ValueD, vd->DataType, vd->coef_Pokaz, vd->coef_Forma);
        Value = std::to_string(ValueD);

        if (FormatStyle->align_vert == 0) FormatStyle->align_vert = 2;
        if (FormatStyle->align_gor == 0) FormatStyle->align_gor = 2;
        DataType = 1;
        break;
    case 2:
        rtn = TDB_CNP_value::getStringItem(idItem, vd->comnp_id, vd->cell_period, vd->PlanId, vd->VidOtch, vd->god, vd->kvrtl, vd->mesjc, vd->den, Value);
        if (!rtn) return;
        DataType = 2;
        break;
    case 3:
        rtn = TDB_CNP_value::getNumberItem(idItem, vd->comnp_id, vd->cell_period, vd->PlanId, vd->VidOtch, vd->god, vd->kvrtl, vd->mesjc, vd->den, ValueD);
        if (!rtn) return;
        ValueI = static_cast<int>(ValueD);
        Value = TDB_Enum::GetEnumName(ValueI);
        DataType = 2;
        break;
    case 4:
        rtn = TDB_CNP_value::getNumberItem(idItem, vd->comnp_id, vd->cell_period, vd->PlanId, vd->VidOtch, vd->god, vd->kvrtl, vd->mesjc, vd->den, ValueD);
        if (!rtn) return;
        ValueI = static_cast<int>(ValueD);
        vd->BPId = ValueI;
        DataType = 2;
        switch (vd->EnumId) {
        case 1:
            Value = TDB_BPoluc::GetNameBP(ValueI);
            avtoEvalOtherByBudgPoluc(p, FormLogicF1, toTblName, ValueI, vd->Row);
            break;
        case 2:
            //vernut Value = TDB_VidProdukcii::GetVidProdKodName(ValueI);
            avtoEvalOtherByVidProdukcii(p, FormLogicF1, toTblName, ValueI, vd->Row);
            break;
        case 3:
            Value = TDB_OKDP::GetOKDPCodeName(ValueI);
            avtoEvalOtherByOKDP(p, FormLogicF1, toTblName, ValueI, vd->Row);
            break;
        case 4:
            Value = TDB_OKVED::GetOKVEDName(ValueI);
            avtoEvalOtherByOKVED(p, FormLogicF1, toTblName, ValueI, vd->Row);
            break;
        }
        break;
    case 5:
        rtn = TDB_CNP_value::getStringItem(idItem, vd->comnp_id, vd->cell_period, vd->PlanId, vd->VidOtch, vd->god, vd->kvrtl, vd->mesjc, vd->den, Value);
        if (!rtn) return;
        DataType = 2;
        break;
    }

    TForm1::SetCellValue(FormLogicF1, FormLogicF1->f1Forms, p->Count(), vd->Row, vd->Col, vd->Row, vd->Col, Value, DataType, FormatStyle);
}

void TevalComonNPokaz_Logic::showZnacenijCNPallPlans(TwwStringList* p, TFormLogicF1* FormLogicF1, PCellValueAtr vd, int nRow, int nCol) {
    std::string Value;
    double ValueD;
    bool rtn;
    TFormatStyle* FormatStyle = new TFormatStyle();
    int DataType = 0;
    std::string toTblName;
    int idItem;

    if (vd->ObjectId == 0)
        idItem = vd->PokazId;
    else
        idItem = vd->ObjectId;

    TFormDopInp::getColFrmtrAtr(FormLogicF1, 0, vd->Col, *FormatStyle);
    toTblName = p->ListStmnt[vd->Col].toTblName;

    switch (vd->pokaz_type) {
    case 1:
        rtn = TDB_CNP_value::getNumberItemallPlans(idItem, vd->comnp_id, vd->cell_period, vd->PlanId,
            vd->VidOtch, vd->god, vd->kvrtl, vd->mesjc, vd->den, ValueD);
        if (!rtn)
            return;
        ValueD = PreobrValue(ValueD, vd->DataType, vd->coef_Pokaz, vd->coef_Forma);
        Value = std::to_string(ValueD);

        if (FormatStyle->align_vert == 0)
            FormatStyle->align_vert = 2;
        if (FormatStyle->align_gor == 0)
            FormatStyle->align_gor = 2;
        DataType = 1;
        break;
    }

    TForm1::SetCellValue(FormLogicF1, FormLogicF1->f1Forms, p->Count(), vd->Row, vd->Col, vd->Row, vd->Col,
        Value, DataType, FormatStyle);
}

void TevalComonNPokaz_Logic::avtoEvalOtherByBudgPoluc(TwwStringList* p, TFormLogicF1* formLogicF1,  std::string toTblName, int budget_poluc_id, int nRow) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    TDB_BPoluc::GetBP(q, budget_poluc_id);
    TevalComonNPokaz_Logic::avtoEvalOther(q, p, formLogicF1, toTblName, nRow);
}

void TevalComonNPokaz_Logic::avtoEvalOtherByVidProdukcii(TwwStringList* p, TFormLogicF1* FormLogicF1,  std::string toTblName, int vidprod_id, int nRow) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    //mak ??? TDB_VidProdukcii::GetVidProdById(q, vidprod_id);
    TevalComonNPokaz_Logic::avtoEvalOther(q, p, FormLogicF1, toTblName, nRow);
}

void TevalComonNPokaz_Logic::avtoEvalOtherByOKDP(TwwStringList* p, TFormLogicF1* formLogicF1,  std::string toTblName, int okdpId, int nRow) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    TDB_OKDP::GetOKDP(q, okdpId);
    TevalComonNPokaz_Logic::avtoEvalOther(q, p, formLogicF1, toTblName, nRow);
}

void TevalComonNPokaz_Logic::avtoEvalOtherByOKVED(TwwStringList* p, TFormLogicF1* FormLogicF1,  std::string toTblName, int okved_id, int nRow) {
        TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
        TDB_OKVED::GetOKVED(q, okved_id);
        TevalComonNPokaz_Logic::avtoEvalOther(q, p, FormLogicF1, toTblName, nRow);
    }

void TevalComonNPokaz_Logic::avtoEvalOther(TQueryUni* q, TwwStringList* p, TFormLogicF1* FormLogicF1, std::string toTblName, int nRow) {
    for (int i = 1; i <= p->MaxHrCol; i++) {
        if (p->ListStmnt[i-1].Stmnt == "ЗНАЧЕНИЕобщНП" && p->ListStmnt[i-1].IsFieldsSprvcnik == 1 && p->ListStmnt[i-1].toTblName == toTblName) {
            TFormatStyle* FormatStyle = new TFormatStyle();
            TFormDopInp::getColFrmtrAtr(FormLogicF1, 0, i, *FormatStyle);
            std::string toColumnName1 = p->ListStmnt[i-1].toColumnName1;
            std::string value = q->FieldByName(toColumnName1).AsString();
            PCellValueAtr vd = TFormDopInp::GetCellValueAtrByRC(FormLogicF1, nRow, i);
            int dataType = vd->pokaz_type;
            int idItem = (vd->ObjectId == 0) ? vd->PokazId : vd->ObjectId;

            switch (vd->pokaz_type) {
            case 1:
                TDB_CNP_value::addNumberItem(idItem, vd->comnp_id, vd->cell_period, vd->PlanId, vd->VidOtch, vd->god, vd->kvrtl, vd->mesjc, vd->den, std::stod(value));
                if (FormatStyle->align_vert == 0) FormatStyle->align_vert = 2;
                if (FormatStyle->align_vert == 0) FormatStyle->align_vert = 2;
                break;
            case 2:
                TDB_CNP_value::addStringItem(idItem, vd->comnp_id, vd->cell_period, vd->PlanId, vd->VidOtch, vd->god, vd->kvrtl, vd->mesjc, vd->den, value);
                break;
            case 3:
                TDB_CNP_value::addNumberItem(idItem, vd->comnp_id, vd->cell_period, vd->PlanId, vd->VidOtch, vd->god, vd->kvrtl, vd->mesjc, vd->den, std::stod(value));
                break;
            case 5:
                TDB_CNP_value::addStringItem(idItem, vd->comnp_id, vd->cell_period, vd->PlanId, vd->VidOtch, vd->god, vd->kvrtl, vd->mesjc, vd->den, value);
                break;
            }

            TForm1::SetCellValue(FormLogicF1, FormLogicF1->f1Forms, p->MaxHrCol, vd->Row, vd->Col, vd->Row, vd->Col, value, dataType, FormatStyle);
        }
    }
}


