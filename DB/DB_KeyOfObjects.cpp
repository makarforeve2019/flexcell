#include "unDefDBconnect.h"
#include "../SystFuncs/SystFuncsLogic.h"
#include "DB_KeyOfObjects.h"

bool TDB_KeyOfObjects::IsKeyObject(int class_id_ForKey, const std::string& str_value, int god, int objectid) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    bool Result = false;
    
    q->SQL->Clear();
    q->SQL->Add("SELECT objectid FROM KeyOfObjects");
    q->SQL->Add("WHERE class_id = :class_id");
    q->SQL->Add("AND value_key = :value_key");
    q->SQL->Add("AND god = :god");
    q->SQL->Add("AND objectid <> :objectid");
    q->ParamByName("class_id")->AsInteger() = class_id_ForKey;
    q->ParamByName("value_key")->AsString() = str_value;
    q->ParamByName("god")->AsInteger() = god;
    q->ParamByName("objectid")->AsInteger() = objectid;
    q->Open();
    if (!q->EoF()) {
        Result = true;
    }
    delete q; // Free the memory allocated for q
    return Result;
}

void TDB_KeyOfObjects::SetKeyObject(int objectid, int class_id_ForKey, const std::string& str_value, int god) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    bool est = false;
    
    q->SQL->Clear();
    q->SQL->Add("SELECT objectid FROM KeyOfObjects");
    q->SQL->Add("WHERE objectid = :objectid");
    q->ParamByName("objectid")->AsInteger() = objectid;
    q->Open();
    est = !q->EoF();
    q->Close();
    
    if (est) {
        q->SQL->Clear();
        q->SQL->Add("UPDATE KeyOfObjects");
        q->SQL->Add("SET value_key = :value_key, god = :god");
        q->SQL->Add("WHERE objectid = :objectid");
        q->ParamByName("objectid")->AsInteger() = objectid;
        q->ParamByName("value_key")->AsString() = str_value;
        q->ParamByName("god")->AsInteger() = god;
        q->ExecSQL();
    } else {
        q->SQL->Clear();
        q->SQL->Add("INSERT INTO KeyOfObjects");
        q->SQL->Add("(objectid, class_id, value_key, god)");
        q->SQL->Add("VALUES (:objectid, :class_id, :value_key, :god)");
        q->ParamByName("objectid")->AsInteger() = objectid;
        q->ParamByName("class_id")->AsInteger() = class_id_ForKey;
        q->ParamByName("value_key")->AsString() = str_value;
        q->ParamByName("god")->AsInteger() = god;
        q->ExecSQL();
    }
    delete q; // Free the memory allocated for q
}

bool TDB_KeyOfObjects::GetIdObjectByKey(int class_id, std::string value_key, int& Objectid, int god) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    bool Result = false;
    
    q->SQL->Clear();
    q->SQL->Add("SELECT objectid FROM KeyOfObjects");
    q->SQL->Add("WHERE class_id = :class_id");
    q->SQL->Add("AND value_key = :value_key");
    q->SQL->Add("AND god = :god");
    q->ParamByName("class_id")->AsInteger() = class_id;
    q->ParamByName("value_key")->AsString() = value_key;
    q->ParamByName("god")->AsInteger() = god;
    q->Open();
    if (!q->EoF()) {
        Result = true;
        Objectid = q->FieldByName("objectid")->AsInteger();
    }
    delete q; // Free the memory allocated for q
    return Result;
}

std::string TDB_KeyOfObjects::GetKeyByIdObject(int Objectid) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    std::string Result = "";
    
    q->SQL->Clear();
    q->SQL->Add("SELECT value_key FROM KeyOfObjects");
    q->SQL->Add("WHERE objectid = :objectid");
    q->ParamByName("objectid")->AsInteger() = Objectid;
    q->Open();
    if (!q->EoF()) {
        Result = q->FieldByName("value_key")->AsString();
    }
    delete q; // Free the memory allocated for q
    return Result;
}

void TDB_KeyOfObjects::DelKeyByIdObjectGod(int Objectid, int god) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    
    q->SQL->Clear();
    q->SQL->Add("DELETE FROM KeyOfObjects");
    q->SQL->Add("WHERE objectid = :objectid");
    q->SQL->Add("AND god = :god");
    q->ParamByName("objectid")->AsInteger() = Objectid;
    q->ParamByName("god")->AsInteger() = god;
    q->ExecSQL();
    delete q; // Free the memory allocated for q
}

std::string TDB_KeyOfObjects::SetKeyObjectAvtoNumber(int objectid, int class_id_ForKey, int god) {
    bool est = false;
    std::string value_key = "";
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    
    q->SQL->Clear();
    q->SQL->Add("SELECT value_key FROM KeyOfObjects");
    q->SQL->Add("WHERE objectid = :objectid");
    q->ParamByName("objectid")->AsInteger() = objectid;
    q->Open();
    if (!q->EoF()) {
        est = true;
        value_key = q->FieldByName("value_key")->AsString();
    }
    q->Close();
    
    if (est) {
        return value_key;
    }
    
    q->SQL->Clear();
    q->SQL->Add("INSERT INTO KeyOfObjects");
    q->SQL->Add("(objectid, class_id, value_key, god)");
    q->SQL->Add("VALUES (:objectid, :class_id, 0, :god)");
    q->ParamByName("god")->AsInteger() = god;
    q->ParamByName("class_id")->AsInteger() = class_id_ForKey;
    q->ParamByName("objectid")->AsInteger() = objectid;
    q->ExecSQL();

    q->SQL->Clear();
    q->SQL->Add("UPDATE KeyOfObjects");
    q->SQL->Add("SET value_key = (SELECT CASE ");
    q->SQL->Add("     WHEN max(cast(value_key as int)) IS NULL THEN 1");
    q->SQL->Add("     ELSE max(cast(value_key as int)) + 1");
    q->SQL->Add("  END FROM KeyOfObjects WHERE class_id = :class_id AND god = :god)");
    q->SQL->Add("WHERE objectid = :objectid");
    q->ParamByName("god")->AsInteger() = god;
    q->ParamByName("class_id")->AsInteger() = class_id_ForKey;
    q->ParamByName("objectid")->AsInteger() = objectid;
    q->ExecSQL();

    delete q; // Free the memory allocated for q
    return TDB_KeyOfObjects::GetKeyByIdObject(objectid);
}