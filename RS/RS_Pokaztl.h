//
//

#ifndef MWSRESTAPI_RS_POKAZTL_H
#define MWSRESTAPI_RS_POKAZTL_H

#include <string>
//#include "../system/system/ClassesUtils.h"
#include "../system/ClassesUtils.h"
#include "../DB/DB_Pokaztl.h"
#include <../nlohmann/json.hpp>
#include "../DB/unDefDBconnect.h"

class Pokaztl {
public:
    static std::string  GetPokaz(int pokaz_id);
    static std::string  GetRootPokaztl(int pokaz_id);
};

#endif //MWSRESTAPI_RS_POKAZTL_H
