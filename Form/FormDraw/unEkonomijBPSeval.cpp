#include "SystFuncsLogic.h"
#include "unDefDBconnect.h"
#include "DB_Pokaztl.h"
#include "TT_SummaBySubjects.h"
#include "DB_KeyOfObjects.h"
#include "DB_Dgvr.h"
#include "DB_BPolucRaj.h"
#include "dmGlobalVars.h"
#include "unEkonomijBPSeval.h"
#include "unGetLastDateFor010.cpp"

TStringList *TEkonomijBPSeval::kontrList = nullptr;
TStringList *TEkonomijBPSeval::kbk15List = nullptr;
TStringList *TEkonomijBPSeval::dgvrList = nullptr;
TStringList *TEkonomijBPSeval::torgiList = nullptr;

void TEkonomijBPSeval::LoadToValList(int god) {
    if (isLoad) return;

    TQueryUni *q = new TQueryUni(DefDBconnect.MWSModulsConn);

    q->SQL->Clear();
    q->SQL->Add("SELECT * FROM EkonomijBPs");
    q->SQL->Add("WHERE id_obmena = :id_obmena");
    q->ParamByName("id_obmena")->AsInteger() = id_obmena;

    q->Open();
    while (!q->EoF()) {
        PEkonBPS EkonBPS = new TEkonBPS;
        EkonBPS->plan_id = q->FieldByName("plan_id")->AsInteger();
        EkonBPS->pokaz_id = q->FieldByName("pokaz_id")->AsInteger();
        EkonBPS->Ekonomij = q->FieldByName("Ekonomij")->AsFloat();
        EkonBPS->cenaKontrakts = q->FieldByName("cenaKontrakts")->AsFloat();
        std::string plan_idSS = q->FieldByName("plan_id")->AsString();
        std::string pokaz_idSS = q->FieldByName("pokaz_id")->AsString();
        valList->AddObject(plan_idSS + "_" + pokaz_idSS, EkonBPS);
        q->Next();
    }

    delete q;
//        TDB_MWSModuls::CloseLinkMWSModuls();

    isLoad = true;
}

double TEkonomijBPSeval::doEval(int plan_id, int pokaz_id, int VidOtc, int god, int NValue) {
    std::string plan_idSS, pokaz_idSS, KBK;
    TDateTime po_naDatu;
    PEkonBPS EkonBPS;
    int Index;
    double dBO15, dKassRasxBO15;
    double dBO15all, dKassRasxBO15all =0;
    double dZakluc, dZaklucNoBO;
//    TList<PKntrkt> wList;

    switch (NValue) {
        case 1:
        case 2:
            return TEkonomijBPSeval::doEvalEconom(plan_id, pokaz_id, VidOtc, god, NValue);
        case 3:
        case 4:
        case 5:
        case 6:
        case 7:
        case 8:
        case 9:
        case 10: {
            if (GlobalVars->vEkonomijBPSeval.reestrDgvrov_id == 0) {
                GlobalVars->vEkonomijBPSeval.reestrDgvrov_id = TDB_Pokaztl::GetIdbyNameYear(0, CastNameReestrDgvrs,
                                                                                            god);
                GlobalVars->vEkonomijBPSeval.class_dgvr_id = TDB_Dgvr::GetIdClassDgvr();
                TDB_Dgvr::GetIdsFieldsDGVR(GlobalVars->vEkonomijBPSeval.N_BO_id,
                                           GlobalVars->vEkonomijBPSeval.N_BO_cellid,
                                           GlobalVars->vEkonomijBPSeval.N_dgvr_id,
                                           GlobalVars->vEkonomijBPSeval.N_dgvr_cellid,
                                           GlobalVars->vEkonomijBPSeval.PostUslug_id,
                                           GlobalVars->vEkonomijBPSeval.PostUslug_cellid,
                                           GlobalVars->vEkonomijBPSeval.field_Finansir_id,
                                           GlobalVars->vEkonomijBPSeval.field_Finansir_cellidTB_KV,
                                           GlobalVars->vEkonomijBPSeval.Data_zakluc_id,
                                           GlobalVars->vEkonomijBPSeval.Data_zakluc_cellid,
                                           GlobalVars->vEkonomijBPSeval.Srok_deistv_id,
                                           GlobalVars->vEkonomijBPSeval.Srok_deistv_cellid,
                                           GlobalVars->vEkonomijBPSeval.soderg_dgvr_id,
                                           GlobalVars->vEkonomijBPSeval.soderg_dgvr_cellid,
                                           GlobalVars->vEkonomijBPSeval.INN_id,
                                           GlobalVars->vEkonomijBPSeval.INN_cellid);
            }
            KBK = TDB_Pokaztl::GetCodeBK(pokaz_id);
            if (NValue == 4 || NValue == 6 || NValue == 8 || NValue == 10) plan_id = 1;
            plan_idSS = std::to_string(plan_id);
            if (!kontrList->Find(plan_idSS, Index))
                TEkonomijBPSeval::SelectKONTRAKTs(plan_idSS, *kontrList, plan_id,
                                                  GlobalVars->vEkonomijBPSeval.reestrDgvrov_id, god);
            kontrList->Find(plan_idSS, Index);
            TList<PKntrkt> wList = *std::get<std::shared_ptr<TList<PKntrkt>>>(dgvrList->Objects[Index]);
            switch (NValue) {
                case 3:
                case 4:
                    TEkonomijBPSeval::evalKONTRAKTsByDB(wList, god, KBK, dZakluc, dZaklucNoBO);
                    return dZakluc;
                case 5:
                case 6:
                    TEkonomijBPSeval::doKONTRAKTby15(wList, god, KBK, dBO15, dKassRasxBO15);
                    return dBO15;
                case 7:
                case 8:
                    TEkonomijBPSeval::doKONTRAKTby15(wList, god, KBK, dBO15, dKassRasxBO15);
                    return dKassRasxBO15;
                case 9:
                case 10:
                    TEkonomijBPSeval::evalKONTRAKTsByDB(wList, god, KBK, dZakluc, dZaklucNoBO);
                    return dZaklucNoBO;
            }
            break;
        }
        case 11:
        case 12:
        case 13:
        case 14: {
            if (GlobalVars->vEkonomijBPSeval.lastdate_15fcu == 0)
                GlobalVars->vEkonomijBPSeval.lastdate_15fcu = GetLastDateFor010(god);
            KBK = TDB_Pokaztl::GetCodeBK(pokaz_id);
            switch (NValue) {
                case 11:
                    TEkonomijBPSeval::evalBO15_KassRasxBO15(dBO15, dKassRasxBO15, god, KBK, plan_id,
                                                            GlobalVars->vEkonomijBPSeval.lastdate_15fcu);
                    return dBO15;
                case 12:
                    TEkonomijBPSeval::evalBO15_KassRasxBO15all(dBO15all, dKassRasxBO15all, god, KBK,
                                                               GlobalVars->vEkonomijBPSeval.lastdate_15fcu);
                    return dBO15all;
                case 13:
                    TEkonomijBPSeval::evalBO15_KassRasxBO15(dBO15, dKassRasxBO15, god, KBK, plan_id,
                                                            GlobalVars->vEkonomijBPSeval.lastdate_15fcu);
                    return dKassRasxBO15;
                case 14:
                    TEkonomijBPSeval::evalBO15_KassRasxBO15all(dBO15all, dKassRasxBO15all, god, KBK,
                                                               GlobalVars->vEkonomijBPSeval.lastdate_15fcu);
                    return dKassRasxBO15all;
            }
            break;
        }
        case 15:
        case 16: {
            if (GlobalVars->vEkonomijBPSeval.lastdate_15fcu == 0)
                GlobalVars->vEkonomijBPSeval.lastdate_15fcu = GetLastDateFor010(god);
            KBK = TDB_Pokaztl::GetCodeBK(pokaz_id);
            if (NValue == 16) plan_id = 1;
            plan_idSS = std::to_string(plan_id);
            if (!dgvrList->Find(plan_idSS, Index)) {
                std::shared_ptr<TList<PDgvr15>> wList;
                dgvrList->AddObject(plan_idSS, wList);
                TEkonomijBPSeval::evalDgvr15allValues(plan_idSS, plan_id, wList, god,
                                                      GlobalVars->vEkonomijBPSeval.lastdate_15fcu);
            }
            dgvrList->Find(plan_idSS, Index);
            TList<PDgvr15> wList = *std::get<std::shared_ptr<TList<PDgvr15>>>(dgvrList->Objects[Index]);
            return TEkonomijBPSeval::evalDgvr15(wList, KBK);
        }
        case 17:
        case 18:
        case 19:
        case 20: {
            KBK = TDB_Pokaztl::GetCodeBK(pokaz_id);
            std::shared_ptr<TList<PTorgi>> wList;
            switch (NValue) {
                case 17:
                case 18:
                    if (NValue == 18) plan_id = 1;
                    plan_idSS = std::to_string(plan_id);
                    if (!torgiList->Find(plan_idSS, Index)) {

                        torgiList->AddObject(plan_idSS, wList);
                        TEkonomijBPSeval::evalTorgiLoadValues(false, plan_id, wList, god);
                    }
                    torgiList->Find(plan_idSS, Index);

                    wList = std::get<std::shared_ptr<TList<PTorgi>>>(torgiList->Objects[Index]);

                    return TEkonomijBPSeval::evalTorgi(wList, KBK);
                case 19:
                case 20:
                    std::shared_ptr<TList<PTorgi>> wList;
                    if (NValue == 20) plan_id = 1;
                    plan_idSS = std::to_string(plan_id) + "$ALL";
                    if (!torgiList->Find(plan_idSS, Index)) {
                        torgiList->AddObject(plan_idSS, wList);
                        TEkonomijBPSeval::evalTorgiLoadValues(true, plan_id, wList, god);
                    }
                    torgiList->Find(plan_idSS, Index);
                    wList = std::get<std::shared_ptr<TList<PTorgi>>>(torgiList->Objects[Index]);
                    return TEkonomijBPSeval::evalTorgi(wList, KBK);
            }
            break;
        }
    }
    return 0.0;
}

double TEkonomijBPSeval::doEvalEconom(int plan_id, int pokaz_id, int VidOtc, int god, int NValue) {
    std::string plan_idSS = std::to_string(plan_id);
    std::string pokaz_idSS = std::to_string(pokaz_id);
    std::time_t po_naDatu = 0;
    int Index, i;
    double d;

    if (GlobalVars->vEkonomijBPSeval.id_obmena == 0) {
        //mak ??? GlobalVars->vEkonomijBPSeval.id_obmena = TDB_MWSModuls::getLast_EkonomijBPs(god, po_naDatu);
        GlobalVars->vEkonomijBPSeval.po_naDatu = po_naDatu;
    }
    GlobalVars->vEkonomijBPSeval.LoadToValList(god);

    switch (NValue) {
        case 1:
            if (GlobalVars->vEkonomijBPSeval.valList->Find(plan_idSS + '_' + pokaz_idSS, Index)) {
                PEkonBPS EkonBPS = std::get<PEkonBPS>(GlobalVars->vEkonomijBPSeval.valList->Objects[Index]);
                return EkonBPS->Ekonomij;
            } else {
                return 0;
            }
        case 2:
            d = 0;
            for (i = 0; i < GlobalVars->vEkonomijBPSeval.valList->Count(); i++) {
                PEkonBPS EkonBPS = std::get<PEkonBPS>(GlobalVars->vEkonomijBPSeval.valList->Objects[i]);
                if (EkonBPS->pokaz_id == pokaz_id)
                    d += EkonBPS->Ekonomij;
            }
            return d;
        default:
            return 0;
    }
}

void TEkonomijBPSeval::SelectKONTRAKTs(std::string plan_idSS, TStringList &kontrList, int plan_id, int pokaz_owner_id,
                                       int god) {
    std::shared_ptr<TList<PKntrkt>> wList;

    kontrList.AddObject(plan_idSS, wList);
    auto Kntrkt = new TKntrkt();
    Kntrkt->pokaz_id = 0;
    Kntrkt->plan_id = 0;
    Kntrkt->KBK = "";
    Kntrkt->nBO = "";
    Kntrkt->dZakluc = 0;
    Kntrkt->dBO = 0;
    Kntrkt->dKassRasxBO = 0;
    //mak ??? wList.Add(Kntrkt);
    TEkonomijBPSeval::SelectKONTRAKTsDalee(plan_idSS, wList, plan_id, pokaz_owner_id, god);
}

void TEkonomijBPSeval::SelectKONTRAKTsDalee(std::string plan_idSS, std::shared_ptr<TList<PKntrkt>> wList, int plan_id,
                                            int pokaz_owner_id, int god) {
    TQueryUni *wq = new TQueryUni(DefDBconnect.Conn);

    wq->SQL->Add("SELECT a.pokaz_id, pokaz_is_razd, pokaz_bk_code, b.plan_id");
    wq->SQL->Add("FROM Pokaztl a, Plan_str b");
    wq->SQL->Add("WHERE a.pokaz_owner_id = :pokaz_owner_id");

    switch (GetDBType()) {
        case TDBType::MSSQL:
            wq->SQL->Add("AND :god BETWEEN YEAR(pokaz_beg_date) AND YEAR(pokaz_end_date)");
            break;
        case TDBType::Postgre:
            wq->SQL->Add("AND :god BETWEEN date_part('year', pokaz_beg_date) AND date_part('year', pokaz_end_date)");
            break;
    }

    wq->SQL->Add("AND b.pokaz_id = a.pokaz_id");

    std::string strPlans;
    if (plan_id == 1) {
        strPlans = TSummaBySubjects::getPlanIds(god);
    } else {
        strPlans = std::to_string(plan_id);
    }
    if (strPlans.empty()) {
        strPlans = "1";
    } else {
        strPlans += ",1";
    }
    strPlans = "AND plan_id IN (" + strPlans + ")";
    wq->SQL->Add(strPlans);

    wq->ParamByName("pokaz_owner_id")->AsInteger() = pokaz_owner_id;
    wq->ParamByName("god")->AsInteger() = god;

    wq->Open();

    while (!wq->EoF()) {
        int wpokaz_id = wq->FieldByName("pokaz_id")->AsInteger();
        int pokaz_is_razd = wq->FieldByName("pokaz_is_razd")->AsInteger();
        switch (pokaz_is_razd) {
            case 1:
                TEkonomijBPSeval::SelectKONTRAKTsDalee(plan_idSS, wList, plan_id, wpokaz_id, god);
                break;
            case 4:
                PKntrkt Kntrkt = new TKntrkt();
                Kntrkt->pokaz_id = wpokaz_id;
                Kntrkt->plan_id = wq->FieldByName("plan_id")->AsInteger();
                Kntrkt->KBK = wq->FieldByName("pokaz_bk_code")->AsString();
                Kntrkt->nBO = TDB_KeyOfObjects::GetKeyByIdObject(Kntrkt->pokaz_id);
                Kntrkt->dZakluc = 0;
                Kntrkt->dBO = 0;
                Kntrkt->dKassRasxBO = 0;
                wList->Add((Kntrkt));
                break;
        }
        wq->Next();
    }
}

void doItemList(TQueryUni *q, PKntrkt Kntrkt, int god) {
    q->SQL->Clear();
    q->SQL->Add("SELECT *");
    q->SQL->Add("FROM Cell_value");
    q->SQL->Add("WHERE objectid = :objectid");
    q->SQL->Add("AND plan_id = :plan_id");
    q->SQL->Add("AND vid_otc = 4");
    switch (GetDBType()) {
        case TDBType::MSSQL:
            q->SQL->Add("AND MONTH(value_date)=3");
            q->SQL->Add("AND YEAR(value_date)=:god");
            break;
        case TDBType::Postgre:
            q->SQL->Add("AND date_part('month', value_date)=3");
            q->SQL->Add("AND date_part('year', value_date)=:god");
            break;
    }
    q->SQL->Add("AND cell_id=:cell_id");
    q->ParamByName("objectid")->AsInteger() = Kntrkt->pokaz_id;
    q->ParamByName("plan_id")->AsInteger() = Kntrkt->plan_id;
    q->ParamByName("god")->AsInteger() = god;
    q->ParamByName("cell_id")->AsInteger() = GlobalVars->vEkonomijBPSeval.field_Finansir_cellidTB_KV;
    q->Open();
    while (!q->EoF()) {
        Kntrkt->dZakluc = q->FieldByName("value")->AsFloat();
        q->Next();
    }
}

void TEkonomijBPSeval::evalKONTRAKTsByDB(TList<PKntrkt> wList, int god, std::string KBK, double &dZakluc,
                                         double &dZaklucNoBO) {
    //mak ??? PKntrkt Kntrkt = wList[0];
    auto Kntrkt = new TKntrkt();
    if (Kntrkt->dZakluc == 0) {
        std::tm value_date = {0, 0, 0, 2, 0, god - 1900}; // this is the date for conditionally constant values
        TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
        for (int i = 1; i < wList.Count(); i++) {
            //mak ??? Kntrkt = wList[i];
            doItemList(q, Kntrkt, god);
        }
        //mak ??? Kntrkt = wList[0];
        Kntrkt->dZakluc = 1; // so that it is not read again
    }
    dZakluc = 0;
    dZaklucNoBO = 0;
    for (int i = 1; i < wList.Count(); i++) {
        //mak ??? Kntrkt = wList[i];
        if (Kntrkt->KBK.find(KBK) == 0) {
            dZakluc += Kntrkt->dZakluc;
            if (Kntrkt->nBO == "") {
                dZaklucNoBO += Kntrkt->dZakluc;
            }
        }
    }
}

void
TEkonomijBPSeval::doKONTRAKTby15(TList<PKntrkt> wList, int god, std::string KBK, double &dBO, double &dKassRasxBO) {
//        std::string ws, unc;
//        int i, wplan_id;
//        PKntrkt Kntrkt;
//        double wdBO, wdKassRasxBO;
//
//        void doItemList2(int ispbo_id) {
//            double wd = 0;
//            q1->SQL->Clear();
//            q1->connection = DefDBconnect->MWSModulsConn;
//            q1->SQL->Add("SELECT summaOpl");
//            q1->SQL->Add("FROM ispolnBOistor");
//            q1->SQL->Add("WHERE ispbo_id = :ispbo_id");
//            q1->Params->ParamByName("ispbo_id")->Value = ispbo_id;
//            q1->Open();
//            while (!q1->EoF()) {
//                if (q1->FieldByName("summaOpl")->AsFloat > wd)
//                    wd = q1->FieldByName("summaOpl")->AsFloat();
//                q1->Next();
//            }
//            wdKassRasxBO += wd;
//        }
//
//        void doItemList() {
//            wdBO = 0;
//            wdKassRasxBO = 0;
//            q->SQL->Clear();
//            q->connection = DefDBconnect.MWSModulsConn;
//            q->SQL->Add("SELECT DISTINCT summa_BO, ispbo_id");
//            q->SQL->Add("FROM ispolnBO B");
//            q->SQL->Add("WHERE god = :god");
//            q->SQL->Add("AND unc = :unc");
//            q->SQL->Add("AND nBO = :nBO");
//            q->SQL->Add("AND KBK = :KBK");
//            q->Params->ParamByName("god")->Value = god;
//            q->Params->ParamByName("unc")->Value = unc;
//            q->Params->ParamByName("nBO")->Value = Kntrkt->nBO;
//            q->Params->ParamByName("KBK")->Value = Kntrkt->KBK;
//            q->Open();
//            while (!q->EoF()) {
//                wdBO += q->FieldByName("summa_BO")->AsFloat();
//                doItemList2(q->FieldByName("ispbo_id")->AsInteger);
//                q->Next();
//            }
//        }
//
//        Kntrkt = wList.front();
//        if (Kntrkt->dBO == 0) {
//            if (LUnc == 0)
//                TDB_MWSModuls::getL_unc();
//            if (!TDB_MWSModuls::LinkMWSModuls(true))
//                return;
//            q = TADOQueryFactory::Create(nullptr);
//            q->connection = DefDBconnect.MWSModulsConn;
//            q1 = TADOQueryFactory::Create(nullptr);
//            q1->connection = DefDBconnect.MWSModulsConn;
//            wplan_id = 0;
//            for (i = 1; i < wList.size(); i++) {
//                Kntrkt = wList[i];
//                if (wplan_id != Kntrkt->plan_id) {
//                    ws = TDB_BPolucRaj::RSPlc_GetBPbyPlanIdTB(god, Kntrkt->plan_id);
//                    unc = ws.substr(ws.length() - LUnc);
//                }
//                wplan_id = Kntrkt->plan_id;
//                doItemList();
//                Kntrkt->dBO = wdBO;
//                Kntrkt->dKassRasxBO = wdKassRasxBO;
//            }
//            delete q;
//            delete q1;
//            TDB_MWSModuls::CloseLinkMWSModuls();
//            Kntrkt = wList.front();
//            Kntrkt->dBO = 1;
//        }
//        dBO = 0;
//        dKassRasxBO = 0;
//        for (i = 1; i < wList.size(); i++) {
//            Kntrkt = wList[i];
//            if (Kntrkt->KBK.find(KBK) == 0) {
//                dBO += Kntrkt->dBO;
//                dKassRasxBO += Kntrkt->dKassRasxBO;
//            }
//        }
}

void
TEkonomijBPSeval::evalBO15_KassRasxBO15(double &dBO15, double &dKassRasxBO15, int god, std::string KBK, int plan_id,
                                        TDateTime lastdate_15fcu) {
    std::string ws, unc, KBKpred, uncPred, sKey;
    int Index;
    PKntrkt15 Kntrkt15;
    TDateTime dateImpFCUend, dateImpFCU;
    double summa_BO;

//mak ???    if (LUnc == 0) {
//        TDB_MWSModuls::getL_unc();
//    }
    ws = TDB_BPolucRaj::RSPlc_GetBPbyPlanIdTB(god, plan_id);
//mak ???    unc = ws.substr(ws.length() - LUnc);

    sKey = KBK + "$" + unc + "$";
    if (GlobalVars->vEkonomijBPSeval.kbk15List->Find(sKey, Index)) {
//        Kntrkt15 = GlobalVars->vEkonomijBPSeval.kbk15List->Objects[Index];
        auto &variant = GlobalVars->vEkonomijBPSeval.kbk15List->Objects.Get(Index);
        auto kntrktListPtr = std::get_if<std::shared_ptr<TList<PKntrkt>>>(&variant);
        auto &kntrktList = *kntrktListPtr;
        dBO15 = Kntrkt15->dBO15;
        dKassRasxBO15 = Kntrkt15->dKassRasxBO15;
        return;
    }

//mak ???    if (!TDB_MWSModuls::LinkMWSModuls(true)) {
//        return;
//    }
    TQueryUni *q = new TQueryUni(DefDBconnect.MWSModulsConn);

    dBO15 = 0;
    dKassRasxBO15 = 0;
    KBKpred = "";
    uncPred = "";

    q->SQL->Add("SELECT unc, KBK, summaNaGod, summaByBO, oplacenoToBO, ostatokFree");
    q->SQL->Add("FROM ispolnKBK, ispolnKBKistor");
    q->SQL->Add("WHERE god = :god");
    q->SQL->Add("AND unc = :unc");
    q->SQL->Add("AND ispolnKBK.KBK LIKE :KBK");
    q->SQL->Add("AND ispolnKBK.ispKBK_id = ispolnKBKistor.ispKBK_id");
    q->SQL->Add("AND ((dateImpFCUend IS NULL and  CAST(FLOOR(CAST(dateImpFCU AS FLOAT)) AS DATETIME) = :dateImpFCU)");
    q->SQL->Add(
            " OR (dateImpFCUend IS NOT NULL and :dateImpFCU BETWEEN CAST(FLOOR(CAST(dateImpFCU AS FLOAT)) AS DATETIME) AND CAST(FLOOR(CAST(dateImpFCUend AS FLOAT)) AS DATETIME)))");
    q->SQL->Add("ORDER BY unc, KBK, KBKistor_id DESC");
    q->ParamByName("god")->AsInteger() = god;
    q->ParamByName("unc")->AsString() = unc;
    q->ParamByName("dateImpFCU")->AsDateTime() = lastdate_15fcu;
    q->ParamByName("KBK")->AsString() = KBK + "%";
    q->Open();
    while (!q->EoF()) {
        if ((uncPred == q->FieldByName("unc")->AsString()) && (KBKpred == q->FieldByName("KBK")->AsString())) {
            goto PNext;
        }
        KBKpred = q->FieldByName("KBK")->AsString();
        uncPred = q->FieldByName("unc")->AsString();

        dBO15 += q->FieldByName("summaByBO")->AsFloat();
        dKassRasxBO15 += q->FieldByName("oplacenoToBO")->AsFloat();
        PNext:
        q->Next();
    }

    /*mak ??? Kntrkt15 = new Kntrkt15; 
    Kntrkt15->dBO15 = dBO15;
    Kntrkt15->dKassRasxBO15 = dKassRasxBO15;
    GlobalVars->vEkonomijBPSeval.kbk15List.AddObject(sKey, Kntrkt15);

   //MAK q->Free();
delete q;
    TDB_MWSModuls::CloseLinkMWSModuls(); */
}

void TEkonomijBPSeval::evalBO15_KassRasxBO15all(double &dBO15all, double &dKassRasxBO15all, int god,
                                                std::string pokaz_bk_code, TDateTime lastdate_15fcu) {
    TQueryUni *q = new TQueryUni(DefDBconnect.MWSModulsConn);
    TDateTime dateImpFCUend;
    TDateTime dateImpFCU;
    double summa_BO = 0.0;
    std::string sKey = pokaz_bk_code + "$" + "_" + "$";
    int Index;
    PKntrkt15 Kntrkt15 = nullptr;
    std::string KBKpred, uncPred;

    if (GlobalVars->vEkonomijBPSeval.kbk15List->Find(sKey, Index)) {
//        Kntrkt15 = std::get<PKntrkt15>(GlobalVars->vEkonomijBPSeval.kbk15List->Objects[Index]);
        auto &variant = GlobalVars->vEkonomijBPSeval.kbk15List->Objects.Get(Index);
        auto kntrktListPtr = std::get_if<std::shared_ptr<TList<PKntrkt>>>(&variant);
        auto &kntrktList = *kntrktListPtr;
        dBO15all = Kntrkt15->dBO15;
        dKassRasxBO15all = Kntrkt15->dKassRasxBO15;
        return;
    }


    dBO15all = 0.0;
    dKassRasxBO15all = 0.0;
    KBKpred = "";
    uncPred = "";

    q->SQL->Add("SELECT unc, KBK, summaNaGod, summaByBO, oplacenoToBO, ostatokFree");
    q->SQL->Add("FROM ispolnKBK, ispolnKBKistor");
    q->SQL->Add("WHERE god = :god");
    q->SQL->Add("AND ispolnKBK.KBK LIKE :KBK");
    q->SQL->Add("AND ispolnKBK.ispKBK_id = ispolnKBKistor.ispKBK_id");
    q->SQL->Add("AND ((dateImpFCUend IS NULL and  CAST(FLOOR(CAST(dateImpFCU AS FLOAT)) AS DATETIME) = :dateImpFCU)");
    q->SQL->Add(
            " OR (dateImpFCUend IS NOT NULL and :dateImpFCU BETWEEN CAST(FLOOR(CAST(dateImpFCU AS FLOAT)) AS DATETIME) AND CAST(FLOOR(CAST(dateImpFCUend AS FLOAT)) AS DATETIME)))");
    q->SQL->Add("ORDER BY unc, KBK, KBKistor_id DESC");
    q->ParamByName("god")->AsInteger() = god;
    q->ParamByName("dateImpFCU")->AsDateTime() = lastdate_15fcu;
    q->ParamByName("KBK")->AsString() = pokaz_bk_code + "%";
    q->Open();
    while (!q->EoF()) {
        if ((uncPred == q->FieldByName("unc")->AsString()) && (KBKpred == q->FieldByName("KBK")->AsString()))
            goto PNext;
        KBKpred = q->FieldByName("KBK")->AsString();
        uncPred = q->FieldByName("unc")->AsString();

        dBO15all += q->FieldByName("summaByBO")->AsFloat();
        dKassRasxBO15all += q->FieldByName("oplacenoToBO")->AsFloat();
        PNext:
        q->Next();
    }

    /*mak ??? Kntrkt15 = new PKntrkt15;
    Kntrkt15->dBO15 = dBO15all;
    Kntrkt15->dKassRasxBO15 = dKassRasxBO15all;
    GlobalVars->vEkonomijBPSeval.kbk15List.AddObject(sKey, Kntrkt15);

    delete q;
    TDB_MWSModuls.CloseLinkMWSModuls();*/
}

void TEkonomijBPSeval::evalDgvr15allValues(std::string plan_idSS, int plan_id, std::shared_ptr<TList<PDgvr15>> wList,
                                           int god, TDateTime lastdate_15fcu) {
//    if (plan_id > 1) {
//        if (LUnc == 0)
//            TDB_MWSModuls::getL_unc();
//        std::string ws = TDB_BPolucRaj::RSPlc_GetBPbyPlanIdTB(god, plan_id);
//        unc = ws.substr(ws.length() - LUnc);
//    }
//
//    if (!TDB_MWSModuls::LinkMWSModuls(true))
//        return;
//
//    ADOQueryUNi* q = ADOQueryFactory::Create(nullptr);
//    q->connection = DefDBconnect.MWSModulsConn;
//
//    q->SQL->Add("SELECT unc, nBO, KBK, dogovor_N, dateRegistr, summa_BO, summaOpl, summaOst, ispolnBO.ispbo_id");
//    q->SQL->Add("FROM ispolnBO, ispolnBOistor");
//    q->SQL->Add("WHERE god = :god");
//    q->SQL->Add("AND ispolnBO.ispbo_id = ispolnBOistor.ispbo_id");
//    q->SQL->Add("AND ispolnBO.dogovor_N = :Pusto");
//    if (plan_id > 1)
//        q->SQL->Add("AND unc = :unc");
//    q->SQL->Add("AND ((dateImpFCUend IS NULL and  CAST(FLOOR(CAST(dateImpFCU AS FLOAT)) AS DATETIME) = :dateImpFCU)");
//    q->SQL->Add(" OR (dateImpFCUend IS NOT NULL and :dateImpFCU BETWEEN CAST(FLOOR(CAST(dateImpFCU AS FLOAT)) AS DATETIME) AND CAST(FLOOR(CAST(dateImpFCUend AS FLOAT)) AS DATETIME)))");
//    q->SQL->Add("ORDER BY KBK");
//    q->ParamByName("god")->AsInteger() = god;
//    q->ParamByName("dateImpFCU")->AsDateTime = lastdate_15fcu;
//    q->ParamByName("Pusto")->AsString = "";
//    if (plan_id > 1)
//        q->ParamByName("unc")->AsString = unc;
//    q->Open();
//
//    std::string wKBK = "";
//
//    while (!q->EoF()) {
//        if (wKBK != q->FieldByName("KBK")->AsString()) {
//            Dgvr15* Dgvr15 = new Dgvr15;
//            wList.push_back(Dgvr15);
//            Dgvr15->KBK = q->FieldByName("KBK")->AsString();
//            Dgvr15->dDgvr15 = 0;
//            wKBK = q->FieldByName("KBK")->AsString();
//        }
//        if (wKBK == q->FieldByName("KBK")->AsString())
//            Dgvr15->dDgvr15 += q->FieldByName("summaOpl")->AsFloat();
//        q->Next();
//    }
//
//   //MAK q->Free();
//    delete q;
//    TDB_MWSModuls::CloseLinkMWSModuls();
}

double TEkonomijBPSeval::evalDgvr15(TList<PDgvr15> &wList, const std::string KBK) {
    double d = 0;
    for (int i = 0; i < wList.Count(); ++i) {
        PDgvr15 Dgvr15 = dynamic_cast<PDgvr15>(wList[i]);
        if (Dgvr15 && KBK.find(Dgvr15->KBK) == 0) {
            d += Dgvr15->dDgvr15;
        }
    }
    return d;
}


void
TEkonomijBPSeval::evalTorgiLoadValues(bool isAllTorgi, int plan_id, std::shared_ptr<TList<TTorgi *>> wList, int god) {
//    if (!TDB_MWSModuls::LinkMWSModuls(true)) return;
//
//    TADOQueryFactory* qMws = TADOQueryFactory::Create(nullptr);
//    qMws->ConnectionString = DefDBconnect::MWSModulsConn;
//
//    qMws->SQL->Clear();
//    qMws->SQL->Add("SELECT *");
//    qMws->SQL->Add("FROM OOCtorgi T, OOCloti L");
//    qMws->SQL->Add("WHERE T.god = :god");
//    qMws->SQL->Add("AND T.rec_id = L.rec_idTorgi");
//    if (plan_id > 1)
//        qMws->SQL->Add("AND T.plan_id = :plan_id");
//    if (isAllTorgi)
//        qMws->SQL->Add("AND ((T.ResultatTorgov <> -1) OR (T.ResultatTorgov IS NULL))");
//    else
//        qMws->SQL->Add("AND ((T.ResultatTorgov = 0) OR (T.ResultatTorgov IS NULL))");
//
//    qMws->Parameters->Parameterize("god", god);
//    if (plan_id > 1)
//        qMws->Parameters->Parameterize("plan_id", plan_id);
//
//    qMws->Open();
//    while (!qMws->EoF()) {
//        Torgi* torgi = new Torgi;
//        torgi->KBK = qMws->FieldByName("KBKmaxceni")->Value.GetString();
//        torgi->SummaLota = qMws->FieldByName("SummaLota")->Value.GetDouble();
//        torgi->plan_id = qMws->FieldByName("plan_id")->Value.GetInteger();
//        wList.push_back(torgi);
//        qMws->MoveNext();
//    }
//
//    qMws->Close();
//    delete qMws;
//    TDB_MWSModuls::CloseLinkMWSModuls();
}

double TEkonomijBPSeval::evalTorgi(std::shared_ptr<TList<PTorgi>> wList, std::string KBK) {
//    double d = 0.0;
//    for (const auto& Torgi : wList) {
//        if (!Torgi->KBK.empty() && Torgi->KBK.find(KBK) == 0) {
//            d += Torgi->SummaLota;
//        }
//    }
//    return d;
    return 0;
}

TEkonomijBPSeval *TEkonomijBPSeval::createObj() {
//    GlobalVars->vEkonomijBPSeval = new TEkonomijBPSeval();
//    GlobalVars->vEkonomijBPSeval.id_obmena = 0;
//    GlobalVars->vEkonomijBPSeval.reestrDgvrov_id = 0;
//
//    GlobalVars->vEkonomijBPSeval.valList = new std::vector<PEkonBPS>;
//    std::sort(GlobalVars->vEkonomijBPSeval->valList->begin(), GlobalVars->vEkonomijBPSeval->valList->end());
//    GlobalVars->vEkonomijBPSeval.isLoad = false;
//
//    GlobalVars->vEkonomijBPSeval.kontrList = new std::vector<std::vector<PKntrkt>>;
//    std::sort(GlobalVars->vEkonomijBPSeval->kontrList->begin(), GlobalVars->vEkonomijBPSeval->kontrList->end());
//    GlobalVars->vEkonomijBPSeval->kbk15List = new std::vector<PKntrkt15>;
//    std::sort(GlobalVars->vEkonomijBPSeval->kbk15List->begin(), GlobalVars->vEkonomijBPSeval->kbk15List->end());
//
//    GlobalVars->vEkonomijBPSeval->dgvrList = new std::vector<std::vector<PDgvr15>>;
//    std::sort(GlobalVars->vEkonomijBPSeval->dgvrList->begin(), GlobalVars->vEkonomijBPSeval->dgvrList->end());
//
//    GlobalVars->vEkonomijBPSeval->torgiList = new std::vector<std::vector<PTorgi>>;
//    std::sort(GlobalVars->vEkonomijBPSeval->torgiList->begin(), GlobalVars->vEkonomijBPSeval->torgiList->end());
    return nullptr;
}

void TEkonomijBPSeval::freeObj(TEkonomijBPSeval *obj) {
//    for (const auto& EkonBPS : *GlobalVars->vEkonomijBPSeval->valList) {
//        delete EkonBPS;
//    }
//    delete GlobalVars->vEkonomijBPSeval->valList;
//
//    for (const auto& wList : *GlobalVars->vEkonomijBPSeval->kontrList) {
//        for (const auto& Kntrkt : wList) {
//            delete Kntrkt;
//        }
//    }
//    delete GlobalVars->vEkonomijBPSeval->kontrList;
//
//    for (const auto& Kntrkt15 : *GlobalVars->vEkonomijBPSeval->kbk15List) {
//        delete Kntrkt15;
//    }
//    delete GlobalVars->vEkonomijBPSeval->kbk15List;
//
//    for (const auto& wList : *GlobalVars->vEkonomijBPSeval->dgvrList) {
//        for (const auto& Dgvr15 : wList) {
//            delete Dgvr15;
//        }
//    }
//    delete GlobalVars->vEkonomijBPSeval->dgvrList;
//
//    for (const auto& wList : *GlobalVars->vEkonomijBPSeval->torgiList) {
//        for (const auto& Torgi : wList) {
//            delete Torgi;
//        }
//    }
//    delete GlobalVars->vEkonomijBPSeval->torgiList;
//
//    delete GlobalVars->vEkonomijBPSeval;
//    GlobalVars->vEkonomijBPSeval = nullptr;
}













