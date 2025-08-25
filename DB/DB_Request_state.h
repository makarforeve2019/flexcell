//
// Created by Alex on 05.11.2024.
//

#ifndef MWSRESTAPI_DB_REQUEST_STATE_H
#define MWSRESTAPI_DB_REQUEST_STATE_H
#include "DB_Common.h"

class TDB_Request_state {
public:
    static bool GetRequestStateOnXRequestId(std::string XRequestId);
    static void InsertRequestStateOnXRequestId(std::string XRequestId, std::string response, int response_code, int created_at);
    static void UpdateResponse(std::string XRequestId, const std::string newResponse);
    static void UpdateResponseCode(std::string XRequestId, int newResponseCode);
};


#endif //MWSRESTAPI_DB_REQUEST_STATE_H
