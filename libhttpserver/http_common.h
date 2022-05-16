#ifndef FLEET_HTTP_COMMON_H
#define FLEET_HTTP_COMMON_H

#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/ssl.hpp>
#include <boost/beast/version.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/ssl/stream.hpp>

#include <string>
#include <unordered_map>

enum RequestType
{
    HEAD,
    GET,
    POST,
    PUT,
    DELETE,
    OPTIONS
};

struct HTTPMessage
{
    bool isRequest;
    RequestType type;
    boost::beast::http::status status;
    std::unordered_map<std::string, std::string> header;
    std::unordered_map<std::string, std::string> query;
    std::string body;

    HTTPMessage();
};

#endif //FLEET_HTTP_COMMON_H
