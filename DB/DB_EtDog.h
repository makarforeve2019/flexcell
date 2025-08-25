
#ifndef DB_ETDOG_H
#define DB_ETDOG_H


#include "DB_Common.h"




class TDB_EtDog {
public:

    static int GetIdClassEtDog();

    static int GetField_id(const std::string& field_name);

    static int GetEtDogNacaloId();

    static int GetEtDogOkoncanieId();
};

#endif
