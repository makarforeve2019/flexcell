//
//

#ifndef MWSRESTAPI_RS_CLASSOFPART_H
#define MWSRESTAPI_RS_CLASSOFPART_H

#include <string>
//#include "../system/system/ClassesUtils.h"
#include "../system/ClassesUtils.h"
#include "../DB/DB_ClassofPart.h"
#include <../nlohmann/json.hpp>
#include "../DB/unDefDBconnect.h"

class ClassofPart {
public:
    static std::string  GetClassConteiners(int class_id);
    static std::string  GetSubElems(int conteiner_id);
};

#endif //MWSRESTAPI_RS_CLASSOFPART_H
