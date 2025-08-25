#ifndef COMMON_H
#define COMMON_H

#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <string>

namespace http = boost::beast::http;
using tcp = boost::asio::ip::tcp;

class Common {
public:
    Common(const std::string& host);
    std::string RequestStateOpen(const std::string& basePath, const std::string& xRequestId, int& responseStatus);
    std::string RequestStateClose(const std::string& basePath, const std::string& xRequestId,
                                  const std::string& response, int responseCode, int& responseStatus);


private:
    std::string host_;
    std::string port_;
    boost::asio::io_context ioc_;
    tcp::resolver resolver_;
    boost::beast::tcp_stream stream_;
};

#endif // COMMON_H
