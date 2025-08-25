#ifndef MWSRESTAPI_DB_DINIZM_H
#define MWSRESTAPI_DB_DINIZM_H


#include "DB_Common.h"

class TDB_Dinizm {
public:
    static void GetPokazDinizm(TQueryUni *qIn, int pokaz_id);
    static void GetAllGrad_dinizm(TQueryUni *qIn, int din_izm_owner_id);
};


#endif //MWSRESTAPI_DB_DINIZM_H
