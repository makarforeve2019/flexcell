#include "SystFuncsLogic.h"
#include "unDefDBconnect.h"
#include "DB_TmpPokaztl.h"
#include "DB_bkMetaDef.h"
#include "DB_EdIzm.h"
#include "unDBType.h"
#include "Form2PlusPatternKBK.h"

// создать шаблон для Like SELECT из шаблона КБК
std::string TForm2PlusPatternKBK::createLikePattern(int CurrYear, std::string shablon) {
    std::string Rzdl, CS, VR, KOSGU, codeKBK;
    TStringList names;
    names.CommaText = shablon;

    Rzdl  = names.Values["Rzdl"];
	Rzdl = Rzdl + DupeString("_", 4-Rzdl.length());
	CS = names.Values["CS"];
	CS = CS + DupeString("_", TDB_bkMetaDef::GetDl_CS(CurrYear)-CS.length());
	VR = names.Values["VR"];
	VR = VR + DupeString("_", 3-VR.length());
	KOSGU = names.Values["KOSGU"];
	KOSGU = KOSGU + DupeString("_", 3-KOSGU.length());
	
    if (codeKBK.empty()) {
        codeKBK = Rzdl + CS + VR + KOSGU;
    } else {
        codeKBK += "%";
    }

    return codeKBK; 

}

// создать TValueData для столбцов - аналог ExecPokaz для строки вида СУММАшаблонКБК
void TForm2PlusPatternKBK::ExecPokazPatternKBK(PDopAtrCol aDopAtrCol, TFormLogicF1* FormLogicF1, int aRow, TwwStringList* p, TQueryUni* q) {
    int god, Kv, Mes;
    std::string Stmnt;
    int i, PlanId, newCurrPlanId;
    std::string note2, nextItem, exprF1;
    int is_filter, is_filter2, ColOnFilter2;
    int VidOtch, coef_Forma, ist_fin_id, PokazId, PeriodType, ValuePeriod, cell_period;
    int GradId, IsIstFin, CoefPlus1;
    TDateTime ValueDate;
    TFormatStyle* FormatStyle = new TFormatStyle();

    note2 = q->FieldByName("note2")->AsString();

    newCurrPlanId = q->FieldByName("plan_id")->AsInteger();
    if (aDopAtrCol->is_UstIstFin) {
        IsIstFin = 1;
        GradId = aDopAtrCol->ist_fin_grad_id;
    } else {
        GradId = 0;
        IsIstFin = 0;
    }

    for (i = 1; i <= p->MaxHrCol; i++) {
        if (i == TFormDopInp::GetNameCol(FormLogicF1)) continue;

        Stmnt = p->ListStmnt[i].Stmnt;

        if (Stmnt != "ЗНАЧЕНИЕСВ" && Stmnt != "ЗНАЧЕНИЕСВИР" && Stmnt != "ФОРМУЛАF1") continue;
        TFormDopInp::getColFrmtrAtr(FormLogicF1, 1, i, *FormatStyle);
        CoefPlus1 = p->ListStmnt[i].CoefPlus1;

        nextItem = TFormDopInp::GetFormulaForColpar(note2, i);

        if (Stmnt == "ФОРМУЛАF1") {
            if (nextItem == "" || nextItem == "^Сумма") {
                exprF1 = p->ListStmnt[i].note1;
                exprF1 = "^" + exprF1;
            } else {
                exprF1 = nextItem;
            }
            exprF1 = TFormDopInp::ReplaceNRowNCol(FormLogicF1->CurrInputRow, i, exprF1);
            TForm1::SetCellValue(FormLogicF1, FormLogicF1->f1Forms, p->MaxHrCol, FormLogicF1->CurrInputRow, i, FormLogicF1->CurrInputRow, i, exprF1, 0, FormatStyle);
            continue;
        }

        if (nextItem == "") continue;
        if (nextItem == "^Сумма") {
            VidOtch = p->ListStmnt[i].vid_otch;
            if (VidOtch == 0) VidOtch = FormLogicF1->CurrVidOtch;
            ist_fin_id = p->ListStmnt[i].ist_fin_grad_id;
            if (aDopAtrCol->is_UstIstFin) ist_fin_id = aDopAtrCol->ist_fin_grad_id;

            PeriodType = p->ListStmnt[i].PeriodType;
            ValuePeriod = p->ListStmnt[i].ValuePeriod;
            PokazId = p->ListStmnt[i].field_id;
            PlanId = p->ListStmnt[i].plan_id;
            if (PlanId == 0) PlanId = FormLogicF1->CurrPlanId;
            if (newCurrPlanId > 0) PlanId = newCurrPlanId;

            // установка года
            god = FormLogicF1->CurrYear;

            if (aDopAtrCol->is_UstGoda) {
                if (aDopAtrCol->is_set_god == 0) {
                    if (aDopAtrCol->god_plus != 0) {
                        god += aDopAtrCol->god_plus;
                    }
                } else {
                    god = aDopAtrCol->god_ravno;
                }
            } else {
                if (p->ListStmnt[i].is_set_god == 0) {
                    if (p->ListStmnt[i].god_plus != 0) {
                        god += p->ListStmnt[i].god_plus;
                    }
                } else {
                    god = p->ListStmnt[i].god_ravno;
                }
            }

            ValueDate = EncodeDate(god, 1, 31);
            Kv = 0;
            if (PeriodType == 4) Kv = ValuePeriod;
            Mes = 0;
            if (PeriodType == 6) Mes = ValuePeriod;

            if (Kv > 0) {
                ValueDate = IncMonth(ValueDate, Kv * 3 - 1);
            } else if (Mes > 0) {
                if (Mes > 1) {
                    ValueDate = IncMonth(ValueDate, Mes - 1);
                }
            } else {
                ValueDate = EncodeDate(god, 12, 31);
            }

            if (aDopAtrCol->is_UstPeriod) {
                if (aDopAtrCol->Period == 4) {
                    if (aDopAtrCol->ValuePeriod > 0) {
                        ValueDate = GetLastDayDate(god, aDopAtrCol->ValuePeriod, 0);
                    }
                } else if (aDopAtrCol->Period == 6) {
                    if (aDopAtrCol->ValuePeriod > 0) {
                        ValueDate = GetLastDayDate(god, 0, aDopAtrCol->ValuePeriod);
                    }
                }
            } else {
                if (PeriodType == 4) {
                    if (ValuePeriod > 0) {
                        ValueDate = GetLastDayDate(god, ValuePeriod, 0);
                    }
                } else if (PeriodType == 6) {
                    if (ValuePeriod > 0) {
                        ValueDate = GetLastDayDate(god, 0, ValuePeriod);
                    }
                }
            }

            cell_period = 0;
            switch (PeriodType) {
            case 1: cell_period = 1; break;
            case 4: cell_period = 2; break;
            case 6: cell_period = 3; break;
            }

            coef_Forma = CoefPlus1 - 1;
            if (coef_Forma < 0) {
                coef_Forma = 0;
            } else {
                coef_Forma = coef_Forma * 3;
            }

            TFormDopInp::AddValueCellAtr(FormLogicF1, FormLogicF1->CurrInputRow, i, 0, ValueDate, 1, VidOtch, PlanId, 0, coef_Forma, 0, PokazId, 1, 0, cell_period, ist_fin_id);
        } else {
            exprF1 = nextItem;
            exprF1 = TFormDopInp::ReplaceNRowNCol(FormLogicF1->CurrInputRow, i, exprF1);
            TForm1::SetCellValue(FormLogicF1, FormLogicF1->f1Forms, p->MaxHrCol, FormLogicF1->CurrInputRow, i, FormLogicF1->CurrInputRow, i, exprF1, 0, FormatStyle);
        }
    }
}

// оператор СУММАшаблонКБК
void TForm2PlusPatternKBK::SummaPatternKBK(TFormLogicF1* FormLogicF1, int aRow, TwwStringList* p, int pokaz_idRoot, const std::string& LikePattern) {
        TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
        
        TDateTime ToDate;
        std::string Stmnt;
        double ValueD;
        std::string Value;

        if (GetDBType() == TDBType::MSSQL) {
            q->SQL->Add("CREATE TABLE #PTB (pokaz_id int, pokaz_is_razd tinyint NULL, pokaz_owner_id int NULL,");  
            q->SQL->Add(" pokaz_vert tinyint NULL, pokaz_type tinyint NULL, pokaz_bk_code varchar(30) NULL, ");
            q->SQL->Add(" rd varchar(10) NULL, cs varchar(10) NULL, vr varchar(10) NULL, ku varchar(10) NULL, is_obrab tinyint NULL)");            
        } else if (GetDBType() == TDBType::Postgre) {
            q->SQL->Add("CREATE TEMP TABLE PTB (pokaz_id int, pokaz_is_razd smallint NULL, pokaz_owner_id int NULL, ");
            q->SQL->Add(" pokaz_vert smallint NULL, pokaz_type smallint NULL, pokaz_bk_code varchar(30) NULL, ");
            q->SQL->Add(" rd varchar(10) NULL, cs varchar(10) NULL, vr varchar(10) NULL, ku varchar(10) NULL, is_obrab smallint NULL)");            
        }
        q->ExecSQL();

        ToDate = EncodeDate(FormLogicF1->CurrYear,12,30);
        TDB_TmpPokaztl::CreateTmpPTB(pokaz_idRoot, ToDate);

        q->SQL->Clear();
        
        if (GetDBType() == TDBType::MSSQL) {
            q->SQL->Add("DELETE FROM #PTB WHERE ku = :ku0");
        } else if (GetDBType() == TDBType::Postgre) {
            q->SQL->Add("DELETE FROM PTB WHERE ku = :ku0");
        }
        q->ParamByName("ku0")->AsString() = "";
        q->ExecSQL();
        // остаются записи, где rd, cs, vr, ku - задан        

        q->SQL->Clear();
        if (GetDBType() == TDBType::MSSQL) {
            q->SQL->Add("DELETE FROM #PTB WHERE pokaz_bk_code NOT LIKE :LikePattern");
        } else if (GetDBType() == TDBType::Postgre) {
            q->SQL->Add("DELETE FROM PTB WHERE pokaz_bk_code NOT LIKE :LikePattern");
        }
        q->ParamByName("LikePattern")->AsString() = LikePattern;
        q->ExecSQL();
        // остаются записи, которые удовлетворяют шаблону КБК        
        int LastCol = FormLogicF1->f1Forms->LastCol_Get();
        for (int j = 1; j <= LastCol; j++) {
            Stmnt = p->ListStmnt[j].Stmnt;
            PCellValueAtr vd = TFormDopInp::GetCellValueAtrByRC(FormLogicF1, aRow, j);
            if (vd == nullptr) continue;

            TFormatStyle* FormatStyle = new TFormatStyle();
            TFormDopInp::getColFrmtrAtr(FormLogicF1, 0, j, *FormatStyle);

            if (Stmnt == "ЗНАЧЕНИЕСВ" || Stmnt == "ЗНАЧЕНИЕСВИР") {
                if (vd->PokazId > 0) {// характеристика объекта
                    q->ExecSQL();
                    q->SQL->Add("SELECT SUM(value) as value FROM ");
                    if (GetDBType() == TDBType::MSSQL) {
                        q->SQL->Add("#PTB a, Cell b, Pokaztl c, Cell_value d");
                    } else if (GetDBType() == TDBType::Postgre) {
                        q->SQL->Add("PTB a, Cell b, Pokaztl c, Cell_value d");
                    }
                    q->SQL->Add("WHERE b.pokaz_id = :PokazId ");
                    q->SQL->Add(" AND b.ist_fin_id = :ist_fin_id ");
                    q->SQL->Add(" AND b.cell_period = :cell_period ");
                    q->SQL->Add(" AND c.pokaz_id = b.pokaz_id ");
                    q->SQL->Add(" AND c.pokaz_type = 1 "); // число
                    q->SQL->Add(" AND d.objectid = a.pokaz_id ");
                    q->SQL->Add(" AND d.cell_id = b.cell_id");                    
                    if (Stmnt != "ЗНАЧЕНИЕСВИР") {
                        q->SQL->Add(" AND d.plan_id = :plan_id");
                    }
                    q->SQL->Add(" AND d.vid_otc = :vid_otc ");
                    q->SQL->Add(" AND d.value_date = :value_date");                    
                    q->ParamByName("PokazId")->AsInteger() = vd->PokazId;
                    q->ParamByName("ist_fin_id")->AsInteger() = vd->ist_fin_id;
                    q->ParamByName("cell_period")->AsInteger() = vd->cell_period;
                    if (Stmnt != "ЗНАЧЕНИЕСВИР") {
                        q->ParamByName("plan_id")->AsInteger() = vd->PlanId;
                    }
                    q->ParamByName("vid_otc")->AsInteger() = vd->VidOtch;
                    q->ParamByName("value_date")->AsDateTime() = vd->ValueDate;
                    q->Open();
                    ValueD = 0;
                    if (!q->EoF()) {
                        ValueD = q->FieldByName("value")->AsFloat();
                    }
                    ValueD = PreobrValue(ValueD, 1, vd->coef_Pokaz, vd->coef_Forma);
                    Value = std::to_string(ValueD);
                    q->Close();
                } else {
                    q->SQL->Add("SELECT SUM(value) as value FROM ");
                    if (GetDBType() == TDBType::MSSQL) {
                        q->SQL->Add("#PTB a, Cell b, Pokaztl c, Cell_value d");
                    } else if (GetDBType() == TDBType::Postgre) {
                        q->SQL->Add("PTB a, Cell b, Pokaztl c, Cell_value d");
                    }
                    q->SQL->Add("WHERE b.pokaz_id = a.pokaz_id ");
                    q->SQL->Add(" AND b.ist_fin_id = :ist_fin_id ");
                    q->SQL->Add(" AND b.cell_period = :cell_period ");
                    q->SQL->Add(" AND c.pokaz_id = b.pokaz_id ");
                    q->SQL->Add(" AND c.pokaz_type = 1 "); // число
                    q->SQL->Add(" AND d.objectid = 0 ");
                    q->SQL->Add(" AND d.cell_id = b.cell_id");                                                                                                                        
                    if (Stmnt != "ЗНАЧЕНИЕСВИР") {
                        q->SQL->Add("AND d.plan_id = :plan_id");
                    }
                    q->SQL->Add("AND d.vid_otc = :vid_otc AND d.value_date = :value_date");
                    q->ParamByName("ist_fin_id")->AsInteger() = vd->ist_fin_id;
                    q->ParamByName("cell_period")->AsInteger() = vd->cell_period;
                    if (Stmnt != "ЗНАЧЕНИЕСВИР") {
                        q->ParamByName("plan_id")->AsInteger() = vd->PlanId;
                    }
                    q->ParamByName("vid_otc")->AsInteger() = vd->VidOtch;
                    q->ParamByName("value_date")->AsDateTime() = vd->ValueDate;
                    q->Open();
                    ValueD = 0;
                    if (!q->EoF()) {
                        ValueD = q->FieldByName("value")->AsFloat();
                    }
                    ValueD = PreobrValue(ValueD, 1, vd->coef_Pokaz, vd->coef_Forma);
                    Value = std::to_string(ValueD);
                    q->Close();
                }
            } else if (Stmnt == "ЗНАЧЕНИЕСВНИ") {
                // TODO: Implement ЗНАЧЕНИЕСВНИ
            }

            TForm1::SetCellValue(FormLogicF1, FormLogicF1->f1Forms, p->MaxHrCol, vd->Row, vd->Col, vd->Row, vd->Col, Value, vd->pokaz_type, FormatStyle);
            FormLogicF1->f1Forms->Selection_Set(vd->Row, vd->Col, vd->Row, vd->Col);
            FormLogicF1->f1Forms->Protection_Set(true, true);
        }

        if (GetDBType() == TDBType::MSSQL) {
            q->SQL->Add("DROP TABLE #PTB");
        } else if (GetDBType() == TDBType::Postgre) {
            q->SQL->Add("DROP TABLE PTB");
        }
        q->ExecSQL();
        delete q;
    }









