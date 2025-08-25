#include "DB_EdIzm.h"
#include "unDefDBconnect.h"
#include "SystFuncsLogic.h"
#include "dmGlobalVars.h"
#include "unIDsClassFieldCell.h"


int TDB_EdIzm::GetId() {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    int result = 0;
    q->SQL->Clear();
    switch (GetDBType()) {
        case TDBType::MSSQL:
            q->SQL->Add("SELECT @@IDENTITY as edizm_id");
            break;
        case TDBType::Postgre:
            q->SQL->Add("SELECT currval(pg_get_serial_sequence('Edizm', 'edizm_id')) as edizm_id");
            break;
    }

    q->Open();
    result = q->FieldByName("edizm_id").AsInteger();
   //MAK q->Free();
delete q;
    return result;
}

bool TDB_EdIzm::GetEdIzm(TQueryUni* qIn, int edizm_id) {
    qIn->SQL->Clear();
    qIn->SQL->Add("SELECT * FROM Edizm WHERE edizm_id = :edizm_id");
    qIn->ParamByName("edizm_id")->AsInteger() = edizm_id;
    qIn->Open();
    return true;
}

std::string TDB_EdIzm::GetEdIzmName(int edizm_id) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    GetEdIzm(q, edizm_id);
    std::string result = q->FieldByName("edizm_name").AsString();
   //MAK q->Free();
delete q;
    return result;
}

int TDB_EdIzm::GetEdIzmId(std::string edizm_name) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    int result = 0;
    q->SQL->Clear();
    q->SQL->Add("SELECT * FROM Edizm WHERE edizm_name = :edizm_name");
    q->ParamByName("edizm_name")->AsString() = edizm_name;
    q->Open();
    if (!q->EoF())
        result = q->FieldByName("edizm_id")->AsInteger();
   //MAK q->Free();
delete q;
    return result;
}

int TDB_EdIzm::GetEdizm_is_gr(int edizm_id) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    GetEdIzm(q, edizm_id);
    int result = q->FieldByName("edizm_is_gr")->AsInteger();
   //MAK q->Free();
delete q;
    return result;
}

int TDB_EdIzm::GetEdizm_order(int edizm_id) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    GetEdIzm(q, edizm_id);
    int result = q->FieldByName("edizm_order")->AsInteger();
   //MAK q->Free();
delete q;
    return result;
}

int TDB_EdIzm::AddEdIzm(std::string edizm_name, int edizm_is_gr, int edizm_owner_id, int edizm_order, int edizm_coef) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    
    q->SQL->Clear();
    q->SQL->Add("INSERT INTO Edizm");
    q->SQL->Add("(edizm_name, edizm_is_gr, edizm_owner_id, edizm_order, edizm_coef)");
    q->SQL->Add("VALUES (:edizm_name, :edizm_is_gr, :edizm_owner_id, :edizm_order, :edizm_coef)");
    
    q->ParamByName("edizm_name")->AsString() = edizm_name;
    q->ParamByName("edizm_is_gr")->AsInteger() = edizm_is_gr;
    q->ParamByName("edizm_owner_id")->AsInteger() = edizm_owner_id;
    q->ParamByName("edizm_order")->AsInteger() = edizm_order;
    q->ParamByName("edizm_coef")->AsInteger() = edizm_coef;
    
    q->ExecSQL();
    int result = TDB_EdIzm::GetId();
    
   //MAK q->Free();
delete q;
    return result;
}

void TDB_EdIzm::GetEdIzmAll(TQueryUni *qIn) {
    qIn->SQL->Clear();
    qIn->SQL->Add("SELECT * FROM Edizm");
    qIn->SQL->Add("ORDER BY edizm_owner_id");
    qIn->Open();
}

void TDB_EdIzm::DelEdIzm(int edizm_id) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    q->SQL->Clear();
    q->SQL->Add("DELETE FROM Edizm");
    q->SQL->Add("WHERE edizm_id = :edizm_id");
    q->ParamByName("edizm_id")->AsInteger() = edizm_id;
    q->ExecSQL();
    
   //MAK q->Free();
delete q;
}

void TDB_EdIzm::UpdEdIzm1(int edizm_id, int edizm_owner_id, int edizm_order) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    
    q->SQL->Clear();
    q->SQL->Add("UPDATE Edizm");
    q->SQL->Add("SET edizm_owner_id = :edizm_owner_id, edizm_order = :edizm_order");
    q->SQL->Add("WHERE edizm_id = :edizm_id");
    q->ParamByName("edizm_id")->AsInteger() = edizm_id;
    q->ParamByName("edizm_owner_id")->AsInteger() = edizm_owner_id;
    q->ParamByName("edizm_order")->AsInteger() = edizm_order;
    q->ExecSQL();
    
   //MAK q->Free();
delete q;
}

void TDB_EdIzm::UpdEdIzm2(int edizm_id, string edizm_name) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    
    q->SQL->Clear();
    q->SQL->Add("UPDATE Edizm");
    q->SQL->Add("SET edizm_name = :edizm_name");
    q->SQL->Add("WHERE edizm_id = :edizm_id");
    q->ParamByName("edizm_id")->AsInteger() = edizm_id;
    q->ParamByName("edizm_name")->AsString() = edizm_name.c_str();
    q->ExecSQL();
    
   //MAK q->Free();
delete q;
}

void TDB_EdIzm::UpdEdIzm3(int edizm_id, string edizm_name, int edizm_coef) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    
    q->SQL->Clear();
    q->SQL->Add("UPDATE Edizm");
    q->SQL->Add("SET edizm_name = :edizm_name, edizm_coef = :edizm_coef");
    q->SQL->Add("WHERE edizm_id = :edizm_id");
    q->ParamByName("edizm_id")->AsInteger() = edizm_id;
    q->ParamByName("edizm_name")->AsString() = edizm_name.c_str();
    q->ParamByName("edizm_coef")->AsInteger() = edizm_coef;
    q->ExecSQL();
    
   //MAK q->Free();
delete q;
}

string TDB_EdIzm::GetEdIzmNameOpt(int edizm_id) {
      return TIDsClassFieldCell::getNameEdIzm(edizm_id);
}

int TDB_EdIzm::GetCoef_PokazOpt(int edizm_id) {
    return TIDsClassFieldCell::getEdIzmCoef(edizm_id);
}
