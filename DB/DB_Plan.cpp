#include "unDefDBconnect.h"
#include "DB_IstFin.h"
#include "DB_Plan.h"
#include "SystFuncsLogic.h"

TStringList slPlans;

// // чтение всех субъектов мониторинга, доступных текущему пользователю function
void TDB_Plan::GetAllPlan_forUser(TQueryUni *qIn, int user_id) {
    qIn->SQL->Clear();
    qIn->SQL->Add("SELECT User_plan.user_id, User_plan.plan_id, User_plan.user_perm, ");
    qIn->SQL->Add("a.plan_id, a.plan_name, a.plan_owner_id, plan_short_name, plan_is_rajon, god_beg, god_end ");
    qIn->SQL->Add("FROM User_plan, Plans a ");
    qIn->SQL->Add("WHERE User_plan.user_id = :idUser ");
    qIn->SQL->Add("AND User_plan.plan_id = a.plan_id ");
    qIn->SQL->Add("AND a.plan_id > 1 ");
    qIn->SQL->Add("ORDER BY a.plan_order");
    qIn->ParamByName("idUser")->AsInteger() = user_id;
    qIn->Open();
}

//void TDB_Plan::GetAllPlan_forUser(TQueryUni* qIn, int user_id) {
//    qIn->SQL->Clear();
//    qIn->SQL->Add("SELECT User_plan.user_id, User_plan.plan_id, User_plan.user_perm,");
//    qIn->SQL->Add("a.plan_id, a.plan_name, a.plan_owner_id, plan_short_name, plan_is_rajon, god_beg, god_end");
//    qIn->SQL->Add("FROM User_plan, Plans a");
//    qIn->SQL->Add("WHERE User_plan.user_id = :idUser");
//    qIn->SQL->Add("AND User_plan.plan_id = a.plan_id");
//    qIn->SQL->Add("AND a.plan_id > 1");
//    qIn->SQL->Add("ORDER BY a.plan_order");
//    qIn->ParamByName("idUser")->AsInteger() = user_id;
//    qIn->Open();
//}

// добавить субъекта мониторинга
int TDB_Plan::AddPlan(int plan_id, const std::string plan_name, const std::string plan_short_name, int plan_owner_id,
                      int plan_order, int plan_is_rajon, const std::string PPP, const std::string FO, const std::string OKATO_kod,
                      int god_beg, int god_end, int isUKP_byKBK, const std::string email) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    q->SQL->Clear();
    q->SQL->Add("INSERT INTO Plans");
    q->SQL->Add("(plan_id, plan_name, plan_short_name, plan_owner_id, plan_order, plan_is_rajon, PPP, FO, OKATO_kod, god_beg, god_end, isUKP_byKBK, email)");
    q->SQL->Add("VALUES (:plan_id, :plan_name, :plan_short_name, :plan_owner_id, :plan_order, :plan_is_rajon, :PPP, :FO, :OKATO_kod, :god_beg, :god_end, :isUKP_byKBK, :email)");
    q->ParamByName("plan_id")->AsInteger() = plan_id;
    q->ParamByName("plan_name")->AsString() = plan_name;
    q->ParamByName("plan_short_name")->AsString() = plan_short_name;
    q->ParamByName("plan_owner_id")->AsInteger() = plan_owner_id;
    q->ParamByName("plan_order")->AsInteger() = plan_order;
    q->ParamByName("plan_is_rajon")->AsInteger() = plan_is_rajon;
    q->ParamByName("PPP")->AsString() = PPP;
    q->ParamByName("FO")->AsString() = FO;
    q->ParamByName("OKATO_kod")->AsString() = OKATO_kod;
    q->ParamByName("god_beg")->AsInteger() = god_beg;
    q->ParamByName("god_end")->AsInteger() = god_end;
    q->ParamByName("isUKP_byKBK")->AsInteger() = isUKP_byKBK;
    q->ParamByName("email")->AsString() = email;
    q->ExecSQL();
    int result = plan_id;
    delete q;
    return result;
}

// чтение субъекта мониторинга по plan_id
bool TDB_Plan::GetPlan(TQueryUni* qIn, int plan_id) {
    qIn->SQL->Clear();
    qIn->SQL->Add("SELECT * FROM Plans");
    qIn->SQL->Add("WHERE plan_id = :plan_id");
    qIn->ParamByName("plan_id")->AsInteger() = plan_id;
    qIn->Open();
    return !qIn->EoF();
}

// // чтение наименования огруга по plan_id (который необязательно округ, может быть ему подчиненный)
std::string TDB_Plan::GetOkrugNamePPP(int plan_id, std::string PPP) {
    int plan_owner_id;
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    TDB_Plan::GetPlan(q, plan_id);
    if (q->FieldByName("plan_is_rajon")->AsInteger() > 0) {
        plan_owner_id = q->FieldByName("plan_owner_id")->AsInteger();
        q->Close();
        TDB_Plan::GetPlan(q, plan_owner_id);
    }
    std::string result = q->FieldByName("plan_name")->AsString();
    PPP = q->FieldByName("PPP")->AsString();
    delete q;
    return result;
}

// получить наименования огруга в дательном падеже - кому
std::string TDB_Plan::GetOkrugNameTo(std::string okrugName) {
    std::string s = okrugName + "у";
//    std::replace(s.begin(), s.end(), "ый", "ому");
    std::string from = "ый";
    std::string to = "ому";
    size_t start_pos = 0;
    while ((start_pos = s.find(from, start_pos)) != std::string::npos) {
        s.replace(start_pos, from.length(), to);
        start_pos += to.length(); // Перемещаемся вперёд на длину заменяемой подстроки
    }
    return s;
}

// получить наименования огруга в родительском падеже - кого
std::string TDB_Plan::GetOrFK(int plan_id) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    q->SQL->Clear();
    q->SQL->Add("SELECT FO FROM Plans");
    q->SQL->Add("WHERE plan_id = :plan_id");
    q->ParamByName("plan_id")->AsInteger() = plan_id;
    q->Open();
    std::string result = q->FieldByName("FO")->AsString();
    delete q;
    return result;
}

// получить наименования огруга в родительском падеже - кого
std::string TDB_Plan::GetOkrugNameGo(std::string okrugName) {
    int L = okrugName.length();
    if (L == 0) return "";
    wchar_t c = okrugName[L - 1];
    std::string s;
    if (c == L'а' || c == L'А') {
        s = okrugName.substr(0, L - 1) + "ов";
    } else {
        s = okrugName + "а";
    }
//    std::replace(s.begin(), s.end(), 'ый', "ого");
    std::string from = "ый";
    std::string to = "ого";
    size_t start_pos = 0;
    while ((start_pos = s.find(from, start_pos)) != std::string::npos) {
        s.replace(start_pos, from.length(), to);
        start_pos += to.length(); // Перемещаемся вперёд на длину заменяемой подстроки
    }
    
//    std::replace(s.begin(), s.end(), "ые ", "ых ");
    from = "ые";
    to = "ых";
    start_pos = 0;
    while ((start_pos = s.find(from, start_pos)) != std::string::npos) {
        s.replace(start_pos, from.length(), to);
        start_pos += to.length(); // Перемещаемся вперёд на длину заменяемой подстроки
    }
    return s;
}

// чтение наименования субъекта мониторинга по plan_id
std::string TDB_Plan::GetPlanName(int plan_id) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    std::string result;
    if (GetPlan(q, plan_id)) {
        result = q->FieldByName("plan_name")->AsString();
    }
    delete q;
    return result;
}

// чтение краткого наименования субъекта мониторинга по plan_id
std::string TDB_Plan::GetPlanShortName(int plan_id) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    std::string result;
    if (TDB_Plan::GetPlan(q, plan_id)) {
        result = q->FieldByName("plan_short_name")->AsString();
    }
    delete q;
    return result;
}

// чтение OKATO_kod субъекта мониторинга по plan_id
std::string TDB_Plan::GetOKATO_kod(int plan_id) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    std::string result;
    if (TDB_Plan::GetPlan(q, plan_id)) {
        result = q->FieldByName("OKATO_kod")->AsString();
    }
    delete q;
    return result;
}

// получение parent_id по plan_id
int TDB_Plan::GetParentIid(int plan_id, bool isShowMSG) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    int result = 0;
    if (GetPlan(q, plan_id)) {
        result = q->FieldByName("parent_id")->AsInteger();
    }
    delete q;
    if (result == 0 && isShowMSG) {
        // SystFuncsLogic.MessageBoxWarning('Владелец в оргподчинении для данного субъекта отсутствует',MsgVnim);
    }
    return result;
}

//получение подведомств учреждения для Управы района plan_id
int TDB_Plan::GetPodVedomstvId(int plan_id, bool isShowMSG, int& plan_id_2podvedomstv) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    int result = 0;
    plan_id_2podvedomstv = 0;
    q->SQL->Clear();
    q->SQL->Add("SELECT * FROM Plans");
    q->SQL->Add("WHERE parent_id = :plan_id");
    q->ParamByName("plan_id")->AsInteger() = plan_id;
    q->Open();
    if (!q->EoF()) {
        result = q->FieldByName("plan_id")->AsInteger();
        q->Next();
        if (!q->EoF())
            plan_id_2podvedomstv = q->FieldByName("plan_id")->AsInteger();
    }
    delete q;
    if (result == 0 && isShowMSG) {
        //-=-SystFuncsLogic.MessageBoxWarning('Б/П, подведомственный Управе района отсутствует',MsgVnim);
    }
    return result;
}


// чтение порядкового номера субъекта мониторинга по plan_id
int TDB_Plan::GetPlan_order(int plan_id) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    int result = 0;
    if (GetPlan(q, plan_id))
        result = q->FieldByName("plan_order")->AsInteger();
    delete q;
    return result;
}

// чтение поля isUKP_byKBK по plan_id
int TDB_Plan::GetIsUKP_byKBK(int plan_id) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    int result = 0;
    if (GetPlan(q, plan_id))
        result = q->FieldByName("isUKP_byKBK")->AsInteger();
    delete q;
    return result;
}

// чтение Max значения plan_id
int TDB_Plan::GetMaxPlan_Id() {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    int result = 0;
    q->SQL->Clear();
    q->SQL->Add("SELECT Max(plan_id) as plan_id FROM Plans");
    q->Open();
    result = q->FieldByName("plan_id")->AsInteger();
    delete q;
    return result;
}

// чтение Max действительного порядкового номера для plan_owner_id или 0
int TDB_Plan::GetMaxPlan_order(int plan_owner_id) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    int result = 0;
    q->SQL->Clear();
    q->SQL->Add("SELECT Max(plan_order) as plan_order FROM Plans");
    q->SQL->Add("WHERE plan_owner_id = :plan_owner_id");
    q->ParamByName("plan_owner_id")->AsInteger() = plan_owner_id;
    q->Open();
    result = q->FieldByName("plan_order")->AsInteger();
    delete q;
    return result;
}

// изменить порядок субъекта мониторинга
void TDB_Plan::UpdPlanOrder(int plan_id, int plan_order) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    q->SQL->Clear();
    q->SQL->Add("UPDATE Plans");
    q->SQL->Add("SET plan_order = :plan_order");
    q->SQL->Add("WHERE plan_id = :plan_id");
    q->ParamByName("plan_id")->AsInteger() = plan_id;
    q->ParamByName("plan_order")->AsInteger() = plan_order;
    q->ExecSQL();
    delete q;
}

// удалить субъект мониторинга
void TDB_Plan::DelPlan(int plan_id) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    q->SQL->Clear();
    q->SQL->Add("DELETE FROM Plans");
    q->SQL->Add("WHERE plan_id = :plan_id");
    q->ParamByName("plan_id")->AsInteger() = plan_id;
    q->ExecSQL();
    delete q;
}

// изменить характеристики субъекта мониторинга
void TDB_Plan::UpdPlan1(int plan_id, const std::string plan_name, const std::string plan_short_name, const std::string PPP, const std::string FO,
                        const std::string OKATO_kod, int isUKP_byKBK, const std::string email) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    q->SQL->Clear();
    q->SQL->Add("UPDATE Plans");
    q->SQL->Add("SET plan_name = :plan_name, plan_short_name = :plan_short_name, PPP = :PPP, FO = :FO, OKATO_kod = :OKATO_kod, isUKP_byKBK = :isUKP_byKBK, email = :email");
    q->SQL->Add("WHERE plan_id = :plan_id");
    q->ParamByName("plan_id")->AsInteger() = plan_id;
    q->ParamByName("plan_name")->AsString() = plan_name;
    q->ParamByName("plan_short_name")->AsString() = plan_short_name;
    q->ParamByName("PPP")->AsString() = PPP;
    q->ParamByName("FO")->AsString() = FO;
    q->ParamByName("OKATO_kod")->AsString() = OKATO_kod;
    q->ParamByName("isUKP_byKBK")->AsInteger() = isUKP_byKBK;
    q->ParamByName("email")->AsString() = email;
    q->ExecSQL();
    delete q;
}

// изменить характеристики субъекта мониторинга 2
void TDB_Plan::UpdPlan2(int plan_id, const std::string plan_name, int plan_is_rajon, const std::string plan_short_name,
                        const std::string FO, const std::string OKATO_kod, int god_beg, int god_end, const std::string email) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    q->SQL->Clear();
    q->SQL->Add("UPDATE Plans");
    q->SQL->Add("SET plan_name = :plan_name, plan_is_rajon = :plan_is_rajon, plan_short_name = :plan_short_name, FO = :FO, OKATO_kod = :OKATO_kod, god_beg = :god_beg, god_end = :god_end, email = :email");
    q->SQL->Add("WHERE plan_id = :plan_id");
    q->ParamByName("plan_id")->AsInteger() = plan_id;
    q->ParamByName("plan_name")->AsString() = plan_name;
    q->ParamByName("plan_is_rajon")->AsInteger() = plan_is_rajon;
    q->ParamByName("plan_short_name")->AsString() = plan_short_name;
    q->ParamByName("FO")->AsString() = FO;
    q->ParamByName("OKATO_kod")->AsString() = OKATO_kod;
    q->ParamByName("god_beg")->AsInteger() = god_beg;
    q->ParamByName("god_end")->AsInteger() = god_end;
    q->ParamByName("email")->AsString() = email;
    q->ExecSQL();
    delete q;
}

// изменить parent_id
void TDB_Plan::UpdParentId(int plan_id, int parent_id) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    q->SQL->Clear();
    q->SQL->Add("UPDATE Plans");
    q->SQL->Add("SET parent_id = :parent_id");
    q->SQL->Add("WHERE plan_id = :plan_id");
    q->ParamByName("plan_id")->AsInteger() = plan_id;
    q->ParamByName("parent_id")->AsInteger() = parent_id;
    q->ExecSQL();
    delete q;
}

// изменить kodSPZ
void TDB_Plan::UpdkodSPZ(int plan_id, std::string kodSPZ) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    q->SQL->Clear();
    q->SQL->Add("UPDATE Plans");
    q->SQL->Add("SET kodSPZ = :kodSPZ");
    q->SQL->Add("WHERE plan_id = :plan_id");
    q->ParamByName("plan_id")->AsInteger() = plan_id;
    q->ParamByName("kodSPZ")->AsString() = kodSPZ;
    q->ExecSQL();
    delete q;
}

// найти plan_id по kodSPZ
int TDB_Plan::findBykodSPZ(std::string kodSPZ) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    int result = 0;
    q->SQL->Clear();
    q->SQL->Add("SELECT plan_id FROM Plans");
    q->SQL->Add("WHERE kodSPZ = :kodSPZ");
    q->ParamByName("kodSPZ")->AsString() = kodSPZ;
    q->Open();
    if (!q->EoF())
        result = q->FieldByName("plan_id")->AsInteger();
    delete q;
    return result;
}

// прочитать kodSPZ по plan_id
std::string TDB_Plan::getkodSPZ(int plan_id) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    std::string result = "";
    q->SQL->Clear();
    q->SQL->Add("SELECT kodSPZ FROM Plans");
    q->SQL->Add("WHERE plan_id = :plan_id");
    q->ParamByName("plan_id")->AsInteger() = plan_id;
    q->Open();
    if (!q->EoF())
        result = q->FieldByName("kodSPZ")->AsString();
    delete q;
    return result;
}

// прочитать kodUBP по plan_id
std::string TDB_Plan::getKodUBP(int plan_id) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    std::string result = "";
    q->SQL->Clear();
    q->SQL->Add("SELECT kodUBP FROM Plans");
    q->SQL->Add("WHERE plan_id = :plan_id");
    q->ParamByName("plan_id")->AsInteger() = plan_id;
    q->Open();
    if (!q->EoF())
        result = q->FieldByName("kodUBP")->AsString();
    delete q;
    return result;
}

// изменить kodUBP
void TDB_Plan::updKodUBP(int plan_id, std::string kodUBP) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    q->SQL->Clear();
    q->SQL->Add("UPDATE Plans");
    q->SQL->Add("SET kodUBP = :kodUBP");
    q->SQL->Add("WHERE plan_id = :plan_id");
    q->ParamByName("plan_id")->AsInteger() = plan_id;
    q->ParamByName("kodUBP")->AsString() = kodUBP;
    q->ExecSQL();
    delete q;
}

// прочитать sinonims по plan_id
std::string TDB_Plan::getSinonims(int plan_id) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    std::string result = "";
    q->SQL->Clear();
    q->SQL->Add("SELECT sinonims FROM Plans");
    q->SQL->Add("WHERE plan_id = :plan_id");
    q->ParamByName("plan_id")->AsInteger() = plan_id;
    q->Open();
    if (!q->EoF())
        result = q->FieldByName("sinonims")->AsString();
    delete q;
    return result;
}

// изменить sinonims
void TDB_Plan::updSinonims(int plan_id, std::string sinonims) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    q->SQL->Clear();
    q->SQL->Add("UPDATE Plans");
    q->SQL->Add("SET sinonims = :sinonims");
    q->SQL->Add("WHERE plan_id = :plan_id");
    q->ParamByName("plan_id")->AsInteger() = plan_id;
    q->ParamByName("sinonims")->AsString() = sinonims;
    q->ExecSQL();
    delete q;
}

// прочитать UNK по plan_id
std::string TDB_Plan::getUNK(int plan_id) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    std::string result = "";
    q->SQL->Clear();
    q->SQL->Add("SELECT UNK FROM Plans");
    q->SQL->Add("WHERE plan_id = :plan_id");
    q->ParamByName("plan_id")->AsInteger() = plan_id;
    q->Open();
    if (!q->EoF())
        result = q->FieldByName("UNK")->AsString();
    delete q;
    return result;
}

// изменить UNK
void TDB_Plan::updUNK(int plan_id, std::string UNK) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    q->SQL->Clear();
    q->SQL->Add("UPDATE Plans");
    q->SQL->Add("SET UNK = :UNK");
    q->SQL->Add("WHERE plan_id = :plan_id");
    q->ParamByName("plan_id")->AsInteger() = plan_id;
    q->ParamByName("UNK")->AsString() = UNK;
    q->ExecSQL();
    delete q;
}

// прочитать nameGosZakazcik по plan_id
std::string TDB_Plan::getNameGosZakazcik(int plan_id) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    std::string result = "";
    q->SQL->Clear();
    q->SQL->Add("SELECT nameGosZakazcik FROM Plans");
    q->SQL->Add("WHERE plan_id = :plan_id");
    q->ParamByName("plan_id")->AsInteger() = plan_id;
    q->Open();
    if (!q->EoF())
        result = q->FieldByName("nameGosZakazcik")->AsString();
    delete q;
    return result;
}

// изменить nameGosZakazcik
void TDB_Plan::updNameGosZakazcik(int plan_id, std::string nameGosZakazcik) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    q->SQL->Clear();
    q->SQL->Add("UPDATE Plans");
    q->SQL->Add("SET nameGosZakazcik = :nameGosZakazcik");
    q->SQL->Add("WHERE plan_id = :plan_id");
    q->ParamByName("plan_id")->AsInteger() = plan_id;
    q->ParamByName("nameGosZakazcik")->AsString() = nameGosZakazcik;
    q->ExecSQL();
    delete q;
}

// прочитать nameOIV по plan_id
std::string TDB_Plan::getNameOIV(int plan_id) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    std::string result = "";
    q->SQL->Clear();
    q->SQL->Add("SELECT nameOIV FROM Plans");
    q->SQL->Add("WHERE plan_id = :plan_id");
    q->ParamByName("plan_id")->AsInteger() = plan_id;
    q->Open();
    if (!q->EoF())
        result = q->FieldByName("nameOIV")->AsString();
    delete q;
    return result;
}

// изменить nameOIV
void TDB_Plan::updNameOIV(int plan_id, std::string nameOIV) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    q->SQL->Clear();
    q->SQL->Add("UPDATE Plans");
    q->SQL->Add("SET nameOIV = :nameOIV");
    q->SQL->Add("WHERE plan_id = :plan_id");
    q->ParamByName("plan_id")->AsInteger() = plan_id;
    q->ParamByName("nameOIV")->AsString() = nameOIV;
    q->ExecSQL();
    delete q;
    
}

// чтение plan_is_rajon субъекта мониторинга по plan_id
int TDB_Plan::GetPlanIsRajon(int plan_id) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    int result = 0;
    if (GetPlan(q, plan_id))
        result = q->FieldByName("plan_is_rajon")->AsInteger();
    delete q;
    return result;
    //Признак района: 1 - это район, 2 - это префектура, 3 - это предприятие
}

// чтение plan_is_rajon субъекта мониторинга через parent_id для заданного plan_id
int TDB_Plan::GetPlanIsRajonForParent(int plan_id) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    int result = 0;
    q->SQL->Clear();
    q->SQL->Add("SELECT b.plan_is_rajon FROM Plans a, Plans b");
    q->SQL->Add("WHERE a.plan_id = :plan_id");
    q->SQL->Add("AND b.plan_id = a.parent_id");
    q->ParamByName("plan_id")->AsInteger() = plan_id;
    q->Open();
    if (!q->EoF())
        result = q->FieldByName("plan_is_rajon")->AsInteger();
    delete q;
    return result;
}

// чтение всех субъектов мониторинга, кроме 1 и ORDER BY plan_owner_id, plan_order
// для которых plan_is_rajon <=3
void TDB_Plan::GetPlan_All_03(TQueryUni *qIn) {
    qIn->SQL->Clear();
    qIn->SQL->Add("SELECT * FROM Plans");
    qIn->SQL->Add("WHERE plan_id > 1");
    qIn->SQL->Add("AND plan_is_rajon <=3");
    qIn->SQL->Add("ORDER BY plan_owner_id, plan_order");
    qIn->Open();
}

// чтение всех субъектов мониторинга, кроме 1 и ORDER BY plan_owner_id, plan_order
// где на plan_is_rajon не ограничений
void TDB_Plan::GetPlan_All(TQueryUni *qIn) {
    qIn->SQL->Clear();
    qIn->SQL->Add("SELECT * FROM Plans");
    qIn->SQL->Add("WHERE plan_id > 1");
    qIn->SQL->Add("ORDER BY plan_owner_id, plan_order");
    qIn->Open();
}

// чтение всех субъектов мониторинга, доступных текущему пользователю
// этот метод такой же, что и GetAllPlan_forUser, только он оставлен для толстого клиента, чтобы меньше переделывать
void TDB_Plan::GetPlan_AllforUser(TQueryUni *qIn) {
    qIn->SQL->Clear();
    qIn->SQL->Add("SELECT User_plan.user_id, User_plan.plan_id, User_plan.user_perm,");
    qIn->SQL->Add("a.plan_id, a.plan_name, a.plan_owner_id, plan_short_name, plan_is_rajon, god_beg, god_end");
    qIn->SQL->Add("FROM User_plan, Plans a");
    qIn->SQL->Add("WHERE User_plan.user_id = :idUser");
    qIn->SQL->Add("AND User_plan.plan_id = a.plan_id");
    qIn->SQL->Add("AND a.plan_id > 1");
    qIn->SQL->Add("ORDER BY a.plan_order");
    qIn->ParamByName("idUser")->AsInteger() = GetUserId();
    qIn->Open();
}

// чтение всех субъектов мониторинга первых трех видов, подчиненных plan_owner_id
void TDB_Plan::GetPlan_ForOwner(TQueryUni *qIn, int plan_owner_id) {
    qIn->SQL->Clear();
    qIn->SQL->Add("SELECT * FROM Plans");
    qIn->SQL->Add("WHERE plan_owner_id = :plan_owner_id");
    qIn->SQL->Add("AND plan_is_rajon <=3");
    qIn->ParamByName("plan_owner_id")->AsInteger() = plan_owner_id;
    qIn->Open();
}

// чтение всех субъектов мониторинга, подчиненных plan_owner_id
bool TDB_Plan::GetSubPlans(TQueryUni *qIn, int plan_owner_id) {
    qIn->SQL->Clear();
    qIn->SQL->Add("SELECT * FROM Plans");
    qIn->SQL->Add("WHERE plan_owner_id = :plan_owner_id");
    qIn->SQL->Add("AND plan_id > 1");
    qIn->SQL->Add("ORDER BY plan_order");
    qIn->ParamByName("plan_owner_id")->AsInteger() = plan_owner_id;
    qIn->Open();
    return !qIn->EoF();
}

// чтение округа (plan_owner_id = 0 и plan_id > 1)
void TDB_Plan::GetPlanOkrug(TQueryUni *qIn) {
    qIn->SQL->Clear();
    qIn->SQL->Add("SELECT * FROM Plans");
    qIn->SQL->Add("WHERE plan_owner_id = 0");
    qIn->SQL->Add("AND plan_id > 1");
    qIn->SQL->Add("AND plan_is_rajon = 0");
    qIn->Open();
}

// выборка ППП округа
std::string TDB_Plan::GetPPPOkrug() {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    TDB_Plan::GetPlanOkrug(q);
    return q->FieldByName("PPP")->AsString();
}

// выборка ФО округа
std::string TDB_Plan::GetFOOkrug() {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    TDB_Plan::GetPlanOkrug(q);
    return q->FieldByName("FO")->AsString();
}

// выборка наименования ФО округа
std::string TDB_Plan::GetNameFOOkrug() {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    TDB_Plan::GetPlanOkrug(q);
    return q->FieldByName("nameGosZakazcik")->AsString();
}

// выборка plan_id округа
int TDB_Plan::GetIdOkrug() {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    TDB_Plan::GetPlanOkrug(q);
    return q->FieldByName("plan_id")->AsInteger();
}

// выборка plan_id Префектуры округа
int TDB_Plan::GetIdPrefOkrug() {
    int plan_owner_id = TDB_Plan::GetIdOkrug();
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    q->SQL->Clear();
    q->SQL->Add("SELECT plan_id FROM Plans");
    q->SQL->Add("WHERE plan_owner_id = :plan_owner_id");
    q->SQL->Add("AND plan_is_rajon = 2");
    q->ParamByName("plan_owner_id")->AsInteger() = plan_owner_id;
    q->Open();
    if (!q->EoF())
        return q->FieldByName("plan_id")->AsInteger();
    else
        return 0;
}

// чтение краткого наименования округа
std::string TDB_Plan::GetShotNameOkrug() {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    TDB_Plan::GetPlanOkrug(q);
    return q->FieldByName("plan_short_name")->AsString();
}

// запретить (разрешить) изменения уточ плана для всех plan_id
void TDB_Plan::SetIsNoUpdUtocPlan(bool isNo) {
    int isNoUpdUtocPlan = isNo ? 1 : 0;
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    q->SQL->Clear();
    q->SQL->Add("UPDATE Plans");
    q->SQL->Add("SET isNoUpdUtocPlan = :isNoUpdUtocPlan");
    q->ParamByName("isNoUpdUtocPlan")->AsInteger() = isNoUpdUtocPlan;
    q->ExecSQL();
}

// запретить (разрешить) изменения уточ плана для plan_id
void TDB_Plan::SetIsNoUpdUtocPlan1(int plan_id, bool isNo) {
    int isNoUpdUtocPlan = isNo ? 1 : 0;
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    q->SQL->Clear();
    q->SQL->Add("UPDATE Plans");
    q->SQL->Add("SET isNoUpdUtocPlan = :isNoUpdUtocPlan");
    q->SQL->Add("WHERE plan_id = :plan_id");
    q->ParamByName("plan_id")->AsInteger() = plan_id;
    q->ParamByName("isNoUpdUtocPlan")->AsInteger() = isNoUpdUtocPlan;
    q->ExecSQL();
}

// запретить (разрешить) изменения уточ плана для всех plan_id (Трад бюджет)
void TDB_Plan::SetIsNoUpdUtocPlanTB(bool isNo) {
    int isNoUpdUtocPlanTB = isNo ? 1 : 0;
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    q->SQL->Clear();
    q->SQL->Add("UPDATE Plans");
    q->SQL->Add("SET isNoUpdUtocPlanTB = :isNoUpdUtocPlanTB");
    q->ParamByName("isNoUpdUtocPlanTB")->AsInteger() = isNoUpdUtocPlanTB;
    q->ExecSQL();
}

// запретить (разрешить) изменения уточ плана для plan_id (Трад бюджет)
void TDB_Plan::SetIsNoUpdUtocPlanTB1(int plan_id, bool isNo) {
    int isNoUpdUtocPlanTB = isNo ? 1 : 0;
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    q->SQL->Clear();
    q->SQL->Add("UPDATE Plans");
    q->SQL->Add("SET isNoUpdUtocPlanTB = :isNoUpdUtocPlanTB");
    q->SQL->Add("WHERE plan_id = :plan_id");
    q->ParamByName("plan_id")->AsInteger() = plan_id;
    q->ParamByName("isNoUpdUtocPlanTB")->AsInteger() = isNoUpdUtocPlanTB;
    q->ExecSQL();
}

// вернуть значение isNoUpdUtocPlan для plan_id
void TDB_Plan::GetIsNoUpdUtocPlan1(int plan_id, int& isNoUpdUtocPlan, int& isNoUpdUtocPlanTB) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    isNoUpdUtocPlan = 0;
    isNoUpdUtocPlanTB = 0;
    if (GetPlan(q, plan_id)) {
        isNoUpdUtocPlan = q->FieldByName("isNoUpdUtocPlan")->AsInteger();
        isNoUpdUtocPlanTB = q->FieldByName("isNoUpdUtocPlanTB")->AsInteger();
    }
}

// разрешено ли менять уточненный план для plan_id  - это новый вариант - ЕЩЕ НЕ ИЗМЕНИЛ
bool TDB_Plan::IsRazreseno(int plan_id, int ist_fin_id) {
    std::string s = std::to_string(plan_id);
    int n, Index, isNoUpdUtocPlan, isNoUpdUtocPlanTB;
    if (!slPlans.Find(s, Index)) {
        TDB_Plan::GetIsNoUpdUtocPlan1(plan_id, isNoUpdUtocPlan, isNoUpdUtocPlanTB);
        n = 0;
        if (isNoUpdUtocPlan > 0) n |= 1;
        if (isNoUpdUtocPlanTB > 0) n |= 2;
        slPlans.AddObject(s, n);
        slPlans.Find(s, Index);
    }
    auto it = std::next(slPlans.items.begin(), Index);
    MyVariantSL& variant = it->second;
    if (ist_fin_id == ist_fin_id_CBRTOkruga){
        int* ptrToInt = std::get_if<int>(&variant);
        return ((*ptrToInt & 1) == 0);
    }
    if (ist_fin_id == ist_fin_id_TrBudg){
        int* ptrToInt = std::get_if<int>(&variant);
        return ((*ptrToInt & 2) == 0);
    }

    return true;
}

// найти plan_id по UNK
int TDB_Plan::findByUNK(std::string UNK) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    q->SQL->Clear();
    q->SQL->Add("SELECT plan_id FROM Plans");
    q->SQL->Add("WHERE UNK = :UNK");
    q->ParamByName("UNK")->AsString() = UNK;
    q->Open();
    if (!q->EoF())
        return q->FieldByName("plan_id")->AsInteger();
    return 0;
}

// найти plan_id по plan_name
int TDB_Plan::findByPlanName(std::string plan_name) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    q->SQL->Clear();
    q->SQL->Add("SELECT plan_id FROM Plans");
    q->SQL->Add("WHERE plan_name = :plan_name");
    q->ParamByName("plan_name")->AsString() = plan_name;
    q->Open();
    if (!q->EoF())
        return q->FieldByName("plan_id")->AsInteger();
    return 0;
}

//функция проверки уникальности имени при добавлении
bool TDB_Plan::UnikName(std::string plan_name, int plan_owner_id) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    q->SQL->Clear();
    q->SQL->Add("SELECT plan_id FROM Plans");
    q->SQL->Add("WHERE plan_owner_id = :plan_owner_id");
    q->SQL->Add("AND plan_name = :plan_name");
    q->ParamByName("plan_owner_id")->AsInteger() = plan_owner_id;
    q->ParamByName("plan_name")->AsString() = plan_name;
    q->Open();
    return !q->EoF();
}

//функция проверки уникальности имени при изменении
bool TDB_Plan::UnikName_Izm(int plan_id, std::string plan_name, int plan_owner_id) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    q->SQL->Clear();
    q->SQL->Add("SELECT plan_id FROM Plans");
    q->SQL->Add("WHERE plan_owner_id = :plan_owner_id");
    q->SQL->Add("AND plan_name = :plan_name");
    q->SQL->Add("AND plan_id <> :plan_id");
    q->ParamByName("plan_owner_id")->AsInteger() = plan_owner_id;
    q->ParamByName("plan_name")->AsString() = plan_name;
    q->ParamByName("plan_id")->AsInteger() = plan_id;
    q->Open();
    return !q->EoF();
}

// выбор plan_id по коду РКАТО
int TDB_Plan::getIdByOKATO(std::string aOKATO) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    q->SQL->Clear();
    q->SQL->Add("SELECT plan_id FROM Plans");
    q->SQL->Add("WHERE OKATO_kod = :aOKATO");
    q->ParamByName("aOKATO")->AsString() = aOKATO;
    q->Open();
    if (!q->EoF())
        return q->FieldByName("plan_id")->AsInteger();
    return 0;
}

// прочитать plan_owner_id
int TDB_Plan::getOwner(int plan_id) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    q->SQL->Clear();
    q->SQL->Add("SELECT plan_owner_id FROM Plans a");
    q->SQL->Add("WHERE a.plan_id = :plan_id");
    q->ParamByName("plan_id")->AsInteger() = plan_id;
    q->Open();
    if (!q->EoF())
        return q->FieldByName("plan_owner_id")->AsInteger();
    return 0;
}









