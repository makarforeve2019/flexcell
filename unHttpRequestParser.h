
#ifndef MWSRESTAPI_UNHTTPREQUESTPARSER_H
#define MWSRESTAPI_UNHTTPREQUESTPARSER_H

#include <string>
#include <map>
#include <vector>

// ����� ��� ������� HTTP-��������
class HttpRequestParser {
public:
    // �����������, ������� ��������� ������ � ���� ������
    HttpRequestParser(const std::string &request);

    // ��������� ������ HTTP-�������
    const std::string& get_method() const;

    // ��������� URL �� �������
    const std::string& get_url() const;

    // ��������� ������ HTTP-���������
    const std::string& get_version() const;

    // ��������� ���������� �������
    const std::map<std::string, std::string>& get_headers() const;

    // ��������� ���� �������
    const std::string& get_body() const;

    std::vector<std::string> get_path_parts() const;


private:
    // ���� ��� �������� ������ �������
    std::string method_;
    std::string url_;
    std::string version_;
    std::map<std::string, std::string> headers_;
    std::string body_;

    // ����� ��� �������� ����� �������
    void parse(const std::string &request);

    // ����� ��� �������� ����������
    void parse_headers(std::istringstream &stream);
};


#endif //MWSRESTAPI_UNHTTPREQUESTPARSER_H
