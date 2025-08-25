#include "unDefDBconnect.h"
//#include "SystFuncsLogic.h"
#include "DB_bkKBK.h"
#include "unDBType.h"
#include "SystFuncsLogic.h"

void TDB_bkKBK::AddKBK0(int kbk_id, int orderN, std::string nameKBK, std::string codeKBK) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    bool b;
    try {
        q->SQL->Clear();
        q->SQL->Add("SELECT kbk_id FROM bkKBK WHERE kbk_id = :kbk_id");
        q->ParamByName("kbk_id")->AsInteger() = kbk_id;
        q->Open();
        b = q->EoF();
        q->Close();
        if (b) {
            q->SQL->Clear();
            q->SQL->Add("INSERT INTO bkKBK (kbk_id, kbk_owner_id, orderN, nameKBK, codeKBK, beg_date, end_date, create_date) "
                        "VALUES (:kbk_id, 0, :orderN, :nameKBK, :codeKBK, :beg_date, :end_date, :create_date)");
            q->ParamByName("kbk_id")->AsInteger() = kbk_id;
            q->ParamByName("orderN")->AsInteger() = orderN;
            q->ParamByName("nameKBK")->AsString() = nameKBK;
            q->ParamByName("codeKBK")->AsString() = codeKBK;
            q->ParamByName("beg_date")->AsDateTime() = EncodeDate(2000, 1, 1);
            q->ParamByName("end_date")->AsDateTime() = EncodeDate(3000, 12, 31);
            q->ParamByName("create_date")->AsDateTime() = Date();
            q->ExecSQL();
        }
    }
    catch (std::exception& e) {
        // Обработка исключения
        std::cerr << "Exception caught: " << e.what() << std::endl;
    }
   //MAK q->Free();
delete q;
}

int TDB_bkKBK::AddKBK(int kbk_owner_id, int vid, std::string nameKBK, std::string codeKBK, int beg_God, int end_God) {
    int kbk_id, orderN;
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    try {
        kbk_id = GetKBK_id(vid);
        orderN = GetOrderN(vid, kbk_owner_id);
        q->SQL->Clear();
        q->SQL->Add("INSERT INTO bkKBK (kbk_id, kbk_owner_id, orderN, nameKBK, codeKBK, beg_date, end_date, create_date) "
                    "VALUES (:kbk_id, :kbk_owner_id, :orderN, :nameKBK, :codeKBK, :beg_date, :end_date, :create_date)");
        q->ParamByName("kbk_id")->AsInteger() = kbk_id;
        q->ParamByName("kbk_owner_id")->AsInteger() = kbk_owner_id;
        q->ParamByName("orderN")->AsInteger() = orderN;
        q->ParamByName("nameKBK")->AsString() = nameKBK;
        q->ParamByName("codeKBK")->AsString() = codeKBK;
        q->ParamByName("beg_date")->AsDateTime() = EncodeDate(beg_God, 1, 1);
        q->ParamByName("end_date")->AsDateTime() = EncodeDate(end_God, 12, 31);
        q->ParamByName("create_date")->AsDateTime() = Date();
        q->ExecSQL();
    }
catch (std::exception& e) {
        // Обработка исключения
        std::cerr << "Exception caught: " << e.what() << std::endl;
    }
   //MAK q->Free();
delete q;
    return kbk_id;
}

int TDB_bkKBK::GetKBK_id(int vid) {
    int result;
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    try {
        switch (vid) {
            case 0:
                result = 1;
                break;
            case 1:
                result = 1000000;
                break;
            case 2:
                result = 2000000;
                break;
            case 3:
                result = 3000000;
                break;
        }

        q->SQL->Clear();
        q->SQL->Add("SELECT MAX(kbk_id) as kbk_id FROM bkKBK");
        switch (vid) {
            case 0:
                q->SQL->Add("WHERE kbk_id >= 1 AND kbk_id <= 999999");
                break;
            case 1:
                q->SQL->Add("WHERE kbk_id >= 1000000 AND kbk_id <= 1999999");
                break;
            case 2:
                q->SQL->Add("WHERE kbk_id >= 2000000 AND kbk_id <= 2999999");
                break;
            case 3:
                q->SQL->Add("WHERE kbk_id >= 3000000 AND kbk_id <= 3999999");
                break;
        }
        q->Open();
        if (!q->EoF()) {
            int maxKbkId = q->FieldByName("kbk_id")->AsInteger();
            if (maxKbkId > 0)
                result = maxKbkId + 1;
        }
    }
catch (std::exception& e) {
        // Обработка исключения
        std::cerr << "Exception caught: " << e.what() << std::endl;
    }
   //MAK q->Free();
delete q;
    return result;
}

int TDB_bkKBK::GetOrderN(int vid, int kbk_owner_id) {
    int result = 1;
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    try {
        q->SQL->Clear();
        q->SQL->Add("SELECT MAX(orderN) as orderN FROM bkKBK WHERE kbk_owner_id = :kbk_owner_id");
        q->ParamByName("kbk_owner_id")->AsInteger() = kbk_owner_id;
        q->Open();
        if (!q->EoF()) {
            result = q->FieldByName("orderN")->AsInteger() + 1;
        }
    }
catch (std::exception& e) {
        // Обработка исключения
        std::cerr << "Exception caught: " << e.what() << std::endl;
    }
   //MAK q->Free();
delete q;
    return result;
}

int TDB_bkKBK::GetNextKBKid(int kbk_owner_id, int kbk_id) {
    bool isB = false;
    std::string codeKBK, ws;
    int result = 0;
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    try {
        q->SQL->Clear();
        q->SQL->Add("SELECT * FROM bkKBK WHERE kbk_owner_id = :kbk_owner_id ORDER BY orderN");
        q->ParamByName("kbk_owner_id")->AsInteger() = kbk_owner_id;
        q->Open();
        while (!q->EoF()) {
            if (isB) {
                codeKBK = q->FieldByName("codeKBK")->AsString();
                ws = codeKBK.substr(0, 2) + "00000";
                if (codeKBK == ws) {
                    result = q->FieldByName("kbk_id")->AsInteger();
                    break;
                }
            } else if (q->FieldByName("kbk_id")->AsInteger() == kbk_id) {
                isB = true;
            }
            q->Next();
        }
    }
catch (std::exception& e) {
        // Обработка исключения
        std::cerr << "Exception caught: " << e.what() << std::endl;
    }
   //MAK q->Free();
delete q;
    return result;
}

void TDB_bkKBK::GetSubAll(TQueryUni* qIn, int kbk_id) {
    qIn->SQL->Clear();
    qIn->SQL->Add("SELECT * FROM bkKBK WHERE kbk_owner_id = :kbk_id ORDER BY orderN");
    qIn->ParamByName("kbk_id")->AsInteger() = kbk_id;
    qIn->Open();
}

void TDB_bkKBK::GetSubAllByGod(TQueryUni* qIn, int kbk_id, int god) {
    qIn->SQL->Clear();
    qIn->SQL->Add("SELECT * FROM bkKBK WHERE kbk_owner_id = :kbk_id");
    switch (GetDBType()) {  //Нужно узнать надо ли под mssql
        case TDBType::MSSQL:
            qIn->SQL->Add("AND :Year BETWEEN YEAR(beg_date) AND YEAR(end_date)");
            break;
        case TDBType::Postgre:
            qIn->SQL->Add("AND :Year BETWEEN date_part('year', beg_date) AND date_part('year', end_date)");
            break;
    }
    qIn->SQL->Add("ORDER BY orderN");
    qIn->ParamByName("kbk_id")->AsInteger() = kbk_id;
    qIn->ParamByName("Year")->AsInteger() = god;
    qIn->Open();
}

void TDB_bkKBK::GetAllByIds(TQueryUni* qIn, int kbk_id0, int kbk_id1) {
    qIn->SQL->Clear();
    qIn->SQL->Add("SELECT * FROM bkKBK WHERE kbk_id >= :kbk_id0 AND kbk_id <= :kbk_id1 ORDER BY orderN");
    qIn->ParamByName("kbk_id0")->AsInteger() = kbk_id0;
    qIn->ParamByName("kbk_id1")->AsInteger() = kbk_id1;
    qIn->Open();
}

void TDB_bkKBK::GetKBKInfo(TQueryUni* qIn, int kbk_id) {
    qIn->SQL->Clear();
    qIn->SQL->Add("SELECT * FROM bkKBK WHERE kbk_id = :kbk_id");
    qIn->ParamByName("kbk_id")->AsInteger() = kbk_id;
    qIn->Open();
}

std::string TDB_bkKBK::GetCodeKBK(int kbk_id) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    try {
        GetKBKInfo(q, kbk_id);
        return q->FieldByName("codeKBK")->AsString();
    } 
catch (std::exception& e) {
        // Обработка исключения
        std::cerr << "Exception caught: " << e.what() << std::endl;
    }
   //MAK q->Free();
delete q;
}

std::string TDB_bkKBK::GetName(int kbk_id) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    try {
        GetKBKInfo(q, kbk_id);
        return q->FieldByName("nameKBK")->AsString();
    } 
catch (std::exception& e) {
        // Обработка исключения
        std::cerr << "Exception caught: " << e.what() << std::endl;
    }
   //MAK q->Free();
delete q;
}

int TDB_bkKBK::GetOwnerId(int kbk_id) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    try {
        GetKBKInfo(q, kbk_id);
        return q->FieldByName("kbk_owner_id")->AsInteger();
    } 
catch (std::exception& e) {
        // Обработка исключения
        std::cerr << "Exception caught: " << e.what() << std::endl;
    }
   //MAK q->Free();
delete q;
}

bool TDB_bkKBK::isOwnerId(int kbk_owner_id) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    try {
        q->SQL->Clear();
        q->SQL->Add("SELECT * FROM bkKBK WHERE kbk_owner_id = :kbk_owner_id");
        q->ParamByName("kbk_owner_id")->AsInteger() = kbk_owner_id;
        q->Open();
        return (!q->EoF());
    } 
catch (std::exception& e) {
        // Обработка исключения
        std::cerr << "Exception caught: " << e.what() << std::endl;
    }
   //MAK q->Free();
delete q;
}

std::string TDB_bkKBK::GetNameByCodeKBK(std::string codeKBK) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    std::string result;
    try {
        q->SQL->Clear();
        q->SQL->Add("SELECT * FROM bkKBK WHERE codeKBK = :codeKBK");
        q->ParamByName("codeKBK")->AsString() = codeKBK;
        q->Open();
        if (!q->EoF())
            result = q->FieldByName("nameKBK")->AsString();
        else
            result = "";
    } 
catch (std::exception& e) {
        // Обработка исключения
        std::cerr << "Exception caught: " << e.what() << std::endl;
    }
   //MAK q->Free();
delete q;
    return result;
}

int TDB_bkKBK::GetKBKidByCodeKBK(std::string codeKBK, int god) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    int result = 0;
    try {
        q->SQL->Clear();
        q->SQL->Add("SELECT * FROM bkKBK WHERE codeKBK = :codeKBK");
        switch (GetDBType()) {
            case TDBType::MSSQL:
                q->SQL->Add("AND :Year BETWEEN YEAR(beg_date) AND YEAR(end_date)");
                break;
            case TDBType::Postgre:
                q->SQL->Add("AND :Year BETWEEN date_part('year', beg_date) AND date_part('year', end_date)");
                break;
        }
        q->ParamByName("codeKBK")->AsString() = codeKBK;
        q->ParamByName("Year")->AsInteger() = god;
        q->Open();
        if (!q->EoF())
            result = q->FieldByName("kbk_id")->AsInteger();
    } 
catch (std::exception& e) {
        // Обработка исключения
        std::cerr << "Exception caught: " << e.what() << std::endl;
    }
   //MAK q->Free();
delete q;
    return result;
}

int TDB_bkKBK::GetKBKidByKOSGU(std::string codeKBK, int god) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    int result = 0;
    try {
        q->SQL->Clear();
        q->SQL->Add("SELECT * FROM bkKBK WHERE kbk_id >= 3000000 AND kbk_id <= 3999999 AND codeKBK = :codeKBK");
        switch (GetDBType()) {
            case TDBType::MSSQL:
                q->SQL->Add("AND :Year BETWEEN YEAR(beg_date) AND YEAR(end_date)");
                break;
            case TDBType::Postgre:
                q->SQL->Add("AND :Year BETWEEN date_part('year', beg_date) AND date_part('year', end_date)");
                break;
        }
         q->ParamByName("codeKBK")->AsString() = codeKBK;
        q->ParamByName("Year")->AsInteger() = god;
        q->Open();
        if (!q->EoF())
            result = q->FieldByName("kbk_id")->AsInteger();
    } 
catch (std::exception& e) {
        // Обработка исключения
        std::cerr << "Exception caught: " << e.what() << std::endl;
    }
   //MAK q->Free();
delete q;
    return result;
}

void TDB_bkKBK::ModKBK(int kbk_id, std::string nameKBK, std::string codeKBK, int beg_God, int end_God) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    try {
        q->SQL->Clear();
        q->SQL->Add("UPDATE bkKBK SET nameKBK=:nameKBK, codeKBK=:codeKBK, beg_date=:beg_date, end_date=:end_date WHERE kbk_id = :kbk_id");
        q->ParamByName("kbk_id")->AsInteger() = kbk_id;
        q->ParamByName("nameKBK")->AsString() = nameKBK;
        q->ParamByName("codeKBK")->AsString() = codeKBK;
        q->ParamByName("beg_date")->AsDateTime() = EncodeDate(beg_God, 1, 1);
        q->ParamByName("end_date")->AsDateTime() = EncodeDate(end_God, 12, 31);
        q->ExecSQL();
    } 
catch (std::exception& e) {
        // Обработка исключения
        std::cerr << "Exception caught: " << e.what() << std::endl;
    }
   //MAK q->Free();
delete q;
}

bool TDB_bkKBK::DelKBK(int kbk_id) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    bool Result = true;
    try {
        GetSubAll(q, kbk_id);
        if (!q->EoF()) {
           //MAK q->Free();
delete q;
            Result = false;
            return Result;
        }
        q->Close();
        q->SQL->Clear();
        q->SQL->Add("DELETE FROM bkKBK WHERE kbk_id = :kbk_id");
        q->ParamByName("kbk_id")->AsInteger() = kbk_id;
        q->ExecSQL();
    }
catch (std::exception& e) {
        // Обработка исключения
        std::cerr << "Exception caught: " << e.what() << std::endl;
    }
   //MAK q->Free();
delete q;
    return Result;
}

void TDB_bkKBK::UpdOrderN(int kbk_id_1, int kbk_id_2) {
    int orderN_1, orderN_2;
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    try {
        GetKBKInfo(q, kbk_id_1);
        orderN_1 = q->FieldByName("orderN")->AsInteger();
        GetKBKInfo(q, kbk_id_2);
        orderN_2 = q->FieldByName("orderN")->AsInteger();
        
        q->SQL->Clear();
        q->SQL->Add("UPDATE bkKBK SET orderN = :orderN_2 WHERE kbk_id = :kbk_id_1");
        q->ParamByName("orderN_2")->AsInteger() = orderN_2;
        q->ParamByName("kbk_id_1")->AsInteger() = kbk_id_1;
        q->ExecSQL();
        
        q->SQL->Clear();
        q->SQL->Add("UPDATE bkKBK SET orderN = :orderN_1 WHERE kbk_id = :kbk_id_2");
        q->ParamByName("orderN_1")->AsInteger() = orderN_1;
        q->ParamByName("kbk_id_2")->AsInteger() = kbk_id_2;
        q->ExecSQL();
    } 
catch (std::exception& e) {
        // Обработка исключения
        std::cerr << "Exception caught: " << e.what() << std::endl;
    }
   //MAK q->Free();
delete q;
}