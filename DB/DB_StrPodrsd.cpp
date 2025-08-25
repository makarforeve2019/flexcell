#include "DB_StrPodrsd.h"
#include "unDefDBconnect.h"
#include "../SystFuncs/SystFuncsLogic.h"


int TDB_StrPodrsd::GetId() {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);

    q->SQL->Clear();
    switch (GetDBType()) {
        case TDBType::MSSQL:
            q->SQL->Add("SELECT @@IDENTITY as strpodr_id");
            break;
        case TDBType::Postgre:
            q->SQL->Add("SELECT currval(pg_get_serial_sequence('StrPodrsd', 'strpodr_id')) as strpodr_id");
            break;
    }
    q->Open();
    int id = q->FieldByName("strpodr_id")->AsInteger();
    delete q;

    return id;
}

int TDB_StrPodrsd::AddStrPodrsd(int plan_id, std::string strpodrName, std::string strpodrCode) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);

    q->SQL->Clear();
    q->SQL->Add("INSERT INTO StrPodrsd");
    q->SQL->Add("(plan_id, strpodrName, strpodrCode)");
    q->SQL->Add("VALUES (:plan_id, :strpodrName, :strpodrCode)");
    q->ParamByName("plan_id")->AsInteger() = plan_id;
    q->ParamByName("strpodrName")->AsString() = strpodrName;
    q->ParamByName("strpodrCode")->AsString() = strpodrCode;
    q->ExecSQL();

    int id = GetId();
    delete q;

    return id;
}

void TDB_StrPodrsd::GetAllStrPodrsd(TQueryUni *qIn) {

    qIn->SQL->Clear();
    qIn->SQL->Add("SELECT * FROM StrPodrsd");
    qIn->Open();
}

bool TDB_StrPodrsd::GetStrPodrsdById(TQueryUni *qIn, int strpodr_id) {
    qIn->SQL->Clear();
    qIn->SQL->Add("SELECT * FROM StrPodrsd WHERE strpodr_id = :strpodr_id");
    qIn->ParamByName("strpodr_id")->AsInteger() = strpodr_id;
    qIn->Open();

    return !qIn->EoF();
}

int TDB_StrPodrsd::GetPlanId(int strpodr_id) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);

    if (!GetStrPodrsdById(q, strpodr_id)) {
        delete q;
        return -1;
    }

    int plan_id = q->FieldByName("plan_id")->AsInteger();
    delete q;

    return plan_id;
}

void TDB_StrPodrsd::UpdStrPodrsd(int strpodr_id, std::string strpodrName, std::string strpodrCode) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);

    q->SQL->Clear();
    q->SQL->Add("UPDATE StrPodrsd SET strpodrName=:strpodrName, strpodrCode=:strpodrCode WHERE strpodr_id = :strpodr_id");
    q->ParamByName("strpodr_id")->AsInteger() = strpodr_id;
    q->ParamByName("strpodrName")->AsString() = strpodrName;
    q->ParamByName("strpodrCode")->AsString() = strpodrCode;
    q->ExecSQL();
    delete q;
}

bool TDB_StrPodrsd::UnikCode(std::string strpodrCode) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    bool result = true;

    q->SQL->Clear();
    q->SQL->Add("SELECT strpodr_id FROM StrPodrsd WHERE strpodrCode = :strpodrCode");
    q->ParamByName("strpodrCode")->AsString() = strpodrCode;
    q->Open();

    if (!q->EoF())
        result = false;

    delete q;
    return result;
}

bool TDB_StrPodrsd::UnikCodeIzm(std::string strpodrCode, int strpodr_id) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    bool result = true;

    q->SQL->Clear();
    q->SQL->Add("SELECT strpodr_id FROM StrPodrsd WHERE strpodrCode = :strpodrCode AND strpodr_id <> :strpodr_id");
    q->ParamByName("strpodrCode")->AsString() = strpodrCode;
    q->ParamByName("strpodr_id")->AsInteger() = strpodr_id;
    q->Open();

    if (!q->EoF())
        result = false;

    delete q;
    return result;
}

void TDB_StrPodrsd::DelStrPodrsd(int strpodr_id) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);

    q->SQL->Clear();
    q->SQL->Add("DELETE FROM StrPodrsd WHERE strpodr_id = :strpodr_id");
    q->ParamByName("strpodr_id")->AsInteger() = strpodr_id;
    q->ExecSQL();
    delete q;
}

int TDB_StrPodrsd::GetStrPodrsdForUser(int user_id, std::string &strpodrName, std::string &strpodrCode) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    int result = 0;

    q->SQL->Clear();
    q->SQL->Add("SELECT * FROM StrPodrsd, Users WHERE user_id = :user_id AND StrPodrsd.strpodr_id = Users.strpodr_id");
    q->ParamByName("user_id")->AsInteger() = user_id;
    q->Open();

    if (!q->EoF()) {
        result = q->FieldByName("strpodr_id")->AsInteger();
        strpodrName = q->FieldByName("strpodrName")->AsString();
        strpodrCode = q->FieldByName("strpodrCode")->AsString();
    }

    delete q;
    return result;
}

void TDB_StrPodrsd::UpdStrPodrsdForUser(int user_id, int strpodr_id) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);

    q->SQL->Clear();
    q->SQL->Add("UPDATE Users SET strpodr_id=:strpodr_id WHERE user_id = :user_id");
    q->ParamByName("user_id")->AsInteger() = user_id;
    q->ParamByName("strpodr_id")->AsInteger() = strpodr_id;
    q->ExecSQL();
    delete q;
}
