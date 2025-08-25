//
// Created by Alex on 05.11.2024.
//

#include "DB_Request_state.h"
#include "unDefDBconnect.h"

bool TDB_Request_state::GetRequestStateOnXRequestId(std::string XRequestId) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    q->SQL->Clear();
    q->SQL->Add("SELECT * FROM request_state");
    q->SQL->Add("WHERE xrequestid = :xrequestid ");
    q->ParamByName("xrequestid")->AsString() = XRequestId;
    q->Open();
    bool result = !q->EoF();
    delete q;
    return result;
}

void TDB_Request_state::InsertRequestStateOnXRequestId( std::string XRequestId, std::string response, int response_code, int created_at) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    q->SQL->Clear();
    q->SQL->Add("INSERT INTO request_state (xrequestid, response, response_code, created_at)");
    q->SQL->Add("VALUES (:xrequestid, :response, :response_code, :created_at)");

    q->ParamByName("xrequestid")->AsString() = XRequestId;
    q->ParamByName("response")->AsString() = response;
    q->ParamByName("response_code")->AsInteger() = response_code;
    q->ParamByName("created_at")->AsInteger() = created_at;

    q->ExecSQL();
    delete q;
}

void TDB_Request_state::UpdateResponse(std::string XRequestId, const string newResponse) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    q->SQL->Clear();
    q->SQL->Add("UPDATE request_state");
    q->SQL->Add("SET response = :response");
    q->SQL->Add("WHERE xrequestid = :xrequestid");
    q->ParamByName("response")->AsString() = newResponse;
    q->ParamByName("xrequestid")->AsString() = XRequestId;

    q->ExecSQL();
    delete q;
}

void TDB_Request_state::UpdateResponseCode(std::string XRequestId, int newResponseCode) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    q->SQL->Clear();
    q->SQL->Add("UPDATE request_state");
    q->SQL->Add("SET response_code = :response_code");
    q->SQL->Add("WHERE xrequestid = :xrequestid");
    q->ParamByName("response_code")->AsInteger() = newResponseCode;
    q->ParamByName("xrequestid")->AsString() = XRequestId;

    q->ExecSQL();
    delete q;
}

