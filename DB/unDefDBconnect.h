//
// Created by Alex on 14.04.2024.
//

#ifndef MWSRESTAPI_UNDEFDBCONNECT_H
#define MWSRESTAPI_UNDEFDBCONNECT_H




#include <iostream>
#include <string>
#include <libpq-fe.h>

using namespace std;

typedef PGconn* TPQConnection;

struct TDefDBconnect {
public:
    TPQConnection Conn;
    std::string PQConnString;
    string DBName;

    TPQConnection MWSModulsConn;
    string MWSModulsPQConnString;
    string MWSDBName;

    TPQConnection DocumDBConn;
    string DocumDBString;
    string DocumDBAdoString;

    bool isDBpostgre_docum;
    bool iDocumDBOpen;

    bool connectOperDB(string& msgError);
    bool connectMWSModulsDB(string& msgError);
    bool connectDocumDB(string& msgError);

    // Добавляем метод проверки подключения
    bool isOperDBConnected() const;
    bool isMWSModulsDBConnected() const;
    bool isDocumDBConnected() const;
};

void CloseConnDataSets();
void CloseMWSModulsConnDataSets();
void CloseDocumDBConnDataSets();

extern TDefDBconnect DefDBconnect;


#endif //MWSRESTAPI_UNDEFDBCONNECT_H
