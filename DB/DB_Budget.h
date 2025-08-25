//
//

#ifndef MWSRESTAPI_DB_BUDGET_H
#define MWSRESTAPI_DB_BUDGET_H

#include <string>
#include "DB_Common.h"
#include "unODataStatements.h"

class TDB_Budget {
public:
    static bool GetProposalsAll(int Year, int plan_id, int user_id, int sost, bool is_orderby, OrderBy proposals_orderby, TQueryUni* qIn);
};


#endif //MWSRESTAPI_DB_BUDGET_H
