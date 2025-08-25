#include "Common.h"
#include "json.hpp"
#include "dmGlobalVars.h"
#include <iostream>
#include <boost/beast/version.hpp>

Common::Common(const std::string& host)
        : host_(host), ioc_(), resolver_(ioc_), stream_(ioc_) {}

std::string Common::RequestStateOpen(const std::string& basePath, const std::string& xRequestId, int& responseStatus) {
    try {
        std::string hostname = host_;
        std::string port = "80"; // Порт по умолчанию
        size_t colon_pos = host_.find(':');
        if (colon_pos != std::string::npos) {
            hostname = host_.substr(0, colon_pos); // Извлекаем имя хоста
            port = host_.substr(colon_pos + 1);    // Извлекаем порт
        }
        // Проверка подключения
     //std::cout << "Resolving host: " << host_ << std::endl;
        auto const results = resolver_.resolve(hostname, port);
     //std::cout << "Attempting to connect..." << std::endl;
        stream_.connect(results);
     //std::cout << "Connected to host: " << host_ << std::endl;

        // Формирование запроса
        std::string target = basePath + "/Common/RequestStateOpen";
        http::request<http::string_body> req{http::verb::post, target, 11};
        req.set(http::field::host, hostname);
        req.set(http::field::content_type, "application/json");
        req.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);
        req.set("From-Service", ServiceName);
        req.body() = R"({"x-Request-ID":")" + xRequestId + R"("})";
        req.prepare_payload();

        // Логируем запрос
     //std::cout << "Request details:" << std::endl;
     //std::cout << "Target: " << target << std::endl;
     //std::cout << "Body: " << req.body() << std::endl;

        // Отправка запроса
        stream_.expires_after(std::chrono::seconds(10));  // Таймаут 10 секунд
        http::write(stream_, req);
     //std::cout << "Request sent." << std::endl;

        // Чтение ответа
        boost::beast::flat_buffer buffer;
        http::response<http::string_body> res;
     //std::cout << "Reading response..." << std::endl;
        http::read(stream_, buffer, res);
     //std::cout << "Response read successfully." << std::endl;

        // Логируем ответ
        responseStatus = res.result_int();
     //std::cout << "Response status: " << responseStatus << std::endl;
     //std::cout << "Response body: " << res.body() << std::endl;

        // Закрываем соединение
        stream_.socket().shutdown(tcp::socket::shutdown_both);
        return res.body();
    } catch (const boost::system::system_error& e) {
        std::cerr << "Boost system error: " << e.what() << std::endl;
        responseStatus = -1;
        return "Server error";
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        responseStatus = -1;
        return "Error occurred";
    }
}


std::string Common::RequestStateClose(const std::string& basePath, const std::string& xRequestId,
                                      const std::string& response, int responseCode, int& responseStatus) {
    try {
        // Резолвим только host_
        std::string hostname = host_;
        std::string port = "80"; // Порт по умолчанию
        size_t colon_pos = host_.find(':');
        if (colon_pos != std::string::npos) {
            hostname = host_.substr(0, colon_pos); // Извлекаем имя хоста
            port = host_.substr(colon_pos + 1);    // Извлекаем порт
        }
        auto const results = resolver_.resolve(hostname, port);
        stream_.connect(results);

        // Формируем путь для запроса
        std::string target = basePath + "/Common/RequestStateClose";

        // Создаём HTTP-запрос
        http::request<http::string_body> req{http::verb::post, target, 11};
        req.set(http::field::host, hostname);
        req.set(http::field::content_type, "application/json");
        req.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);
        req.set("From-Service", ServiceName);

        // Формируем тело запроса
        nlohmann::json requestBody = {
                {"x-Request-ID", xRequestId},
                {"response", response},
                {"response_code", responseCode}
        };
        req.body() = requestBody.dump();
        req.prepare_payload();

        // Отправляем HTTP-запрос
        http::write(stream_, req);

        // Читаем ответ
        boost::beast::flat_buffer buffer;
        http::response<http::string_body> res;
        http::read(stream_, buffer, res);

        // Возвращаем код ответа
        responseStatus = res.result_int();

        // Закрываем соединение
        stream_.socket().shutdown(tcp::socket::shutdown_both);

        return res.body();
    } catch (const std::exception& e) {
     //std::cout << "Error during RequestStateClose: " << e.what() << std::endl;
        responseStatus = -1;
        return "";
    }
}
