#include <string>
#include "SystFuncsLogic.h"
#include "unDefDBconnect.h"
#include "DB_NeOdnoznPlategka.h"
#include "DB_BPoluc.h"
#include "DB_BPolucRaj.h"

bool TDB_BPolucRaj::GetBP_IdByLicScet(std::string rasc_scet, int &budget_poluc_id, int &kod_oshibki, int &plan_id)
{
    int kol = 0, bpId = 0, plId = 0;
    budget_poluc_id = 0;
    plan_id = 0;

    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);

    q->SQL->Clear();
    q->SQL->Add("SELECT rasc_scet, Budget_poluc.budget_poluc_id, plan_id");
    q->SQL->Add("FROM Budget_poluc, Budget_poluc_rajon");
    q->SQL->Add("WHERE rasc_scet = :rasc_scet");
    q->ParamByName("rasc_scet")->AsString() = rasc_scet;
    q->Open();

    while (!q->EoF())
    {
        kol++;
        if (budget_poluc_id == 0)
            budget_poluc_id = q->FieldByName("budget_poluc_id")->AsInteger();
        else
            bpId = q->FieldByName("budget_poluc_id")->AsInteger();

        if (plan_id == 0)
            plan_id = q->FieldByName("plan_id")->AsInteger();
        else
            plId = q->FieldByName("plan_id")->AsInteger();

        q->Next();
    }

    delete q;

    switch (kol)
    {
    case 0:
        kod_oshibki = KO_inDbNoRascScet;
        return false;
    case 1:
        kod_oshibki = 0;
        return true;
    default:
        kod_oshibki = KO_RascScetNeUnik;
        if ((budget_poluc_id == bpId) && (plan_id != plId))
            kod_oshibki = KO_inDbBP;
        return false;
    }
}

int TDB_BPolucRaj::getBP1forTBpolnoe(int god, int plan_id, std::string &rasc_scet, std::string &short_name, std::string &INN, std::string &KPP)
{
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    int result = 0;

    q->SQL->Clear();
    q->SQL->Add("SELECT *");
    q->SQL->Add("FROM Budget_poluc_rajon, Budget_poluc");
    q->SQL->Add("WHERE plan_id = :plan_id");
    q->SQL->Add("AND Budget_poluc_rajon.budget_poluc_id = Budget_poluc.budget_poluc_id");
    q->ParamByName("plan_id")->AsInteger() = plan_id;
    q->Open();

    while (!q->EoF())
    {
        if (god < q->FieldByName("god_beg")->AsInteger())
            goto PNext;
        if (god > q->FieldByName("god_end")->AsInteger())
            goto PNext;

        rasc_scet = q->FieldByName("rasc_scet")->AsString();
        short_name = q->FieldByName("name")->AsString();
        INN = q->FieldByName("INN")->AsString();
        KPP = q->FieldByName("KPP")->AsString();
        result = q->FieldByName("budget_poluc_id")->AsInteger();
        break;

    PNext:
        q->Next();
    }

    delete q;
    return result;
}

std::string TDB_BPolucRaj::GetRsByPlan_id(int plan_id)
{
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    std::string result;

    q->SQL->Clear();
    q->SQL->Add("SELECT bp.rasc_scet FROM budget_poluc_rajon as bpr");
    q->SQL->Add("INNER JOIN budget_poluc as bp ON bp.budget_poluc_id = bpr.budget_poluc_id");
    q->SQL->Add("WHERE bpr.plan_id = :plan_id");
    q->ParamByName("plan_id")->AsInteger() = plan_id;
    q->Open();

    if (!q->EoF())
    {
        result = q->FieldByName("rasc_scet")->AsString();
    }
    else
    {
        result = "";
    }

    delete q;
    return result;
}

bool TDB_BPolucRaj::GetBP_IdByINN(std::string INN, int& budget_poluc_id, int& kod_oshibki, int& plan_id)
{
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    int kol = 0, bpId = 0, plId = 0;
    bool Result = false;

    q->SQL->Clear();
    q->SQL->Add("SELECT INN, Budget_poluc.budget_poluc_id, plan_id");
    q->SQL->Add("FROM Budget_poluc, Budget_poluc_rajon");
    q->SQL->Add("WHERE INN = :INN");
    q->SQL->Add("AND Budget_poluc.budget_poluc_id = Budget_poluc_rajon.budget_poluc_id");
    q->ParamByName("INN")->AsString() = INN;
    q->Open();

    while (!q->EoF())
    {
        kol++;
        if (budget_poluc_id == 0)
            budget_poluc_id = q->FieldByName("budget_poluc_id")->AsInteger();
        else
            bpId = q->FieldByName("budget_poluc_id")->AsInteger();

        if (plan_id == 0)
            plan_id = q->FieldByName("plan_id")->AsInteger();
        else
            plId = q->FieldByName("plan_id")->AsInteger();

        q->Next();
    }

    delete q;

    switch (kol)
    {
        case 0:
            Result = false;
            kod_oshibki = KO_inDbNoRascScet;
            break;
        case 1:
            Result = true;
            kod_oshibki = 0;
            break;
        default:
            Result = false;
            kod_oshibki = KO_RascScetNeUnik;
            if (budget_poluc_id == bpId && plan_id != plId)
                kod_oshibki = KO_inDbBP;
            break;
    }

    return Result;
}

bool TDB_BPolucRaj::IsBPByPlanId(int plan_id)
{
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    bool Result = false;

    q->SQL->Clear();
    q->SQL->Add("SELECT budget_poluc_id");
    q->SQL->Add("FROM Budget_poluc_rajon");
    q->SQL->Add("WHERE plan_id = :plan_id");
    q->ParamByName("plan_id")->AsInteger() = plan_id;
    q->Open();

    Result = !q->EoF();

    delete q;

    return Result;
}

std::string TDB_BPolucRaj::IsBPOtherPlanId(int plan_id, int budget_poluc_id)
{
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    std::string Result = "";

    q->SQL->Clear();
    q->SQL->Add("SELECT plan_name");
    q->SQL->Add("FROM Budget_poluc_rajon, Plans");
    q->SQL->Add("WHERE Budget_poluc_rajon.plan_id <> :plan_id");
    q->SQL->Add("AND budget_poluc_id = :budget_poluc_id");
    q->SQL->Add("AND Plans.plan_id = Budget_poluc_rajon.plan_id");
    q->ParamByName("plan_id")->AsInteger() = plan_id;
    q->ParamByName("budget_poluc_id")->AsInteger() = budget_poluc_id;
    q->Open();

    if (!q->EoF())
        Result = q->FieldByName("plan_name")->AsString();

    delete q;

    return Result;
}

int TDB_BPolucRaj::getBP1forTB(int god, int plan_id, std::string& rasc_scet, std::string& short_name, std::string& INN) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    int Result = 0;

    try {
        q->SQL->Clear();
        q->SQL->Add("SELECT *");
        q->SQL->Add("FROM Budget_poluc_rajon, Budget_poluc");
        q->SQL->Add("WHERE plan_id = :plan_id");
        q->SQL->Add("AND Budget_poluc_rajon.budget_poluc_id = Budget_poluc.budget_poluc_id");
        q->ParamByName("plan_id")->AsInteger() = plan_id;
        q->Open();

        while (!q->EoF()) {
            int god_beg = q->FieldByName("god_beg")->AsInteger();
            int god_end = q->FieldByName("god_end")->AsInteger();

            if (god >= god_beg && god <= god_end) {
                rasc_scet = q->FieldByName("rasc_scet")->AsString();
                short_name = q->FieldByName("short_name")->AsString();
                INN = q->FieldByName("INN")->AsString();
                Result = q->FieldByName("budget_poluc_id")->AsInteger();
                break;
            }

            q->Next();
        }
    } catch (std::exception& e) {
        std::cerr << "Exception in getBP1forTB: " << e.what() << std::endl;
    }

    delete q;
    return Result;
}

void TDB_BPolucRaj::GetBPbyPlanIdTBPlc(int god, TQueryUni *qIn, int plan_id)
{
    qIn->SQL->Clear();
    qIn->SQL->Add("SELECT *");
    qIn->SQL->Add("FROM Budget_poluc_rajon, Budget_poluc");
    qIn->SQL->Add("WHERE plan_id = :plan_id");
    qIn->SQL->Add("AND Budget_poluc.budget_poluc_id = Budget_poluc_rajon.budget_poluc_id");
    qIn->SQL->Add("AND :god BETWEEN god_beg AND god_end");
    qIn->SQL->Add("AND short_name LIKE :patern");
    qIn->ParamByName("plan_id")->AsInteger() = plan_id;
    qIn->ParamByName("god")->AsInteger() = god;
    qIn->ParamByName("patern")->AsString() = "%(получат-тб)";
    qIn->Open();
}

std::string TDB_BPolucRaj::GetBPNamebyPlanIdTBPlc(int god, int plan_id)
{
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    TDB_BPolucRaj::GetBPbyPlanIdTBPlc(god, q, plan_id);
    std::string result = DelSkobka(q->FieldByName("short_name")->AsString());
    delete q;
    return result;
}

void TDB_BPolucRaj::GetBPbyPlanIdTBRsp(TQueryUni *qIn, int plan_id)
{
    qIn->SQL->Clear();
    qIn->SQL->Add("SELECT rasc_scet, short_name, Budget_poluc.budget_poluc_id as budget_poluc_id, god_end"); //++++++
    qIn->SQL->Add("FROM Budget_poluc_rajon, Budget_poluc");
    qIn->SQL->Add("WHERE plan_id = :plan_id");
    qIn->SQL->Add("AND Budget_poluc.budget_poluc_id = Budget_poluc_rajon.budget_poluc_id");
    qIn->SQL->Add("AND short_name LIKE :patern");
    qIn->ParamByName("plan_id")->AsInteger() = plan_id;
    qIn->ParamByName("patern")->AsString() = "%(распоряд-тб)";
    qIn->Open();
}

std::string RSPlcbyPlanId;
int byPlanIdPlc = 0;

std::string TDB_BPolucRaj::RSPlc_GetBPbyPlanIdTB(int god, int plan_id)
{
    if (byPlanIdPlc != 0)
    {
        if (byPlanIdPlc == plan_id)
        {
            return RSPlcbyPlanId;
        }
    }

    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);

    TDB_BPolucRaj::GetBPbyPlanIdTBPlc(god, q, plan_id);

    std::string Result = q->FieldByName("rasc_scet")->AsString();

    delete q;
    
    RSPlcbyPlanId = Result;
    byPlanIdPlc = plan_id;

    return Result;
}

std::string RSRspbyPlanId;
int byPlanIdRsp = 0;

std::string TDB_BPolucRaj::RSRsp_GetBPbyPlanIdTB(int plan_id)
{
    if (byPlanIdRsp != 0)
    {
        if (byPlanIdRsp == plan_id)
        {
            return RSRspbyPlanId;
        }
    }

    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);

    TDB_BPolucRaj::GetBPbyPlanIdTBRsp(q, plan_id);

    std::string Result = q->FieldByName("rasc_scet")->AsString();

    delete q;
    
    RSRspbyPlanId = Result;
    byPlanIdRsp = plan_id;

    return Result;
}

int TDB_BPolucRaj::idPlc_GetBPbyPlanIdTB(int god, int plan_id, std::string &short_name)
{
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);

    TDB_BPolucRaj::GetBPbyPlanIdTBPlc(god, q, plan_id);

    short_name = q->FieldByName("short_name")->AsString();
    int Result = q->FieldByName("budget_poluc_id")->AsInteger();

    delete q;

    return Result;
}

int TDB_BPolucRaj::idRsp_GetBPbyPlanIdTB(int plan_id, std::string &short_name, int &god_end)
{
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);

    TDB_BPolucRaj::GetBPbyPlanIdTBRsp(q, plan_id);

    short_name = q->FieldByName("short_name")->AsString();
    god_end = q->FieldByName("god_end")->AsInteger();
    int Result = q->FieldByName("budget_poluc_id")->AsInteger();

    delete q;

    return Result;
}

int TDB_BPolucRaj::idRsp_GetBPbyPlanIdTB(int plan_id, std::string& short_name) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    TDB_BPolucRaj::GetBPbyPlanIdTBRsp(q, plan_id);
    short_name = q->FieldByName("short_name").AsString();
    int result = q->FieldByName("budget_poluc_id").AsInteger();
    return result;
}



int TDB_BPolucRaj::BPPlcTB_GetBPbyPlanId(int god, int plan_id, std::string &short_name)
{
    if (wplan_id == plan_id)
    {
        short_name = wshort_name;
        return wbudget_poluc_id;
    }

    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);

    TDB_BPolucRaj::GetBPbyPlanIdTBPlc(god, q, plan_id);

    short_name = DelSkobka(q->FieldByName("short_name")->AsString());
    int Result = q->FieldByName("budget_poluc_id")->AsInteger();

    wplan_id = plan_id;
    wshort_name = short_name;
    wbudget_poluc_id = Result;

    delete q;

    return Result;
}


