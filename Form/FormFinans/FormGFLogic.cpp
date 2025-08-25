#include "SystFuncsLogic.h"
#include "SystFuncsAltWin.h"
#include "unDefDBconnect.h"
#include "Form2Plus.h"
#include "FormDopInp.h"
#include "DB_Cell.h"
#include "DB_DenegnPotok.h"
#include "UpdDBbyValue.h"
#include "FormGF_Logic.h"
#include "DB_CellValue.h"
#include "FormGFLogic.h"

void TFormGFLogic::UpdPlGrFin(TFormLogicF1* FormLogicF1) {
    int i, InitKolCol;
    TwwStringList* p;
    PAtrRow* AtrRow;
    TDateTime dt, BegDate, EndDate;
    int Y, M, D;
    PCellValueAtr vd;
    TDateTime DateLastGrFinForMonth;
    std::string s, attrib3;
    int id_FromPref;

    //mak ??? DateLastGrFinForMonth = TDB_MWSModuls::GetDateLastGrFinForMonth(2, FormLogicF1->CurrYear, FormLogicF1->CurrMonth, id_FromPref);
    p = TFormDopInp::GetTww(FormLogicF1);
    if (p->vid_form != 2) return; // {график финансирования}
    if (p->work_with != 1) return;
    if (!(p->work_with_gf == 10 || p->work_with_gf == 13)) return;
    InitKolCol = p->InitKolCol;
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    BegDate = EncodeDate(FormLogicF1->CurrYear, FormLogicF1->CurrMonth, 1);
    EndDate = EncodeDate(FormLogicF1->CurrYear, FormLogicF1->CurrMonth, DaysInAMonth(FormLogicF1->CurrYear, FormLogicF1->CurrMonth));

    q->SQL->Clear();
    q->SQL->Add("SELECT den_potok_id,plan_id,pokaz_id,cell_id,vid_otc,value_date,value,coef,str_value,attrib1,attrib2,attrib3,attrib4,attrib5,objectid,reg_nomer,fromXML,id_FromPref");
    q->SQL->Add("FROM Denegn_potok");
    q->SQL->Add("WHERE plan_id = :plan_id");
    q->SQL->Add("AND vid_otc = 4");
    q->SQL->Add("AND objectid = 0"); // здесь нужен objectid, поскольку выбираем только мероприятия
    q->SQL->Add("AND value_date BETWEEN :BegDate AND :EndDate");
    q->ParamByName("plan_id")->AsInteger() = FormLogicF1->CurrPlanId;
    q->ParamByName("BegDate")->AsDateTime() = BegDate;
    q->ParamByName("EndDate")->AsDateTime() = EndDate;
    q->Open();
    while (!q->EoF()) {
        attrib3 = q->FieldByName("attrib3")->AsString();
        s = attrib3.substr(8, 19);
        dt = StrToDateTime(s);
        if (DateLastGrFinForMonth > 0 && dt < DateLastGrFinForMonth) goto LL;
        DecodeDate(q->FieldByName("value_date")->AsDateTime(), Y, M, D);
        for (i = 1; i <= FormLogicF1->f1Forms->LastRow; i++) {
//            AtrRow = p->ListRows.Items[i];
            PAtrRow AtrRow = ((*p->ListRows)[i]);

            if (AtrRow == nullptr || AtrRow->CurRowVid != 2 || !AtrRow->IsUpd) continue;
            vd = TFormDopInp::GetCellValueAtrByRC(FormLogicF1, i, D + InitKolCol);
            if (vd == nullptr || vd->PokazId != q->FieldByName("pokaz_id")->AsInteger()) continue;
            FormLogicF1->f1Forms->NumberRC(i,D + InitKolCol) = PreobrValue(q->FieldByName("value")->AsFloat(), vd->DataType, vd->coef_Pokaz, vd->coef_Forma);
            vd->DPId = q->FieldByName("den_potok_id")->AsInteger();
        }
    LL:
        q->Next();
    }
    delete q;
}


void TFormGFLogic::UpdFactGrFin(TFormLogicF1* FormLogicF1) {
    int i, InitKolCol, id, NColMes;
    TwwStringList* p;
    PAtrRow AtrRow;
    TDateTime BegDate, EndDate;
    int Y, M, D;
    PCellValueAtr vd1, vd;
    int Row1, Col1, Row2, Col2;
    int crFinansir;

    crFinansir = SystFuncs::SystFuncsAltWin::GetColorFromRGB(76, 202, 80); // green
    FormLogicF1->f1Forms->Selection_Get(0, Row1, Col1, Row2, Col2);
    p = TFormDopInp::GetTww(FormLogicF1);
    if (p->vid_form != 2) { // not financing graph
        return;
    }
    if (p->work_with != 1) {
        return;
    }
    if (!(p->work_with_gf == 11 || p->work_with_gf == 12)) {
        return;
    }
    InitKolCol = p->InitKolCol;
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    BegDate = EncodeDate(FormLogicF1->CurrYear, FormLogicF1->CurrMonth, 1);
    EndDate = EncodeDate(FormLogicF1->CurrYear, FormLogicF1->CurrMonth, DaysInAMonth(FormLogicF1->CurrYear, FormLogicF1->CurrMonth));
    q->SQL->Clear();
    q->SQL->Add("SELECT den_potok_id,plan_id,pokaz_id,cell_id,vid_otc,value_date,value,coef,str_value,");
    q->SQL->Add("attrib1,attrib2,attrib3,attrib4,attrib5,objectid,reg_nomer,fromXML,id_FromPref");
    q->SQL->Add("FROM Denegn_potok");
    q->SQL->Add("WHERE plan_id = :plan_id");
    q->SQL->Add("AND vid_otc = 5");
    q->SQL->Add("AND objectid = 0"); // we need objectid, because we select only activities
    q->SQL->Add("AND value_date BETWEEN :BegDate AND :EndDate");
    q->ParamByName("plan_id")->AsInteger() = FormLogicF1->CurrPlanId;
    q->ParamByName("BegDate")->AsDateTime() = BegDate;
    q->ParamByName("EndDate")->AsDateTime() = EndDate;
    q->Open();
    while (!q->EoF()) {
        // find the needed cell
        DecodeDate(q->FieldByName("value_date")->AsDateTime(), Y, M, D); // here D is the column number
        for (i = 1; i <= FormLogicF1->f1Forms->LastRow; i++) {
//            AtrRow = p->ListRows.Items[i];

            PAtrRow AtrRow = ((*p->ListRows)[i]);

            if (AtrRow == nullptr) {
                continue;
            }
            if (AtrRow->CurRowVid != 2) {
                continue;
            }
            if (q->FieldByName("objectid")->AsInteger() == 0) {
                id = q->FieldByName("pokaz_id")->AsInteger();
            } else {
                id = q->FieldByName("objectid")->AsInteger();
            }
            if (AtrRow->ObjectId != id) {
                continue;
            }
            if (!AtrRow->IsUpd) {
                continue;
            }
            NColMes = TFormGF_Logic::IsMesFact(FormLogicF1);
            vd = TFormDopInp::GetCellValueAtrByRC(FormLogicF1, i, NColMes);
            if (vd == nullptr) {
                continue;
            }
            vd1 = TFormDopInp::GetCellValueAtrByRC(FormLogicF1, i, D + InitKolCol);
            if (vd1 == nullptr) {
                TFormDopInp::AddValueCellAtr(FormLogicF1, i, D + InitKolCol, vd->CellId,
                    q->FieldByName("value_date")->AsDateTime(), 30 + deltaDType, 5, vd->PlanId, vd->coef_Pokaz, vd->coef_Forma,
                    vd->ObjectId, vd->PokazId, 0, vd->pokaz_type, vd->cell_period, vd->ist_fin_id);
            }
            vd1 = TFormDopInp::GetCellValueAtrByRC(FormLogicF1, i, D + InitKolCol);
            if (vd1 == nullptr) {
                continue;
            }
            if (vd1->PokazId != q->FieldByName("pokaz_id")->AsInteger()) {
                continue;
            }
            FormLogicF1->f1Forms->Selection_Set(i, D + InitKolCol, i, D + InitKolCol);
            FormLogicF1->f1Forms->Number = PreobrValue(q->FieldByName("value")->AsFloat(), vd1->DataType, vd1->coef_Pokaz, vd1->coef_Forma);
            FormLogicF1->f1Forms->FontColor = crFinansir; // green
            vd1->DPId = q->FieldByName("den_potok_id")->AsInteger();
        }
        q->Next();
    }
    delete q;
    FormLogicF1->f1Forms->Selection_Set(Row1, Col1, Row2, Col2);
}

void TFormGFLogic::UpdFactKv(TFormLogicF1* FormLogicF1) {
    TwwStringList* p = TFormDopInp::GetTww(FormLogicF1);
    for (int i = 1; i <= FormLogicF1->f1Forms->LastRow; i++) {
//        PAtrRow* AtrRow = p->ListRows[i-1];

        PAtrRow AtrRow = ((*p->ListRows)[i-1]);

        if (AtrRow == nullptr || AtrRow->CurRowVid != 2 || !AtrRow->IsUpd)
            continue;
        for (int j = 1; j <= FormLogicF1->f1Forms->LastCol; j++) {
            PCellValueAtr vd = TFormDopInp::GetCellValueAtrByRC(FormLogicF1, i, j);
            if (vd == nullptr || vd->pokaz_type != 1 || vd->VidOtch != 5 || vd->EvalType != 0 || vd->cell_period != 2)
                continue;
            FormLogicF1->f1Forms->NumberRC(vd->Row, vd->Col) = TDB_CellValue::GetValue(vd->ObjectId, vd->CellId, vd->PlanId, vd->VidOtch, vd->ValueDate);
        }
    }
}

void TFormGFLogic::SetIsUpdToFalse(TFormLogicF1* FormLogicF1) {
    TwwStringList* p = TFormDopInp::GetTww(FormLogicF1);
    for (int i = 1; i <= FormLogicF1->f1Forms->LastRow; i++) {
//        AtrRow* AtrRow = p->ListRows.Items[i];

        PAtrRow AtrRow = ((*p->ListRows)[i]);

        if (AtrRow == nullptr) continue;
        if (AtrRow->CurRowVid != 2) continue;
        if (!AtrRow->IsUpd) continue;
        TForm2Plus::ExecForMonth(FormLogicF1, i);
        AtrRow->IsUpd = false;
    }
}

