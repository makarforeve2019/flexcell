#include "DB_OKVED.h"
#include "unDefDBconnect.h"
#include "../SystFuncs/SystFuncsLogic.h"


int TDB_OKVED::GetId() {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);

    int result = 0;

    q->SQL->Clear();
    
    switch (GetDBType()) {
        case TDBType::MSSQL:
            q->SQL->Add("SELECT @@IDENTITY as okved_id");
            break;
        case TDBType::Postgre:
            q->SQL->Add("SELECT currval(pg_get_serial_sequence(''OKVED'', ''okved_id'')) as okved_id");
            break;
    }

    q->Open();
    
    if (!q->EoF()) {
        result = q->FieldByName("okved_id")->AsInteger();
    }

    delete q;

    return result;
}

int TDB_OKVED::AddItem(int ownerId, std::string okved_kod, std::string okved_name, int orderN, std::string Primecanie) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);

    int resultId = 0;

    q->SQL->Clear();
    q->SQL->Add("INSERT INTO OKVED");
    q->SQL->Add("(ownerId, okved_kod, okved_name, orderN, Primecanie)");
    q->SQL->Add("values (:ownerId, :okved_kod, :okved_name, :orderN, :Primecanie)");

    q->ParamByName("ownerId")->AsInteger() = ownerId;
    q->ParamByName("okved_kod")->AsString() = okved_kod;
    q->ParamByName("okved_name")->AsString() = okved_name;
    q->ParamByName("orderN")->AsInteger() = orderN;
    q->ParamByName("Primecanie")->AsString() = Primecanie;

    q->ExecSQL();

    delete q;

    resultId = TDB_OKVED::GetId();

    return resultId;
}

void TDB_OKVED::GetAllItemsOwner(TQueryUni* qIn, int ownerId) {
    qIn->SQL->Clear();
    qIn->SQL->Add("SELECT * FROM OKVED WHERE ownerId = :ownerId ORDER BY orderN");
    qIn->ParamByName("ownerId")->AsInteger() = ownerId;
    qIn->Open();
}

int TDB_OKVED::GetMaxOrderNbyOwner(int ownerId) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);

    int maxOrderN = 0;

    q->SQL->Clear();
    q->SQL->Add("SELECT MAX(orderN) as orderN FROM OKVED WHERE ownerId = :ownerId");
    q->ParamByName("ownerId")->AsInteger() = ownerId;
    q->Open();
    
    maxOrderN = q->FieldByName("orderN")->AsInteger();
    
    delete q;

    return maxOrderN;
}

void TDB_OKVED::GetItemsBySubstr(TQueryUni* qIn, std::string substr) {
    qIn->SQL->Clear();
    qIn->SQL->Add("SELECT * FROM OKVED WHERE okved_name LIKE :substr AND ownerId > 0");
    qIn->ParamByName("substr")->AsString() = substr;
    qIn->Open();
}

void TDB_OKVED::GetItemByCode(TQueryUni* qIn, std::string okved_kod) {
    qIn->SQL->Clear();
    qIn->SQL->Add("SELECT * FROM OKVED WHERE okved_kod = :okved_kod");
    qIn->ParamByName("okved_kod")->AsString() = okved_kod;
    qIn->Open();
}

void TDB_OKVED::GetItemsBySubstrInUzel(TQueryUni* qIn, std::string substr, std::string inUzel) {
    qIn->SQL->Clear();
    qIn->SQL->Add("SELECT * FROM OKVED WHERE okved_name LIKE :substr AND okved_kod LIKE :inUzel");
    qIn->ParamByName("substr")->AsString() = substr;
    qIn->ParamByName("inUzel")->AsString() = inUzel + "%";
    qIn->Open();
}

bool TDB_OKVED::isItemsOwner(int ownerId) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);

    bool result = false;

    q->SQL->Clear();
    switch (GetDBType()) {
        case TDBType::MSSQL:
            q->SQL->Add("SELECT TOP 1 ownerId FROM OKVED WHERE ownerId = :ownerId");
            break;
        case TDBType::Postgre:
            q->SQL->Add("SELECT ownerId FROM OKVED WHERE ownerId = :ownerId LIMIT 1");
            break;
    }
    q->ParamByName("ownerId")->AsInteger() = ownerId;
    q->Open();
    result = !q->EoF();
    
    delete q;

    return result;
}

bool TDB_OKVED::GetOKVED(TQueryUni* qIn, int okved_id) {
    bool result = true;
    qIn->SQL->Clear();
    qIn->SQL->Add("SELECT * FROM OKVED WHERE okved_id = :okved_id");
    qIn->ParamByName("okved_id")->AsInteger() = okved_id;
    qIn->Open();
    if (qIn->EoF() && okved_id > 0) {
        result = false;
    }
    return result;
}

std::string TDB_OKVED::GetOKVEDName(int okved_id) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    std::string result;
    if (TDB_OKVED::GetOKVED(q, okved_id)) {
        result = q->FieldByName("okved_name")->AsString();
    } else {
        result = "???";
    }
    delete q;
    return result;
}

std::string TDB_OKVED::GetOKVEDCode(int okved_id) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    std::string result;
    if (TDB_OKVED::GetOKVED(q, okved_id)) {
        result = q->FieldByName("okved_kod")->AsString();
    } else {
        result = "???";
    }
    delete q;
    return result;
}

std::string TDB_OKVED::GetPrimecanie(int okved_id) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    TDB_OKVED::GetOKVED(q, okved_id);
    std::string result = q->FieldByName("Primecanie")->AsString();
    delete q;
    return result;
}

int TDB_OKVED::GetOkvedIdByCode(std::string okved_kod) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    TDB_OKVED::GetItemByCode(q, okved_kod);
    int result = 0;
    if (!q->EoF()) {
        result = q->FieldByName("okved_id")->AsInteger();
    }
    delete q;
    return result;
}

void TDB_OKVED::UpdPrimecanie(int okved_id, std::string Primecanie) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    q->SQL->Clear();
    q->SQL->Add("UPDATE OKVED SET Primecanie = :Primecanie WHERE okved_id = :okved_id");
    q->ParamByName("okved_id")->AsInteger() = okved_id;
    q->ParamByName("Primecanie")->AsString() = Primecanie;
    q->ExecSQL();
    delete q;
}
