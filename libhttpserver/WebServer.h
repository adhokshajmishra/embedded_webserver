//
// Created by adhokshajmishra on 12/10/20.
//

#ifndef FLEET_WEBSERVER_H
#define FLEET_WEBSERVER_H

#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/ssl.hpp>
#include <boost/beast/version.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/ssl/stream.hpp>
#include <boost/config.hpp>
#include <cstdlib>
#include <functional>
#include <iostream>
#include <memory>
#include <string>
#include <thread>
#include <unordered_map>
#include <vector>

#include <memory>
#include "http_common.h"
#include "RequestRouter.h"

class WebServer {
private:
    std::string ssl_certificate, ssl_private_key, diffie_hellman_key, private_key_password;
    std::string host;
    unsigned short port;
    void do_session( boost::asio::ip::tcp::socket& socket, boost::asio::ssl::context& ctx);
    template<class Body, class Allocator, class Send> void handle_request(boost::beast::http::request<Body,
            boost::beast::http::basic_fields<Allocator>>&& req, Send&& send);
    RequestRouter router;
public:
    WebServer(RequestRouter router, std::string host = "0.0.0.0", unsigned short port = 1234);
    void setTlsCertificates(std::string ssl_certificate, std::string ssl_private_key, std::string diffie_hellman_key, std::string private_key_password);
    void run();
};


#endif //FLEET_WEBSERVER_H
