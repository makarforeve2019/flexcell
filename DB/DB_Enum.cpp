#include "unDefDBconnect.h"
#include "../SystFuncs/SystFuncsLogic.h"
#include "DB_Enum.h" 

bool TDB_Enum::GetEnum(TQueryUni *qIn, int enum_id) {
    bool Result = true;
    qIn->SQL->Clear();
    qIn->SQL->Add("SELECT * FROM Enum WHERE enum_id = :enum_id");
    qIn->ParamByName("enum_id")->AsInteger() = enum_id;
    qIn->Open();
    
    if (qIn->EoF() && (enum_id > 0)) {
        Result = false;
    }
    
    return Result;
}

bool TDB_Enum::GetEnumByName(TQueryUni *qIn, std::string enum_name){
    bool Result = true;
    qIn->SQL->Clear();
    qIn->SQL->Add("SELECT * FROM Enum WHERE enum_name = :enum_name");
    qIn->ParamByName("enum_name")->AsString() = enum_name;
    qIn->Open();
    
    if (qIn->EoF()) {
        Result = false;
        throw std::runtime_error("Перечисление по имени " + enum_name + " не найдено");
    }
    
    return Result;
}

std::string TDB_Enum::GetEnumName(int enum_id) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    std::string Result = "???";
    
    if (GetEnum(q, enum_id)) {
        Result = q->FieldByName("enum_name")->AsString();
    }
    
   //MAK q->Free();
delete q;
    return Result;
}

int TDB_Enum::GetOrder(int enum_id) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    if (GetEnum(q, enum_id)) {
        return q->FieldByName("enum_order")->AsInteger();
    } else {
        return 0;
    }
}

int TDB_Enum::GetNPP(int enum_id) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    if (GetEnum(q, enum_id)) {
        int enum_owner_id = q->FieldByName("enum_owner_id")->AsInteger();
        TDB_Enum::GetEnumGradAll(q, enum_owner_id);
        int nPP = -1;
        while (!q->EoF()) {
            nPP++;
            if (enum_id == q->FieldByName("enum_id")->AsInteger()) {
                break;
            }
            q->Next();
        }
        return nPP;
    } else {
        return -1;
    }
}

int TDB_Enum::GetEnumId(std::string enum_name){
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    GetEnumByName(q, enum_name);
    return q->FieldByName("enum_id")->AsInteger();
}

int TDB_Enum::GetEnumGradId(int enum_id, std::string enumGrad_name){
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    int result = 0;
    
    q->SQL->Clear();
    q->SQL->Add("SELECT * FROM Enum WHERE enum_owner_id = :enum_id AND enum_name = :enum_name");
    q->ParamByName("enum_id")->AsInteger() = enum_id;
    q->ParamByName("enum_name")->AsString() = enumGrad_name;
    q->Open();
    
    if (!q->EoF())
        result = q->FieldByName("enum_id")->AsInteger();

    return result;
}

void TDB_Enum::GetEnumAll(TQueryUni *qIn) {
    qIn->SQL->Clear();
    qIn->SQL->Add("SELECT * FROM Enum WHERE enum_owner_id = 0 ORDER BY enum_order");
    qIn->Open();
}

void TDB_Enum::GetEnumGradAll(TQueryUni *qIn, int enum_owner_id) {
    qIn->SQL->Clear();
    qIn->SQL->Add("SELECT * FROM Enum WHERE enum_owner_id = :enum_owner_id ORDER BY enum_order");
    qIn->ParamByName("enum_owner_id")->AsInteger() = enum_owner_id;
    qIn->Open();
}

std::string TDB_Enum::GetTextGradAll(int enum_owner_id) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    std::string result = "";
    TDB_Enum::GetEnumGradAll(q, enum_owner_id);
    while (!q->EoF()) {
        if (!result.empty()) result += ",";
        result += q->FieldByName("enum_name")->AsString();
        q->Next();
    }
   //MAK q->Free();
delete q;
    return result;
}

int TDB_Enum::AddEnum(std::string enum_name) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    int maxOrder = 0;
    
    q->SQL->Clear();
    q->SQL->Add("SELECT MAX(enum_order) as max_order FROM Enum WHERE enum_owner_id = 0");
    q->Open();
    if (!q->EoF()) {
        maxOrder = q->FieldByName("max_order")->AsInteger();
    }
    q->Close();
    
    q->SQL->Clear();
    q->SQL->Add("INSERT INTO Enum (enum_name, enum_owner_id, enum_order) VALUES (:enum_name, 0, :enum_order)");
    q->ParamByName("enum_name")->AsString() = enum_name;
    q->ParamByName("enum_order")->AsInteger() = maxOrder + 1;
    q->ExecSQL();
    
    q->SQL->Clear();
    q->SQL->Add("SELECT enum_id FROM Enum WHERE enum_name = :enum_name AND enum_owner_id = 0");
    q->ParamByName("enum_name")->AsString() = enum_name;
    q->Open();
    int result = q->FieldByName("enum_id")->AsInteger();
   //MAK q->Free();
delete q;
    
    return result;
}

int TDB_Enum::AddEnumGrad(std::string enum_name, int enum_owner_id) {
    int maxOrder = 0;
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);

    q->SQL->Clear();
    q->SQL->Add("SELECT MAX(enum_order) as max_order FROM Enum WHERE enum_owner_id = :enum_owner_id");
    q->ParamByName("enum_owner_id")->AsInteger() = enum_owner_id;
    q->Open();
    if (!q->EoF()) {
        maxOrder = q->FieldByName("max_order")->AsInteger();
    }
    q->Close();

    q->SQL->Clear();
    q->SQL->Add("INSERT INTO Enum (enum_name, enum_owner_id, enum_order) VALUES (:enum_name, :enum_owner_id, :enum_order)");
    q->ParamByName("enum_name")->AsString() = enum_name;
    q->ParamByName("enum_order")->AsInteger() = maxOrder + 1;
    q->ParamByName("enum_owner_id")->AsInteger() = enum_owner_id;
    q->ExecSQL();

    q->SQL->Clear();
    q->SQL->Add("SELECT enum_id FROM Enum WHERE enum_name = :enum_name AND enum_owner_id = :enum_owner_id");
    q->ParamByName("enum_name")->AsString() = enum_name;
    q->ParamByName("enum_owner_id")->AsInteger() = enum_owner_id;
    q->Open();
    int result = q->FieldByName("enum_id")->AsInteger();

   //MAK q->Free();
delete q;
    return result;
}

void TDB_Enum::UpdNameEnum(std::string enum_name, int enum_id) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    
    q->SQL->Clear();
    q->SQL->Add("UPDATE Enum SET enum_name = :enum_name WHERE enum_id = :enum_id");
    q->ParamByName("enum_name")->AsString() = enum_name;
    q->ParamByName("enum_id")->AsInteger() = enum_id;
    q->ExecSQL();

   //MAK q->Free();
delete q;
}

void TDB_Enum::DelEnum(int enum_id) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    
    q->SQL->Clear();
    q->SQL->Add("DELETE FROM Enum WHERE enum_id = :enum_id");
    q->ParamByName("enum_id")->AsInteger() = enum_id;
    q->ExecSQL();

   //MAK q->Free();
delete q;
}

void TDB_Enum::ChangeOrders(int enum_id_1, int enum_order_1, int enum_id_2, int enum_order_2) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    
    q->SQL->Clear();
    q->SQL->Add("UPDATE Enum SET enum_order = :enum_order_2 WHERE enum_id = :enum_id_1");
    q->ParamByName("enum_order_2")->AsInteger() = enum_order_2;
    q->ParamByName("enum_id_1")->AsInteger() = enum_id_1;
    q->ExecSQL();

    q->SQL->Clear();
    q->SQL->Add("UPDATE Enum SET enum_order = :enum_order_1 WHERE enum_id = :enum_id_2");
    q->ParamByName("enum_order_1")->AsInteger() = enum_order_1;
    q->ParamByName("enum_id_2")->AsInteger() = enum_id_2;
    q->ExecSQL();

   //MAK q->Free();
delete q;
}

int TDB_Enum::GetEnumIdGrad(int enum_id, int nP_P) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    int result = 0;
    int n = 0;

    q->SQL->Clear();
    q->SQL->Add("SELECT * FROM Enum WHERE enum_owner_id = :enum_id ORDER BY enum_owner_id, enum_order");
    q->ParamByName("enum_id")->AsInteger() = enum_id;
    q->Open();

    while (!q->EoF()) {
        n++;
        if (n == nP_P) {
            result = q->FieldByName("enum_id")->AsInteger();
            break;
        }
        q->Next();
    }

   //MAK q->Free();
delete q;
    return result;
}
