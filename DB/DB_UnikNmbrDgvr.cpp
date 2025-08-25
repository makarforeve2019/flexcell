#include "DB_UnikNmbrDgvr.h"
#include "unDefDBconnect.h"
#include "../SystFuncs/SystFuncsLogic.h"


int TDB_UnikNmbrDgvr::GetId() {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);

    q->SQL->Clear();

    switch (GetDBType()) {
        case TDBType::MSSQL:
            q->SQL->Add("SELECT @@IDENTITY as undgvr_id");
            break;
        case TDBType::Postgre:
            q->SQL->Add("SELECT currval(pg_get_serial_sequence('UnikNmbrDgvr', 'undgvr_id')) as undgvr_id");
            break;
        // Handle other database types if needed
    }

    q->Open();
    int result = q->FieldByName("undgvr_id")->AsInteger();
    delete q; // Don't forget to free the memory allocated for the object
    return result;
}

int TDB_UnikNmbrDgvr::AddNewItem(int god, int plan_id) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);// Using arrow operator to access Conn

    q->SQL->Clear();
    q->SQL->Add("INSERT INTO UnikNmbrDgvr");
    q->SQL->Add("(plan_id, god, curUnikNmbrDgvr)");
    q->SQL->Add("VALUES (:plan_id, :god, 0)");
    q->ParamByName("plan_id")->AsInteger() = plan_id;
    q->ParamByName("god")->AsInteger() = god;
    q->ExecSQL();
    
    int result = TDB_UnikNmbrDgvr::GetId(); // GetId() function call
    delete q; // Don't forget to free the memory allocated for the object
    return result;
}

int TDB_UnikNmbrDgvr::GetNextUnikNmbr(int god, int plan_id) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);// Using arrow operator to access Conn

    int UnikNmbr = 0, curUnikNmbrDgvr = 0, undgvr_id = 0;

    q->SQL->Add("SELECT * FROM UnikNmbrDgvr WHERE plan_id = :plan_id AND god = :god");
    q->ParamByName("plan_id")->AsInteger() = plan_id;
    q->ParamByName("god")->AsInteger() = god;
    q->Open();
    
    if (!q->EoF()) {
        undgvr_id = q->FieldByName("undgvr_id")->AsInteger();
        curUnikNmbrDgvr = q->FieldByName("curUnikNmbrDgvr")->AsInteger();
        UnikNmbr = curUnikNmbrDgvr + 1;
    } else {
        undgvr_id = TDB_UnikNmbrDgvr::AddNewItem(god, plan_id);
        UnikNmbr = 1;
    }

    TDB_UnikNmbrDgvr::UpdItem(undgvr_id, UnikNmbr); // UpdItem() function call
    delete q; // Don't forget to free the memory allocated for the object
    return UnikNmbr;
}

int TDB_UnikNmbrDgvr::GetCurUnikNmbr(int god, int plan_id, int &aundgvr_id) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);// Using arrow operator to access Conn

    int UnikNmbr = 0, curUnikNmbrDgvr = 0, undgvr_id = 0;

    q->SQL->Add("SELECT * FROM UnikNmbrDgvr WHERE plan_id = :plan_id AND god = :god");
    q->ParamByName("plan_id")->AsInteger() = plan_id;
    q->ParamByName("god")->AsInteger() = god;
    q->Open();
    
    if (!q->EoF()) {
        undgvr_id = q->FieldByName("undgvr_id")->AsInteger();
        curUnikNmbrDgvr = q->FieldByName("curUnikNmbrDgvr")->AsInteger();
        UnikNmbr = curUnikNmbrDgvr;
    } else {
        undgvr_id = TDB_UnikNmbrDgvr::AddNewItem(god, plan_id);
        UnikNmbr = 0;
    }

    aundgvr_id = undgvr_id;
    delete q; // Don't forget to free the memory allocated for the object
    return UnikNmbr;
}

void TDB_UnikNmbrDgvr::UpdItem(int undgvr_id, int curUnikNmbrDgvr) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);// Using arrow operator to access Conn

    q->SQL->Add("UPDATE UnikNmbrDgvr SET curUnikNmbrDgvr = :curUnikNmbrDgvr WHERE undgvr_id = :undgvr_id");
    q->ParamByName("undgvr_id")->AsInteger() = undgvr_id;
    q->ParamByName("curUnikNmbrDgvr")->AsInteger() = curUnikNmbrDgvr;
    q->ExecSQL();

    delete q; // Don't forget to free the memory allocated for the object
}
