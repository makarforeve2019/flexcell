#include "unDefDBconnect.h"
#include "DB_IstFin.h"

void TDB_IstFin::GetIstFin_All(TQueryUni* qIn) {
    qIn->SQL->Clear(); 
    qIn->SQL->Add("SELECT * FROM Ist_fin");
    qIn->SQL->Add("ORDER BY ist_fin_order");
    qIn->Open();
}

int TDB_IstFin::GetTermIstFin() {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    q->SQL->Clear();
    q->SQL->Add("SELECT DISTINCT ist_fin_id FROM Ist_fin");
    q->SQL->Add("WHERE ist_fin_order > 1");
    q->Open();
    int result = (q->EoF()) ? 0 : q->FieldByName("ist_fin_id")->AsInteger();
   //MAK q->Free();
delete q;
    return result;
}

bool TDB_IstFin::GetIstFin(TQueryUni* qIn, int ist_fin_id) {
    bool result = true;
    qIn->SQL->Clear(); 
    qIn->SQL->Add("SELECT * FROM Ist_fin");
    qIn->SQL->Add("WHERE ist_fin_id = :ist_fin_id");
    qIn->ParamByName("ist_fin_id")->AsInteger() = ist_fin_id;
    qIn->Open();
    if (qIn->EoF())
        result = false;
    return result;
}

std::string TDB_IstFin::getIstFinName(int ist_fin_id) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    q->SQL->Clear();
    q->SQL->Add("SELECT ist_fin_name FROM Ist_fin");
    q->SQL->Add("WHERE ist_fin_id = :ist_fin_id");
    q->ParamByName("ist_fin_id")->AsInteger() = ist_fin_id;
    q->Open();
    std::string result = (!q->EoF()) ? q->FieldByName("ist_fin_name")->AsString() : "";
    q->Close();
   //MAK q->Free();
delete q;
    return result;
}

int TDB_IstFin::getOrder(int ist_fin_id) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    q->SQL->Clear();
    q->SQL->Add("SELECT ist_fin_order FROM Ist_fin");
    q->SQL->Add("WHERE ist_fin_id = :ist_fin_id");
    q->ParamByName("ist_fin_id")->AsInteger() = ist_fin_id;
    q->Open();
    int result = (!q->EoF()) ? q->FieldByName("ist_fin_order")->AsInteger() : 0;
    q->Close();
   //MAK q->Free();
delete q;
    return result;
}

int TDB_IstFin::GetIstFinIdBySubs(int SubsidPokazId) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    q->SQL->Clear();
    q->SQL->Add("SELECT DISTINCT ist_fin_id FROM Ist_fin");
    q->SQL->Add("WHERE pokaz_id = :pokaz_id");
    q->ParamByName("pokaz_id")->AsInteger() = SubsidPokazId;
    q->Open();
    int result = (q->EoF()) ? 0 : q->FieldByName("ist_fin_id")->AsInteger();
   //MAK q->Free();
delete q;
    return result;
}

void TDB_IstFin::InsertVsego() {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    q->SQL->Clear();
    q->SQL->Add("INSERT INTO Ist_fin");
    q->SQL->Add("(ist_fin_name, ist_fin_order)");
    q->SQL->Add("values (:ist_fin_name, 1)");
    q->ParamByName("ist_fin_name")->AsString() = "Всего";
    q->ExecSQL();
   //MAK q->Free();
delete q;
}

int TDB_IstFin::GetIdVsego() {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    q->SQL->Clear();
    q->SQL->Add("SELECT ist_fin_id FROM Ist_fin");
    q->SQL->Add("WHERE ist_fin_order = 1");
    q->Open();
    int result = (!q->EoF()) ? q->FieldByName("ist_fin_id")->AsInteger() : 0;
   //MAK q->Free();
delete q;
    return result;
}

int TDB_IstFin::InsertIstFin(string ist_name) {
    int maxOrder = 0;
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    
    // Retrieve the maximum ist_fin_order
    q->SQL->Clear();
    q->SQL->Add("SELECT MAX(ist_fin_order) as max_order FROM Ist_fin");
    q->Open();
    if (!q->EoF())
        maxOrder = q->FieldByName("max_order")->AsInteger();
    q->Close();

    // Insert new record
    q->SQL->Clear();
    q->SQL->Add("INSERT INTO Ist_fin");
    q->SQL->Add("(ist_fin_name, ist_fin_order)");
    q->SQL->Add("values (:ist_fin_name, :ist_fin_order)");
    q->ParamByName("ist_fin_name")->AsString() = ist_name;
    q->ParamByName("ist_fin_order")->AsInteger() = maxOrder + 1;
    q->ExecSQL();

    // Retrieve the id of the inserted record
    q->SQL->Clear();
    q->SQL->Add("SELECT ist_fin_id FROM Ist_fin");
    q->SQL->Add("WHERE ist_fin_name = :ist_fin_name");
    q->ParamByName("ist_fin_name")->AsString() = ist_name;
    q->Open();
    int result = (!q->EoF()) ? q->FieldByName("ist_fin_id")->AsInteger() : 0;
    
   //MAK q->Free();
delete q;
    return result;
}

void TDB_IstFin::ModIstFin(string ist_name, int ist_fin_id) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    q->SQL->Clear();
    q->SQL->Add("UPDATE Ist_fin");
    q->SQL->Add("SET ist_fin_name = :ist_fin_name");
    q->SQL->Add("WHERE ist_fin_id = :ist_fin_id");
    q->ParamByName("ist_fin_name")->AsString() = ist_name;
    q->ParamByName("ist_fin_id")->AsInteger() = ist_fin_id;
    q->ExecSQL();
   //MAK q->Free();
delete q;
}

void TDB_IstFin::Modorder(int ist_fin_order, int ist_fin_id) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    q->SQL->Clear();
    q->SQL->Add("UPDATE Ist_fin");
    q->SQL->Add("SET ist_fin_order = :ist_fin_order");
    q->SQL->Add("WHERE ist_fin_id = :ist_fin_id");
    q->ParamByName("ist_fin_order")->AsInteger() = ist_fin_order;
    q->ParamByName("ist_fin_id")->AsInteger() = ist_fin_id;
    q->ExecSQL();
   //MAK q->Free();
delete q;
}

int TDB_IstFin::UnikName(string ist_name) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    int result = 0;
    q->SQL->Clear();
    q->SQL->Add("SELECT ist_fin_id FROM Ist_fin");
    q->SQL->Add("WHERE ist_fin_name = :ist_fin_name");
    q->ParamByName("ist_fin_name")->AsString() = ist_name;
    q->Open();
    if (!q->EoF())
        result = q->FieldByName("ist_fin_id")->AsInteger();
   //MAK q->Free();
delete q;
    return result;
}

bool TDB_IstFin::UnikName_Izm(string ist_name, int ist_fin_id) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    bool result = true;
    q->SQL->Clear();
    q->SQL->Add("SELECT ist_fin_id FROM Ist_fin");
    q->SQL->Add("WHERE ist_fin_name = :ist_fin_name");
    q->SQL->Add("AND ist_fin_id <> :ist_fin_id");
    q->ParamByName("ist_fin_name")->AsString() = ist_name;
    q->ParamByName("ist_fin_id")->AsInteger() = ist_fin_id;
    q->Open();
    if (!q->EoF())
        result = false;
   //MAK q->Free();
delete q;
    return result;
}

void TDB_IstFin::DelIstFin(int ist_fin_id) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    q->SQL->Clear();
    q->SQL->Add("DELETE FROM Ist_fin");
    q->SQL->Add("WHERE ist_fin_id = :ist_fin_id");
    q->ParamByName("ist_fin_id")->AsInteger() = ist_fin_id;
    q->ExecSQL();
   //MAK q->Free();
delete q;
}

int TDB_IstFin::getIstFinIdByPokazId(int pokaz_id) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    int result = 0;
    q->SQL->Clear();
    q->SQL->Add("SELECT ist_fin_id FROM Ist_fin");
    q->SQL->Add("WHERE pokaz_id = :pokaz_id");
    q->ParamByName("pokaz_id")->AsInteger() = pokaz_id;
    q->Open();
    if (!q->EoF())
        result = q->FieldByName("ist_fin_id")->AsInteger();
    q->Close();
   //MAK q->Free();
delete q;
    return result;
}

std::string TDB_IstFin::getCodeCelSubsByIstFinId(int ist_fin_id) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    string result = "";
    q->SQL->Clear();
    q->SQL->Add("SELECT pokaz_code FROM pokaztl");
    q->SQL->Add("WHERE pokaz_id = (SELECT pokaz_id FROM ist_fin WHERE ist_fin_id = :ist_fin_id)");
    q->ParamByName("ist_fin_id")->AsInteger() = ist_fin_id;
    q->Open();
    if (!q->EoF())
        result = q->FieldByName("pokaz_code")->AsString();
   //MAK q->Free();
delete q;
    return result;
}

int TDB_IstFin::getPokazIdIFbyIstFinId(int ist_fin_id) {
    int Result = 0;
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    q->SQL->Clear();
    q->SQL->Add("SELECT pokaz_id FROM Ist_fin");
    q->SQL->Add("WHERE ist_fin_id = :ist_fin_id");
    q->ParamByName("ist_fin_id")->AsInteger() = ist_fin_id;
    q->Open();
    if (!q->EoF())
        Result = q->FieldByName("pokaz_id")->AsInteger();
   //MAK q->Free();
delete q;
    return Result;
}


