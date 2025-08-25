//
//

#ifndef MWSRESTAPI_RS_FORMS_H
#define MWSRESTAPI_RS_FORMS_H

#include <string>
//#include "../system/system/ClassesUtils.h"
#include "../system/ClassesUtils.h"
#include "../DB/DB_Forms.h"
#include <../nlohmann/json.hpp>
#include "../DB/unDefDBconnect.h"

class Forms {
public:
    static std::string GetAllFormAlbum(int album_id, TDateTime byDate);
    static std::string Get1FormWithoutAlbum(int album_id, TDateTime byDate);
};


#endif //MWSRESTAPI_RS_FORMS_H
