#ifndef USER_MANAGER_H
#define USER_MANAGER_H

#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <string>
#include "json.hpp"

namespace http = boost::beast::http;
using tcp = boost::asio::ip::tcp;
using json = nlohmann::json;

struct Response {
    json data;
    std::string body;
    int status;
};

class UserManager {
public:
    UserManager(const std::string& host);

    // Методы для работы с пользователями
    Response UpdateUserPermissions(const std::string& token, int& responseStatus);
    Response CreateUser(const std::string& token, int& responseStatus);

private:
    std::string host_;  // Хост сервера
    boost::asio::io_context ioc_;  // Контекст ввода-вывода
    tcp::resolver resolver_;  // Резолвер для DNS
    boost::beast::tcp_stream stream_;  // TCP поток для передачи данных

    // Метод отправки POST-запроса
    Response sendPostRequest(const std::string& target, const std::string& token, int& responseStatus);
};

#endif // USER_MANAGER_H
