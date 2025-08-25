#include "../SystFuncs/SystFuncsLogic.h"
#include "DB_GurnProtokol.h"
#include "unDefDBconnect.h"

void TDB_GurnProtokol::Gurn_AddItem(std::string Operation, std::string UserLogin, int userId) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    q->SQL->Clear();
    q->SQL->Add("INSERT INTO GurnUserSesii");
    q->SQL->Add("(Operation, UserLogin, userId, createDate)");
    q->SQL->Add("VALUES (:Operation, :UserLogin, :userId, :createDate)");
    q->ParamByName("Operation")->AsString() = Operation;
    q->ParamByName("UserLogin")->AsString() = UserLogin;
    q->ParamByName("userId")->AsInteger() = userId;
    q->ParamByName("createDate")->AsDateTime() = Now();
    q->ExecSQL();
    delete q;
}

void TDB_GurnProtokol::Protokol_AddItem(std::string Operation, std::string UserLogin, int userId,
                                        int param_id, std::string paramName) {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    q->SQL->Clear();
    q->SQL->Add("INSERT INTO ProtokolUserWorks");
    q->SQL->Add("(Operation, UserLogin, userId, createDate, param_id, paramName)");
    q->SQL->Add("VALUES (:Operation, :UserLogin, :userId, :createDate, :param_id, :paramName)");
    q->ParamByName("Operation")->AsString() = Operation;
    q->ParamByName("UserLogin")->AsString() = UserLogin;
    q->ParamByName("userId")->AsInteger() = userId;
    q->ParamByName("createDate")->AsDateTime() = Now();
    q->ParamByName("param_id")->AsInteger() = param_id;
    q->ParamByName("paramName")->AsString() = paramName;
    q->ExecSQL();
    delete q;
}

void TDB_GurnProtokol::execProtokolAddItem(int aCell_id, int aPlan_id, int aVid_otc, TDateTime aDate,
                                           double aValue, int objectid, double aOldValue) {
    string s, s1;
    switch (aVid_otc) {
        case 1:
            s1 = "Дир-ое зад-ие";
            break;
        case 2:
            s1 = "Заявка-предлож.";
            break;
        case 3:
            s1 = "План";
            break;
        case 4:
            s1 = "Уточ. план";
            break;
        case 5:
            s1 = "Факт";
            break;
        case 6:
            s1 = "Норма";
            break;
        case 7:
            s1 = "Цел-й ор-р";
            break;
        case 8:
            s1 = "Порог-е зн-е";
            break;
        case 9:
            s1 = "Папка A";
            break;
        case 10:
            s1 = "Папка B";
            break;
        case 11:
            s1 = "Папка C";
            break;
        case 12:
            s1 = "Прогноз на год";
            break;
        case 13:
            s1 = "Прогноз среднес-й";
            break;
        case 14:
            s1 = "Прогноз долгос-й";
            break;
        default: {
            stringstream ss;
            ss << "vid" << static_cast<int>(aVid_otc);
            s1 = ss.str();
        }
    }

    time_t t = time(nullptr);
    struct tm *now = localtime(&t);

    if (now == nullptr) { // Проверяем, что указатель не равен nullptr
        throw std::runtime_error("localtime failed to convert time");
    }

    char dateStr[11];
    strftime(dateStr, sizeof(dateStr), "%Y-%m-%d", now);

    stringstream ss;
    ss << "plan_id=" << aPlan_id << ";objectid=" << objectid << ";cell_id=" << aCell_id << ";date=" << dateStr
       << ";value=" << aValue << ";Oldvalue=" << aOldValue;
    s = ss.str();

    Protokol_AddItem("Ввод для " + s1, GetUserLogin(), GetUserId(), 0, s);
}