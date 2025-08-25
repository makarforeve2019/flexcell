//
// Created by Alex on 14.04.2024.
//

#ifndef MWSRESTAPI_UNWEBMODULEUNIT_H
#define MWSRESTAPI_UNWEBMODULEUNIT_H

#include <string>
#include <map>
#include <vector>
#include <boost/beast/http.hpp>

namespace http = boost::beast::http;

extern int GlobalPort;
extern int AuthOption;
extern std::string AuthExternalType;
extern std::string AuthExternalServer;
extern std::string DefaultContext;
extern bool UpdateUserPermissions;
extern bool CreateUser;


std::string handle_request(const std::string& body, const std::string& url, std::string& authHeader, boost::beast::http::status& status, http::response<http::string_body>& httpresponse, std::string adminPermissionHeader, std::string  xRequestId, std::string xContextId);
void setGlobalPort(int port);
int getGlobalPort();

#endif //MWSRESTAPI_UNWEBMODULEUNIT_H
