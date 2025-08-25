
#ifndef MWSRESTAPI_RS_AUTH_H
#define MWSRESTAPI_RS_AUTH_H


#include <string>
#include <boost/beast/http.hpp>
namespace http = boost::beast::http;

class Auth {
public:
    static std::string CheckUserPermissions(std::string &url, std::string accessToken, boost::beast::http::status& status);
    static std::string CheckUserPermissionsOnlyEXP(std::string &url, std::string accessToken, boost::beast::http::status& status, int& exp);
    static std::string CheckUserProfilIdFromUserFull(std::string UserFullJsonStr, boost::beast::http::status &status, int exp, std::string accessToken);
    static std::string ValidateTokenLocallyByStoredToken(std::string accessToken, boost::beast::http::status& status);
    static std::string ValidateTokenLocally(std::string accessToken, boost::beast::http::status& status);
    static std::string CheckToken(std::string &url, std::string &accessToken, boost::beast::http::status &status);
    static std::string currentUserFull(std::string &url, std::string &accessToken, boost::beast::http::status &status);
    static std::string getLocalUserFull(int user_id);
    static bool checkUserAdm(std::string accessToken);
};


#endif //MWSRESTAPI_RS_AUTH_H
