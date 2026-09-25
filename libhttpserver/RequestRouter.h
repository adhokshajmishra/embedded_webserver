#ifndef FLEET_REQUESTROUTER_H
#define FLEET_REQUESTROUTER_H

#include <functional>
#include <string>
#include <unordered_map>
#include <vector>

#include "http_common.h"

class ChainRouter
{
public:
    using Handler = std::function<HTTPMessage(const HTTPMessage&)>;

private:
    std::string path;
    std::unordered_map<RequestType, std::vector<Handler>> request_handlers;
public:
    ChainRouter route(std::string);

    ChainRouter addHandler(RequestType, Handler);
    ChainRouter get(Handler);
    ChainRouter put(Handler);
    ChainRouter post(Handler);
    ChainRouter patch(Handler);
    ChainRouter delete_(Handler);
    ChainRouter head(Handler);
    ChainRouter query(Handler);
    ChainRouter all(Handler);

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
