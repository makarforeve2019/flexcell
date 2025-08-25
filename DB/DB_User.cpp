
#include "DB_User.h"
#include "unDefDBconnect.h"
#include "DB_NMrpr.h" 
#include "../SystFuncs/SystFuncsLogic.h"
#include "../GlobalVars/dmGlobalVars.h"

bool TDB_User::RegistryUserADByPassword(int UserId, std::string aPassWord, int &LastYear, int &Active, bool isAD, TDateTime &ChangePassDate) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);

    q->SQL->Add("SELECT * FROM Users");
    q->SQL->Add("WHERE user_id = :UserId");
    q->SQL->Add("AND user_password = :user_password");
    q->ParamByName("UserId")->AsInteger() = UserId;
    q->ParamByName("user_password")->AsString() = aPassWord;
    q->Open();

    bool Result = !q->EoF();
    if (Result) {
        LastYear = q->FieldByName("LastYear")->AsInteger();
        Active = q->FieldByName("Active")->AsInteger();
        ChangePassDate = q->FieldByName("changepassdate")->AsDateTime();
    }
    
   //MAK q->Free();
delete q;
    return Result;
}

void TDB_User::ResetLockTime(int user_id) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    try { 
        q->SQL->Clear();
        q->SQL->Add("UPDATE Users");
        q->SQL->Add("SET lock_time = NULL, active = 0");
        q->SQL->Add("WHERE user_id = :user_id");
        q->SQL->Add("AND lock_time IS NOT NULL");
        q->ParamByName("user_id")->AsInteger() = user_id;
        q->ExecSQL();
        } catch (std::exception& e) {
        // Обработка исключения
        ////std::cerr << "Exception caught: " << e.what() << std::endl;
    }
   //MAK q->Free();
delete q;
}

void TDB_User::ResetLoginAttempts(int user_id) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    try {

        q->SQL->Clear();
        q->SQL->Add("UPDATE Users");
        q->SQL->Add("SET login_attempts = 0");
        q->SQL->Add("WHERE user_id = :user_id");
        q->ParamByName("user_id")->AsInteger() = user_id;
        q->ExecSQL();
	}catch (std::exception& e) {
        // Обработка исключения
        //std::cerr << "Exception caught: " << e.what() << std::endl;
    }
   //MAK q->Free();
delete q;
}

bool TDB_User::GetUserInfo(TQueryUni *qIn, std::string user_name, std::string user_password) {
    bool Result = false;

    qIn->SQL->Clear();
    qIn->SQL->Add("SELECT * FROM Users");
    qIn->SQL->Add("WHERE user_name = :user_name");
    qIn->SQL->Add("AND user_password = :user_password");
    qIn->ParamByName("user_name")->AsString() = user_name;
    qIn->ParamByName("user_password")->AsString() = user_password;
    qIn->Open();
    Result = qIn->EoF();
    return Result;
}

int TDB_User::GetId() {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    int result = 0;
    try {

        q->SQL->Clear();
        switch (GetDBType()) {
            case TDBType::MSSQL:
                q->SQL->Add("SELECT @@IDENTITY as user_id");
                break;
            case TDBType::Postgre:
                q->SQL->Add("SELECT currval(pg_get_serial_sequence('users', 'user_id')) as user_id");
                break;
        }
        q->Open();
        result = q->FieldByName("user_id")->AsInteger();
        q->Close();
	}   catch (std::exception& e) {
        // Обработка исключения
        //std::cerr << "Exception caught: " << e.what() << std::endl;
    }
   //MAK q->Free();
delete q;
    return result;
}

void TDB_User::GetAllUser(TQueryUni *qIn) {
        qIn->SQL->Clear();

        qIn->SQL->Add("SELECT * FROM Users");
        qIn->Open();
}

void TDB_User::GetUser(TQueryUni *qIn, int user_id) {
        qIn->SQL->Clear();

        qIn->SQL->Add("SELECT * FROM Users");
        qIn->SQL->Add("WHERE user_id = :user_id");
        qIn->ParamByName("user_id")->AsInteger() = user_id;
        qIn->Open();
}

std::string TDB_User::GetPasswordSysadm(std::string &user_name) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    std::string result;
    try {

        if (GlobalVars->PasswordSysadm == "") {
            q->SQL->Clear();
            q->SQL->Add("SELECT user_password, user_name FROM Users");
            q->SQL->Add("WHERE user_adm = 2"); // with administrator rights
            q->Open();
        if (!q->EoF()) {
            GlobalVars->PasswordSysadm = q->FieldByName("user_password")->AsString();
            GlobalVars->NameSysadm = q->FieldByName("user_name")->AsString();
        }

        }
        q->Close();
        user_name = GlobalVars->NameSysadm;
        result = GlobalVars->PasswordSysadm;
}catch (std::exception& e) {
        // Обработка исключения
        //std::cerr << "Exception caught: " << e.what() << std::endl;
    }
   //MAK q->Free();
delete q;
    return result;
}

std::string TDB_User::GetspisokRazd(int user_id) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    std::string result;
    try {

        result = "";
        q->SQL->Clear();
        q->SQL->Add("SELECT spisokRazd FROM Users");
        q->SQL->Add("WHERE user_id = :user_id");
        q->ParamByName("user_id")->AsInteger() = user_id;
        q->Open();
        if (!q->EoF()) {
            result = q->FieldByName("spisokRazd")->AsString();
        }
        q->Close();
}catch (std::exception& e) {
        // Обработка исключения
        //std::cerr << "Exception caught: " << e.what() << std::endl;
    }
   //MAK q->Free();
delete q;
    return result;
}

int TDB_User::GetUserAdm(int user_id) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    int result = 0;
    try {

        q->SQL->Clear();
        q->SQL->Add("SELECT user_adm FROM Users");
        q->SQL->Add("WHERE user_id = :user_id");
        q->ParamByName("user_id")->AsInteger() = user_id;
        q->Open();
        if (!q->EoF()) {
            result = q->FieldByName("user_adm")->AsInteger();
        }
        q->Close();
}catch (std::exception& e) {
        // Обработка исключения
        //std::cerr << "Exception caught: " << e.what() << std::endl;
    }
   //MAK q->Free();
delete q;
    return result;
}

std::string TDB_User::GetUserPassword(int user_id) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    std::string result = "";
    try {

        q->SQL->Clear();
        q->SQL->Add("SELECT user_password FROM Users");
        q->SQL->Add("WHERE user_id = :user_id");
        q->ParamByName("user_id")->AsInteger() = user_id;
        q->Open();
        if (!q->EoF()) {
            result = q->FieldByName("user_password")->AsString();
        }
        q->Close();
}catch (std::exception& e) {
        // Обработка исключения
        //std::cerr << "Exception caught: " << e.what() << std::endl;
    }
   //MAK q->Free();
delete q;
    return result;
}

std::string TDB_User::GetUserPlanName(std::string AUserName) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    std::string result = "";
    try {

        q->SQL->Clear();
        q->SQL->Add("SELECT p.plan_name FROM users as u");
        q->SQL->Add("INNER JOIN user_plan as up ON up.user_id = u.user_id");
        q->SQL->Add("INNER JOIN plans as p ON p.plan_id = up.plan_id");
        q->SQL->Add("WHERE u.user_name = :user_name");
        q->ParamByName("user_name")->AsString() = AUserName;
        q->Open();
        while (!q->EoF()) {
            result += q->FieldByName("plan_name")->AsString()+ "; ";
            q->Next();
        }
        q->Close();
}catch (std::exception& e) {
        // Обработка исключения
        //std::cerr << "Exception caught: " << e.what() << std::endl;
    }
   //MAK q->Free();
delete q;
    return result;
}

std::string TDB_User::GetUserFIOshort(int user_id) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    std::string result = "";
    try {

        q->SQL->Clear();
        q->SQL->Add("SELECT FIOshort FROM Users");
        q->SQL->Add("WHERE user_id = :user_id");
        q->ParamByName("user_id")->AsInteger() = user_id;
        q->Open();
        if (!q->EoF()) {
            result = q->FieldByName("FIOshort")->AsString().c_str();
        }
        q->Close();
}catch (std::exception& e) {
        // Обработка исключения
        //std::cerr << "Exception caught: " << e.what() << std::endl;
    }
   //MAK q->Free();
delete q;
    return result;
}

bool TDB_User::GetUserFamImjOtc(int user_id, std::string &Fam, std::string &Imj, std::string &Otc) {
    std::string FIO = TDB_User::GetUserFIOshort(user_id);
    if (FIO.empty()) {
        return false;
    }
    bool Result = true;
    Fam = ""; 
    Imj = ""; 
    Otc = "";
    std::replace(FIO.begin(), FIO.end(), '.', ' ');

    size_t nOffset = 0;
    size_t nPos = FIO.find(' ', nOffset);
    if (nPos == std::string::npos) {
        return false;
    }

    Fam = FIO.substr(nOffset, nPos - nOffset);
    Fam = Trim(Fam);

    nOffset = nPos + 1;
    nPos = FIO.find(' ', nOffset);
    if (nPos == std::string::npos) {
        return false;
    }
    Imj = FIO.substr(nOffset, nPos - nOffset);
    Imj = Trim(Imj);

    nOffset = nPos + 1;
    Otc = FIO.substr(nOffset);
    Otc = Trim(Otc);

    if (Imj.length() <= 1 || Otc.length() <= 1) {
        Result = false;
    }

    return Result;
}

void TDB_User::GetUserTlfns(int user_id, std::string &TELmestn, std::string &TELgorod, std::string &TELmobil,
                            std::string &FIOshort, std::string &dolgnost) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);

    q->SQL->Clear();
    q->SQL->Add("SELECT * FROM Users");
    q->SQL->Add("WHERE user_id = :user_id");
    q->ParamByName("user_id")->AsInteger();
    q->Open();
    TELmestn = q->FieldByName("TELmestn")->AsString();
    TELgorod = q->FieldByName("TELgorod")->AsString();
    TELmobil = q->FieldByName("TELmobil")->AsString();
    FIOshort = q->FieldByName("FIOshort")->AsString();
    dolgnost = q->FieldByName("dolgnost")->AsString();
    q->Close();
   //MAK q->Free();
delete q;
}

bool TDB_User::IsUserLocked(int user_id) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    bool Result = false;
    q->SQL->Clear();
    q->SQL->Add("SELECT active FROM Users");
    q->SQL->Add("WHERE user_id = :user_id");
    q->ParamByName("user_id")->AsInteger() = user_id;
    q->Open();
    Result = q->FieldByName("active")->AsInteger() == 1;
    q->Close();
   //MAK q->Free();
delete q;

    return Result;
}

void TDB_User::LockUser(int user_id, int LockDurationMinutes) {
    TDateTime LockTime = Now() + EncodeTime(0, LockDurationMinutes, 0, 0);
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    try {

        q->SQL->Clear();
        q->SQL->Add("UPDATE Users");
        q->SQL->Add("SET lock_time = :lock_time, active = :active");
        q->SQL->Add("WHERE user_id = :user_id");
        q->ParamByName("user_id")->AsInteger() = user_id;
        q->ParamByName("lock_time")->AsDateTime() = LockTime;
        q->ParamByName("active")->AsInteger() = 1;
        q->ExecSQL();
}catch (std::exception& e) {
        // Обработка исключения
        //std::cerr << "Exception caught: " << e.what() << std::endl;
    }
   //MAK q->Free();
delete q;
}

int TDB_User::GetUserId(std::string user_name) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    int Result = 0;
        q->SQL->Clear();
        q->SQL->Add("SELECT user_id FROM Users");
        q->SQL->Add("WHERE user_name = :user_name");
        q->ParamByName("user_name")->AsString() = user_name;
        q->Open();
        if (!q->EoF()) {
            Result = q->FieldByName("user_id")->AsInteger();
        }

        q->Close();
       //MAK q->Free();
delete q;

    return Result;
}

std::string TDB_User::GetUserName(int user_id) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    std::string Result = "";
    try {

        q->SQL->Clear();
        q->SQL->Add("SELECT user_name FROM Users");
        q->SQL->Add("WHERE user_id = :user_id");
        q->ParamByName("user_id")->AsInteger() = user_id;
        q->Open();
        if (!q->EoF()) {
            Result = q->FieldByName("user_name")->AsString();
        }
}catch (std::exception& e) {
        // Обработка исключения
        //std::cerr << "Exception caught: " << e.what() << std::endl;
    }
   //MAK q->Free();
delete q;
    return Result;
}

int TDB_User::GetIsECP(int user_id) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    int Result = 0;
    try {

        q->SQL->Clear();
        q->SQL->Add("SELECT isECP FROM Users");
        q->SQL->Add("WHERE user_id = :user_id");
        q->ParamByName("user_id")->AsInteger() = user_id;
        q->Open();
        if (!q->EoF()) {
            Result = q->FieldByName("isECP")->AsInteger();
        }
}catch (std::exception& e) {
        // Обработка исключения
        //std::cerr << "Exception caught: " << e.what() << std::endl;
    }
   //MAK q->Free();
delete q;
    return Result;
}

TDateTime TDB_User::GetLockTime(int user_id) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    TDateTime Result = 0;


    q->SQL->Clear();
    q->SQL->Add("SELECT lock_time FROM Users");
    q->SQL->Add("WHERE user_id = :user_id");
    q->ParamByName("user_id")->AsInteger() = user_id;
    q->Open();
    if (!q->EoF()) {
        Result = q->FieldByName("lock_time")->AsDateTime();
    }

    q->Close();
   //MAK q->Free();
delete q;

    return Result;
}

int TDB_User::GetLoginAttempts(int user_id) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    int Result = 0;

        q->SQL->Clear();
        q->SQL->Add("SELECT login_attempts FROM Users");
        q->SQL->Add("WHERE user_id = :user_id");
        q->ParamByName("user_id")->AsInteger() = user_id;
        q->Open();
        if (!q->EoF()) {
            Result = q->FieldByName("login_attempts")->AsInteger();
        }
    q->Close();
   //MAK q->Free();
delete q;
    return Result;
}

int TDB_User::AddUser(std::string user_name, std::string user_password, std::string user_comm, int user_adm, std::string spisokRazd,
                      std::string FIOshort, std::string TELmestn, std::string TELgorod, std::string TELmobil, std::string signal_codes,
                      std::string email) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    int Result = 0;

        q->SQL->Clear();
        q->SQL->Add("INSERT INTO Users");
        q->SQL->Add("(user_name, user_password, user_comm, user_adm, spisokRazd, FIOshort, TELmestn, TELgorod, TELmobil, signal_codes, email)");
        q->SQL->Add("VALUES (:user_name, :user_password, :user_comm, :user_adm, :spisokRazd, :FIOshort, :TELmestn, :TELgorod, :TELmobil, :signal_codes, :email)");
        q->ParamByName("user_name")->AsString() = user_name;
        q->ParamByName("user_password")->AsString() = user_password;
        q->ParamByName("user_comm")->AsString() = user_comm;
        q->ParamByName("user_adm")->AsInteger() = user_adm;
        q->ParamByName("spisokRazd")->AsString() = spisokRazd;
        q->ParamByName("FIOshort")->AsString() = FIOshort;
        q->ParamByName("TELmestn")->AsString() = TELmestn;
        q->ParamByName("TELgorod")->AsString() = TELgorod;
        q->ParamByName("TELmobil")->AsString() = TELmobil;
        q->ParamByName("signal_codes")->AsString() = signal_codes;
        q->ParamByName("email")->AsString() = email;
        q->ExecSQL();
        Result = TDB_User::GetId();
       //MAK q->Free();
delete q;
    return Result;
}

void TDB_User::UpdUser1(int user_id, std::string user_comm, std::string spisokRazd, std::string FIOshort,
                         std::string TELmestn, std::string TELgorod, std::string TELmobil, std::string signal_codes, std::string email) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);

    q->SQL->Clear();
    q->SQL->Add("UPDATE Users");
    q->SQL->Add("SET user_comm = :user_comm, spisokRazd = :spisokRazd, FIOshort = :FIOshort, TELmestn = :TELmestn, ");
    q->SQL->Add("TELgorod = :TELgorod, TELmobil = :TELmobil, signal_codes = :signal_codes, email = :email");
    q->SQL->Add("WHERE user_id = :user_id");
    q->ParamByName("user_id")->AsInteger() = user_id;
    q->ParamByName("user_comm")->AsString()= user_comm;
    q->ParamByName("spisokRazd")->AsString() = spisokRazd;
    q->ParamByName("FIOshort")->AsString() = FIOshort;
    q->ParamByName("TELmestn")->AsString() = TELmestn;
    q->ParamByName("TELgorod")->AsString() = TELgorod;
    q->ParamByName("TELmobil")->AsString() = TELmobil;
    q->ParamByName("signal_codes")->AsString() = signal_codes;
    q->ParamByName("email")->AsString() = email;
    q->ExecSQL();
   //MAK q->Free();
delete q;
}

void TDB_User::UpdUser2(int user_id, int user_adm) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    try {

        q->SQL->Clear();
        q->SQL->Add("UPDATE Users");
        q->SQL->Add("SET user_adm = :user_adm");
        q->SQL->Add("WHERE user_id = :user_id");
        q->ParamByName("user_id")->AsInteger() = user_id;
        q->ParamByName("user_adm")->AsInteger() = user_adm;
        q->ExecSQL();
}catch (std::exception& e) {
        // Обработка исключения
        //std::cerr << "Exception caught: " << e.what() << std::endl;
    }
   //MAK q->Free();
delete q;
}

void TDB_User::UpdUser3(int user_id, std::string user_password) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    try {

        q->SQL->Clear();
        q->SQL->Add("UPDATE Users");
        q->SQL->Add("SET user_password = :user_password");
        q->SQL->Add("WHERE user_id = :user_id");
        q->ParamByName("user_id")->AsInteger() = user_id;
        q->ParamByName("user_password")->AsString() = user_password;
        q->ExecSQL();
}catch (std::exception& e) {
        // Обработка исключения
        //std::cerr << "Exception caught: " << e.what() << std::endl;
    }
   //MAK q->Free();
delete q;
}

void TDB_User::UpdUser4(int user_id, int LastYear) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    try {

        q->SQL->Clear();
        q->SQL->Add("UPDATE Users");
        q->SQL->Add("SET LastYear = :LastYear");
        q->SQL->Add("WHERE user_id = :user_id");
        q->ParamByName("user_id")->AsInteger() = user_id;
        q->ParamByName("LastYear")->AsInteger() = LastYear;
        q->ExecSQL();
}catch (std::exception& e) {
        // Обработка исключения
        //std::cerr << "Exception caught: " << e.what() << std::endl;
    }
   //MAK q->Free();
delete q;
}

void TDB_User::UpdIsECP(int user_id, int isECP) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    try {

        q->SQL->Clear();
        q->SQL->Add("UPDATE Users");
        q->SQL->Add("SET isECP = :isECP");
        q->SQL->Add("WHERE user_id = :user_id");
        q->ParamByName("user_id")->AsInteger() = user_id;
        q->ParamByName("isECP")->AsInteger() = isECP;
        q->ExecSQL();
    }
    catch (std::exception& e) {
       //MAK q->Free();
delete q;
    }
}

void TDB_User::UpdIsGRBSDefaultOwner(int user_id, int isGRBSDefaultOwner) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    try {

        q->SQL->Clear();
        if (isGRBSDefaultOwner == 1) {
            q->SQL->Add("UPDATE Users");
            q->SQL->Add("SET isGRBSDefaultOwner = 0");
            q->ExecSQL();
        }
        q->SQL->Clear();
        q->SQL->Add("UPDATE Users");
        q->SQL->Add("SET isGRBSDefaultOwner = :isGRBSDefaultOwner");
        q->SQL->Add("WHERE user_id = :user_id");
        q->ParamByName("user_id")->AsInteger() = user_id;
        q->ParamByName("isGRBSDefaultOwner")->AsInteger() = isGRBSDefaultOwner;
        q->ExecSQL();
    }
    catch (std::exception& e) {
       //MAK q->Free();
delete q;
    }
}

void TDB_User::UpdPassword(int user_id, std::string Password) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    try {

        q->SQL->Clear();
        q->SQL->Add("UPDATE Users");
        q->SQL->Add("SET user_password = :user_password");
        q->SQL->Add("WHERE user_id = :user_id");
        q->ParamByName("user_id")->AsInteger() = user_id;
        q->ParamByName("user_password")->AsString() = Password;
        q->ExecSQL();
    }
    catch (std::exception& e) {
       //MAK q->Free();
delete q;
    }
}

void TDB_User::UpdateLoginAttempts(int user_id, int LoginAttempts) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    try {

        q->SQL->Clear();
        q->SQL->Add("UPDATE Users");
        q->SQL->Add("SET login_attempts = :login_attempts");
        q->SQL->Add("WHERE user_id = :user_id");
        q->ParamByName("user_id")->AsInteger() = user_id;
        q->ParamByName("login_attempts")->AsInteger() = LoginAttempts;
        q->ExecSQL();
    }
    catch (std::exception& e) {
       //MAK q->Free();
delete q;
    }
}

void TDB_User::UpdChangePassDate(int user_id, TDateTime value_date) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    try {

        q->SQL->Clear();
        q->SQL->Add("UPDATE Users");
        q->SQL->Add("SET changepassdate = :changepassdate");
        q->SQL->Add("WHERE user_id = :user_id");
        q->ParamByName("user_id")->AsInteger() = user_id;
        q->ParamByName("changepassdate")->AsDateTime() = value_date;
        q->ExecSQL();
    }
    catch (std::exception& e) {
       //MAK q->Free();
delete q;
    }
}

void TDB_User::UpdDolgnost(int user_id, std::string dolgnost) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    try {

        q->SQL->Clear();
        q->SQL->Add("UPDATE Users");
        q->SQL->Add("SET dolgnost = :dolgnost");
        q->SQL->Add("WHERE user_id = :user_id");
        q->ParamByName("user_id")->AsInteger() = user_id;
        q->ParamByName("dolgnost")->AsString() = dolgnost;
        q->ExecSQL();
    }
    catch (std::exception& e) {
       //MAK q->Free();
delete q;
    }
}

void TDB_User::DelUser(int user_id) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    try {

        q->SQL->Clear();
        q->SQL->Add("DELETE FROM Users");
        q->SQL->Add("WHERE user_id = :user_id");
        q->ParamByName("user_id")->AsInteger() = user_id;
        q->ExecSQL();
    }
    catch (std::exception& e) {
       //MAK q->Free();
delete q;
    }
}

void TDB_User::UserGrantall(std::string aLogin) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    try {

        // делаем доступ к таблицам, доступных всем
        for (int j = 1; j < tables_num; j++) {
            q->SQL->Clear();
            q->SQL->Add("USE " + TDB_NMrpr::GetCurDBName());
            // даем права доступа к таблицам
            q->SQL->Add("grant all on " + tables_all[j] + " to PUBLIC");
            q->ExecSQL();
        }
    }
    catch (std::exception& e) {
       //MAK q->Free();
delete q;
    }
}

int TDB_User::GetUserByProfileID(int profileid) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    int Result = 0;
    try {
        q->SQL->Clear();
        q->SQL->Add("SELECT user_id FROM Users");
        q->SQL->Add("WHERE profileid = :profileid");
        q->ParamByName("profileid")->AsString() = IntToStr(profileid);
        q->Open();
        if (!q->EoF()) {
            Result = q->FieldByName("user_id")->AsInteger();
        }
    }
    catch (std::exception& e) {
    }
   //MAK q->Free();
delete q;
    return Result;
}

bool TDB_User::SaveUserToken(int user_id, std::string accessToken, TDateTime expDate) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    bool Result = true;
    try {
        q->SQL->Clear();
        q->SQL->Add("UPDATE Users");
        q->SQL->Add("SET access_token = :access_token, exp_token = :exp_token ");
        q->SQL->Add("WHERE user_id = :user_id");
        q->ParamByName("user_id")->AsInteger() = user_id;
        q->ParamByName("access_token")->AsString() = accessToken;
        q->ParamByName("exp_token")->AsDateTime() = expDate;
        if (!q->ExecSQL()) Result = false;
    }
    catch (std::exception& e) {
        Result = false;
    }
   //MAK q->Free();
delete q;
    return Result;
}

bool TDB_User::GetUserTokenByProfileID(int profileid, std::string& accessToken, TDateTime& expDate) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    bool Result = true;
    try {
        q->SQL->Clear();
        q->SQL->Add("SELECT access_token, exp_token FROM Users");
        q->SQL->Add("WHERE profileid = :profileid");
        q->ParamByName("profileid")->AsString() = IntToStr(profileid);
        q->Open();
        if (!q->EoF()) {
            accessToken = q->FieldByName("access_token")->AsString();
            expDate = q->FieldByName("exp_token")->AsDateTime();
        }
        else {
            Result = false;
        };
    }
    catch (std::exception& e) {
        Result = false;
    }
   //MAK q->Free();
delete q;
    return Result;
}

bool TDB_User::GetUserProfileIDByToken(std::string accessToken, int& user_id, std::string& profileid, TDateTime& expDate) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    bool Result = true;
    try {
        q->SQL->Clear();
        q->SQL->Add("SELECT exp_token, user_id, profileid FROM Users");
        q->SQL->Add("WHERE access_token = :access_token");
        q->ParamByName("access_token")->AsString() = accessToken;
        q->Open();
        if (!q->EoF()) {
            user_id = q->FieldByName("user_id")->AsInteger();
            expDate = q->FieldByName("exp_token")->AsDateTime();
            profileid = q->FieldByName("profileid")->AsString();
        }
        else {
            Result = false;
        };
    }
    catch (std::exception& e) {
        Result = false;
    }
   //MAK q->Free();
delete q;
    return Result;
}
