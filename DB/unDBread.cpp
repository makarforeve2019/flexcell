//
// Created by Alex on 14.04.2024.
//

#include "unDBread.h"
#include "unDefDBconnect.h"
#include <thread>
#include <chrono>

using namespace std;

#if defined(_WIN32) || defined(_WIN64)
#define CALLSPEC __fastcall
#else
#define CALLSPEC // Для Linux это будет пустым определением
#endif

int ConnectDBAtStartProc = 0;
int ConnectDBTimeout = 10;
int ConnectDBAttempts = 2;

int TDBread::ConnectDB(string admUserName, string admUserPassword,
                       string aDatabaseName, string aServerName, string aPort, string aUserName_docum,
                       string aUserPassword_docum, string aDatabaseName_docum, string aServerName_docum,
                       string aPort_docum, bool aIsAnotherServer_docum, bool isDBpostgre_docum)
{
    string s;
    int result = 1;
    string MsgError;

    // Формирование строки подключения к центральной базе данных
    DefDBconnect.PQConnString = "host=" + aServerName +
                                " dbname=" + aDatabaseName +
                                " user=" + admUserName +
                                " password=" + admUserPassword +
                                " client_encoding=UTF8";

    if (!aPort.empty())
        DefDBconnect.PQConnString += " port=" + aPort;

    // Цикл для попыток подключения к центральной базе данных
    for (int attempt = 0; attempt < ConnectDBAttempts; ++attempt) {
        // Если соединение с центральной базой данных уже было открыто, закрываем его
        if (DefDBconnect.Conn != nullptr && PQstatus(DefDBconnect.Conn) == CONNECTION_OK)
            PQfinish(DefDBconnect.Conn);

        // Подключение к центральной базе данных
        DefDBconnect.DBName = aDatabaseName;
        DefDBconnect.Conn = PQconnectdb(DefDBconnect.PQConnString.c_str());

        if (DefDBconnect.Conn == nullptr || PQstatus(DefDBconnect.Conn) != CONNECTION_OK) {
            // Обработка ошибки подключения к центральной базе данных
            MsgError = "Связь с центральной базой данных не устанавливается. \n Информация об ошибке: " + string(PQerrorMessage(DefDBconnect.Conn));
            result = 1;
            if (attempt > 0) {
                std::cout << "   - Повторная попытка подключения к центральной базе данных, попытка #" << (attempt + 1) << std::endl;
            }
            // Если это не последняя попытка, ждем перед повтором
            if (attempt < ConnectDBAttempts) {
                std::this_thread::sleep_for(std::chrono::seconds(ConnectDBTimeout));
            }
        } else {
            // Успешное подключение к центральной базе данных
            result = 0;
            MsgError.clear(); // Сброс сообщения об ошибке
            break; // Выходим из цикла, если подключение удалось
        }
    }

    // Если подключение к центральной базе данных удалось, пробуем подключиться к базе данных документов
    if (result == 0) {
        TDBread::ConnectDBdocum(admUserName, admUserPassword, aDatabaseName, aServerName, aPort, aUserName_docum, aUserPassword_docum, aDatabaseName_docum, aServerName_docum, aPort_docum, aIsAnotherServer_docum, isDBpostgre_docum);
    }

    // Подготовка строки подключения к базе данных MWSModuls
    if (DefDBconnect.MWSModulsConn != nullptr && PQstatus(DefDBconnect.MWSModulsConn) == CONNECTION_OK)
        PQfinish(DefDBconnect.MWSModulsConn);

    DefDBconnect.MWSModulsPQConnString = "host=" + aServerName +
                                         " dbname=MWSModuls" +
                                         " user=" + admUserName +
                                         " password=" + admUserPassword +
                                         " client_encoding=UTF8";

    if (!aPort.empty())
        DefDBconnect.MWSModulsPQConnString += " port=" + aPort;

    // Цикл для попыток подключения к базе данных MWSModuls
    for (int attempt = 0; attempt < ConnectDBAttempts; ++attempt) {
        DefDBconnect.MWSModulsConn = PQconnectdb(DefDBconnect.MWSModulsPQConnString.c_str());

        if (DefDBconnect.MWSModulsConn == nullptr || PQstatus(DefDBconnect.MWSModulsConn) != CONNECTION_OK) {
            // Обработка ошибки подключения к базе данных MWSModuls
            MsgError = "Связь с базой данных MWSModuls не устанавливается. \n Информация об ошибке: " + string(PQerrorMessage(DefDBconnect.MWSModulsConn));
            result = 1;
            if (attempt > 0) {
                std::cout << "   - Повторная попытка подключения к MWSModuls, попытка #" << (attempt + 1) << std::endl;
            }
            // Если это не последняя попытка, ждем перед повтором
            if (attempt < ConnectDBAttempts) {
                std::this_thread::sleep_for(std::chrono::seconds(ConnectDBTimeout));
            }
        } else {
            // Успешное подключение к базе данных MWSModuls
            result = 0;
            MsgError.clear(); // Сброс сообщения об ошибке
            break; // Выходим из цикла, если подключение удалось
        }
    }

    return result;
}




void CALLSPEC TDBread::ConnectDBdocum(string admUserName, string admUserPassword,
                                      string aDatabaseName, string aServerName, string aPort, string aUserName_docum,
                                      string aUserPassword_docum, string aDatabaseName_docum, string aServerName_docum,
                                      string aPort_docum, bool aIsAnotherServer_docum, bool isDBpostgre_docum)
{
    string ServerName, DatabaseName, UserName, UserPassword, Port, s;
    string MsgError;

    // Подключение базы *_docum
    if (aIsAnotherServer_docum) {
        ServerName = aServerName_docum;
        DatabaseName = aDatabaseName_docum;
        UserName = aUserName_docum;
        UserPassword = aUserPassword_docum;
        Port = aPort_docum;
    } else {
        ServerName = aServerName;
        DatabaseName = aDatabaseName_docum;
        UserName = admUserName;
        UserPassword = admUserPassword;
        Port = aPort;
        isDBpostgre_docum = true;
    }

    DefDBconnect.isDBpostgre_docum = isDBpostgre_docum;

    // Формирование строки подключения к базе данных документов
    if (!isDBpostgre_docum) { // MSSQL
        s = ServerName;
        if (!Port.empty())
            s = s + "," + Port;
        DefDBconnect.DocumDBString = "DriverID=MSSQL;Server=" + s +
                                     ";Database=" + DatabaseName + ";User_Name=" + UserName + ";Password=" + UserPassword;
    } else { // PostgreSQL
        DefDBconnect.DocumDBString = "host=" + ServerName +
                                     " dbname=" + DatabaseName +
                                     " user=" + UserName +
                                     " password=" + UserPassword +
                                     " client_encoding=UTF8";
        if (!Port.empty())
            DefDBconnect.DocumDBString += " port=" + Port;
    }

    // Закрытие предыдущего соединения с базой данных документов
    if (DefDBconnect.DocumDBConn != nullptr && PQstatus(DefDBconnect.DocumDBConn) == CONNECTION_OK)
        PQfinish(DefDBconnect.DocumDBConn);

    // Цикл для попыток подключения к базе данных документов
    for (int attempt = 0; attempt < ConnectDBAttempts; ++attempt) {
        // Подключение к базе данных документов
        DefDBconnect.DBName = aDatabaseName_docum;
        DefDBconnect.DocumDBConn = PQconnectdb(DefDBconnect.DocumDBString.c_str());

        if (DefDBconnect.DocumDBConn != nullptr && PQstatus(DefDBconnect.DocumDBConn) == CONNECTION_OK) {
            // Успешное подключение
            DefDBconnect.iDocumDBOpen = true;
            MsgError.clear(); // Сброс сообщения об ошибке
            //std::cout << "   - Успешное подключение к базе данных Docum" << std::endl;
            break; // Выходим из цикла, если подключение удалось
        } else {
            // Ошибка при подключении
            DefDBconnect.iDocumDBOpen = false;
            MsgError = "Ошибка при подключении к базе данных документов. \n Информация об ошибке: " + string(PQerrorMessage(DefDBconnect.DocumDBConn));
            if (attempt > 0) {
                std::cout << "   - Повторная попытка подключения к базе данных документов, попытка #" << (attempt + 1) << std::endl;
            }
            // Если это не последняя попытка, ждем перед повтором
            if (attempt < ConnectDBAttempts) {
                std::this_thread::sleep_for(std::chrono::seconds(ConnectDBTimeout));
            } else {
                std::cerr << "   - Связь с базой данных Docum не устанавливается." << std::endl;
                std::cerr << MsgError << std::endl;
            }
        }
    }
}


std::vector<Plan> TDBread::ExecuteQuery(const std::string& query, string& MsgError)
{
    std::vector<Plan> plans;

    if (DefDBconnect.Conn != nullptr && PQstatus(DefDBconnect.Conn) == CONNECTION_OK)
    {
        PGresult* res = PQexec(DefDBconnect.Conn, query.c_str());

        if (PQresultStatus(res) != PGRES_TUPLES_OK)
        {
            MsgError = PQerrorMessage(DefDBconnect.Conn);
        }
        else
        {
            int rows = PQntuples(res);
            for (int i = 0; i < rows; i++)
            {
                Plan plan;
                plan.id = std::stoi(PQgetvalue(res, i, 0)); // Предполагаем, что id - это первый столбец
                plan.name = PQgetvalue(res, i, 1); // Предполагаем, что name - это второй столбец
                plans.push_back(plan);
            }
        }
        PQclear(res);
    }
    else
    {
        MsgError = "Соединение с базой данных не установлено.";
    }

    return plans;
}

void TDBread::DisconnectDB() {
    // Закрытие соединения с центральной базой данных
    if (DefDBconnect.Conn != nullptr && PQstatus(DefDBconnect.Conn) == CONNECTION_OK) {
        PQfinish(DefDBconnect.Conn);
        DefDBconnect.Conn = nullptr;
        //std::cout << "   - Соединение с центральной базой данных закрыто." << std::endl;
    } else {
        //std::cout << "   - Соединение с центральной базой данных уже закрыто или не установлено." << std::endl;
    }

    // Закрытие соединения с базой данных документов
    if (DefDBconnect.DocumDBConn != nullptr && PQstatus(DefDBconnect.DocumDBConn) == CONNECTION_OK) {
        PQfinish(DefDBconnect.DocumDBConn);
        DefDBconnect.DocumDBConn = nullptr;
        //std::cout << "   - Соединение с базой данных Docum закрыто." << std::endl;
    } else {
        //std::cout << "   - Соединение с базой данных Docum уже закрыто или не установлено." << std::endl;
    }

    // Закрытие соединения с базой данных MWSModuls
    if (DefDBconnect.MWSModulsConn != nullptr && PQstatus(DefDBconnect.MWSModulsConn) == CONNECTION_OK) {
        PQfinish(DefDBconnect.MWSModulsConn);
        DefDBconnect.MWSModulsConn = nullptr;
        //std::cout << "   - Соединение с базой данных MWSModuls закрыто." << std::endl;
    } else {
        //std::cout << "   - Соединение с базой данных MWSModuls уже закрыто или не установлено." << std::endl;
    }
}

bool TDBread::TestDBConnection(const string &admUserName, const string &admUserPassword, const string &aDatabaseName,
                               const string &aServerName, const string &aPort, const string &aUserName_docum,
                               const string &aUserPassword_docum, const string &aDatabaseName_docum,
                               const string &aServerName_docum, const string &aPort_docum, bool aIsAnotherServer_docum,
                               bool isDBpostgre_docum, string &errorMessage) {
    bool isConnected = true;

    // Проверка подключения к центральной базе данных
    std::string centralDBConnString = "host=" + aServerName +
                                      " dbname=" + aDatabaseName +
                                      " user=" + admUserName +
                                      " password=" + admUserPassword +
                                      " client_encoding=UTF8";
    if (!aPort.empty())
        centralDBConnString += " port=" + aPort;

    PGconn* centralConn = PQconnectdb(centralDBConnString.c_str());
    if (centralConn == nullptr || PQstatus(centralConn) != CONNECTION_OK) {
        errorMessage = "Связь с центральной базой данных не устанавливается. \nИнформация об ошибке: " + string(PQerrorMessage(centralConn));
        std::cerr << errorMessage << std::endl;
        isConnected = false;
    } else {
        std::cout << "   - Успешное подключение к центральной базе данных" << std::endl;
    }
    PQfinish(centralConn);

    // Проверка подключения к базе данных документов
    std::string documDBConnString = "host=" + (aIsAnotherServer_docum ? aServerName_docum : aServerName) +
                                    " dbname=" + aDatabaseName_docum +
                                    " user=" + (aIsAnotherServer_docum ? aUserName_docum : admUserName) +
                                    " password=" + (aIsAnotherServer_docum ? aUserPassword_docum : admUserPassword) +
                                    " client_encoding=UTF8";
    if (!aPort_docum.empty())
        documDBConnString += " port=" + (aIsAnotherServer_docum ? aPort_docum : aPort);

    PGconn* documConn = PQconnectdb(documDBConnString.c_str());
    if (documConn == nullptr || PQstatus(documConn) != CONNECTION_OK) {
        errorMessage = "Связь с базой данных документов не устанавливается. \nИнформация об ошибке: " + string(PQerrorMessage(documConn));
        std::cerr << errorMessage << std::endl;
        isConnected = false;
    } else {
        std::cout << "   - Успешное подключение к базе данных документов" << std::endl;
    }
    PQfinish(documConn);

    // Проверка подключения к базе данных MWSModuls
    std::string mwsModulsConnString = "host=" + aServerName +
                                      " dbname=MWSModuls" +
                                      " user=" + admUserName +
                                      " password=" + admUserPassword +
                                      " client_encoding=UTF8";
    if (!aPort.empty())
        mwsModulsConnString += " port=" + aPort;

    PGconn* mwsModulsConn = PQconnectdb(mwsModulsConnString.c_str());
    if (mwsModulsConn == nullptr || PQstatus(mwsModulsConn) != CONNECTION_OK) {
        errorMessage = "Связь с базой данных MWSModuls не устанавливается. \nИнформация об ошибке: " + string(PQerrorMessage(mwsModulsConn));
        std::cerr << errorMessage << std::endl;
        isConnected = false;
    } else {
        std::cout << "   - Успешное подключение к базе данных MWSModuls" << std::endl;
    }
    PQfinish(mwsModulsConn);

    return isConnected;
}

