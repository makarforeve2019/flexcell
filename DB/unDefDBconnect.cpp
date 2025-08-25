//
// Created by Alex on 14.04.2024.
//

#include <string>

#include "unDefDBconnect.h"
using namespace std;

TDefDBconnect DefDBconnect;

bool TDefDBconnect::connectOperDB(string & msgError)
{
    /* Установить подключение к базе данных */
    Conn = PQconnectdb(PQConnString.c_str());
    /* Убедиться, что соединение с сервером установлено успешно */
    if (PQstatus(Conn) != CONNECTION_OK)
    {
        //fprintf(stderr, "Connection to database failed: %s", PQerrorMessage(conn));
        msgError = PQerrorMessage(Conn);
        //exit_nicely(Conn);
        return false;
    }
    else
    {
        return true;
    };
}

bool TDefDBconnect::connectMWSModulsDB(string &msgError)
{
    /* Установить подключение к базе данных */
    MWSModulsConn = PQconnectdb(MWSModulsPQConnString.c_str());
    /* Убедиться, что соединение с сервером установлено успешно */
    if (PQstatus(MWSModulsConn) != CONNECTION_OK)
    {
        msgError = PQerrorMessage(MWSModulsConn);
        //exit_nicely(MWSModulsConn);
        return false;
    }
    else
    {
        return true;
    }
}

bool TDefDBconnect::connectDocumDB(string &msgError)
{
    /* Установить подключение к базе данных */
    DocumDBConn = PQconnectdb(DocumDBString.c_str());
    /* Убедиться, что соединение с сервером установлено успешно */
    if (PQstatus(DocumDBConn) != CONNECTION_OK)
    {
        msgError = PQerrorMessage(DocumDBConn);
        //exit_nicely(DocumDBConn);
        return false;
    }
    else
    {
        return true;
    }
}

bool TDefDBconnect::isOperDBConnected() const {
    return PQstatus(Conn) == CONNECTION_OK;
}

bool TDefDBconnect::isMWSModulsDBConnected() const {
    return PQstatus(MWSModulsConn) == CONNECTION_OK;
}

bool TDefDBconnect::isDocumDBConnected() const {
    return PQstatus(DocumDBConn) == CONNECTION_OK;
}


void CloseDataSets(TPQConnection CustomConnection)
{
    PQfinish(CustomConnection);
}

void CloseConnDataSets()
{
    CloseDataSets(DefDBconnect.Conn);
}

void CloseMWSModulsConnDataSets() {
    PQfinish(DefDBconnect.MWSModulsConn);
}

void CloseDocumDBConnDataSets() {
    PQfinish(DefDBconnect.DocumDBConn);
}
