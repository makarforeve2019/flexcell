#include "unWebModuleUnit.h"
#include "RS/RS_UserCtgr.h"
#include "RS/RS_Album.h"
#include "RS/RS_Plan.h"
#include "RS/RS_Forms.h"
#include "RS/RS_Pokaztl.h"
#include "RS/RS_ClassofPart.h"
#include "RS/RS_Document.h"
#include "system/ClassesUtils.h"
#include "RS_Spreadsheet.h"
#include "unHttpRequestParser.h"
#include "unServerConst.h"
#include "RS_SQLCommand.h"
#include "RS_Auth.h"
#include "RS_N3Table.h"
#include "RS_Budget.h"
#include "TracerManager.h"
#include "DB_TraceConfig.h"
#include "dmGlobalVars.h"
#include "OtherServices/UserManager.h"

#include <string>
#include <vector>
#include <map>
#include <sstream>
#include <boost/algorithm/string.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/beast/http.hpp>
#include <sstream>
#include <iomanip>



int AuthOption = 0;
std::string AuthExternalType;
std::string AuthExternalServer;
std::string DefaultContext;
bool UpdateUserPermissions;
bool CreateUser;

int GlobalPort;   // !!!prt
void setGlobalPort(int port) {
    GlobalPort = port;
}
int getGlobalPort() {
    return GlobalPort;
}

// Функция для приведения строки к нижнему регистру
std::string toLower(const std::string& str) {
    std::string lowerStr = str;
    std::transform(lowerStr.begin(), lowerStr.end(), lowerStr.begin(), ::tolower);
    return lowerStr;
}

// Проверка токена на внешнем сервере в зависимости от опций авторизации
std::string checkExternalToken(std::string accessToken, std::string& authHeader, boost::beast::http::status& status) {
    std::string response;
    if (AuthOption == 1) {
        std::string AuthExternalServerLoc = AuthExternalServer + "auth/current-user-permissions";
        std::string content = Auth::CheckUserPermissions(AuthExternalServerLoc, accessToken, status);

        // Извлекаем profileid из ответа
        int profileId = 0;
        try {
            nlohmann::json jsonContent = nlohmann::json::parse(content);
            if (jsonContent.contains("profileid") && !jsonContent["profileid"].is_null()) {
                profileId = jsonContent["profileid"].get<int>();
                TracerManager::setTraceProfileId(profileId); // Устанавливаем profileid
            }
        } catch (const nlohmann::json::exception& e) {
            TracerManager::Instance().AddSpanLog("Ошибка парсинга profileid", {{"error", e.what()}},
                                                 TracerManager::LOG_LEVEL_ERROR);
        }

        if (status == http::status::see_other) {
            if (CreateUser == true) {
                UserManager userManager(endpointNg);
                int responseStatus = 0;
                auto createResponse = userManager.CreateUser(authHeaderGlob, responseStatus);

                if (responseStatus == 200) {
                    TracerManager::Instance().AddSpanLog(
                            "CreateUser Success",
                            {
                                    {"status", std::to_string(static_cast<int>(status))},
                                    {"responseStatus", std::to_string(responseStatus)},
                                    {"createResponse", createResponse.data.dump(4)}
                            },
                            TracerManager::LOG_LEVEL_INFO
                    );
                } else {
                    TracerManager::Instance().AddSpanLog(
                            "CreateUser Failed",
                            {
                                    {"status", std::to_string(static_cast<int>(status))},
                                    {"responseStatus", std::to_string(responseStatus)},
                                    {"createResponse", createResponse.data.dump(4)}
                            },
                            TracerManager::LOG_LEVEL_ERROR
                    );
                }
            } else {
                status = boost::beast::http::status::unauthorized;
            }
        }
        TracerManager::Instance().AddSpanLog("После CheckUserPermissions", {{"AuthOption", std::to_string(AuthOption)},
                                                                            {"response", content},
                                                                            {"status", std::to_string(static_cast<int>(status))}},
                                             (status == http::status::ok) ? TracerManager::LOG_LEVEL_INFO : TracerManager::LOG_LEVEL_ERROR);
        response = content;
    } else if (AuthOption == 2) {
        std::string AuthExternalServerLoc = AuthExternalServer + "auth/current-user-permissions";
        int exp;
        std::string contentErr = Auth::CheckUserPermissionsOnlyEXP(AuthExternalServerLoc, accessToken, status, exp);
        if (status != http::status::ok) {
            authHeader = "401 Unauthorized";
            contentErr = contentErr + " CheckUserPermissionsOnlyEXP 401";
            TracerManager::Instance().AddSpanLog("Ошибка CheckUserPermissionsOnlyEXP", {{"errordesr", contentErr},
                                                                                        {"accessToken", accessToken},
                                                                                        {"exp", std::to_string(exp)},
                                                                                        {"status", std::to_string(static_cast<int>(status))}},
                                                 TracerManager::LOG_LEVEL_ERROR);
            return contentErr;
        }

        AuthExternalServerLoc = AuthExternalServer + "auth/current-user/full";
        std::string UserFullJsonStr = Auth::currentUserFull(AuthExternalServerLoc, accessToken, status);

        // Извлекаем profileid из UserFullJsonStr
        int profileId = 0;
        try {
            nlohmann::json jsonContent = nlohmann::json::parse(UserFullJsonStr);
            if (jsonContent.contains("profileid") && !jsonContent["profileid"].is_null()) {
                profileId = jsonContent["profileid"].get<int>();
                TracerManager::setTraceProfileId(profileId); // Устанавливаем profileid
            }
        } catch (const nlohmann::json::exception& e) {
            TracerManager::Instance().AddSpanLog("Ошибка парсинга profileid", {{"error", e.what()}},
                                                 TracerManager::LOG_LEVEL_ERROR);
        }

        if (status != http::status::ok) {
            TracerManager::Instance().AddSpanLog("Ошибка currentUserFull", {{"UserFullJsonStr", UserFullJsonStr},
                                                                            {"accessToken", accessToken},
                                                                            {"status", std::to_string(static_cast<int>(status))}},
                                                 TracerManager::LOG_LEVEL_ERROR);
            return UserFullJsonStr;
        }

        std::string content = Auth::CheckUserProfilIdFromUserFull(UserFullJsonStr, status, exp, accessToken);
        if (status != http::status::ok) {
            if (status == http::status::see_other) {
                if (CreateUser == true) {
                    UserManager userManager(endpointNg);
                    int responseStatus = 0;
                    auto createResponse = userManager.CreateUser(authHeaderGlob, responseStatus);

                    if (responseStatus == 200) {
                        std::unordered_map<std::string, std::string> tags = {
                                {"status", std::to_string(static_cast<int>(status))},
                                {"responseStatus", std::to_string(responseStatus)},
                                {"createResponse", createResponse.data.dump(4)}
                        };
                        TracerManager::Instance().AddSpanLog(
                                "CreateUser Success",
                                tags,
                                TracerManager::LOG_LEVEL_INFO
                        );
                    } else {
                        std::unordered_map<std::string, std::string> tags = {
                                {"status", std::to_string(static_cast<int>(status))},
                                {"responseStatus", std::to_string(responseStatus)},
                                {"createResponse", createResponse.data.dump(4)}
                        };
                        TracerManager::Instance().AddSpanLog(
                                "CreateUser Failed",
                                tags,
                                TracerManager::LOG_LEVEL_ERROR
                        );
                        status = boost::beast::http::status::unauthorized;
                    }
                } else {
                    status = boost::beast::http::status::unauthorized;
                    TracerManager::Instance().AddSpanLog("Ошибка CheckUserProfilIdFromUserFull", {{"UserFullJsonStr", UserFullJsonStr},
                                                                                                  {"accessToken", accessToken},
                                                                                                  {"response", content},
                                                                                                  {"status", std::to_string(static_cast<int>(status))}},
                                                         TracerManager::LOG_LEVEL_ERROR);
                }
            } else {
                status = boost::beast::http::status::unauthorized;
            }
        }
        response = content;
    }
    return response;
}

std::string urlDecode(const std::string &value) {
    std::string result;
    char ch;
    int i, ii;
    for (i = 0; i < value.length(); i++) {
        if (int(value[i]) == 37) {
            sscanf(value.substr(i + 1, 2).c_str(), "%x", &ii);
            ch = static_cast<char>(ii);
            result += ch;
            i = i + 2;
        } else {
            result += value[i];
        }
    }
    return result;
}

// Функция для извлечения информации о сортировке
bool query_order(const std::string& query_string, OrderBy& orderby) {
    // Декодирование строки запроса
    std::string ws = urlDecode(query_string);
    std::string orderby_param = "$orderby=";

    // Поиск параметра $orderby=
    size_t orderby_pos = ws.find(orderby_param);
    if (orderby_pos != std::string::npos) {
        // Извлечение строки после $orderby=
        size_t start_pos = orderby_pos + orderby_param.length();
        std::string orderby_value = ws.substr(start_pos);

        // Поиск первого разделителя: пробел или плюс
        size_t space_pos = orderby_value.find(' ');
        if (space_pos == std::string::npos) {
            space_pos = orderby_value.find('+');
        }

        if (space_pos != std::string::npos) {
            // Если найден разделитель, разделяем на поле и порядок
            orderby.fieldname = orderby_value.substr(0, space_pos);
            orderby.order = orderby_value.substr(space_pos + 1);
        } else {
            // Если разделителя нет, то вся строка — это поле сортировки
            orderby.fieldname = orderby_value;
            orderby.order = "";
        }
        return true;
    }

    // Если параметр $orderby= отсутствует
    orderby.fieldname = "";
    orderby.order = "";
    return false;
}

std::map<std::string, std::string> parse_query_string(const std::string& query_string) {
    std::map<std::string, std::string> query_params;
    std::vector<std::string> pairs;
    boost::split(pairs, query_string, boost::is_any_of("&"));
    for (const auto& pair : pairs) {
        size_t eq_pos = pair.find('=');
        std::string key = pair.substr(0, eq_pos);
        std::string value = eq_pos != std::string::npos ? pair.substr(eq_pos + 1) : "";
        query_params[key] = value;
    }
    return query_params;
}

std::string extractToken(const std::string& authHeader) {
    std::string prefix = "Bearer ";
    std::string decodedHeader = urlDecode(authHeader);
    auto pos = decodedHeader.find(prefix);
    if (pos == 0) { // Убедитесь, что строка начинается с "Bearer "
        return decodedHeader.substr(prefix.length());
    }
    return ""; // Возвращаем пустую строку, если формат не совпадает
}


std::string handle_request(const std::string& body, const std::string& url, std::string& authHeader, boost::beast::http::status& status, http::response<http::string_body>& httpresponse, std::string adminPermissionHeader, std::string xRequestId, std::string xContextId) {
//    std::cout << "-------------------------------------------------------------------------------------------"<< endl;
    status = http::status::ok;
    std::string Requestcontent = body;
    std::string full_path = url;
    size_t question_mark_position = full_path.find('?');
    std::string path = full_path.substr(0, question_mark_position);
    std::string query_string = question_mark_position != std::string::npos ? full_path.substr(question_mark_position + 1) : "";
    std::map<std::string, std::string> query_params = parse_query_string(query_string);
    std::string accessToken = extractToken(authHeader);

    // Загружаем конфигурацию трассировки
    TracerManager::trace_config_json.clear();
    TracerManager::trace_config_json = TDB_TraceConfig::GetTraceConfigAsJson();
    std::string trace_config_json_string = TracerManager::trace_config_json.dump();
    // Инициализируем трассировку
    if (trace_config_json_string.empty()) {
        TracerManager::DisableTracing();
    } else {
        TracerManager::InitializeTracing(xRequestId);
        TracerManager::InitSlots();
        TracerManager::setTraceUserId(xContextId);
    }

    std::cout << "URI запроса:" << FromService+"->"+url << std::endl;

    std::istringstream iss(path);
    std::string part;
    std::vector<std::string> parts;
    // Читаем первую часть до первого слеша (или до конца строки, если слешей нет)
    std::getline(iss, part, '/');
    parts.push_back(part);  // Добавляем первую часть в вектор, даже если она пуста

    // Читаем остальные части между слешами
    while (std::getline(iss, part, '/')) {
        if (!part.empty()) {  // Игнорируем пустые строки, возникающие из-за двойных слешей
            parts.push_back(part);
        }
    }
    std::string response;

    if (parts.size() >= 3 && parts[1] == "actuator") {
        if (parts[2] == "liveness") {
            response = R"({"status": "UP"})";
            status = http::status::ok;
            httpresponse.body() = response;
            httpresponse.prepare_payload();
            return response;
        } else if (parts[2] == "readiness") {
            std::string message = "";
            bool allConnected = true;

            if (!DefDBconnect.isOperDBConnected()) {
                message += "Основная база данных не доступна. ";
                allConnected = false;
            }
            if (!DefDBconnect.isDocumDBConnected()) {
                message += "База данных DocumDB не доступна. ";
                allConnected = false;
            }

            if (!DefDBconnect.isMWSModulsDBConnected()) {
                message += "База данных MWSModuls не доступна. ";
                allConnected = false;
            }

            if (allConnected) {
                response = R"({"status": "OK", "message": null, "workMode": "ROUTINE"})";
                status = http::status::ok;
            } else {
                response = R"({"status": "Service Unavailable", "message": ")" + message + R"(", "workMode": "ROUTINE"})";
                status = http::status::service_unavailable;
            }

            httpresponse.body() = response;
            httpresponse.prepare_payload();
            return response;
        }
    }

    if (parts.size() >= 5) {
        std::string MWSRestAPI = parts[1];
        std::string UBPRosleshoz = parts[2];
        std::string className = parts[3];
        std::string methodName = parts[4];

        TracerManager::Instance().StartSpan(methodName);
        TracerManager::Instance().AddSpanTag("url", url);
        TracerManager::Instance().AddSpanTag("body", body);
        TracerManager::Instance().AddSpanTag("ServiceName", ServiceName);
        TracerManager::Instance().AddSpanDefaultTags();

        // Обработка метода
        size_t query_pos = methodName.find('?');
        if (query_pos != std::string::npos) {
            methodName = methodName.substr(0, query_pos);
        }

        try {
            if (MWSRestAPI == "MWSRestAPI" && UBPRosleshoz == "UBPRosleshoz") {
                if (className == "auth") {
                    TracerManager::Instance().CheckClassTrace("auth");
                    if (methodName == "get-user-context") {
                        TracerManager::Instance().StartServiceTrace("get-user-context");
                        if (AuthOption == 0) {
                            response = "{\"context_id\": " + DefaultContext + "}";
                        } else if ((AuthOption == 1) || (AuthOption == 2)) {
                            response = checkExternalToken(accessToken, authHeader, status);
                            std::cout << "get-user-context        " +  response +"    "+ std::to_string(static_cast<int>(status)) << std::endl;
                            TracerManager::Instance().AddSpanLog("После checkExternalToken", {{"AuthOption", std::to_string(AuthOption)},
                                                                                              {"response", response},
                                                                                              {"status", std::to_string(static_cast<int>(status))}},
                                                                 (status == http::status::ok) ? TracerManager::LOG_LEVEL_INFO : TracerManager::LOG_LEVEL_ERROR);
                            if ((status == http::status::ok) && (UpdateUserPermissions == true)){
//                                UserManager userManager(endpointNg);
                                UserManager userManager("ng");
                                int responseStatus = 0;
                                auto updateResponse = userManager.UpdateUserPermissions(authHeaderGlob, responseStatus);
                                if (responseStatus == 200) {
                                    TracerManager::Instance().AddSpanLog(
                                            "UpdateUserPermissions Success",
                                            {
                                                    {"responseStatus", std::to_string(responseStatus)},
                                                    {"responseBody", updateResponse.body},         // Сырое тело ответа от сервера
                                                    {"parsedResponse", updateResponse.data.dump(4)} // Распарсенный JSON
                                            },
                                            TracerManager::LOG_LEVEL_INFO
                                    );
                                } else {
                                    TracerManager::Instance().AddSpanLog(
                                            "UpdateUserPermissions Failed",
                                            {
                                                    {"responseStatus", std::to_string(responseStatus)},
                                                    {"responseBody", updateResponse.body},         // Сырое тело ответа от сервера (если есть)
                                                    {"parsedResponse", updateResponse.data.dump(4)} // Информация об ошибке или распарсенный JSON
                                            },
                                            TracerManager::LOG_LEVEL_ERROR
                                    );
                                    status = boost::beast::http::status::unauthorized;
                                }
                            }
                        }
                        TracerManager::Instance().EndServiceTrace();
                    } else if (methodName == "current-user-full") {
                        TracerManager::Instance().StartServiceTrace("current-user-full");
                        if (AuthOption == 1 || AuthOption == 2) {
                            std::string AuthExternalServerLoc = AuthExternalServer + "auth/current-user/full";
                            std::string content = Auth::currentUserFull(AuthExternalServerLoc, accessToken, status);
                            response = content;
                            TracerManager::Instance().AddSpanLog("После currentUserFull", {{"AuthExternalServerLoc", AuthExternalServerLoc},
                                                                                              {"response", content},
                                                                                              {"status", std::to_string(static_cast<int>(status))}},
                                                                 (status == http::status::ok) ? TracerManager::LOG_LEVEL_INFO : TracerManager::LOG_LEVEL_ERROR);
                        }
                        TracerManager::Instance().EndServiceTrace();
                    } else if (methodName == "check-authorization") {
                        TracerManager::Instance().StartServiceTrace("check-authorization");
                        if ((AuthOption == 1 || AuthOption == 2) && (accessToken != "AdminToolsToken")) {
                            TracerManager::Instance().AddSpanLog("Перед проверкой токена", {{"accessToken", accessToken}},
                                                                 TracerManager::LOG_LEVEL_INFO);
                            // Проверяем токен
                            std::string content = Auth::ValidateTokenLocallyByStoredToken(accessToken, status);
                            TracerManager::Instance().AddSpanLog("Результат валидации токена", {{"ValidateTokenStatus", std::to_string(static_cast<int>(status))}},
                                                                 (status == http::status::ok) ? TracerManager::LOG_LEVEL_INFO : TracerManager::LOG_LEVEL_ERROR);
                            if (status != http::status::ok) {
                                // Повторно проверяем токен
                                response = checkExternalToken(accessToken, authHeader, status);
                                TracerManager::Instance().AddSpanLog("Результат повторной валидации токена", {{"ExternalTokenStatus", std::to_string(static_cast<int>(status))},
                                                                                                              {"accessToken", accessToken},
                                                                                                              {"status", std::to_string(static_cast<int>(status))},
                                                                                                              {"response", response}},
                                                                     (status == http::status::ok) ? TracerManager::LOG_LEVEL_INFO : TracerManager::LOG_LEVEL_ERROR);
                            } else {
                                // Проверка прав администратора, если требуется
                                if (toLower(adminPermissionHeader) == "true") {
                                    bool res = Auth::checkUserAdm(accessToken);
                                    TracerManager::Instance().AddSpanLog("Проверка прав администратора", {{"isAdmin", res ? "true" : "false"}},
                                                                         (res) ? TracerManager::LOG_LEVEL_INFO : TracerManager::LOG_LEVEL_ERROR);
                                }
                            }
                        }
                        TracerManager::Instance().EndServiceTrace();
                    }
                }  else if (className == "Userctgr") {
                    if (methodName == "GetUserctgrAll") {
                        int context_id = std::stoi(query_params["context_id"]);
                        std::string content = Userctgr::GetUserctgrAll(context_id);
                        if (content == "") status = http::status::bad_request;
                        response = content;
                    } else if (methodName == "GetUserctgrContent") {
                        int userctgr_id = std::stoi(query_params["userctgr_id"]);
                        int year = std::stoi(query_params["year"]);;
                        std::string content = Userctgr::GetUserctgrContent(userctgr_id, year);
                        if (content == "") status = http::status::bad_request;
                        response = content;
                    } else if (methodName == "GetUserctgrUserAllEndDate") {
                        int context_id = std::stoi(query_params["context_id"]);
                        std::string content = Userctgr::GetUserctgrUserAllEndDate(context_id);
                        if (content == "") status = http::status::bad_request;
                        response = content;
                    } else if (methodName == "GetUserUserctgrRO") {
                        int context_id = std::stoi(query_params["context_id"]);
                        //int year = std::stoi(query_params["year"]);
                        int userctgr_id = std::stoi(query_params["userctgr_id"]);
                        std::string content = Userctgr::GetUserUserctgrRO(context_id, userctgr_id);
                        if (content == "") status = http::status::bad_request;
                        response = content;
                    } else if (methodName == "ReadOnlyGrpsCtgrs") {
                        int context_id = std::stoi(query_params["context_id"]);
                        int year = std::stoi(query_params["year"]);
                        int userctgr_id = std::stoi(query_params["userctgr_id"]);
                        std::string content = Userctgr::ReadOnlyGrpsCtgrs(context_id, userctgr_id, year);
                        if (content == "") status = http::status::bad_request;
                        response = content;
                    } else {
                        status = http::status::bad_request;
                        response = "Userctgr method not found.";
                    }
                } else if (className == "Album") {
                    if (methodName == "GetAlbumInfoYQM") {
                        int album_id = std::stoi(query_params["album_id"]);
                        std::string content = Album::GetAlbumInfoYQM(album_id);
                        response = content;
                    } else if (methodName == "GetAlbForms") {
                        int album_id = std::stoi(query_params["album_id"]);
                        std::string content = Album::GetAlbForms(album_id);
                        response = content;
                    } else if (methodName == "GetAlbumZaPeriod") {
                        int idSelAlbum = std::stoi(query_params["idSelAlbum"]);
                        int year = std::stoi(query_params["year"]);
                        std::string content = Album::GetAlbumZaPeriod(idSelAlbum, year);
                        response = content;
                    } else if (methodName == "GetAccessAlbumForUser") {
                        int context_id = std::stoi(query_params["context_id"]);
                        int album_id = std::stoi(query_params["album_id"]);
                        std::string content = Album::GetAccessAlbumForUser(context_id, album_id);
                        response = content;
                    } else if (methodName == "GetAccessBlockAlbumForUser") {
                        int context_id = std::stoi(query_params["context_id"]);
                        int album_id = std::stoi(query_params["album_id"]);
                        std::string content = Album::GetAccessBlockAlbumForUser(context_id, album_id);
                        response = content;
                    } else {
                        status = http::status::bad_request;
                        response = "Album method not found";
                    }
                } else if (className == "Plan") {
                    if (methodName == "GetAllPlanForUser") {
                        int context_id = std::stoi(query_params["context_id"]);
                        std::string content = Plan::GetAllPlanForUser(context_id);
                        response = content;
                    } else if (methodName == "LoadPlanPermForCurUser") {
                        int context_id = std::stoi(query_params["context_id"]);
                        std::string content = Plan::LoadPlanPermForCurUser(context_id);
                        response = content;
                    } else {
                        status = http::status::bad_request;
                        response = "Plan method not found";
                    }
                } else if (className == "Forms") {
                    if (methodName == "GetAllFormAlbum") {
                        int album_id = std::stoi(query_params["album_id"]);
                        int bydate_param = std::stoi(query_params["bydate"]);
                        TDateTime bydate_t = TDateTime(bydate_param);
                        std::string content = Forms::GetAllFormAlbum(album_id, bydate_t);
                        response = content;
                    } else if (methodName == "Get1FormWithoutAlbum") {
                        int album_id = std::stoi(query_params["album_id"]);
                        int bydate_param = std::stoi(query_params["bydate"]);
                        TDateTime bydate_t = TDateTime(bydate_param);
                        std::string content = Forms::Get1FormWithoutAlbum(album_id, bydate_t);
                        response = content;
                    } else {
                        status = http::status::bad_request;
                        response = "Forms method not found";
                    }
                } else if (className == "Pokaztl") {
                    if (methodName == "GetPokaz") {
                        //int context_id = std::stoi(query_params["context_id"]);
                        int pokaz_id = std::stoi(query_params["pokaz_id"]);
                        std::string content = Pokaztl::GetPokaz(pokaz_id);
                        response = content;
                    } else if (methodName == "GetRootPokaztl") {
                        //int context_id = std::stoi(query_params["context_id"]);
                        int pokaz_id = std::stoi(query_params["pokaz_id"]);
                        std::string content = Pokaztl::GetRootPokaztl(pokaz_id);
                        response = content;
                    } else {
                        status = http::status::bad_request;
                        response = "Pokaztl method not found";
                    }
                } else if (className == "ClassofPart") {
                    if (methodName == "GetClassConteiners") {
                        //int context_id = std::stoi(query_params["context_id"]);
                        int class_id = std::stoi(query_params["class_id"]);
                        std::string content = ClassofPart::GetClassConteiners(class_id);
                        response = content;
                    } else if (methodName == "GetSubElems") {
                        //int context_id = std::stoi(query_params["context_id"]);
                        int conteiner_id = std::stoi(query_params["conteiner_id"]);
                        std::string content = ClassofPart::GetSubElems(conteiner_id);
                        response = content;
                    } else {
                        status = http::status::bad_request;
                        response = "ClassofPart method not found";
                    }
                } else if (className == "Spreadsheet") {
                    TracerManager::Instance().CheckClassTrace("Spreadsheet");
                    if (methodName == "OpenForm") {
                        TracerManager::Instance().StartServiceTrace("OpenForm");
                        std::string content = Spreadsheet::OpenForm(GlobalPort, Requestcontent, httpresponse, xRequestId);
                        std::cout << "methodName: OpenForm" << std::endl;
                        std::cout << "Requestcontent: " << Requestcontent << std::endl;
                        TracerManager::Instance().EndServiceTrace();
                    } else if (methodName == "CellValueAtr") {
                        std::string TblSessionId = query_params["TblSessionId"];
                        int nSheet = StrToInt(query_params["nSheet"]);
                        int nRow = StrToInt(query_params["nRow"]);
                        int nCol = StrToInt(query_params["nCol"]);
                        std::string content = Spreadsheet::CellValueAtr(TblSessionId, nSheet, nRow, nCol);
                        response = content;
                        std::cout << "methodName: CellValueAtr" << std::endl;
                        std::cout << "TblSessionId: " << TblSessionId << std::endl;
                    }  else if (methodName == "CellEndingEdit") {
                        std::string content = Spreadsheet::CellUpdate(Requestcontent, status);
                        response = content;
                        std::cout << "methodName: CellEndingEdit" << std::endl;
                        std::cout << "Requestcontent: " << Requestcontent << std::endl;
                    } else if (methodName == "CellUpdateBatch") {
                        std::string content = Spreadsheet::CellUpdateBatch(Requestcontent);
                        response = content;
                        std::cout << "methodName: CellEndingEdit" << std::endl;
                        std::cout << "Requestcontent: " << Requestcontent << std::endl;
                    } else if (methodName == "CloseForm") {
                        std::string content = Spreadsheet::CloseForm(Requestcontent);
                        response = content;
                        std::cout << "methodName: CloseForm" << std::endl;
                        std::cout << "Requestcontent: " << Requestcontent << std::endl;
                    } else if (methodName == "Close") {
                        std::string content = Spreadsheet::Close(Requestcontent);
                        response = content;
                        std::cout << "methodName: Close" << std::endl;
                        std::cout << "Requestcontent: " << Requestcontent << std::endl;
                    } else if (methodName == "GetTww") {
                        std::string context_id = query_params["context_id"];
                        std::string TblSessionId = query_params["TblSessionId"];
                        int nSheet = StrToInt(query_params["nSheet"]);
                        std::string content = Spreadsheet::GetTww(context_id, TblSessionId, nSheet);
                        response = content;
                        std::cout << "methodName: GetTww" << std::endl;
                        std::cout << "TblSessionId: " << TblSessionId << std::endl;
                    } else if (methodName == "GetAttRow") {
                        std::string context_id = query_params["context_id"];
                        std::string TblSessionId = query_params["TblSessionId"];
                        int nSheet = StrToInt(query_params["nSheet"]);
                        int nRow = StrToInt(query_params["nRow"]);
                        std::string content = Spreadsheet::GetAttRow(context_id, TblSessionId, nSheet, nRow);
                        response = content;
                        std::cout << "methodName: GetAttRow" << std::endl;
                        std::cout << "TblSessionId: " << TblSessionId << std::endl;
                    } else if (methodName == "GetNumberActiveSessions") {
                        std::string content = Spreadsheet::GetNumberActiveSessions();
                        response = content;
                        std::cout << "methodName: GetNumberActiveSessions" << std::endl;
                        std::cout << "content: " << content << std::endl;
                    } else if (methodName == "ButtonClick") {
                        std::string content = Spreadsheet::ButtonClick(Requestcontent);
                        response = content;
                        std::cout << "methodName: ButtonClick" << std::endl;
                        std::cout << "Requestcontent: " << Requestcontent << std::endl;
                    } else if (methodName == "insertPokaztl") {
                        std::string content = Spreadsheet::insertPokaztl(GlobalPort, Requestcontent);
                        response = content;
                        std::cout << "methodName: insertPokaztl" << std::endl;
                        std::cout << "Requestcontent: " << Requestcontent << std::endl;

                    } else if (methodName == "changePokaztl") {
                        std::string content = Spreadsheet::changePokaztl(GlobalPort, Requestcontent);
                        response = content;
                        std::cout << "methodName: changePokaztl" << std::endl;
                        std::cout << "Requestcontent: " << Requestcontent << std::endl;

                    } else if (methodName == "getPokaztl") {
                        std::string context_id = query_params["context_id"];
                        std::string TblSessionId = query_params["TblSessionId"];
                        int nSheet = StrToInt(query_params["nSheet"]);
                        int nRow = StrToInt(query_params["nRow"]);
                        std::string content = Spreadsheet::getPokaztl(context_id, TblSessionId, nSheet, nRow);
                        response = content;
                        std::cout << "methodName: getPokaztl" << std::endl;
                        std::cout << "TblSessionId: " << TblSessionId << std::endl;
                    } else if (methodName == "deletePokaztl") {
                        std::string content = Spreadsheet::deletePokaztl(GlobalPort, Requestcontent);
                        response = content;
                        std::cout << "methodName: deletePokaztl" << std::endl;
                        std::cout << "Requestcontent: " << Requestcontent << std::endl;
                    } else if (methodName == "BudgetChange") {
                        std::string content = Spreadsheet::BudgetChange(Requestcontent);
                        response = content;
                        std::cout << "methodName: BudgetChange" << std::endl;
                        std::cout << "Requestcontent: " << Requestcontent << std::endl;
                    } else if (methodName == "ExportInPDF") {
                        int nSheet = StrToInt(query_params["nSheet"]);
                        std::string TblSessionId = query_params["TblSessionId"];
                        std::string content = Spreadsheet::ExportInPDF(TblSessionId, nSheet);
                        response = content;
                        std::cout << "methodName: ExportInPDF" << std::endl;
                        std::cout << "TblSessionId: " << TblSessionId << std::endl;
                    } else if (methodName == "ExportInXLS") {
                        int nSheet = StrToInt(query_params["nSheet"]);
                        std::string TblSessionId = query_params["TblSessionId"];
                        std::string content = Spreadsheet::ExportInXLS(TblSessionId, nSheet);
                        response = content;
                        std::cout << "methodName: ExportInXLS" << std::endl;
                        std::cout << "TblSessionId: " << TblSessionId << std::endl;
                    } else if (methodName == "GetJsonForm") {
                        std::string TblSessionId = query_params["TblSessionId"];
                        std::string content = Spreadsheet::GetJsonForm(TblSessionId);
                        response = content;
                        std::cout << "methodName: GetJsonForm" << std::endl;
                        std::cout << "TblSessionId: " << TblSessionId << std::endl;
                    }
                } else if (className == "SpreadsheetAttr") {
                    if (methodName == "AllCellValueAtr") {
                        std::string content = SpreadsheetAttr::AllCellValueAtr(Requestcontent);
                        response = content;
                        std::cout << "methodName: CellValueAtr" << std::endl;
                    }
                } else if (className == "SQLCommand") {
                    if (methodName == "Open") {
                        std::string errorMessage;
//                    std::string JSONString = query_params["JSONString"]; //получение JSON-строки запроса из параметров запроса
                        //JSONString = "{\"SQL\":\"SELECT * FROM Plans\\r\\nWHERE plan_id = :plan_id\\r\\n\",\"Params\":[{\"Name\":\"plan_id\",\"Value\":69,\"Type\":\"ftInteger\"}]}";
                        std::string sinonimDB = query_params["sinonimDB"];
                        std::string content = RS_SQLCommand::Open(Requestcontent, errorMessage, sinonimDB);
                        response = content;
                    } else if (methodName == "ExecSQL") {
                        std::string errorMessage;
//                    std::string JSONString = query_params["JSONString"]; //получение JSON-строки запроса из параметров запроса
                        //JSONString = "{\"SQL\":\"UPDATE Plans\\r\\nSET plan_name = :plan_name, plan_is_rajon = :plan_is_rajon, plan_short_name = :plan_short_name, FO = :FO, OKATO_kod = :OKATO_kod, god_beg = :god_beg, god_end = :god_end, email = :email\\r\\nWHERE plan_id = :plan_id\\r\\n\",\"Params\":[{\"Name\":\"plan_id\",\"Value\":69,\"Type\":\"ftInteger\"},{\"Name\":\"plan_name\",\"Value\":\"Калужская область111\",\"Type\":\"ftString\"},{\"Name\":\"plan_is_rajon\",\"Value\":4,\"Type\":\"ftInteger\"},{\"Name\":\"plan_short_name\",\"Value\":\"Калужская область\",\"Type\":\"ftString\"},{\"Name\":\"FO\",\"Value\":\"\",\"Type\":\"ftString\"},{\"Name\":\"OKATO_kod\",\"Value\":\"29000000\",\"Type\":\"ftString\"},{\"Name\":\"god_beg\",\"Value\":2000,\"Type\":\"ftInteger\"},{\"Name\":\"god_end\",\"Value\":2050,\"Type\":\"ftInteger\"},{\"Name\":\"email\",\"Value\":\"\",\"Type\":\"ftString\"}]}";
                        std::string sinonimDB = query_params["sinonimDB"];
                        bool content = RS_SQLCommand::ExecSQL(Requestcontent, errorMessage, sinonimDB);
                        if (content) {
                            response = "true";
                        } else {
                            response = "false";
                        }
                    }
                } else if (className == "Document") {
                    TracerManager::Instance().CheckClassTrace("Document");
                    if (methodName == "GetByPokaz") {
                        TracerManager::Instance().StartServiceTrace("GetByPokaz");
                        int PokazId = std::stoi(query_params["PokazId"]);
                        int comnp_id = std::stoi(query_params["comnp_id"]);
                        int cell_period = std::stoi(query_params["cell_period"]);
                        int God = std::stoi(query_params["God"]);
                        int Kvrtl = std::stoi(query_params["Kvrtl"]);
                        int Mesjc = std::stoi(query_params["Mesjc"]);
                        int Den = std::stoi(query_params["Den"]);
                        int PlanId = std::stoi(query_params["PlanId"]);
                        int VidOtch = std::stoi(query_params["VidOtch"]);
                        std::string content = Document::DocumentPokaz(
                                PokazId, comnp_id, cell_period, God, Kvrtl, Mesjc, Den, PlanId, VidOtch, status);
                        response = content;
                        TracerManager::Instance().EndServiceTrace();
                    } else if (methodName == "DownloadFile") {
                        TracerManager::Instance().StartServiceTrace("DownloadFile");
                        int documentId = std::stoi(query_params["documentId"]);
                        int fileId = std::stoi(query_params["fileId"]);
                        std::string content = Document::DocumentContent(documentId, fileId, status);
                        response = content;
                        TracerManager::Instance().EndServiceTrace();
                    } else if (methodName == "CreateInSpreadsheet") {
                        TracerManager::Instance().StartServiceTrace("CreateInSpreadsheet");
                        std::string content = Document::DocumentCreate(true, Requestcontent, status);
                        response = content;
                        TracerManager::Instance().EndServiceTrace();
                    } else if (methodName == "UpdateInSpreadsheet") {
                        TracerManager::Instance().StartServiceTrace("UpdateInSpreadsheet");
                        std::string content = Document::DocumentUpdate(true, Requestcontent, status);
                        response = content;
                        TracerManager::Instance().EndServiceTrace();
                    } else if (methodName == "GetById") {
                        TracerManager::Instance().StartServiceTrace("GetById");
                        std::string documentIdHex = query_params["documentId"];
                        std::string content = Document::DocumentById(documentIdHex, status);
                        response = content;
                        TracerManager::Instance().EndServiceTrace();
                    } else if (methodName == "Create") {
                        TracerManager::Instance().StartServiceTrace("Create");
                        std::string content = Document::DocumentCreate(false, Requestcontent, status);
                        response = content;
                        TracerManager::Instance().EndServiceTrace();
                    } else if (methodName == "Update") {
                        TracerManager::Instance().StartServiceTrace("Update");
                        std::string content = Document::DocumentUpdate(false, Requestcontent, status);
                        response = content;
                        TracerManager::Instance().EndServiceTrace();
                    }
                } else if (className == "DataMart") {
                    try {
                        if (methodName == "CreateContent") {
                            std::string TblName = query_params["TblName"];
                            int Year = StrToInt(query_params["Year"]);
                            std::string content = N3Table::n3tableload(TblName, Year);
                            response = content;
                        } else if (methodName == "Open") {
                            std::string errorMessage;
                            std::string sinonimDB = query_params["sinonimDB"];
                            std::string content = RS_SQLCommand::Open(Requestcontent, errorMessage, sinonimDB);
                            response = content;
                        } else if (methodName == "ExecSQL") {
                            std::string errorMessage;
                            std::string sinonimDB = query_params["sinonimDB"];
                            bool content = RS_SQLCommand::ExecSQL(Requestcontent, errorMessage, sinonimDB);
                            if (content) {
                                response = "true";
                            } else {
                                response = "false";
                            }
                        }
                    } catch (const std::exception &e) {
                        response = "error: " + std::string(e.what());
                    } catch (...) {
                        response = "error: unknown exception";
                    }
                } else if (className == "Budget") {
                    if ( methodName == "AcceptChanges") {
                        std::string content = Budget::AcceptChanges(Requestcontent, status);
                        response = content;
                    } else if ( methodName == "Proposals") {
                        int year = StrToInt(query_params["year"]);
                        int plan_id = 0;
                        int user_id = 0;
                        int sost = 0;
                        try {
                            plan_id = StrToInt(query_params["plan_id"]);
                        }
                        catch (const std::exception& e) {
                        }
                        try {
                            user_id = StrToInt(query_params["user_id"]);
                        }
                        catch (const std::exception& e) {
                        }
                        try {
                            sost = StrToInt(query_params["sost"]);
                        }
                        catch (const std::exception& e) {
                        }

                        OrderBy proposals_orderby;
                        bool is_orderby = query_order(query_string, proposals_orderby);
                        std::string content = Budget::Proposals(year, plan_id, user_id, sost, is_orderby, proposals_orderby);
                        response = content;
                    }
                } else {
                    status = http::status::bad_request;
                    response = "Class not found";
                    TracerManager::Instance().AddSpanLog("Class not found", {},
                                                         TracerManager::LOG_LEVEL_ERROR);
                }
            } else {
                status = http::status::bad_request;
                response = "API or category not found";
                TracerManager::Instance().AddSpanLog("API or category not found", {},
                                                     TracerManager::LOG_LEVEL_ERROR);
            }
        }
        catch (const std::exception& e) {
            status = http::status::bad_request;
            response = "Invalid parameters of request:" + std::string(e.what());
            TracerManager::Instance().AddSpanLog("Exception: Invalid parameters of request", {{"exception", e.what()}},
                                                 TracerManager::LOG_LEVEL_ERROR);
        }

    } else {
        status = http::status::bad_request;
        response = "Invalid request format";
        TracerManager::Instance().AddSpanLog("Exception: Invalid request format", {},
                                             TracerManager::LOG_LEVEL_ERROR);
    }

    TracerManager::Instance().AddSpanLog("Rest response", {{"response", response },
                                                           {"status", std::to_string(static_cast<int>(status))},
                                                           {"authHeader", authHeader}},
                                         TracerManager::LOG_LEVEL_MANDATORYINFO);
    TracerManager::Instance().EndAllSpans();

    return response;
}