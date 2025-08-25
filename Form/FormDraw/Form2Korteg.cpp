#include "../SystFuncs/SystFuncsLogic.h"
#include "../DB/DB_FormStr.h"
#include "FormDopInp.h"
#include "TT_Korteg.h"
#include "../DB/unDefDBconnect.h"
#include "../DB/unDBType.h"
#include "../DB/DB_Pokaztl.h"
#include "../DB/DB_PlanStr.h"
#include "../DB/DB_Plan.h"
#include "Form2find.h"
#include "../DB/DB_ComonNPokaz.h"
#include "Form1.h"
#include "../DB/DB_EdIzm.h"
#include "../Pokazateli/unIDsClassFieldCell.h"
#include "Form2Korteg.h"

void TForm2Korteg::doForm2Korteg(TQueryUni* aQOwnerPlanStr, int aPlan_id, PDopAtrCol aDopAtrCol, std::string aStmntRow) {
    int nClmn;
    std::string Stmnt;  // текущий обрабатываемый в ExecPokaz показатель
    int stmntClass;
    int I, Index, wCurrCol;
    TQueryUni *qDefPokaz;
    int pokaz_is_razd;
    TDateTime PokazDate;
    std::string pokaz_bk_codeKorteg, ws;
    int pokaz_idKorteg;
    int otkuda;
    TFormatStyle FormatStyle;
    int plan_id_2podvedomstv;
    int wpokaz_id, wwpokaz_id, wplan_id, newCurrPlanId = 0;
    PAtrRow AtrRow;
    TForm2Korteg *Form2Korteg;
    PAtrStmnt AtrStmnt;

    for (I = qDefPokazList.Count() - 1; I >= 0; I--) {

        qDefPokaz = std::get<TQueryUni *>(qDefPokazList.Objects[I]);
        delete qDefPokaz;
        qDefPokazList.Delete(I);
    }
    pokaz_bk_codeKorteg = aQOwnerPlanStr->FieldByName("pokaz_bk_code")->AsString();
    pokaz_idKorteg = aQOwnerPlanStr->FieldByName("pokaz_id")->AsInteger();
    for (nClmn = 1; nClmn <= p->MaxHrCol; nClmn++) {
        // если в наименовании стоит значение - Ошибка в определении формы
        if (nClmn == p->PokazCol)
            continue;
        AtrStmnt = &(p->ListStmnt[nClmn]);
        Stmnt = AtrStmnt->Stmnt;
        qDefPokaz = aQOwnerPlanStr;
        pokaz_is_razd = aQOwnerPlanStr->FieldByName("pokaz_is_razd")->AsInteger();
        switch (FormLogicF1->quForms->FieldByName("work_with")->AsInteger()) {
            case 1: {
                otkuda = TFormDopInpMF::GetPokazIdForCol2(note2, nClmn, CurrObject.isSimvR);
                CurrPokazId = T_Korteg::FindByKBK_Plc_Rsp(otkuda, pokaz_bk_codeKorteg, pokaz_idKorteg,
                                                          FormLogicF1->CurrYear, true);
                if (CurrPokazId == 0)
                    CurrObject.pokaz_id = 0;
                if (otkuda == 5) //{Пустая ячека}
                {
                    goto LNext;
                }
                if (otkuda == 0 && CurrPokazId == 0) {
                    CurrPokazId = pokaz_idKorteg;
                    CurrObject.pokaz_id = CurrPokazId;
                    CurrObject.pokaz_is_razd = pokaz_is_razd;
                }
                if (otkuda > 10) {
                    CurrPokazId = otkuda;
                    CurrObject.pokaz_id = CurrPokazId;
                    CurrObject.pokaz_is_razd = pokaz_is_razd;
                    if (qDefPokazList.Find(IntToStr(CurrPokazId), Index)) {
                        int wi = 0;
                        qDefPokaz = qDefPokazList.getObjectByKey<TQueryUni*>(IntToStr(CurrPokazId));
                        wi = 1;
                    } else {
                        TQueryUni *qDefPokaz = new TQueryUni(DefDBconnect.Conn);
                        qDefPokazList.AddObject(IntToStr(CurrPokazId), qDefPokaz);
                        wplan_id = AtrStmnt->plan_id;
                        qDefPokaz->SQL->Add("SELECT b.* FROM Pokaztl b");
                        qDefPokaz->SQL->Add("WHERE b.pokaz_id = " + IntToStr(CurrPokazId));
                        switch (GetDBType()) {
                            case TDBType::MSSQL: {
                                qDefPokaz->SQL->Add("AND " + IntToStr(FormLogicF1->CurrYear) +
                                                    " BETWEEN YEAR(b.pokaz_beg_date) AND YEAR(b.pokaz_end_date)");
                                break;
                            }
                            case TDBType::Postgre: {
                                qDefPokaz->SQL->Add("AND " + IntToStr(FormLogicF1->CurrYear) +
                                                    " BETWEEN date_part('year', b.pokaz_beg_date) AND date_part('year', b.pokaz_end_date)");
                                break;
                            }
                        }
                        qDefPokaz->Open();
                    }
                }
                qDefPokaz->Locate("pokaz_id", CurrPokazId);
                break;
            }
        }
        PlanId = AtrStmnt->plan_id;
        if (PlanId == 0)
            PlanId = FormLogicF1->CurrPlanId;
        if (aPlan_id > 0)
            PlanId = aPlan_id;
        if (FormLogicF1->quForms->FieldByName("work_with")->AsInteger() == 4 && PlanId == 0 && newCurrPlanId > 0)
            PlanId = newCurrPlanId;

        if (PlanId == 1)
            PlanId = TDB_Plan::GetPodVedomstvId(FormLogicF1->CurrPlanId, false, plan_id_2podvedomstv);

        if ((PlanId == 0 || PlanId == 1) && aDopAtrCol->is_planId)
            PlanId = aDopAtrCol->plan_id;
        if (otkuda == 3)
            PlanId = TDB_Plan::GetParentIid(PlanId, true);
        if (otkuda == 4)
            PlanId = TDB_Plan::GetPodVedomstvId(PlanId, true, plan_id_2podvedomstv);
        VidOtch = AtrStmnt->vid_otch;
        if (VidOtch == 0)
            VidOtch = FormLogicF1->CurrVidOtch;
        PokazId = CurrPokazId;
        PokazType = qDefPokaz->FieldByName("pokaz_type")->AsInteger();
        if (PokazType == 4)
            EnumId = qDefPokaz->FieldByName("enum_id")->AsInteger();
        wCurrCol = nClmn;
        stmntClass = TForm2find::findByStmnt(Stmnt);
        if (stmntClass == 12) {
            //(Stmnt = 'ЗНАЧЕНИЕСВ') or (Stmnt = 'ЗНАЧЕНИЕСВ_ДООПРЕД') or (Stmnt = 'ЗНАЧЕНИЕНВ') or
            //(Stmnt = 'ХарактСВобъекта') or (Stmnt = 'КонкатСтрокСВобъек') or
            //(Stmnt = 'ПРОЦЕНТСВ') or (Stmnt = 'ПРОЦЕНТНВ') or
            //(Stmnt = 'ОТКЛОНЕНИЕСВ') or (Stmnt = 'ОТКЛОНЕНИЕНВ') or
            //(Stmnt = 'ЗНАЧЕНИЕСВНИ') or (Stmnt = 'ЗНАЧЕНИЕНВНИ') or
            //(Stmnt = 'ПРОЦЕНТСВНИ') or (Stmnt = 'ПРОЦЕНТНВНИ') or
            //(Stmnt = 'КБК-Дгвр:Сумма№БО'{'ОТКЛОНЕНИЕСВНИ'}) or (Stmnt = 'КБК-Дгвр:СуммаБО'{'ОТКЛОНЕНИЕНВНИ'}) or
            //(Stmnt = 'ОТКЛОНЕНИЕСВНачКВ') or (Stmnt = 'ОТКЛОНЕНИЕНВНачКВ') or
            //(Stmnt = 'ЗНАЧЕНИЕСВИР') or (Stmnt = 'ЗНАЧЕНИЕНВИР') or
            //(Stmnt = 'УТОЧНЕНИЕСВ') or (Stmnt = 'УТОЧНЕНИЕСВНИ') or (Stmnt = 'УТОЧНЕНИЕСВПосле') or
            //(Stmnt = 'Мрпр-Дгвр:Сумма№БО'{'УТОЧНЕНИЕНВ'}) or (Stmnt = 'Мрпр-Дгвр:СуммаБО'{'УТОЧНЕНИЕНВНИ'}) or
            //(Stmnt = 'ФактКвДату'{'УТОЧНЕНИЕСВдф'}) or (Stmnt = 'СуммаПлГрНИВМесяце'{'УТОЧНЕНИЕСВНИдф'}) or
            //(Stmnt = 'ПлнОСТАТОК') or (Stmnt = 'ФктОСТАТОК') or (Stmnt = 'ПлнОСТАТОКНИ') or (Stmnt = 'ФактВсегоВНачале') or (Stmnt = 'УточПланВклДату') or
            //(Stmnt = 'СуммаПриход') or (Stmnt = 'СуммаВозврат') or (Stmnt = 'СуммаПлГрНИВНачале') or (Stmnt = 'СуммаПлГрНИГод') or
            //(Stmnt = 'ПДГ_УТ_КассПл') or (Stmnt = 'УТВ_УТ_КассПл') or (Stmnt = 'Экономия/Резерв') or
            //(Stmnt = 'отчетCO_RP_015') or (Stmnt = 'СведенияОбЭкономии') or
            //(Stmnt = 'КонкатСтрок'{специально пускаем в обработку, чтобы найти первый cell_id})
            coef_Pokaz = TDB_EdIzm::GetCoef_PokazOpt(aQOwnerPlanStr->FieldByName("edizm_id")->AsInteger());
            // здесь нельзя использовать stmntClass, т.к. перечисленные ниже входят в 12 класс
            if ((Stmnt == "ПРОЦЕНТНВ") || (Stmnt == "ЗНАЧЕНИЕНВНИ") || (Stmnt == "ПРОЦЕНТНВНИ") ||
                (Stmnt == "ОТКЛОНЕНИЕНВНачКВ") || (Stmnt == "ЗНАЧЕНИЕНВИР")) {
                if (!GetNaturVid()) {
                    goto LNext;
                }
            }
                coef_Forma = AtrStmnt->CoefPlus1 - 1;
                if (coef_Forma < 0)
                    coef_Forma = coef_Pokaz; // // вариант Как у показателя
                else
                    coef_Forma = coef_Forma * 3;
                PeriodType = AtrStmnt->PeriodType;
                period = PeriodType;
                ValuePeriod = AtrStmnt->ValuePeriod;
                if (aDopAtrCol->is_UstPeriod) {
                    PeriodType = aDopAtrCol->Period;
                    period = PeriodType;
                    ValuePeriod = aDopAtrCol->ValuePeriod;
                }
                FormLogicF1->quCell->Params0 = PokazId;
                FormLogicF1->quCell->Params3 = AtrStmnt->din_izm;
                PeriodT = 0;
                Cnt = 1;
                switch (PeriodType) {
                    case 1:
                    case 2:
                    case 3: {
                        FormLogicF1->quCell->Params1 = 1;
                        Kv = 0;
                        Mes = 0;
                        switch (PeriodType) {
                            case 2: {
                                PeriodT = 2;
                                break;
                            }
                            case 3: {
                                PeriodT = 3;
                                break;
                            }
                        }
                        break;
                    }
                    case 4:
                    case 5: {
                        FormLogicF1->quCell->Params1 = 2;
                        Kv = FormLogicF1->CurrQuarter;
                        Mes = 0;
                        if (PeriodType == 5)
                            PeriodT = 4;
                        break;
                    }
                    case 6:
                    case 7: {
                        FormLogicF1->quCell->Params1 = 3;
                        Kv = 0;
                        Mes = FormLogicF1->CurrMonth;
                        if (PeriodType == 7)
                            Mes = 1; // // это условно
                        break;
                    }
                    default: {
                        FormLogicF1->quCell->Params1 = 0;
                        Kv = 0;
                        Mes = 0;
                        break;
                    }
                }
                switch (PeriodT) {
                    case 2: {
                        Cnt = 5;
                        break;
                    }
                    case 3: {
                        Cnt = 13;
                        break;
                    }
                    case 4: {
                        Cnt = 4;
                        PeriodT = 3;
                        break;
                    }
                }
                if (PeriodT == 0)
                    PeriodType = FormLogicF1->quCell->Params1;
                else
                    PeriodType = PeriodT;
                PT = 0;
                if (qDefPokaz->FieldByName("pokaz_is_god")->AsInteger() == 1)
                    PT = 1;
                if (qDefPokaz->FieldByName("pokaz_is_kv")->AsInteger() == 1)
                    PT = 2;
                if (qDefPokaz->FieldByName("pokaz_is_mes")->AsInteger() == 1)
                    PT = 3;
                IsIstFin = AtrStmnt->is_ist_fin;
                if (IsIstFin == 1) {
                    IstFinGradId = AtrStmnt->ist_fin_grad_id;
                    if (qDefPokaz->FieldByName("pokaz_is_ist_fin")->AsInteger() == 0) {
                        IstFinGradId = 0;
                        IsIstFin = 0;
                    }
                } else
                    IstFinGradId = 0;
                if (aDopAtrCol->is_UstIstFin) {
                    if (qDefPokaz->FieldByName("pokaz_is_ist_fin")->AsInteger() == 0) {
                        IstFinGradId = 0;
                        IsIstFin = 0;
                    } else {
                        IsIstFin = 1;
                        IstFinGradId = aDopAtrCol->ist_fin_grad_id;
                        AtrRow = TFormDopInp::GetListRowsItem(FormLogicF1, FormLogicF1->CurrInputRow);
                        AtrRow->ist_fin_grad_id = IstFinGradId;
                    }
                }
                IsGroup = AtrStmnt->IsGroup;
                AtrStmnt = &(p->ListStmnt[nClmn]);
                AtrRow = TFormDopInp::GetListRowsItem(FormLogicF1, FormLogicF1->CurrInputRow);
                for (I = 1; I <= Cnt; I++) {
                    if (IsIstFin == 1 && IstFinGradId <= 0) {
                        if (FormLogicF1->FormDM->quIstFin->Active() == false)
                            FormLogicF1->FormDM->quIstFin->Open();
                        FormLogicF1->FormDM->quIstFin->First();
                        if (!FormLogicF1->FormDM->quIstFin->EoF()) {
                            while (!FormLogicF1->FormDM->quIstFin->EoF()) {
                                IstFinGradId = FormLogicF1->FormDM->quIstFin->FieldByName("ist_fin_id")->AsInteger();
                                InputWithDinIzm(FormLogicF1->CurrInputRow, nClmn, AtrStmnt, AtrRow, Stmnt, qDefPokaz,
                                                aDopAtrCol);
                                FormLogicF1->FormDM->quIstFin->Next();
                            }
                        }
                    } else
                        InputWithDinIzm(FormLogicF1->CurrInputRow, nClmn, AtrStmnt, AtrRow, Stmnt, qDefPokaz,
                                        aDopAtrCol);
                    switch (PeriodT) {
                        case 2: {
                            Kv = I;
                            Mes = 0;
                            break;
                        }
                        case 3: {
                            Kv = 0;
                            Mes = I;
                            break;
                        }
                        default: {
                            break;
                        }
                    }
                    if (I == 1)
                        FormLogicF1->quCell->Params1 = PeriodT;

                }
        } else if (stmntClass == 15) {
            //-- (Stmnt = 'ЗНАЧЕНИЕобщНП') or (Stmnt = 'ЗНАЧЕНИЕобщНПИР') or 'СВОДобщНПпоСУБЪвАльб' or 'СВОДобщНПпоСУБЪвА/n'
            if (!(pokaz_is_razd == 2 || pokaz_is_razd == 4))
                goto LNext;
            coef_Forma = AtrStmnt->CoefPlus1 - 1;
            if (coef_Forma < 0)
                coef_Forma = coef_Pokaz;
            else
                coef_Forma = coef_Forma * 3;
            SetValueDataForComNP(nClmn, Stmnt, aDopAtrCol);
        } else if (stmntClass == 1) {
            // Stmnt = 'ПРОИЗВСТОЛБ'
            ////?????
        } else if (stmntClass == 2) {
            //(Stmnt = 'ааЗначВырМнрг') or (Stmnt = 'ааДельтаЭтлн') or (Stmnt = 'ааИдентифТенд')
            TFormDopInp::AddValueCellAtr(FormLogicF1, FormLogicF1->CurrInputRow, wCurrCol, 0, 0, 0, VidOtch, PlanId,
                                         0, 0, CurrObject.pokaz_id, PokazId, 0, 0, 0, 0);
        } else if (stmntClass == 3) {
            //(Stmnt = 'ааПрирост') or (Stmnt = 'ааПроцПриростБаза')
            switch (p->ListStmnt[wCurrCol].is_set_god) {
                case 0: //{наследуется}
                {
                    PokazDate = EncodeDate(FormLogicF1->CurrYear + p->ListStmnt[wCurrCol].god_plus, 12, 31);
                    break;
                }
                case 1: //{задается}
                {
                    PokazDate = EncodeDate(p->ListStmnt[wCurrCol].god_ravno, 12, 31);
                    break;
                }
            }
            TFormDopInp::AddValueCellAtr(FormLogicF1, FormLogicF1->CurrInputRow, wCurrCol, 0, PokazDate, 0, VidOtch,
                                         PlanId, 0, 0, CurrObject.pokaz_id, PokazId, 0, 0, 0, 0);
        } else if (stmntClass == 4) {
            // (Stmnt = 'РАСЧСЧЕТ') or (Stmnt = 'ИНН') or (Stmnt = 'ЮрАдрес')
            TFormDopInp::AddValueCellAtr(FormLogicF1, FormLogicF1->CurrInputRow, wCurrCol, 0, 0, 0, VidOtch, PlanId,
                                         0, 0, CurrObject.pokaz_id, PokazId, 1, 0, 0, 0);
        } else if (stmntClass == 5) {
            // (Stmnt = 'БДЖПОЛУЧТЛЬ') or (Stmnt = 'БДЖПОЛУЧТЛЬ_нет()')
            TFormDopInp::AddValueCellAtr(FormLogicF1, FormLogicF1->CurrInputRow, wCurrCol, 0, 0, 2, VidOtch, PlanId,
                                         0, 0, CurrObject.pokaz_id, PokazId, 1, 0, 0, 0);
        } else if (stmntClass == 6) {
            // (Stmnt = 'РАСЧСЧЕТ2') or (Stmnt = 'ЛицевойСчетТБ')
            TFormDopInp::AddValueCellAtr(FormLogicF1, FormLogicF1->CurrInputRow, wCurrCol, 0, 0, 0, VidOtch, PlanId,
                                         0, 0, CurrObject.pokaz_id, PokazId, 1, 0, 0, 0);
        } else if (stmntClass == 7) {
            //(Stmnt = 'НАИМПОКАЗ') or ( = 'НАИМЦелСт') or (Stmnt = 'КОДБК') or
            // (Stmnt = 'КОДБК1') or (Stmnt = 'КОДБКРздл') or (Stmnt = 'КОДБКРздл1') or (Stmnt = 'КОДБКЦелСт') or
            // (Stmnt = 'КОДБКЦелСт1') or (Stmnt = 'КОДБКВидРасх') or (Stmnt = 'КОДБКЭкКл') or
            //(Stmnt = 'НомерМрпр') or (Stmnt = 'КБК-Дгвр:Кол-во') or (Stmnt = 'КБК-Дгвр:Кол-воБО') or
            // (Stmnt = 'Владелец') or (Stmnt = 'forUnikIdentif') or (Stmnt = 'ПолныйНомер') or
            // (Stmnt = 'ИмяИстФин') or (Stmnt = 'ДатаСоздОбъекта')
            doStmnt(nClmn, FormLogicF1->CurrInputRow, aQOwnerPlanStr, Stmnt, wCurrCol);
            TFormDopInp::AddValueCellAtr(FormLogicF1, FormLogicF1->CurrInputRow, wCurrCol, 0, 0, 0, VidOtch, PlanId,
                                         0, 0, CurrObject.pokaz_id, PokazId, 1, 0, 0, 0);
        } else if (stmntClass == 14) {
            //(stmnt = 'Примечание') or (stmnt = 'Ведомство') or
            doStmnt(nClmn, FormLogicF1->CurrInputRow, aQOwnerPlanStr, Stmnt, wCurrCol);
            TFormDopInp::AddValueCellAtr(FormLogicF1, FormLogicF1->CurrInputRow, wCurrCol, 0, 0, 0, VidOtch, PlanId,
                                         0, 0, CurrObject.pokaz_id, PokazId, 0, 0, 0, 0);
        } else if (stmntClass == 8) {
            // Stmnt = 'ЕДИЗМ'
            doStmnt(nClmn, FormLogicF1->CurrInputRow, aQOwnerPlanStr, Stmnt, wCurrCol);
            TFormDopInp::AddValueCellAtr(FormLogicF1, FormLogicF1->CurrInputRow, wCurrCol,
                                         aQOwnerPlanStr->FieldByName("edizm_id")->AsInteger(), 0, 0, VidOtch,
                                         PlanId, 0, 0, CurrObject.pokaz_id, PokazId, 1, 0, 0, 0);
        } else if (stmntClass == 9) {
            // Stmnt = 'ЕДИЗМНВ'
            if (!GetNaturVid())
                goto LNext;
            doStmnt(nClmn, FormLogicF1->CurrInputRow, aQOwnerPlanStr, Stmnt, wCurrCol);
            TFormDopInp::AddValueCellAtr(FormLogicF1, FormLogicF1->CurrInputRow, wCurrCol,
                                         FormLogicF1->FormDM->quPokaztl->FieldByName("edizm_id")->AsInteger(), 0, 0,
                                         VidOtch, PlanId, 0, 0, CurrObject.pokaz_id, PokazId, 1, 0, 0, 0);
        } else if (stmntClass == 10) {
            // Stmnt = 'ЕДИЗМобщНП'
            doStmnt(nClmn, FormLogicF1->CurrInputRow, aQOwnerPlanStr, Stmnt, wCurrCol);
            TFormDopInp::AddValueCellAtr(FormLogicF1, FormLogicF1->CurrInputRow, wCurrCol,
                                         TIDsClassFieldCell::getCNPedizm(AtrStmnt->natur_vid), 0, 0, VidOtch,
                                         PlanId, 0, 0, CurrObject.pokaz_id, PokazId, 1, 0, 0, 0);
        } else if (stmntClass == 11) {
            // (Stmnt = 'ФОРМУЛАF1')
            TFormDopInp::AddValueCellAtr(FormLogicF1, FormLogicF1->CurrInputRow, wCurrCol, 0, 0, 0, VidOtch, PlanId,
                                         0, 0, CurrObject.pokaz_id, PokazId, 1, 0, 0, 0);
            // вставляем формулу в F1
            ws = p->ListStmnt[wCurrCol].note1;
            if (!ws.empty() && ws[0] == '[')
                ws = TFormDopInp::GetSubFormulaByNpar(ws, FormLogicF1->CurrInputRow);
            Value = TFormDopInp::ReplaceNRowNCol(FormLogicF1->CurrInputRow, wCurrCol, ws);
            TFormDopInp::getColFrmtrAtr(FormLogicF1, 1, wCurrCol, FormatStyle);
            TForm1::SetCellValue(FormLogicF1, FormLogicF1->f1Forms, p->MaxHrCol, FormLogicF1->CurrInputRow,
                                 wCurrCol, FormLogicF1->CurrInputRow, wCurrCol, Value, 0, &FormatStyle);
        } else if (stmntClass == 16) {
            (Stmnt = "КОРТЕЖ");
            doStmnt(nClmn, FormLogicF1->CurrInputRow, aQOwnerPlanStr, Stmnt, wCurrCol);
            TFormDopInp::AddValueCellAtr(FormLogicF1, FormLogicF1->CurrInputRow, wCurrCol, 0, 0, 2,
                                         VidOtch, PlanId, 0, 0, CurrObject.pokaz_id, PokazId, 0, 0, 0, 0);
        }

        LNext:;
        }
        CurrPokazId = pokaz_idKorteg;
    }
