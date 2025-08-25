#include "unDefDBconnect.h"
#include "Form2Plus.h"
#include "DB_Pokaztl.h"
#include "DB_BPolucRaj.h"
#include "DB_bkMetaDef.h"
#include "dmGlobalVars.h"
#include "unDBType.h"
#include "SystFuncsLogic.h"
#include "unCO_RP_015eval.h"
#include "unGetLastDateFor010.cpp"

void TCO_RP_015eval::LoadToValList(int god) {
    static bool isLoad = false;
    if (isLoad) return;

//    int LUnc = 0;
    if (LUnc == 0) {
        //mak ??? LUnc = TDB_MWSModuls::getL_unc();
    }

    TQueryUni *q = new TQueryUni(DefDBconnect.MWSModulsConn);

    double dBO15 = 0.0;
    double dKassRasxBO15 = 0.0;

    q->SQL->Add("SELECT unc, KBK, summaNaGod, summaByBO, oplacenoToBO, ostatokFree, oplacenoAll, ispolnKBKistor.ispKBK_id as ispKBK_id");
    q->SQL->Add("FROM ispolnKBK, ispolnKBKistor");
    q->SQL->Add("WHERE god = :god");
    q->SQL->Add("AND ispolnKBK.ispKBK_id = ispolnKBKistor.ispKBK_id");
    switch (GetDBType()) {
        case TDBType::MSSQL:
            q->SQL->Add("AND CAST(FLOOR(CAST(dateImpFCU AS FLOAT)) AS DATETIME) <= :dateImpFCU");
            break;
        case TDBType::Postgre:
            q->SQL->Add("AND CAST(dateImpFCU AS DATE) <= :dateImpFCU");
            break;
    }
    q->SQL->Add("ORDER BY unc, KBK, dateImpFCU DESC");
    q->ParamByName("god")->AsInteger() = god;
    q->ParamByName("dateImpFCU")->AsDateTime() = lastdate_15fcu;
    q->Open();

    int WispKBK_id = 0;
    TStringList valList;
    while (!q->EoF()) {
        if (WispKBK_id == q->FieldByName("ispKBK_id")->AsInteger()) {
            q->Next();
            continue;
        }
        WispKBK_id = q->FieldByName("ispKBK_id")->AsInteger();

        std::string unc = q->FieldByName("unc")->AsString();
        std::string KBK = q->FieldByName("KBK")->AsString();
        dBO15 = q->FieldByName("summaByBO")->AsFloat();
        dKassRasxBO15 = q->FieldByName("oplacenoToBO")->AsFloat();
        double summaNaGod15 = q->FieldByName("summaNaGod")->AsFloat();
        double ostatokFree15 = q->FieldByName("ostatokFree")->AsFloat();
        double oplacenoAll = q->FieldByName("oplacenoAll")->AsFloat();

        std::string key = unc + "_" + KBK;
        int Index;
        valList.Find(key, Index);
        if (Index) {
            q->Next();
            continue;
        }

        Pbov15 bov15 = new Tbov15;
        bov15->unc = unc;
        bov15->KBK = KBK;
        bov15->dBO15 = dBO15;
        bov15->dKassRasxBO15 = dKassRasxBO15;
        bov15->summaNaGod15 = summaNaGod15;
        bov15->ostatokFree15 = ostatokFree15;
        bov15->oplacenoAll = oplacenoAll;
        if (bov15->dBO15 == 0) {
            bov15->ostatokFree15 = bov15->summaNaGod15 - bov15->oplacenoAll;
        }
        valList.AddObject(key, bov15);

        q->Next();
    }
    //TDB_MWSModuls::CloseLinkMWSModuls();

    isLoad = true;
    delete q;
}


double TCO_RP_015eval::doEval(int plan_id, int pokaz_id, int VidOtc, int god, int NValue) {
    std::string pokaz_bk_code;
    std::string unc;
    std::string ws;
    double dBO15 = 0, dKassRasxBO15 = 0, summaNaGod15 = 0, ostatokFree15 = 0;
    double dBO15all = 0, dKassRasxBO15all = 0, summaNaGod15all = 0, ostatokFree15all = 0;
    int i;
    TStringList valList;

    if (GlobalVars->vCO_RP_015eval.lastdate_15fcu == 0)
        GlobalVars->vCO_RP_015eval.lastdate_15fcu = GetLastDateFor010(god);

    GlobalVars->vCO_RP_015eval.LoadToValList(god);

    switch (NValue) {
    case 1: { // Сумма бюджетных обязательств
        pokaz_bk_code = TDB_Pokaztl::GetCodeBK(pokaz_id);

        // убираем последние 0 из КБК
        while (!pokaz_bk_code.empty() && pokaz_bk_code.back() == '0')
            pokaz_bk_code.pop_back();

        ws = TDB_BPolucRaj::RSPlc_GetBPbyPlanIdTB(god, plan_id);
        unc = ws.substr(ws.length() - LUnc);

        for (i = 0; i < GlobalVars->vCO_RP_015eval.valList.Count(); i++) {
            Pbov15 bov15 = std::get<Pbov15>(GlobalVars->vCO_RP_015eval.valList.Objects[i]);
            if (bov15->KBK.length() < TDB_bkMetaDef::GetDl_All(god))
                continue;
            if (unc == bov15->unc && bov15->KBK.find(pokaz_bk_code) == 0) {
                dBO15 += bov15->dBO15;
                dKassRasxBO15 += bov15->dKassRasxBO15;
            }
        }
        return dBO15;
    }
    case 2: { // Сумма кассового исполнения бюджетных обязательств
        pokaz_bk_code = TDB_Pokaztl::GetCodeBK(pokaz_id);
        ws = TDB_BPolucRaj::RSPlc_GetBPbyPlanIdTB(god, plan_id);
        unc = ws.substr(ws.length() - LUnc);

        for (i = 0; i < GlobalVars->vCO_RP_015eval.valList.Count(); i++) {
            Pbov15 bov15 = std::get<Pbov15>(GlobalVars->vCO_RP_015eval.valList.Objects[i]);
            if (unc == bov15->unc && bov15->KBK.find(pokaz_bk_code) == 0) {
                dBO15 += bov15->dBO15;
                dKassRasxBO15 += bov15->dKassRasxBO15;
            }
        }
        return dKassRasxBO15;
    }
    case 3: { // Сумма бюджетных обязательств для всех субъектов
        pokaz_bk_code = TDB_Pokaztl::GetCodeBK(pokaz_id);

        // убираем последние 0 из КБК
        while (!pokaz_bk_code.empty() && pokaz_bk_code.back() == '0')
            pokaz_bk_code.pop_back();

        for (i = 0; i < GlobalVars->vCO_RP_015eval.valList.Count(); i++) {
            Pbov15 bov15 = std::get<Pbov15>(GlobalVars->vCO_RP_015eval.valList.Objects[i]);
            if (bov15->KBK.length() < TDB_bkMetaDef::GetDl_All(god))
                continue;
            if (bov15->KBK.find(pokaz_bk_code) == 0)
                dBO15all += bov15->dBO15;
        }
        return dBO15all;
    }
    case 4: { // Сумма кассового исполнения бюджетных обязательств для всех субъектов
        pokaz_bk_code = TDB_Pokaztl::GetCodeBK(pokaz_id);
        TForm2Plus::SummaKONTRAKTeval15all(true, true, dBO15all, dKassRasxBO15all, god, 2, pokaz_bk_code, GlobalVars->vCO_RP_015eval.lastdate_15fcu);
        return dKassRasxBO15all;
    }
    case 5: { // Сумма на год
        pokaz_bk_code = TDB_Pokaztl::GetCodeBK(pokaz_id);
        ws = TDB_BPolucRaj::RSPlc_GetBPbyPlanIdTB(god, plan_id);
        unc = ws.substr(ws.length() - LUnc);
        for (i = 0; i < GlobalVars->vCO_RP_015eval.valList.Count(); i++) {
            Pbov15 bov15 = std::get<Pbov15>(GlobalVars->vCO_RP_015eval.valList.Objects[i]);
            if (unc == bov15->unc && bov15->KBK.find(pokaz_bk_code) == 0)
                summaNaGod15 += bov15->summaNaGod15;
        }
        return summaNaGod15;
    }
    case 6: { // Сумма на год для всех субъектов
        pokaz_bk_code = TDB_Pokaztl::GetCodeBK(pokaz_id);
        for (i = 0; i < GlobalVars->vCO_RP_015eval.valList.Count(); i++) {
            Pbov15 bov15 = std::get<Pbov15>(GlobalVars->vCO_RP_015eval.valList.Objects[i]);
            if (bov15->KBK.find(pokaz_bk_code) == 0)
                summaNaGod15all += bov15->summaNaGod15;
        }
        return summaNaGod15all;
    }
    case 7: { // Свободный остаток
        pokaz_bk_code = TDB_Pokaztl::GetCodeBK(pokaz_id);

        // убираем последние 0 из КБК
        while (!pokaz_bk_code.empty() && pokaz_bk_code.back() == '0')
            pokaz_bk_code.pop_back();

        ws = TDB_BPolucRaj::RSPlc_GetBPbyPlanIdTB(god, plan_id);
        unc = ws.substr(ws.length() - LUnc);
        for (i = 0; i < GlobalVars->vCO_RP_015eval.valList.Count(); i++) {
            Pbov15 bov15 = std::get<Pbov15>(GlobalVars->vCO_RP_015eval.valList.Objects[i]);
            if (bov15->KBK.length() < TDB_bkMetaDef::GetDl_All(god))
                continue;
            if (unc == bov15->unc && bov15->KBK.find(pokaz_bk_code) == 0)
                ostatokFree15 += bov15->ostatokFree15;
        }
        return ostatokFree15;
    }
    case 8: { // Свободный остаток для всех субъектов
        pokaz_bk_code = TDB_Pokaztl::GetCodeBK(pokaz_id);

        // убираем последние 0 из КБК
        while (!pokaz_bk_code.empty() && pokaz_bk_code.back() == '0')
            pokaz_bk_code.pop_back();

        for (i = 0; i < GlobalVars->vCO_RP_015eval.valList.Count(); i++) {
            Pbov15 bov15 = std::get<Pbov15>(GlobalVars->vCO_RP_015eval.valList.Objects[i]);
            if (bov15->KBK.length() < TDB_bkMetaDef::GetDl_All(god))
                continue;
            if (bov15->KBK.find(pokaz_bk_code) == 0)
                ostatokFree15all += bov15->ostatokFree15;
        }
        return ostatokFree15all;
    }
    }
    return 0.0;
}

void TCO_RP_015eval::createObj() {
//    GlobalVars->vCO_RP_015eval = TCO_RP_015eval();
    GlobalVars->vCO_RP_015eval.lastdate_15fcu = 0;
//    GlobalVars->vCO_RP_015eval.valList = new std::vector<Bov15*>();
    //GlobalVars->vCO_RP_015eval.valList.Sorted = true;
    GlobalVars->vCO_RP_015eval.isLoad = false;
}

void TCO_RP_015eval::freeObj() {
    /*mak ???for (int i = 0; i < GlobalVars->vCO_RP_015eval.valList.Size(); i++)
        delete (GlobalVars->vCO_RP_015eval.valList)[i];
    delete GlobalVars->vCO_RP_015eval.valList;
    delete GlobalVars->vCO_RP_015eval;
    GlobalVars->vCO_RP_015eval = nullptr;*/
}




























