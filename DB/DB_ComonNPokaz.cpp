#include "DB_ComonNPokaz.h"
#include "unDefDBconnect.h"
#include "../SystFuncs/SystFuncsLogic.h"

int TDB_ComonNPokaz::GetId() {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    
    int comnp_id = -1;
    
    q->SQL->Clear();
    switch (GetDBType()) {
        case TDBType::MSSQL:
            q->SQL->Add("SELECT @@IDENTITY as comnp_id");
            break;
        case TDBType::Postgre:
            q->SQL->Add("SELECT currval(pg_get_serial_sequence('ComonNPokaz', 'comnp_id')) as comnp_id");
            break;
    }


    q->Open();
    if (!q->EoF()) {
        comnp_id = q->FieldByName("comnp_id")->AsInteger();
    }
    q->Close();
   //MAK q->Free();
delete q;
    
    return comnp_id;
}

void TDB_ComonNPokaz::GetAllItems(TQueryUni *qIn) {
    qIn->SQL->Clear();
    qIn->SQL->Add("SELECT * FROM ComonNPokaz ORDER BY orderN");
    qIn->Open();
}

bool TDB_ComonNPokaz::UnikAddName(std::string comnp_name) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);;
    bool result = true;

    q->SQL->Clear();
    q->SQL->Add("SELECT comnp_id FROM ComonNPokaz WHERE comnp_name = :comnp_name");
    q->ParamByName("comnp_name")->AsString() = comnp_name;
    q->Open();
    
    if (!q->EoF()) {
        result = false;
    }
    q->Close();
   //MAK q->Free();
delete q;
    return result;
}

bool TDB_ComonNPokaz::UnikUpdName(std::string comnp_name, int comnp_id){
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    bool result = true;

    q->SQL->Clear();
    q->SQL->Add("SELECT comnp_id FROM ComonNPokaz WHERE comnp_name = :comnp_name AND comnp_id <> :comnp_id");
    q->ParamByName("comnp_name")->AsString() = comnp_name;
    q->ParamByName("comnp_id")->AsInteger() = comnp_id;
    q->Open();

    if (!q->EoF()) {
        result = false;
    }
    q->Close();
   //MAK q->Free();
delete q;
    return result;
}

int TDB_ComonNPokaz::GetMaxOrderN() {
    TQueryUni *QMax = new TQueryUni(DefDBconnect.Conn);
    QMax->SQL->Clear();
    QMax->SQL->Add("SELECT MAX(orderN) as MaxOrderN FROM ComonNPokaz");
    QMax->Open();
    int result = QMax->FieldByName("MaxOrderN")->AsInteger();
    QMax->Close();
    //MAK QMax->Free();
    delete QMax;
    return result;
}

void TDB_ComonNPokaz::UpdOrderN1N2(int comnp_id1, int comnp_id2) {
    int orderN1, orderN2;
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    
    TDB_ComonNPokaz::GetComonNPokaz(q, comnp_id1);
    orderN1 = q->FieldByName("orderN")->AsInteger();
    q->Close();
    
    TDB_ComonNPokaz::GetComonNPokaz(q, comnp_id2);
    orderN2 = q->FieldByName("orderN")->AsInteger();
   //MAK q->Free();
delete q;

    TDB_ComonNPokaz::UpdorderN(comnp_id1, 99999999);
    TDB_ComonNPokaz::UpdorderN(comnp_id2, orderN1);
    TDB_ComonNPokaz::UpdorderN(comnp_id1, orderN2);
}

int TDB_ComonNPokaz::AddComonNPokaz(std::string comnp_name, int comnp_type, int enum_id, int edizm_id, std::string primecanie, std::string toTblName,
                                    int IsFieldsSprvcnik, std::string toColumnName1, int is_const, unsigned char is_mes, unsigned char is_kv, unsigned char is_god) {
    int orderN = TDB_ComonNPokaz::GetMaxOrderN() + 1;
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    
    q->SQL->Clear();
    q->SQL->Add("INSERT INTO ComonNPokaz (comnp_name, comnp_type, enum_id, edizm_id, primecanie, toTblName, IsFieldsSprvcnik, "
        "toColumnName1, orderN, is_const, is_mes, is_kv, is_god) VALUES (:comnp_name, :comnp_type, :enum_id, :edizm_id, "
        ":primecanie, :toTblName, :IsFieldsSprvcnik, :toColumnName1, :orderN, :is_const, :is_mes, :is_kv, :is_god)");
    
    q->ParamByName("comnp_name")->AsString() = comnp_name;
    q->ParamByName("comnp_type")->AsInteger() = comnp_type;
    q->ParamByName("enum_id")->AsInteger() = enum_id;
    q->ParamByName("edizm_id")->AsInteger() = edizm_id;
    q->ParamByName("primecanie")->AsString() = primecanie;
    q->ParamByName("toTblName")->AsString() = toTblName;
    q->ParamByName("IsFieldsSprvcnik")->AsInteger() = IsFieldsSprvcnik;
    q->ParamByName("toColumnName1")->AsString() = toColumnName1;
    q->ParamByName("orderN")->AsInteger() = orderN;
    q->ParamByName("is_const")->AsInteger() = is_const;
    q->ParamByName("is_mes")->AsInteger() = is_mes;
    q->ParamByName("is_kv")->AsInteger() = is_kv;
    q->ParamByName("is_god")->AsInteger() = is_god;
    q->ExecSQL();

   //MAK q->Free();
delete q;
    return TDB_ComonNPokaz::GetId();
}

void TDB_ComonNPokaz::UpdComonNPokaz(int comnp_id, std::string comnp_name, int edizm_id, std::string primecanie, int enum_id) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    q->SQL->Clear();
    q->SQL->Add("UPDATE ComonNPokaz SET comnp_name=:comnp_name, edizm_id=:edizm_id, primecanie=:primecanie, enum_id=:enum_id WHERE comnp_id = :comnp_id");
    q->ParamByName("comnp_id")->AsInteger() = comnp_id;
    q->ParamByName("comnp_name")->AsString() = comnp_name;
    q->ParamByName("edizm_id")->AsInteger() = edizm_id;
    q->ParamByName("primecanie")->AsString() = primecanie;
    q->ParamByName("enum_id")->AsInteger() = enum_id;
    q->ExecSQL();
   //MAK q->Free();
delete q;
}

void TDB_ComonNPokaz::UpdorderN(int comnp_id, int orderN) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    q->SQL->Clear();
    q->SQL->Add("UPDATE ComonNPokaz SET orderN=:orderN WHERE comnp_id = :comnp_id");
    q->ParamByName("comnp_id")->AsInteger() = comnp_id;
    q->ParamByName("orderN")->AsInteger() = orderN;
    q->ExecSQL();
   //MAK q->Free();
delete q;
}

void TDB_ComonNPokaz::GetComonNPokaz(TQueryUni *qIn, int comnp_id) {
    qIn->SQL->Clear();
    qIn->SQL->Add("SELECT * FROM ComonNPokaz WHERE comnp_id = :comnp_id");
    qIn->ParamByName("comnp_id")->AsInteger() = comnp_id;
    qIn->Open();
}

int TDB_ComonNPokaz::GetTypeComonNPokaz(int comnp_id) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    TDB_ComonNPokaz::GetComonNPokaz(q, comnp_id);
    int result = q->FieldByName("comnp_type")->AsInteger();
   //MAK q->Free();
delete q;
    return result;
}

bool TDB_ComonNPokaz::isComonNPokaz(int comnp_id) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    TDB_ComonNPokaz::GetComonNPokaz(q, comnp_id);
    bool result = !q->EoF();
   //MAK q->Free();
delete q;
    return result;
}

std::string TDB_ComonNPokaz::GetComnpName(int comnp_id) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    TDB_ComonNPokaz::GetComonNPokaz(q, comnp_id);
    std::string result = q->FieldByName("comnp_name")->AsString();
   //MAK q->Free();
delete q;
    return result;
}

int TDB_ComonNPokaz::GetIsFieldsSprvcnik(int comnp_id) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    TDB_ComonNPokaz::GetComonNPokaz(q, comnp_id);
    int result = q->FieldByName("IsFieldsSprvcnik")->AsInteger();
   //MAK q->Free();
delete q;
    return result;
}

int TDB_ComonNPokaz::GetEnum_id(int comnp_id) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    TDB_ComonNPokaz::GetComonNPokaz(q, comnp_id);
    int result = q->FieldByName("enum_id")->AsInteger();
    return result;
}

int TDB_ComonNPokaz::GetorderN(int comnp_id) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    TDB_ComonNPokaz::GetComonNPokaz(q, comnp_id);
    int result = q->FieldByName("orderN")->AsInteger();
   //MAK q->Free();
delete q;
    return result;
}

bool TDB_ComonNPokaz::GetComnpIdByComnpName(std::string comnp_name, int& comnp_id) {
    bool result = true;
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    q->SQL->Clear();
    q->SQL->Add("SELECT comnp_id");
    q->SQL->Add("FROM ComonNPokaz");
    q->SQL->Add("WHERE comnp_name = :comnp_name");
    q->ParamByName("comnp_name")->AsString() = comnp_name;
    q->Open();
    if (!q->EoF()) {
        comnp_id = q->FieldByName("comnp_id")->AsInteger();
        result = true;
    } else {
        result = false;
    }
   //MAK q->Free();
delete q;
    return result;
}

void TDB_ComonNPokaz::DelComonNPokaz(int comnp_id) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    q->SQL->Clear();
    q->SQL->Add("DELETE FROM ComonNPokaz");
    q->SQL->Add("WHERE comnp_id = :comnp_id");
    q->ParamByName("comnp_id")->AsInteger() = comnp_id;
    q->ExecSQL();
   //MAK q->Free();
delete q;
}

void TDB_ComonNPokaz::GetInfo(int comnp_id, unsigned char& IsFieldsSprvcnik, std::string& toTblName, std::string& toColumnName1) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    TDB_ComonNPokaz::GetComonNPokaz(q, comnp_id);
    IsFieldsSprvcnik = q->FieldByName("IsFieldsSprvcnik")->AsInteger();
    toTblName = q->FieldByName("toTblName")->AsString();
    toColumnName1 = q->FieldByName("toColumnName1")->AsString();
   //MAK q->Free();
delete q;
}

int TDB_ComonNPokaz::GetEdizm_id(int comnp_id) {
TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    TDB_ComonNPokaz::GetComonNPokaz(q, comnp_id);
    int result = q->FieldByName("edizm_id")->AsInteger();
   //MAK q->Free();
delete q;
    return result;
}

int TDB_ComonNPokaz::GetComnp_type(int comnp_id) {
TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    TDB_ComonNPokaz::GetComonNPokaz(q, comnp_id);
    int result = q->FieldByName("comnp_type")->AsInteger();
   //MAK q->Free();
delete q;
    return result;
}
