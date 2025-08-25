#include "DB_BPoluc.h"
#include "unDefDBconnect.h"
#include "DB_Nat_Pokaz_Vid.h"
#include "DB_BPolucRaj.h"
#include "DB_Pokaztl.h"
#include "DB_TmpPokaztl.h"
#include "SystFuncsLogic.h"

int TDB_BPoluc::GetId() {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    
    int budget_poluc_id = 0;
    q->SQL->Clear();
    switch (GetDBType()) {
        case TDBType::MSSQL:
            q->SQL->Add("SELECT @@IDENTITY as budget_poluc_id");
            break;
        case TDBType::Postgre:
            q->SQL->Add("SELECT currval(pg_get_serial_sequence('Budget_poluc', 'budget_poluc_id')) as budget_poluc_id");
            break;
    }

    q->Open();
    if (!q->EoF()) {
        budget_poluc_id = q->FieldByName("budget_poluc_id")->AsInteger();
    }
   //MAK q->Free();
delete q;
    return budget_poluc_id;
}

int TDB_BPoluc::AddBP(std::string short_name, std::string name_, std::string rasc_scet,
                      std::string bank_rekviz, std::string rasc_scet2, int plan_id2, std::string INN,
                      std::string JurAddres, int god_beg, int god_end, std::string CodeInSvReestr) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    
    int budget_poluc_id = 0;
    q->SQL->Clear();
    q->SQL->Add("INSERT INTO Budget_poluc ");
    q->SQL->Add("(short_name,name,rasc_scet,bank_rekviz,rasc_scet2,plan_id2,INN,JurAddres,god_beg,god_end,CodeInSvReestr)");
    q->SQL->Add("values (:ShortName,:Name,:RascScet,:BankRekviz,:rasc_scet2,:plan_id2,:INN,:JurAddres,:god_beg,:god_end,:CodeInSvReestr)");
    q->ParamByName("ShortName")->AsString() = short_name;
    q->ParamByName("Name")->AsString() = name_;
    q->ParamByName("RascScet")->AsString() = rasc_scet;
    q->ParamByName("BankRekviz")->AsString() = bank_rekviz;
    q->ParamByName("rasc_scet2")->AsString() = rasc_scet2;
    q->ParamByName("plan_id2")->AsInteger() = plan_id2;
    q->ParamByName("INN")->AsString() = INN;
    q->ParamByName("JurAddres")->AsString() = JurAddres;
    q->ParamByName("god_beg")->AsInteger() = god_beg;
    q->ParamByName("god_end")->AsInteger() = god_end;
    q->ParamByName("CodeInSvReestr")->AsString() = CodeInSvReestr;
    q->ExecSQL();
    
    budget_poluc_id = GetId();
   //MAK q->Free();
delete q;
    
    return budget_poluc_id;
}

void TDB_BPoluc::UpdBP(int budget_poluc_id, std::string short_name, std::string name_, std::string rasc_scet,
                       std::string bank_rekviz, std::string rasc_scet2, int plan_id2, std::string INN,
                       std::string JurAddres, int god_beg, int god_end, std::string CodeInSvReestr,
                       std::string KPP) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    
    q->SQL->Clear();
    q->SQL->Add("UPDATE Budget_poluc ");
    q->SQL->Add("SET short_name=:ShortName, name=:Name, rasc_scet=:RascScet, bank_rekviz=:BankRekviz, ");
    q->SQL->Add("rasc_scet2=:rasc_scet2,plan_id2=:plan_id2,INN=:INN,JurAddres=:JurAddres,god_beg=:god_beg, ");
    q->SQL->Add("god_end=:god_end,CodeInSvReestr=:CodeInSvReestr,KPP=:KPP ");
    q->SQL->Add("WHERE budget_poluc_id=:budget_poluc_id");
    
    q->ParamByName("budget_poluc_id")->AsInteger() = budget_poluc_id;
    q->ParamByName("ShortName")->AsString() = short_name;
    q->ParamByName("Name")->AsString() = name_;
    q->ParamByName("RascScet")->AsString() = rasc_scet;
    q->ParamByName("BankRekviz")->AsString() = bank_rekviz;
    q->ParamByName("rasc_scet2")->AsString() = rasc_scet2;
    q->ParamByName("plan_id2")->AsInteger() = plan_id2;
    q->ParamByName("INN")->AsString() = INN;
    q->ParamByName("JurAddres")->AsString() = JurAddres;
    q->ParamByName("god_beg")->AsInteger() = god_beg;
    q->ParamByName("god_end")->AsInteger() = god_end;
    q->ParamByName("CodeInSvReestr")->AsString() = CodeInSvReestr;
    q->ParamByName("KPP")->AsString() = KPP;
    
    q->ExecSQL();
   //MAK q->Free();
delete q;
}

void TDB_BPoluc::GetAllBPs(TQueryUni *qIn) {
    qIn->SQL->Clear();
    qIn->SQL->Add("SELECT * FROM Budget_poluc");
    qIn->SQL->Add("ORDER BY name");
    qIn->Open();
}

void TDB_BPoluc::GetAllBPsRaj(TQueryUni *qIn, int plan_id) {
    qIn->SQL->Clear();
    qIn->SQL->Add("SELECT * FROM Budget_poluc_rajon, Budget_poluc");
    qIn->SQL->Add("WHERE plan_id = :plan_id");
    qIn->SQL->Add("AND Budget_poluc.budget_poluc_id = Budget_poluc_rajon.budget_poluc_id");
    qIn->SQL->Add("ORDER BY name");
    qIn->ParamByName("plan_id")->AsInteger() = plan_id;
    qIn->Open();
}

bool TDB_BPoluc::GetBP(TQueryUni *qIn, int budget_poluc_id) {
    bool result = true;
    qIn->SQL->Clear();
    qIn->SQL->Add("SELECT * FROM Budget_poluc");
    qIn->SQL->Add("WHERE budget_poluc_id = :budget_poluc_id");
    qIn->ParamByName("budget_poluc_id")->AsInteger() = budget_poluc_id;
    qIn->Open();
    if (qIn->EoF())
        result = false;
    return result;
}

std::string TDB_BPoluc::GetNameBP(int budget_poluc_id) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    TDB_BPoluc::GetBP(q, budget_poluc_id);
    std::string result = q->FieldByName("name")->AsString();
   //MAK q->Free();
delete q;
    return result;
}

std::string TDB_BPoluc::GetRascScetBP(int budget_poluc_id) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    TDB_BPoluc::GetBP(q, budget_poluc_id);
    std::string result = q->FieldByName("rasc_scet")->AsString();
   //MAK q->Free();
delete q;
    return result;
}

std::string TDB_BPoluc::GetRascScet1(int PlanId) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    std::string result = "";
    
    q->SQL->Clear();
    q->SQL->Add("SELECT * FROM Budget_poluc");
    q->SQL->Add("WHERE plan_id2 = :plan_id2");
    q->ParamByName("plan_id2")->AsInteger() = PlanId;
    q->Open();
    
    if (!q->EoF()) {
        result = q->FieldByName("rasc_scet")->AsString();
    }
    
   //MAK q->Free();
delete q;
    return result;
}

std::string TDB_BPoluc::GetINNBP(int budget_poluc_id) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    TDB_BPoluc::GetBP(q, budget_poluc_id);
    std::string result = q->FieldByName("INN")->AsString();
   //MAK q->Free();
delete q;
    return result;
}

std::string TDB_BPoluc::GetJurAddresBP(int budget_poluc_id) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    TDB_BPoluc::GetBP(q, budget_poluc_id);
    std::string result = q->FieldByName("JurAddres")->AsString();
   //MAK q->Free();
delete q;
    return result;
}

int TDB_BPoluc::GetId_NPVid() {
    int result = id_npVidBP;
    if (id_npVidBP == 0) {
        result = TDB_Nat_Pokaz_Vid::GetNPVId("Бюджетополучатель");
        if (result > 0) {
            id_npVidBP = result;
        }
    }
    return result;
}

int TDB_BPoluc::GetIdBP(int pokaz_id, int np_vid_id, int Year, int plan_id) {
    double d;
    std::string short_name;
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);

    int result = 0;
    q->SQL->Clear();
    q->SQL->Add("SELECT Cell_value.value");
    q->SQL->Add("FROM Pokaztl a, Pokaztl b, Cell, Cell_value");
    q->SQL->Add("WHERE a.pokaz_id = :pokaz_id");
    q->SQL->Add("AND b.pokaz_base_id = a.pokaz_id");
    q->SQL->Add("AND b.np_vid_id  = :np_vid_id");
    q->SQL->Add("AND Cell.pokaz_id = b.pokaz_id");
    q->SQL->Add("AND Cell.cell_period = 0");
    q->SQL->Add("AND Cell_value.objectid = 0");
    q->SQL->Add("AND Cell_value.plan_id = :plan_id");
    switch (GetDBType()) {
        case TDBType::MSSQL:
            q->SQL->Add("AND YEAR(Cell_value.value_date) = :Year");
            break;
        case TDBType::Postgre:
            q->SQL->Add("AND date_part('year', Cell_value.value_date) = :Year");
            break;
    }

    q->SQL->Add("AND Cell_value.vid_otc = 4");
    q->SQL->Add("ORDER BY Cell_value.value_date DESC");
    q->ParamByName("pokaz_id")->AsInteger() = pokaz_id;
    q->ParamByName("np_vid_id")->AsInteger() = np_vid_id;
    q->ParamByName("plan_id")->AsInteger() = plan_id;
    q->ParamByName("Year")->AsInteger() = Year;
    q->Open();
    if (!q->EoF()) {
        d = q->FieldByName("value")->AsFloat();
        result = static_cast<int>(d);
    } else {
        result = TDB_BPolucRaj::idPlc_GetBPbyPlanIdTB(Year, plan_id, short_name);
    }
   //MAK q->Free();
delete q;
    return result;
}

std::string TDB_BPoluc::GetRascScet1Name(int PlanId, std::string& NameBP) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);

    std::string result = "";
    q->SQL->Clear();
    q->SQL->Add("SELECT * FROM Budget_poluc");
    q->SQL->Add("WHERE plan_id2 = :plan_id2");
    q->ParamByName("plan_id2")->AsInteger() = PlanId;
    q->Open();
    if (!q->EoF()) {
        result = q->FieldByName("rasc_scet")->AsString();
        NameBP = q->FieldByName("name")->AsString();
    }
   //MAK q->Free();
delete q;
    return result;
}

std::string TDB_BPoluc::GetRascScet2(int plan_id) {
    std::string NameBP, INN;
    return TDB_BPoluc::GetRascScet2Name(plan_id, NameBP, INN);
}

std::string TDB_BPoluc::GetRascScet2Name(int plan_id, std::string& NameBP, std::string& INN) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    std::string Result;
    q->SQL->Clear();
    q->SQL->Add("SELECT * FROM Budget_poluc");
    q->SQL->Add("WHERE plan_id2 = :plan_id2");
    q->ParamByName("plan_id2")->AsInteger() = plan_id;
    q->Open();
    if (!q->EoF()) {
        Result = q->FieldByName("rasc_scet2")->AsString();
        NameBP = DelSkobka(q->FieldByName("name")->AsString());
        INN = DelSkobka(q->FieldByName("INN")->AsString());
    }
   //MAK q->Free();
delete q;
    return Result;
}

void TDB_BPoluc::GetMrprbyBP(int pokaz_id, TQueryUni* qIn, int budget_poluc_id, int Year, int plan_id) {
    double bp_id = budget_poluc_id;
    int np_vid_id = TDB_BPoluc::GetId_NPVid();
    int root_Pokaz_id;
    std::string root_Pokaz_name;
    int root_Pokaz_is_razd;
    int root_Pokaz_type;
    int root_Pokaz_vert;
    
    TDB_Pokaztl::GetRootPokaztl(pokaz_id, root_Pokaz_id, root_Pokaz_name, root_Pokaz_is_razd, root_Pokaz_type, root_Pokaz_vert);
    TDB_TmpPokaztl::CreateTmpPokaztl(Year, root_Pokaz_id, plan_id);

    qIn->SQL->Clear();
    qIn->SQL->Add("SELECT DISTINCT a.pokaz_id, a.pokaz_name, a.pokaz_code, a.pokaz_bk_code");
    qIn->SQL->Add("FROM #TmpPokaztl t, Pokaztl a, Pokaztl b, Cell, Cell_value");
    qIn->SQL->Add("WHERE a.pokaz_id = t.pokaz_id");
    qIn->SQL->Add("AND b.pokaz_base_id = a.pokaz_id");
    qIn->SQL->Add("AND b.np_vid_id  = :np_vid_id");
    qIn->SQL->Add("AND Cell.pokaz_id = b.pokaz_id");
    qIn->SQL->Add("AND Cell.cell_period = 0");
    qIn->SQL->Add("AND Cell_value.objectid = 0");
    qIn->SQL->Add("AND Cell_value.plan_id = :plan_id");
    switch (GetDBType()) {
        case TDBType::MSSQL:
            qIn->SQL->Add("AND YEAR(Cell_value.value_date) = :Year");
            break;
        case TDBType::Postgre:
            qIn->SQL->Add("AND date_part(''year'', Cell_value.value_date) = :Year");
            break;
    }

    qIn->SQL->Add("AND Cell_value.vid_otc = 4");
    qIn->SQL->Add("AND Cell_value.value = :bp_id");
    qIn->SQL->Add("ORDER BY a.pokaz_code");
    qIn->ParamByName("np_vid_id")->AsInteger() = np_vid_id;
    qIn->ParamByName("plan_id")->AsInteger() = plan_id;
    qIn->ParamByName("Year")->AsInteger() = Year;
    qIn->ParamByName("bp_id")->AsFloat() = bp_id;
    qIn->Open();

    TDB_TmpPokaztl::DropTmpPokaztl();
}

bool TDB_BPoluc::IsRasporjd(std::string rasc_scet) {
    std::string s = rasc_scet.substr(0, 2);
    if (s == "01" || s == "02") {
        return true;
    }
    return false;
}

bool TDB_BPoluc::IsPolucat(std::string rasc_scet) {
    std::string s = rasc_scet.substr(0, 2);
    if (s == "03" || s == "04" || s == "05" || s == "09" || s == "26" || s == "27") {
        return true;
    }
    return false;
}

int TDB_BPoluc::GetBPatr_byLicScet(std::string rasc_scet, std::string& nameBPL, std::string& INN, int& plan_id) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    int result = 0;
    nameBPL = "";
    INN = "";
    plan_id = 0;

    q->SQL->Clear();
    q->SQL->Add("SELECT rasc_scet, Budget_poluc.name as nameBPL, INN, Budget_poluc.budget_poluc_id, plan_id");
    q->SQL->Add("FROM Budget_poluc, Budget_poluc_rajon");
    q->SQL->Add("WHERE rasc_scet = :rasc_scet");
    q->ParamByName("rasc_scet")->AsString() = rasc_scet;
    q->Open();
    if (!q->EoF()) {
        nameBPL = q->FieldByName("nameBPL")->AsString();
        INN = q->FieldByName("INN")->AsString();
        plan_id = q->FieldByName("plan_id")->AsInteger();
        result = q->FieldByName("budget_poluc_id")->AsInteger();
    }
   //MAK q->Free();
delete q;
    return result;
}
