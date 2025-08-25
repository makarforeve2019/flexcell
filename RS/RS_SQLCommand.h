#ifndef MWSRESTAPI_RS_SQLCOMMAND_H
#define MWSRESTAPI_RS_SQLCOMMAND_H


#include <string>

class RS_SQLCommand {
public:
    static bool ExecSQL(std::string JSONString, std::string& errorMessage, std::string sinonimDB);
    static std::string Open(std::string JSONString, std::string& errorMessage, std::string sinonimDB);
};


#endif //MWSRESTAPI_RS_SQLCOMMAND_H
