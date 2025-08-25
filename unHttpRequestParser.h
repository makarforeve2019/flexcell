
#ifndef MWSRESTAPI_UNHTTPREQUESTPARSER_H
#define MWSRESTAPI_UNHTTPREQUESTPARSER_H

#include <string>
#include <map>
#include <vector>

// Класс для разбора HTTP-запросов
class HttpRequestParser {
public:
    // Конструктор, который принимает запрос в виде строки
    HttpRequestParser(const std::string &request);

    // Получение метода HTTP-запроса
    const std::string& get_method() const;

    // Получение URL из запроса
    const std::string& get_url() const;

    // Получение версии HTTP-протокола
    const std::string& get_version() const;

    // Получение заголовков запроса
    const std::map<std::string, std::string>& get_headers() const;

    // Получение тела запроса
    const std::string& get_body() const;

    std::vector<std::string> get_path_parts() const;


private:
    // Поля для хранения данных запроса
    std::string method_;
    std::string url_;
    std::string version_;
    std::map<std::string, std::string> headers_;
    std::string body_;

    // Метод для парсинга всего запроса
    void parse(const std::string &request);

    // Метод для парсинга заголовков
    void parse_headers(std::istringstream &stream);
};


#endif //MWSRESTAPI_UNHTTPREQUESTPARSER_H
