//
// Created by Alex on 14.04.2024.
//

#ifndef MWSRESTAPI_UNDBREAD_H
#define MWSRESTAPI_UNDBREAD_H



#include <iostream>
#include <string>
#include <vector>

using namespace std;

struct Plan {
    int id;
    std::string name;
};

extern int ConnectDBAtStartProc;
extern int ConnectDBTimeout;
extern int ConnectDBAttempts;

class TDBread {
public:
    static int ConnectDB(string admUserName, string admUserPassword,
                         string aDatabaseName, string aServerName, string aPort, string aUserName_docum,
                         string aUserPassword_docum, string aDatabaseName_docum, string aServerName_docum,
                         string aPort_docum, bool aIsAnotherServer_docum, bool isDBpostgre_docum);
    static void ConnectDBdocum(string admUserName, string admUserPassword,
                               string aDatabaseName, string aServerName, string aPort, string aUserName_docum,
                               string aUserPassword_docum, string aDatabaseName_docum, string aServerName_docum,
                               string aPort_docum, bool aIsAnotherServer_docum, bool isDBpostgre_docum );


    static bool TestDBConnection(const std::string& admUserName, const std::string& admUserPassword,
                                 const std::string& aDatabaseName, const std::string& aServerName, const std::string& aPort,
                                 const std::string& aUserName_docum, const std::string& aUserPassword_docum,
                                 const std::string& aDatabaseName_docum, const std::string& aServerName_docum, const std::string& aPort_docum,
                                 bool aIsAnotherServer_docum, bool isDBpostgre_docum, std::string& errorMessage);

    static void DisconnectDB();

    static std::vector<Plan> ExecuteQuery(const std::string& query, std::string& MsgError);
};

#endif //MWSRESTAPI_UNDBREAD_H

