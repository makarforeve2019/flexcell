//
// Created by Alex on 30.04.2024.
//

#ifndef MWSRESTAPI_DB_SYSTSETTINGS_H
#define MWSRESTAPI_DB_SYSTSETTINGS_H

#include "DB_Common.h"
#include <string>

class TDB_SystSettings {
public:
    static std::string GetValyeByOboznac(std::string Oboznac);
    static std::string GetAppSpace();
};


#endif //MWSRESTAPI_DB_SYSTSETTINGS_H
