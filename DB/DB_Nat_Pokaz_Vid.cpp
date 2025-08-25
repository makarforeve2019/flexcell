#include "unDefDBconnect.h"
#include "../SystFuncs/SystFuncsLogic.h"
#include "DB_Nat_Pokaz_Vid.h"

void TDB_Nat_Pokaz_Vid::GetNPVAll(TQueryUni* qIn) {

    qIn->SQL->Clear();
    qIn->SQL->Add("SELECT * FROM Nat_pokaz_vid ORDER BY np_vid_name");
    qIn->Open();
}

// чтение всей информации про np_vid_id
void TDB_Nat_Pokaz_Vid::GetNPV(TQueryUni* qIn, int np_vid_id) {

    qIn->SQL->Clear();
    qIn->SQL->Add("SELECT * FROM Nat_pokaz_vid WHERE np_vid_id = :np_vid_id");
    qIn->ParamByName("np_vid_id")->AsInteger() = np_vid_id;
    qIn->Open();
}

// чтение наименования про np_vid_id
std::string TDB_Nat_Pokaz_Vid::GetNPVName(int np_vid_id) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    GetNPV(q, np_vid_id);
    std::string result = q->FieldByName("np_vid_name")->AsString();
    delete q;
    return result;
}

// чтение enum_id
int TDB_Nat_Pokaz_Vid::GetEnum_id(int np_vid_id) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    GetNPV(q, np_vid_id);
    int result = q->FieldByName("enum_id")->AsInteger();
    delete q;
    return result;
}

// чтение edizm_id
int TDB_Nat_Pokaz_Vid::GetEdizm_id(int np_vid_id) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    GetNPV(q, np_vid_id);
    int result = q->FieldByName("edizm_id")->AsInteger();
    delete q;
    return result;
}

int TDB_Nat_Pokaz_Vid::GetNPVId(std::string np_vid_name) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    q->SQL->Clear();
    q->SQL->Add("SELECT * FROM Nat_pokaz_vid WHERE np_vid_name = :np_vid_name");
    q->ParamByName("np_vid_name")->AsString() = np_vid_name;
    q->Open();
    int result = (q->EoF()) ? 0 : q->FieldByName("np_vid_id")->AsInteger();
    delete q;
    return result;
}

// удаление элемента
void TDB_Nat_Pokaz_Vid::DelItem(int np_vid_id) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    q->SQL->Clear();
    q->SQL->Add("DELETE FROM Nat_pokaz_vid WHERE np_vid_id = :np_vid_id");
    q->ParamByName("np_vid_id")->AsInteger() = np_vid_id;
    q->ExecSQL();
    delete q;
}

// изменяем наименование
void TDB_Nat_Pokaz_Vid::UpdName(std::string np_vid_name, int np_vid_id) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    q->SQL->Clear();
    q->SQL->Add("UPDATE Nat_pokaz_vid SET np_vid_name = :np_vid_name WHERE np_vid_id = :np_vid_id");
    q->ParamByName("np_vid_name")->AsString() = np_vid_name;
    q->ParamByName("np_vid_id")->AsInteger() = np_vid_id;
    q->ExecSQL();
    delete q;
}

void TDB_Nat_Pokaz_Vid::UpdEnum_id(int enum_id, int np_vid_id) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    q->SQL->Clear();
    q->SQL->Add("UPDATE Nat_pokaz_vid SET enum_id = :enum_id WHERE np_vid_id = :np_vid_id");
    q->ParamByName("enum_id")->AsInteger() = enum_id;
    q->ParamByName("np_vid_id")->AsInteger() = np_vid_id;
    q->ExecSQL();
    delete q;
}

// изменяем edizm_id
void TDB_Nat_Pokaz_Vid::UpdEdizm_id(int edizm_id, int np_vid_id) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    q->SQL->Clear();
    q->SQL->Add("UPDATE Nat_pokaz_vid SET edizm_id = :edizm_id WHERE np_vid_id = :np_vid_id");
    q->ParamByName("edizm_id")->AsInteger() = edizm_id;
    q->ParamByName("np_vid_id")->AsInteger() = np_vid_id;
    q->ExecSQL();
    delete q;
}

int TDB_Nat_Pokaz_Vid::InsertItem(std::string np_vid_name, int np_vid_type, int enum_id, int edizm_id) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    q->SQL->Clear();
    q->SQL->Add("INSERT INTO Nat_pokaz_vid (np_vid_name, np_vid_type, enum_id, edizm_id) "
                "VALUES (:np_vid_name, :np_vid_type, :enum_id, :edizm_id)");
    q->ParamByName("np_vid_name")->AsString() = np_vid_name;
    q->ParamByName("np_vid_type")->AsInteger() = np_vid_type;
    q->ParamByName("enum_id")->AsInteger() = enum_id;
    q->ParamByName("edizm_id")->AsInteger() = edizm_id;
    q->ExecSQL();
    q->SQL->Clear();
    switch (GetDBType()) {
        case TDBType::MSSQL:
            q->SQL->Add("SELECT @@IDENTITY as np_vid_id");
            break;
        case TDBType::Postgre:
            q->SQL->Add("SELECT currval(pg_get_serial_sequence('Nat_pokaz_vid', 'np_vid_id')) as np_vid_id");
            break;
        // Handle other database types if necessary
    }
    q->Open();
    int result = q->FieldByName("np_vid_id")->AsInteger();
    delete q;
    return result;
}