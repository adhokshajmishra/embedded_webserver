#ifndef FLEET_REQUESTROUTER_H
#define FLEET_REQUESTROUTER_H

#include <functional>
#include <string>
#include <unordered_map>
#include <vector>

#include "http_common.h"

class ChainRouter
{
private:
    std::string path;
    std::vector<std::function<HTTPMessage(const HTTPMessage&)>> common_handler, get_handler, post_handler, put_handler, delete_handler, head_handler;
public:
    ChainRouter route(std::string);
    ChainRouter get(std::function<HTTPMessage(const HTTPMessage&)>);
    ChainRouter put(std::function<HTTPMessage(const HTTPMessage&)>);
    ChainRouter post(std::function<HTTPMessage(const HTTPMessage&)>);
    ChainRouter delete_(std::function<HTTPMessage(const HTTPMessage&)>);
    ChainRouter head(std::function<HTTPMessage(const HTTPMessage&)>);
    ChainRouter all(std::function<HTTPMessage(const HTTPMessage&)>);
    HTTPMessage operator()(const HTTPMessage&);

    friend std::string destination(const ChainRouter& router);
};

class RequestRouter {
private:
    std::unordered_map<std::string, ChainRouter> route_handler;
    std::function<HTTPMessage(const std::string&, const HTTPMessage&)> default_handler;
    std::function<bool(const std::string&, HTTPMessage&)> pre_handler, post_handler;
protected:
public:
    RequestRouter();
    RequestRouter(std::function<HTTPMessage(const std::string&, const HTTPMessage&)> default_request_handler,
                  std::function<bool(const std::string&, HTTPMessage&)> pre_invoke_handler,
                  std::function<bool(const std::string&, HTTPMessage&)> post_invoke_handler);
    ChainRouter& operator[](const std::string& path);
    RequestRouter use(const ChainRouter&);
    RequestRouter use(const std::vector<ChainRouter>&);
    HTTPMessage run(const std::string&, HTTPMessage&);
};

HTTPMessage default_req_handler(const std::string& destination, const HTTPMessage& request);

#endif //FLEET_REQUESTROUTER_H
