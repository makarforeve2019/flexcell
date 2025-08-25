#ifndef MWSRESTAPI_RS_BUDGET_H
#define MWSRESTAPI_RS_BUDGET_H


#include <string>
#include <boost/beast/http.hpp>
#include "unODataStatements.h"

namespace http = boost::beast::http;

class Budget {
public:
    static std::string AcceptChanges(std::string JsonString, boost::beast::http::status& status);
    static std::string Proposals(int Year, int plan_id, int user_id, int sost, bool is_orderby, OrderBy proposals_orderby);
    static std::string getAllInfoZayavki(int grnlUpdBP_id, int& countSogl, std::string approvalData);
};


#endif //MWSRESTAPI_RS_BUDGET_H
