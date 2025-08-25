#include "unDefDBconnect.h"
#include "../SystFuncs/SystFuncsLogic.h"
 
#include "DB_OKDP.h"

int TDB_OKDP::GetId() {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    int result = 0;

    q->SQL->Clear();
    switch (GetDBType()) {
        case TDBType::MSSQL:
            q->SQL->Add("SELECT @@IDENTITY as okdpId");
            break;
        case TDBType::Postgre:
            q->SQL->Add("SELECT currval(pg_get_serial_sequence('OKDP', 'okdpId')) as okdpId");
            break;
        // Handle other database types if needed
    }

    q->Open();
    if (!q->EoF()) {
        result = q->FieldByName("okdpId")->AsInteger();
    }
    delete q; // Free the allocated memory

    return result;
}

int TDB_OKDP::AddItem(int ownerId, const std::string& codeOKDP, const std::string& nameOKDP) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);

    q->SQL->Clear();
    q->SQL->Add("INSERT INTO OKDP (ownerId, codeOKDP, nameOKDP) VALUES (:ownerId, :codeOKDP, :nameOKDP)");
    q->ParamByName("ownerId")->AsInteger() = ownerId;
    q->ParamByName("codeOKDP")->AsString() = codeOKDP;
    q->ParamByName("nameOKDP")->AsString() = nameOKDP;
    q->ExecSQL();

    int result = GetId(); // Assuming GetId retrieves the last inserted ID
    delete q;
    return result;
}

void TDB_OKDP::GetAllItemsOwner(TQueryUni* qIn, int ownerId) {
    qIn->SQL->Clear();
    qIn->SQL->Add("SELECT * FROM OKDP WHERE ownerId = :ownerId");
    qIn->ParamByName("ownerId")->AsInteger() = ownerId;
    qIn->Open();
}

void TDB_OKDP::GetItemsBySubstr(TQueryUni* qIn, const std::string& substr) {
    qIn->SQL->Clear();
    qIn->SQL->Add("SELECT * FROM OKDP WHERE nameOKDP LIKE :substr");
    qIn->ParamByName("substr")->AsString() = substr;
    qIn->Open();
}

void TDB_OKDP::GetItemsByCode(TQueryUni* qIn, const std::string& Code) {
    qIn->SQL->Clear();
    qIn->SQL->Add("SELECT * FROM OKDP WHERE codeOKDP LIKE :code");
    qIn->ParamByName("code")->AsString() = Code;
    qIn->Open();
}

void TDB_OKDP::GetItemsBySubstrInUzel(TQueryUni* qIn, const std::string& substr, const std::string& inUzel) {
    qIn->SQL->Clear();
    qIn->SQL->Add("SELECT * FROM OKDP WHERE nameOKDP LIKE :substr AND codeOKDP LIKE :inUzel");
    qIn->ParamByName("substr")->AsString() = substr;
    qIn->ParamByName("inUzel")->AsString() = inUzel + "%";
    qIn->Open();
}

std::string TDB_OKDP::GetOKDPCode(int okdpId) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);

    if (TDB_OKDP::GetOKDP(q, okdpId)) {
        std::string result = q->FieldByName("codeOKDP")->AsString();
        delete q;
        return result;
    } else {
        delete q;
        return "???";
    }
}

bool TDB_OKDP::isItemsOwner(int ownerId) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    bool Result = false;

    q->SQL->Clear();
    switch (GetDBType()) {
        case TDBType::MSSQL: {
            q->SQL->Add("SELECT TOP 1 ownerId FROM OKDP WHERE ownerId = :ownerId");
            break;
        }
        case TDBType::Postgre: {
            q->SQL->Add("SELECT ownerId FROM OKDP WHERE ownerId = :ownerId LIMIT 1");
            break;
        }
    }
    q->ParamByName("ownerId")->AsInteger() = ownerId;
    q->Open();
    if (!q->EoF()) Result = true;

    delete q;
    return Result;
}

void TDB_OKDP::GetAllItems(TQueryUni* qIn) {
    qIn->SQL->Clear();
    qIn->SQL->Add("SELECT * FROM OKDP");
    qIn->Open();
}

bool TDB_OKDP::GetOKDP(TQueryUni* qIn, int okdpId) {
    bool Result = true;
    qIn->SQL->Clear();
    qIn->SQL->Add("SELECT * FROM OKDP WHERE okdpId = :okdpId");
    qIn->ParamByName("okdpId")->AsInteger() = okdpId;
    qIn->Open();
    if (qIn->EoF() && (okdpId > 0)) {
        Result = false;
    }
    return Result;
}

std::string TDB_OKDP::GetOKDPCodeName(int okdpId) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    std::string Result;
    if (TDB_OKDP::GetOKDP(q, okdpId)) {
        Result = q->FieldByName("codeOKDP")->AsString() + " " + q->FieldByName("nameOKDP")->AsString();
    }
    else {
        Result = "???";
    }
    delete q;
    return Result;
}

std::string TDB_OKDP::GetOKDPName(int okdpId) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    std::string Result;
    if (TDB_OKDP::GetOKDP(q, okdpId)) {
        Result = q->FieldByName("nameOKDP")->AsString();
    }
    else {
        Result = "???";
    }
    delete q;
    return Result;
}

