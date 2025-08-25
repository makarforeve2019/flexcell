// DB_TraceConfig.cpp

#include "DB_TraceConfig.h"
#include <exception>
#include <iostream>
#include "unDefDBconnect.h"


nlohmann::json TDB_TraceConfig::GetTraceConfigAsJson() {
    TQueryUni *q = new TQueryUni(DefDBconnect.Conn);
    nlohmann::json result = {
            {"tracelevel", nullptr},
            {"services", nlohmann::json::array()},
            {"classes", nlohmann::json::array()},
            {"perf_func", nullptr},
            {"userid", nullptr}
    };

    try {
        q->SQL->Clear();
        q->SQL->Add("SELECT property, value FROM traceconfig WHERE active = TRUE");
        q->Open();

        while (!q->EoF()) {
            std::string property = q->FieldByName("property")->AsString();
            std::string value = q->FieldByName("value")->AsString();

            if (property == "tracelevel") {
                result["tracelevel"] = value;
            } else if (property == "service") {
                result["services"].push_back(value);
            } else if (property == "class") {
                result["classes"].push_back(value);
            } else if (property == "perf_func") {
                result["perf_func"] = value == "1";
            } else if (property == "userid") {
                result["userid"] = value;
            } else if (property == "slot1_db") {
                result["slot1_db"] = value == "1";
            } else if (property == "slot1_userid") {
                result["slot1_userid"] = value;
            } else if (property == "slot1_perf_func") {
                result["slot1_perf_func"] = value == "1";
            } else if (property == "postsend") {
                result["postsend"] = value == "1";
            }

            q->Next();
        }

        q->Close();
    } catch (std::exception& e) {
        std::cerr << "Exception caught while fetching trace config: " << e.what() << std::endl;
    }

    delete q;
    return result;
}
