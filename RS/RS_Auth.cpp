#include "RS_Auth.h"
#include "ClassesUtils.h"
#include "../unWebModuleUnit.h"
#include "DB_User.h"
#include <boost/beast/core/detail/base64.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/asio/ssl.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>


#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <json.hpp>
#include "DB_User.h"
#include "json.hpp"
#include "TracerManager.h"


std::string base64url_decode(const std::string& input) {
    std::string base64 = input;
    std::replace(base64.begin(), base64.end(), '-', '+');
    std::replace(base64.begin(), base64.end(), '_', '/');
    while (base64.size() % 4) {
        base64 += '=';
    }

    std::vector<unsigned char> decoded(boost::beast::detail::base64::decoded_size(base64.size()));
    auto result = boost::beast::detail::base64::decode(decoded.data(), base64.data(), base64.size());
    decoded.resize(result.first);

    return std::string(decoded.begin(), decoded.end());
}

std::string decode_jwt(const std::string& jwt, int& exp, int& profileId) {
    auto first_dot = jwt.find('.');
    auto second_dot = jwt.find('.', first_dot + 1);

    if (first_dot == std::string::npos || second_dot == std::string::npos) {
        throw std::runtime_error("Invalid JWT format");
    }

    std::string header = base64url_decode(jwt.substr(0, first_dot));
    std::string payload = base64url_decode(jwt.substr(first_dot + 1, second_dot - first_dot - 1));

    nlohmann::json payload_json = nlohmann::json::parse(payload);
    exp = payload_json["exp"].get<int>();
    profileId = payload_json["profileId"].get<int>();

    return "Header: " + header + "\nPayload: " + payload;
}

int getUnixTimestamp() {
    return static_cast<int>(std::time(nullptr));
}

std::string Auth::CheckUserPermissions(std::string &url, std::string accessToken, boost::beast::http::status& status) {
    status = http::status::ok;
    int exp, profileId;

    try {
        // Проверяем токен на сервера авторизации
        std::string statusTocken = Auth::CheckToken(url, accessToken, status);
        if (status != http::status::ok) {
            status = http::status::unauthorized;
            TracerManager::Instance().AddSpanLog("Ошибка Auth::CheckUserPermissions после CheckToken", {{"errordesr", "statusTocken"},
                                                                                                        {"accessToken", accessToken},
                                                                                                        {"statusToken", statusTocken},
                                                                                                        {"status", std::to_string(static_cast<int>(status))}},
                                                 TracerManager::LOG_LEVEL_ERROR);
            return statusTocken;
        }

        // Декодируем токен
        std::string res = decode_jwt(accessToken, exp, profileId);
        TDateTime expDate = UnixTimestampToDateTime(exp);
        std::string expDatews = DateTimeToStr(expDate);
        int NowUnix = getUnixTimestamp();

        if (exp < NowUnix) {
            status = http::status::unauthorized;
            TracerManager::Instance().AddSpanLog("Ошибка Auth::CheckUserPermissions проверка exp", {{"errordesr", "statusTocken"},
                                                                       {"exp", std::to_string(exp)},
                                                                       {"status", std::to_string(static_cast<int>(status))}},
                                                 TracerManager::LOG_LEVEL_ERROR);
            return "401 Unauthorized";
        } else {
            int userid = TDB_User::GetUserByProfileID(profileId);
            if (!userid) {
                status = boost::beast::http::status::see_other;
                TracerManager::Instance().AddSpanLog("Ошибка Auth::CheckUserPermissions. User not found", {
                                                                                                        {"profileId", std::to_string(profileId)},
                                                                                                        {"status", std::to_string(static_cast<int>(status))}},
                                                     TracerManager::LOG_LEVEL_ERROR);
                return "User not found";
            } else {
                if (TDB_User::SaveUserToken(userid, accessToken, expDate))  {
                    return "{\"context_id\": " + IntToStr(userid) + "}";
                } else {
                    status = http::status::unauthorized;
                    TracerManager::Instance().AddSpanLog("Ошибка Auth::CheckUserPermissions. Can't save token", {
                                                                 {"profileId", std::to_string(profileId)},
                                                                 {"userid", std::to_string(userid)},
                                                                 {"accessToken", accessToken},
                                                                 {"status", std::to_string(static_cast<int>(status))}},
                                                         TracerManager::LOG_LEVEL_ERROR);
                    return "Can't save token";
                }
            }
        }
    } catch (const std::exception& e) {
        status = http::status::unauthorized;
        TracerManager::Instance().AddSpanLog("Ошибка Auth::CheckUserPermissions. Исключение", {
                                                     {"accessToken", accessToken},
                                                     {"исключение", e.what()},
                                                     {"status", std::to_string(static_cast<int>(status))}},
                                             TracerManager::LOG_LEVEL_ERROR);
        return std::string("Error: ") + e.what();
    }
}

std::string decode_jwt(const std::string& jwt, int& exp) {
    auto first_dot = jwt.find('.');
    auto second_dot = jwt.find('.', first_dot + 1);

    if (first_dot == std::string::npos || second_dot == std::string::npos) {
        throw std::runtime_error("Invalid JWT format");
    }

    std::string header = base64url_decode(jwt.substr(0, first_dot));
    std::string payload = base64url_decode(jwt.substr(first_dot + 1, second_dot - first_dot - 1));
//    std::cout << "payload: " << payload<< std::endl;

    nlohmann::json payload_json = nlohmann::json::parse(payload);
//    std::cout << "parse exp" << std::endl;
    exp = payload_json["exp"].get<int>();

    return "Header: " + header + "\nPayload: " + payload;
}

std::string Auth::CheckUserPermissionsOnlyEXP(std::string &url, std::string accessToken, boost::beast::http::status &status, int& exp) {
    status = http::status::ok;

    try {
        // Проверяем токен на сервере авторизации
        std::string statusTocken = Auth::CheckToken(url, accessToken, status);
        if (status != http::status::ok) {
            // Токен не валидирован на сервере
            TracerManager::Instance().AddSpanLog("Ошибка Auth::CheckToken", {{"errordesr", "Токен не валидирован на сервере"},
                                                                             {"accessToken", accessToken},
                                                                             {"statusToken", statusTocken},
                                                                             {"Север валидации", url}},
                                                 TracerManager::LOG_LEVEL_ERROR);
            status = http::status::unauthorized;
            return statusTocken;
        }

        // Декодируем токен и проверяем дату истечения
        std::string res = decode_jwt(accessToken, exp);
        TDateTime expDate = UnixTimestampToDateTime(exp);

        if (expDate < Now()) {
            TracerManager::Instance().AddSpanLog("Ошибка токен expired", {{"errordesr", "Токен не валидирован на сервере"},
                                                                          {"accessToken", accessToken},
                                                                          {"expDate", expDate.ToString()}},
                                                 TracerManager::LOG_LEVEL_ERROR);
            status = http::status::unauthorized;
            return "401 Unauthorized";
        }

    } catch (const std::exception& e) {
        // Логируем исключение в трассировке, если оно происходит
        TracerManager::Instance().AddSpanLog("исключение в CheckUserPermissionsOnlyEXP", {
                                                                      {"accessToken", accessToken},
                                                                      {"Север валидации", url},
                                                                      {"reason", e.what()}},
                                             TracerManager::LOG_LEVEL_ERROR);
        status = http::status::unauthorized;
        return std::string("Error: ") + e.what();
    }

    return "";
}

std::string Auth::ValidateTokenLocally(std::string accessToken, boost::beast::http::status& status) {
    status = http::status::ok;
    int exp, profileId;
    std::string result = "";
    //return result;   // временно

    try {
        //Декодируем токен
        std::string res = decode_jwt(accessToken, exp, profileId);
        TDateTime expDate = UnixTimestampToDateTime(exp);
        std::string stored_accessToken;
        TDateTime stored_expDate;
        if (TDB_User::GetUserTokenByProfileID(profileId, stored_accessToken, stored_expDate)) {
            if (stored_accessToken != accessToken) {
                status = http::status::unauthorized;
                result = "token not correct; token" + accessToken + ", stored token: " + stored_accessToken;
            }
            if (expDate < Now()) {
                status = http::status::unauthorized;
                result = "token expired";
            }
        }
        else {
            status = http::status::unauthorized;
            result = "not found token by profileid, profileid:" + IntToStr(profileId);
        }
    } catch (const std::exception& e) {
        status = http::status::unauthorized;
        result = std::string("Error: ") + e.what();
    }
    return result;
}

std::string Auth::ValidateTokenLocallyByStoredToken(std::string accessToken, boost::beast::http::status& status) {
    status = http::status::ok;
    int exp;
    std::string result = "";
    //return result;   // временно
    try {
        TDateTime stored_expDate;
        int user_id;
        std::string profileid;
        if (!TDB_User::GetUserProfileIDByToken(accessToken, user_id, profileid, stored_expDate)) {
            status = http::status::unauthorized;
            result = "not found token; token" + accessToken;
        }
        if (stored_expDate < Now()) {
            status = http::status::unauthorized;
            result = "token expired:" + stored_expDate.ToString();
        }
    } catch (const std::exception& e) {
        status = http::status::unauthorized;
        result = std::string("Error: ") + e.what();
    }
    return result;
}

std::string Auth::CheckToken(std::string &url, std::string &accessToken, boost::beast::http::status &status) {
    status = http::status::ok;
    try {
//        std::cout << "Начало проверки токена." << std::endl;

        // Разбор URL
        auto const pos = url.find("://");
        if (pos == std::string::npos) {
            TracerManager::Instance().AddSpanLog("Ошибка в CheckToken.Invalid URL", {
                                                         {"Север валидации", url}},
                                                 TracerManager::LOG_LEVEL_ERROR);
            throw std::runtime_error("Invalid URL");
        }
        auto const protocol = url.substr(0, pos);
        auto const path = url.substr(pos + 3);
        auto const host_end = path.find('/');
        auto const host_port = path.substr(0, host_end);
        auto const target = path.substr(host_end);

        // Извлечение хоста и порта
        auto const colon_pos = host_port.find(':');
        std::string host;
        std::string port;
        if (colon_pos != std::string::npos) {
            host = host_port.substr(0, colon_pos);
            port = host_port.substr(colon_pos + 1);
        } else {
            host = host_port;
            port = (protocol == "https") ? "443" : "80";
        }

//        std::cout << "Protocol: " << protocol << std::endl;
//        std::cout << "Host: " << host << std::endl;
//        std::cout << "Port: " << port << std::endl;
//        std::cout << "Target: " << target << std::endl;

        // Создаем I/O context
        boost::asio::io_context ioc;

        if (protocol == "https") {
            // Создаем SSL context
            boost::asio::ssl::context ctx{boost::asio::ssl::context::sslv23_client};
            ctx.set_default_verify_paths();
//            std::cout << "SSL context initialized." << std::endl;

            // Создаем Resolver и SSL Socket
            boost::asio::ip::tcp::resolver resolver(ioc);
            boost::asio::ssl::stream<boost::asio::ip::tcp::socket> stream(ioc, ctx);

            // Разрешаем адрес
            auto const results = resolver.resolve(host, port);
            if (results.empty()) {
                TracerManager::Instance().AddSpanLog("Ошибка в CheckToken.Host not found", {
                                                             {"Север валидации", url}},
                                                     TracerManager::LOG_LEVEL_ERROR);
                throw std::runtime_error("Host not found: " + host);
            }

            // Подключаемся к серверу
            boost::asio::connect(stream.next_layer(), results.begin(), results.end());
            // Выполняем SSL handshake
            stream.handshake(boost::asio::ssl::stream_base::client);

            // Создаем HTTP запрос
            boost::beast::http::request<boost::beast::http::string_body> req{boost::beast::http::verb::get, target, 11};
            req.set(boost::beast::http::field::host, host);
            req.set(boost::beast::http::field::user_agent, BOOST_BEAST_VERSION_STRING);
            req.set(boost::beast::http::field::authorization, "Bearer " + accessToken);

            // Логируем полный URL
            std::string full_url = protocol + "://" + host + ':' + port + target;

            // Логируем запрос

            // Отправляем запрос
            boost::beast::http::write(stream, req);
            // Буфер для хранения данных
            boost::beast::flat_buffer buffer;
            boost::beast::http::response<boost::beast::http::string_body> res;

            // Читаем ответ
            boost::beast::http::read(stream, buffer, res);
            boost::system::error_code ec;
            stream.shutdown(ec);
            if (ec == boost::asio::error::eof) {
                ec.assign(0, ec.category());
            }
//            if (ec) {
////                std::cerr << "Shutdown error: " << ec.message() << std::endl;
//                throw boost::system::system_error{ec};
//            }

            // Возвращаем статус ответа
            //std::string ws = std::to_string(res.result_int());
            if (res.result_int() != 200) {
                TracerManager::Instance().AddSpanLog("Ошибка в CheckToken. res.result_int() != 200", {
                                                             {"Результат", std::to_string(static_cast<int>(res.result_int())) }},
                                                     TracerManager::LOG_LEVEL_ERROR);
                status = http::status::unauthorized;
            }
            std::string ws = res.body();
            //std::cout << "Валидация токена на сервере успешна! Status: " << ws << std::endl;
            return ws;

        } else {
            // Создаем Resolver и обычный Socket для HTTP
            boost::asio::ip::tcp::resolver resolver(ioc);
            boost::asio::ip::tcp::socket socket(ioc);

            // Разрешаем адрес
//            std::cout << "Resolving address for host: " << host << " on port: " << port << std::endl;
            auto const results = resolver.resolve(host, port);
            if (results.empty()) {
//                throw std::runtime_error("Host not found: " + host);
            }
//            std::cout << "Address resolved: " << results.begin()->endpoint() << std::endl;

            // Подключаемся к серверу
//            std::cout << "Connecting to server..." << std::endl;
            boost::asio::connect(socket, results.begin(), results.end());
//            std::cout << "Connected to server." << std::endl;

            // Создаем HTTP запрос
            boost::beast::http::request<boost::beast::http::string_body> req{boost::beast::http::verb::get, target, 11};
            req.set(boost::beast::http::field::host, host);
            req.set(boost::beast::http::field::user_agent, BOOST_BEAST_VERSION_STRING);
            req.set(boost::beast::http::field::authorization, "Bearer " + accessToken);

            // Логируем полный URL
            std::string full_url = protocol + "://" + host + ':' + port + target;
//            std::cout << "Full URL: " << full_url << std::endl;

            // Логируем запрос
//            std::cout << "Request: " << req << std::endl;

            // Отправляем запрос
//            std::cout << "Sending request..." << std::endl;
            boost::beast::http::write(socket, req);
//            std::cout << "Request sent." << std::endl;

            // Буфер для хранения данных
            boost::beast::flat_buffer buffer;
            boost::beast::http::response<boost::beast::http::string_body> res;

            // Читаем ответ
//            std::cout << "Reading response..." << std::endl;
            boost::beast::http::read(socket, buffer, res);
//            std::cout << "Response received." << std::endl;

            // Логируем полный ответ
//            std::cout << "Full response: " << res << std::endl;

            // Завершаем соединение
//            std::cout << "Shutting down connection..." << std::endl;
            boost::system::error_code ec;
            socket.shutdown(boost::asio::ip::tcp::socket::shutdown_both, ec);
            if (ec) {
//                std::cerr << "Shutdown error: " << ec.message() << std::endl;
            }

            // Возвращаем статус ответа
            //std::string ws = std::to_string(res.result_int());
            std::string ws = res.body();
            if (res.result_int() != 200) {
                TracerManager::Instance().AddSpanLog("Ошибка в CheckToken. res.result_int() != 200", {
                                                             {"Результат", std::to_string(static_cast<int>(res.result_int())) }},
                                                     TracerManager::LOG_LEVEL_ERROR);
                status = http::status::unauthorized;
            }
//            std::cout << "Валидация токена на сервере успешна! Status: " << ws << std::endl;
            return ws;
        }
    } catch (const boost::system::system_error& e) {
        std::cerr << "Boost system error: " << e.what() << std::endl;
        status = http::status::unauthorized;
        return std::string("Boost system error: ") + e.what();
    } catch (const std::exception& e) {
        std::cerr << "Валидация токена на сервере не успешна! Error: " << e.what() << std::endl;
        status = http::status::unauthorized;
        return std::string("Error: ") + e.what();
    } catch (...) {
        std::cerr << "Unknown error occurred during token validation." << std::endl;
        status = http::status::unauthorized;
        return "Unknown error";
    }
}

std::string Auth::currentUserFull(std::string &url, std::string &accessToken, boost::beast::http::status &status) {
    status = http::status::ok;

    try {
        // Разбор URL
        auto const pos = url.find("://");
        if (pos == std::string::npos) {
            TracerManager::Instance().AddSpanLog("ошибка в currentUserFull: pos == std::string::npos", {
                                                         {"url", url}},
                                                 TracerManager::LOG_LEVEL_ERROR);
            throw std::runtime_error("Invalid URL");
        }
        auto const protocol = url.substr(0, pos);
        auto const path = url.substr(pos + 3);
        auto const host_end = path.find('/');
        auto const host_port = path.substr(0, host_end);
        auto const target = path.substr(host_end);

        // Извлечение хоста и порта
        auto const colon_pos = host_port.find(':');
        std::string host;
        std::string port;
        if (colon_pos != std::string::npos) {
            host = host_port.substr(0, colon_pos);
            port = host_port.substr(colon_pos + 1);
        } else {
            host = host_port;
            port = (protocol == "https") ? "443" : "80";
        }

        // Создаем I/O context
        boost::asio::io_context ioc;

        if (protocol == "https") {
            // SSL context
            boost::asio::ssl::context ctx{boost::asio::ssl::context::sslv23_client};
            ctx.set_default_verify_paths();

            // Resolver и SSL Socket
            boost::asio::ip::tcp::resolver resolver(ioc);
            boost::asio::ssl::stream<boost::asio::ip::tcp::socket> stream(ioc, ctx);

            // Resolve
            auto const results = resolver.resolve(host, port);
            if (results.empty()) {
                TracerManager::Instance().AddSpanLog("ошибка в currentUserFull: results.empty()", {
                                                             {"url", url},
                                                             {"host", host},
                                                             {"port", port},
                                                             },
                                                     TracerManager::LOG_LEVEL_ERROR);
                throw std::runtime_error("Host not found: " + host);
            }

            // Connect и SSL handshake
            boost::asio::connect(stream.next_layer(), results.begin(), results.end());
            stream.handshake(boost::asio::ssl::stream_base::client);

            // HTTP запрос
            boost::beast::http::request<boost::beast::http::string_body> req{boost::beast::http::verb::get, target, 11};
            req.set(boost::beast::http::field::host, host);
            req.set(boost::beast::http::field::user_agent, BOOST_BEAST_VERSION_STRING);
            req.set(boost::beast::http::field::authorization, "Bearer " + accessToken);

            // Отправляем запрос и читаем ответ
            boost::beast::http::write(stream, req);
            boost::beast::flat_buffer buffer;
            boost::beast::http::response<boost::beast::http::string_body> res;
            boost::beast::http::read(stream, buffer, res);

            // Завершаем SSL соединение
            boost::system::error_code ec;
            stream.shutdown(ec);

            // Проверка статуса ответа
            if (res.result_int() != 200) {
                TracerManager::Instance().AddSpanLog("ошибка в currentUserFull. Ответ сервера валидации SSL", {
                                                             {"url", url},
                                                             {"requets body", res.body()},
                                                             {"status", std::to_string(static_cast<int>(res.result_int()))}
                                                     },
                                                     TracerManager::LOG_LEVEL_ERROR);
                status = http::status::unauthorized;
                return res.body();
            }

            return res.body();
        } else {
            // HTTP Socket
            boost::asio::ip::tcp::resolver resolver(ioc);
            boost::asio::ip::tcp::socket socket(ioc);

            // Resolve
            auto const results = resolver.resolve(host, port);
            if (results.empty()) {
                TracerManager::Instance().AddSpanLog("ошибка в currentUserFull. Host not found. Socket", {
                                                             {"url", url},
                                                             {"host", host},
                                                             {"port", port}
                                                             },
                                                     TracerManager::LOG_LEVEL_ERROR);
                throw std::runtime_error("Host not found: " + host);
            }

            // Connect и запрос
            boost::asio::connect(socket, results.begin(), results.end());
            boost::beast::http::request<boost::beast::http::string_body> req{boost::beast::http::verb::get, target, 11};
            req.set(boost::beast::http::field::host, host);
            req.set(boost::beast::http::field::user_agent, BOOST_BEAST_VERSION_STRING);
            req.set(boost::beast::http::field::authorization, "Bearer " + accessToken);

            // Отправляем запрос и читаем ответ
            boost::beast::http::write(socket, req);
            boost::beast::flat_buffer buffer;
            boost::beast::http::response<boost::beast::http::string_body> res;
            boost::beast::http::read(socket, buffer, res);

            // Завершаем соединение
            boost::system::error_code ec;
            socket.shutdown(boost::asio::ip::tcp::socket::shutdown_both, ec);

            // Проверка статуса ответа
            if (res.result_int() != 200) {
                TracerManager::Instance().AddSpanLog("ошибка в currentUserFull. Ответ сервера валидации.Socket", {
                                                             {"url", url},
                                                             {"requets body", res.body()},
                                                             {"status", std::to_string(static_cast<int>(res.result_int()))}
                                                     },
                                                     TracerManager::LOG_LEVEL_ERROR);
                status = http::status::unauthorized;
                return res.body();
            }

            return res.body();
        }
    } catch (const std::exception& e) {
        TracerManager::Instance().AddSpanLog("ошибка в currentUserFull. Исключение", {
                                                     {"url", url},
                                                     {"reason", e.what()}
                                             },
                                             TracerManager::LOG_LEVEL_ERROR);
        status = http::status::unauthorized;
        return std::string("Error: ") + e.what();
    }
}

std::string Auth::getLocalUserFull(int user_id) {
    std::string response = "";
    std::string lastName;
    std::string firstName;
    std::string middleName;
    if (TDB_User::GetUserFamImjOtc(user_id, lastName, firstName, middleName)) {
        response = "{\"userPersonalInfo\": {\"firstName\": \"" + firstName
                + "\", " + "\"lastName\": \"" + lastName + "\" } }";
    }
    return response;
}

std::string Auth::CheckUserProfilIdFromUserFull(std::string UserFullJsonStr, boost::beast::http::status &status, int exp, std::string accessToken) {
    status = boost::beast::http::status::ok;
    std::string ws = "";

    try {
        // Парсим JSON строку
        auto jsonObject = nlohmann::json::parse(UserFullJsonStr);

        // Извлекаем значение profileId
        if (jsonObject.contains("profileId")) {
            int profileId;
            if (jsonObject["profileId"].is_string()) {
                profileId = StrToInt(jsonObject["profileId"].get<std::string>());
            } else if (jsonObject["profileId"].is_number_integer()) {
                profileId = jsonObject["profileId"].get<int>();
            } else {
                status = boost::beast::http::status::unauthorized;
                ws = "Invalid profileId type in JSON";
                return ws;
            }

            TDateTime expDate = UnixTimestampToDateTime(exp);
            int userid = TDB_User::GetUserByProfileID(profileId);

            if (userid == 0) {
                status = boost::beast::http::status::see_other;
                ws = "User not found by profileId: " + std::to_string(profileId);
            } else {
                if (TDB_User::SaveUserToken(userid, accessToken, expDate)) {
                    ws = "{\"context_id\": " + std::to_string(userid) + "}";
                } else {
                    status = boost::beast::http::status::unauthorized;
                    ws = "Can't save token";
                }
            }
        } else {
            status = boost::beast::http::status::unauthorized;
            ws = "Key 'profileId' not found in JSON";
        }
    } catch (const nlohmann::json::parse_error& e) {
        status = boost::beast::http::status::unauthorized;
        ws = "JSON parse error: " + std::string(e.what());
    } catch (const std::exception& e) {
        status = boost::beast::http::status::internal_server_error;
        ws = "Internal server error: " + std::string(e.what());
    }

    return ws;
}


bool Auth::checkUserAdm(std::string accessToken) {
    int profileId;
    int exp;
    std::string res = decode_jwt(accessToken, exp, profileId);
    int userid = TDB_User::GetUserByProfileID(profileId);
    int UserAdm = TDB_User::GetUserAdm(userid);
    if (UserAdm == 2) {
        return true;
    }
    return false;
}


