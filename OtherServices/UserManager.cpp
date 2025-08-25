#include "UserManager.h"
#include "dmGlobalVars.h"
#include <boost/beast/http.hpp>
#include <boost/beast/core.hpp>
#include <sstream>

UserManager::UserManager(const std::string& host)
        : host_(host), resolver_(ioc_), stream_(ioc_) {}

Response UserManager::UpdateUserPermissions(const std::string& token, int& responseStatus) {  // Убрано UserManager::
    return sendPostRequest("/Role/UpdateUserPermissions", token, responseStatus);
}

Response UserManager::CreateUser(const std::string& token, int& responseStatus) {  // Убрано UserManager::
    return sendPostRequest("/Role/CreateUser", token, responseStatus);
}

Response UserManager::sendPostRequest(const std::string& target, const std::string& token, int& responseStatus) {  // Убрано UserManager::
    Response result;
    try {
        std::cout << "Resolving host: " << host_ << " with service: http" << std::endl;
        std::string hostname = host_;
        std::string port = "80"; // Порт по умолчанию
        size_t colon_pos = host_.find(':');
        if (colon_pos != std::string::npos) {
            hostname = host_.substr(0, colon_pos);
            port = host_.substr(colon_pos + 1);
        }
        auto const results = resolver_.resolve(hostname, port);
        std::cout << "Connection resolving successful. Connecting..." << std::endl;
        stream_.connect(results);
        std::cout << "Connected to host: " << hostname << std::endl;

        http::request<http::string_body> req{http::verb::post, target, 11};
        req.set(http::field::host, hostname);
        req.set(http::field::authorization, token);
        req.set(http::field::content_type, "application/json");
        req.set("From-Service", ServiceName);
        req.prepare_payload();

        std::cout << "Sending request to: " << target << std::endl;
        std::cout << "Headers:" << std::endl;
        for (const auto& header : req) {
            std::cout << "  " << header.name_string() << ": " << header.value() << std::endl;
        }
        std::cout << "Body: " << req.body() << std::endl;

        http::write(stream_, req);
        std::cout << "Request sent successfully." << std::endl;

        boost::beast::flat_buffer buffer;
        http::response<http::string_body> res;
        std::cout << "Reading response..." << std::endl;
        http::read(stream_, buffer, res);
        std::cout << "Response read successfully." << std::endl;

        responseStatus = res.result_int();
        result.body = res.body();
        result.status = responseStatus;
        std::cout << "Response status: " << responseStatus << std::endl;
        std::cout << "Response body: " << res.body() << std::endl;

        if (responseStatus == 200) {
            result.data = json::parse(res.body());
        } else {
            result.data = json{{"error", "Request failed"}, {"status", responseStatus}};
            try {
                if (!res.body().empty()) {
                    result.data = json::parse(res.body());
                }
            } catch (const json::parse_error&) {
                // Если тело не JSON, оставляем как есть
            }
        }

        stream_.socket().shutdown(tcp::socket::shutdown_both);
    } catch (const boost::system::system_error& e) {
        std::cerr << "Boost system error: " << e.what() << " (" << e.code() << ")" << std::endl;
        responseStatus = 500;
        result.status = responseStatus;
        result.data = json{{"error", e.what()}, {"code", e.code().value()}, {"message", e.code().message()}, {"status", responseStatus}};
        result.body = "";
    } catch (const std::exception& e) {
        std::cerr << "Standard exception: " << e.what() << std::endl;
        responseStatus = 500;
        result.status = responseStatus;
        result.data = json{{"error", e.what()}, {"status", responseStatus}};
        result.body = "";
    } catch (...) {
        std::cerr << "Unknown error occurred." << std::endl;
        responseStatus = 500;
        result.status = responseStatus;
        result.data = json{{"error", "Unknown error"}, {"status", responseStatus}};
        result.body = "";
    }
    return result;
}
