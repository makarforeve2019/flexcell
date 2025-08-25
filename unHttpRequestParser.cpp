#include "unHttpRequestParser.h"
#include <sstream>
#include <iostream>
#include <boost/algorithm/string.hpp>

// Конструктор, который принимает запрос в виде строки
HttpRequestParser::HttpRequestParser(const std::string &request) {
    parse(request);
}

// Получение метода HTTP-запроса
const std::string& HttpRequestParser::get_method() const {
    return method_;
}

// Получение URL из запроса
const std::string& HttpRequestParser::get_url() const {
    return url_;
}

// Получение версии HTTP-протокола
const std::string& HttpRequestParser::get_version() const {
    return version_;
}

// Получение заголовков запроса
const std::map<std::string, std::string>& HttpRequestParser::get_headers() const {
    return headers_;
}

// Получение тела запроса
const std::string& HttpRequestParser::get_body() const {
    return body_;
}

// Метод для парсинга всего запроса
void HttpRequestParser::parse(const std::string &request) {
    size_t header_end_pos = request.find("\r\n\r\n");
    if (header_end_pos != std::string::npos) {
        std::istringstream request_stream(request.substr(0, header_end_pos + 4));

        std::string request_line;
        std::getline(request_stream, request_line);

        std::vector<std::string> request_parts;
        boost::split(request_parts, request_line, boost::is_any_of(" "));

        if (request_parts.size() >= 3) {
            method_ = request_parts[0];
            url_ = request_parts[1];
            version_ = request_parts[2];
        }

        parse_headers(request_stream);

        if (headers_.find("Content-Length") != headers_.end()) {
            size_t content_length = std::stoul(headers_["Content-Length"]);
            size_t body_start_pos = header_end_pos + 4; // после \r\n\r\n
            body_ = request.substr(body_start_pos, content_length);

            // Проверяем считанное тело
//            std::cout << "Read body: " << body_ << std::endl;
        }
    }
}

// Метод для парсинга заголовков
void HttpRequestParser::parse_headers(std::istringstream &stream) {
    std::string line;
    while (std::getline(stream, line) && !line.empty()) {
        size_t delimiter_pos = line.find(":");
        if (delimiter_pos != std::string::npos) {
            std::string key = line.substr(0, delimiter_pos);
            std::string value = line.substr(delimiter_pos + 1);
            boost::trim(key);
            boost::trim(value);
            headers_[key] = value;
        }
    }
}

std::vector<std::string> HttpRequestParser::get_path_parts() const {
    std::vector<std::string> parts;
    boost::split(parts, url_, boost::is_any_of("/"), boost::token_compress_on);
    return parts;
}
