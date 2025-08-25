// DB_TraceConfig.h

#ifndef MWSRESTAPI_DB_TRACECONFIG_H
#define MWSRESTAPI_DB_TRACECONFIG_H

#include <string>
#include "json.hpp"
#include "DB_Common.h"

class TDB_TraceConfig {
public:
    static nlohmann::json GetTraceConfigAsJson();
};

#endif //MWSRESTAPI_DB_TRACECONFIG_H
