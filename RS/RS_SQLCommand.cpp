
#include "RS_SQLCommand.h"
#include "ClassesUtils.h"
#include "json.hpp"
#include "unDefDBconnect.h"
#include "DB_Common.h"
#include "DB_Pokaztl.h"


using json = nlohmann::json;

bool RS_SQLCommand::ExecSQL(std::string JSONString, std::string& errorMessage, std::string sinonimDB) {
    json jsonObject;
    TQueryUni *q;
    bool result = false;

    // Подключаемся к базе данных
    if (sinonimDB == "") {
        q = new TQueryUni(DefDBconnect.Conn);
    } else if (sinonimDB == "docum" || sinonimDB == "Docum" ) {
        q = new TQueryUni(DefDBconnect.DocumDBConn);
    } else if (sinonimDB == "mwsmoduls" || sinonimDB == "MWSModuls" ) {
        q = new TQueryUni(DefDBconnect.MWSModulsConn);
    }

    try {
        jsonObject = json::parse(JSONString);
        if (jsonObject.is_null() || !jsonObject.contains("SQL")) {
            errorMessage = "Ошибка: неверный JSON объект или отсутствует SQL";
            return false;
        }

        std::string sqlText = jsonObject["SQL"];
        if (jsonObject.contains("Params")) {
            json paramsArray = jsonObject["Params"];

            // Сортируем параметры по длине имен в убывающем порядке
            std::vector<json> sortedParams(paramsArray.begin(), paramsArray.end());
            std::sort(sortedParams.begin(), sortedParams.end(), [](const json &a, const json &b) {
                return a["Name"].get<std::string>().size() > b["Name"].get<std::string>().size();
            });

            for (const auto& param : sortedParams) {
                std::string paramName = param["Name"];
                std::string placeholder = ":" + paramName;
                std::string value;

                // Проверяем, является ли значение null
                if (param["Value"].is_null()) {
                    value = "NULL";
                } else {
                    std::string paramType = param["Type"];

                    if (paramType == "ftInteger") {
                        value = std::to_string(param["Value"].get<int>());
                    } else if (paramType == "ftFloat") {
                        size_t pos = 0;
                        value = std::to_string(param["Value"].get<double>());
                        while ((pos = value.find(',', pos)) != std::string::npos) {
                            value[pos] = '.';
                            ++pos;
                        }
                    } else if (paramType == "ftString") {
                        std::string arg = param["Value"].get<std::string>();
                        size_t length = arg.length();
                        char* buffer = new char[2 * length + 1];
                        size_t escapedLength = PQescapeString(buffer, arg.c_str(), length);
                        std::string escapedString(buffer, escapedLength);
                        delete[] buffer;
                        value = "'" + escapedString + "'";
                    } else if (paramType == "ftDateTime") {
                        std::string ws = param["Value"].get<std::string>();
                        if (ws != "0") {
                            TDateTime dateTime = StrToDateTime(ws);
                            value = "'" + DateTimeToStr(dateTime) + "'";
                        } else {
                            value = "'1899-12-30'";
                        }
                    } else if (paramType == "ftLargeint") {
                        value = std::to_string(param["Value"].get<long long>());
                    } else if (paramType == "ftBoolean") {
                        value = param["Value"].get<bool>() ? "TRUE" : "FALSE";
                    }
                }

                // Замена плейсхолдера значением
                size_t start_pos = 0;
                while ((start_pos = sqlText.find(placeholder, start_pos)) != std::string::npos) {
                    sqlText.replace(start_pos, placeholder.length(), value);
                    start_pos += value.length();
                }
            }
        }

        q->SQL->Add(sqlText);
        result = q->ExecSQL();
        delete q;
    } catch (const std::exception& e) {
        errorMessage = "Ошибка парсинга JSON: " + std::string(e.what());
        std::cout << errorMessage << std::endl;
        TracerManager::Instance().AddSpanLog("Exception.RS_SQLCommand::ExecSQL", {{"errordesr", errorMessage}},
                                             TracerManager::LOG_LEVEL_ERROR);
        return false;
    }

    return result;
}

std::string RS_SQLCommand::Open(std::string JSONString, std::string& errorMessage, std::string sinonimDB) {
    json jsonObject;
    TQueryUni *q;
    std::string JSONStringRes;

    // Подключаемся к базе данных
    if (sinonimDB == "") {
        q = new TQueryUni(DefDBconnect.Conn);
    } else if (sinonimDB == "docum" || sinonimDB == "Docum") {
        q = new TQueryUni(DefDBconnect.DocumDBConn);
    } else if (sinonimDB == "mwsmoduls" || sinonimDB == "MWSModuls") {
        q = new TQueryUni(DefDBconnect.MWSModulsConn);
    }

    try {
//        // Вывод JSON строки для отладки
//        std::cout << "Parsing JSON string: " << JSONString << std::endl;

        jsonObject = json::parse(JSONString);

        if (jsonObject.is_null() || !jsonObject.contains("SQL")) {
            errorMessage = "Ошибка: неверный JSON объект или отсутствует SQL";
            return "";
        }
        json contextsArray;
        if (jsonObject.contains("Contexts")) {
            contextsArray = jsonObject["Contexts"];

            // Проверяем, является ли contextsArray массивом
            if (contextsArray.is_array()) {
                for (const auto& context : contextsArray) {
                    if (context.contains("Name") && context["Name"] == "TmpPokaztlTB") {
                        if (context.contains("Params")) {
                            json params = context["Params"];
                            if (params.contains("CurrYear") && params.contains("CastName")) {
                                int currYear = params["CurrYear"];
                                std::string castName = params["CastName"];
                                TDB_Pokaztl::CreateTmpPokaztlTB(currYear, castName);
                            } else {
                                errorMessage = "Ошибка: контекст TmpPokaztlTB не содержит необходимых параметров";
                                return "";
                            }
                        }
                    }
                }
            } else {
                errorMessage = "Ошибка: Contexts не является массивом";
                return "";
            }
        }

        std::string sqlText = jsonObject["SQL"];
        if (jsonObject.contains("Params")) {
            json paramsArray = jsonObject["Params"];

            // Сортируем параметры по длине имен в убывающем порядке
            std::vector<json> sortedParams(paramsArray.begin(), paramsArray.end());
            std::sort(sortedParams.begin(), sortedParams.end(), [](const json &a, const json &b) {
                return a["Name"].get<std::string>().size() > b["Name"].get<std::string>().size();
            });

            for (const auto& param : sortedParams) {
                std::string paramName = param["Name"];
                auto paramValue = param["Value"];
                std::string paramType = param["Type"];

                std::string placeholder = ":" + paramName;
                std::string value;

                if (paramType == "ftInteger") {
                    value = std::to_string(paramValue.get<int>());
                } else if (paramType == "ftFloat") {
                    size_t pos = 0;
                    value = std::to_string(paramValue.get<double>());
                    while ((pos = value.find(',', pos)) != std::string::npos) {
                        value[pos] = '.';
                        ++pos;
                    }
                } else if ((paramType == "ftString") || (paramType == "ftWideMemo")) {
                    std::string arg = paramValue.get<std::string>();
                    size_t length = arg.length();
                    char* buffer = new char[2 * length + 1];
                    size_t escapedLength = PQescapeString(buffer, arg.c_str(), length);
                    std::string escapedString(buffer, escapedLength);
                    delete[] buffer;
                    value = "'" + escapedString + "'";
                } else if (paramType == "ftDateTime") {
                    std::string ws = paramValue.get<std::string>();
                    if (ws != "0") {
                        try {
                            TDateTime dateTime = StrToDateTime(ws);
                            value = "'" + dateTime.ToString() + "'";
                        } catch (const std::exception& e) {
                            errorMessage = "Ошибка парсинга даты: " + std::string(e.what());
                            std::cout << errorMessage << std::endl;
                            TracerManager::Instance().AddSpanLog("Exception.RS_SQLCommand::Open", {{"errordesr", errorMessage}},
                                                                 TracerManager::LOG_LEVEL_ERROR);
                            return "";
                        }
                    } else {
                        value = "'1899-12-30'";
                    }
                } else if (paramType == "ftLargeint") {
                    value = std::to_string(paramValue.get<long long>());
                } else if (paramType == "ftBoolean") {
                    value = paramValue.get<bool>() ? "TRUE" : "FALSE";
                }

                size_t start_pos = 0;
                while ((start_pos = sqlText.find(placeholder, start_pos)) != std::string::npos) {
                    sqlText.replace(start_pos, placeholder.length(), value);
                    start_pos += value.length();
                }
            }
        }

        JSONStringRes = to_string(q->openQueryGetJson(sqlText));
        if (jsonObject.contains("Contexts")) {
            contextsArray = jsonObject["Contexts"];

            // Проверяем, является ли contextsArray массивом
            if (contextsArray.is_array()) {
                for (const auto& context : contextsArray) {
                    if (context.contains("Name") && context["Name"] == "TmpPokaztlTB") {
                        TDB_Pokaztl::DropTmpPokaztlTB();
                    }
                }
            } else {
                errorMessage = "Ошибка: Contexts не является массивом";
                return "";
            }
        }
        delete q;
    } catch (const std::exception& e) {
        errorMessage = "Ошибка парсинга JSON: " + std::string(e.what());
        std::cout << errorMessage << std::endl;
        TracerManager::Instance().AddSpanLog("Exception.RS_SQLCommand::Open", {{"errordesr", errorMessage}},
                                             TracerManager::LOG_LEVEL_ERROR);
        return "";
    }

    return JSONStringRes;
}
