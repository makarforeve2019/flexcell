#ifndef DB_GURNPROTOKOL_H
#define DB_GURNPROTOKOL_H

#include <string>
#include "DB_Common.h"

class TDB_GurnProtokol {
public:
    static void Gurn_AddItem(std::string Operation, std::string UserLogin, int userId);
    static void Protokol_AddItem(std::string Operation, std::string UserLogin, int userId,
        int param_id, std::string paramName);
    static void execProtokolAddItem(int aCell_id, int aPlan_id, int aVid_otc, TDateTime aDate,
        double aValue, int objectid, double aOldValue);
};

#endif // DB_GURNPROTOKOL_H
