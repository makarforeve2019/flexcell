

#include "unDefDBconnect.h"
#include "dmGlobalVars.h"
#include "unDB_RP_067eval.h"

void TDB_RP_067eval::LoadToValList(int god, bool isKeyByPlanId) {
    if (isLoad) return;

 
    TQueryUni *q = new TQueryUni(DefDBconnect.MWSModulsConn);

    q->SQL->Clear();
    q->SQL->Add("SELECT * FROM db_rp_067");
    q->SQL->Add("WHERE id_obmena = :id_obmena");
    q->ParamByName("id_obmena")->AsInteger() = id_obmena;

    q->Open();
    while (!q->EoF()) {
        Tbov67* bov67 = new Tbov67();
        bov67->plan_id = q->FieldByName("plan_id")->AsInteger();
        bov67->pokaz_id = q->FieldByName("pokaz_id")->AsInteger();
        bov67->f1_1 = q->FieldByName("f1_1")->AsFloat();
        bov67->f1_2_1 = q->FieldByName("f1_2_1")->AsFloat();
        bov67->f1_2_2 = q->FieldByName("f1_2_2")->AsFloat();
        bov67->f1_2_3 = q->FieldByName("f1_2_3")->AsFloat();
        bov67->f1_3 = q->FieldByName("f1_3")->AsFloat();
        bov67->f1_4 = q->FieldByName("f1_4")->AsFloat();
        bov67->snig_ek_byMer = q->FieldByName("snig_ek_byMer")->AsFloat();
        bov67->snig_ek_other = q->FieldByName("snig_ek_other")->AsFloat();
        bov67->fact_zarezerv = q->FieldByName("fact_zarezerv")->AsFloat();
        bov67->ostatokLBO = q->FieldByName("ostatokLBO")->AsFloat();
        bov67->Itogorez = q->FieldByName("f1_1")->AsFloat() + q->FieldByName("f1_2_1")->AsFloat() + q->FieldByName("f1_2_2")->AsFloat() +
                          q->FieldByName("f1_2_3")->AsFloat() + q->FieldByName("f1_3")->AsFloat() + q->FieldByName("f1_4")->AsFloat() +
                          q->FieldByName("snig_ek_byMer")->AsFloat() + q->FieldByName("snig_ek_other")->AsFloat();
        bov67->ostatokLBOgrbs = q->FieldByName("ostatokLBOgrbs")->AsFloat();

        std::string plan_idSS = q->FieldByName("plan_id")->AsString();
        std::string pokaz_idSS = q->FieldByName("pokaz_id")->AsString();
        std::string sKey = isKeyByPlanId ? plan_idSS + "_" + pokaz_idSS : plan_idSS + "_" + q->FieldByName("KBK")->AsString();
//        valList.AddObject(sKey, bov67);

        q->Next();
    }

   //MAK q->Free();
delete q;

    isLoad = true;
}


double TDB_RP_067eval::doEval(int plan_id, int pokaz_id, int VidOtc, int god, int Nvalue) {
    return TDB_RP_067eval::doEvalUni(true, plan_id, pokaz_id, "", VidOtc, god, Nvalue);
}

double TDB_RP_067eval::doEvalByKBK(int plan_id, std::string KBK, int god, int Nvalue) {
    return TDB_RP_067eval::doEvalUni(false, plan_id, 0, KBK, 4, god, Nvalue);
}

double TDB_RP_067eval::doEvalUni(bool isKeyByPlanId, int plan_id, int pokaz_id, std::string KBK, int VidOtc, int god, int Nvalue) {
    std::string plan_idSS = std::to_string(plan_id);
    std::string pokaz_idSS = std::to_string(pokaz_id);
    std::string sKey = isKeyByPlanId ? plan_idSS + "_" + pokaz_idSS : plan_idSS + "_" + KBK;

    /*mak if (GlobalVars->vDB_RP_067eval->valList.find(sKey, Index)== true) {
        Bov67* bov67(GlobalVars->vDB_RP_067eval.valList.Objects[Index]);
        switch (Nvalue) {
        case 1: return bov67->f1_1;
        case 2: return bov67->f1_2_1;
        case 3: return bov67->f1_2_2;
        case 4: return bov67->f1_2_3;
        case 5: return bov67->f1_3;
        case 6: return bov67->f1_4;
        case 7: return bov67->snig_ek_byMer;
        case 8: return bov67->snig_ek_other;
        case 9: return bov67->fact_zarezerv;
        case 10: return bov67->ostatokLBO;
        case 11: return bov67->Itogorez;
        case 12: return bov67->ostatokLBOgrbs;
        }
    }*/
    return 0.0;
}

void TDB_RP_067eval::createObj() {
//    GlobalVars->vDB_RP_067eval = new TDB_RP_067eval();
    GlobalVars->vDB_RP_067eval.id_obmena = 0;
//    GlobalVars->vDB_RP_067eval->valList = std::unordered_map<std::string, Bov67*>();
    GlobalVars->vDB_RP_067eval.isLoad = false;
}

void TDB_RP_067eval::freeObj() {
   /*mak for (auto& pair : GlobalVars::vDB_RP_067eval->valList) {
        delete pair.second;
    }
    GlobalVars::vDB_RP_067eval->valList.clear();
    delete GlobalVars::vDB_RP_067eval;
    GlobalVars::vDB_RP_067eval = nullptr;*/
}

//�������������