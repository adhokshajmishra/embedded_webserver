#include "RequestRouter.h"

std::string destination(const ChainRouter& router)
{
    return router.path;
}

bool default_invoke_handler(const std::string& destination, HTTPMessage& request)
{
    return true;
}

HTTPMessage default_req_handler(const std::string& destination, const HTTPMessage& request)
{
    HTTPMessage response;

    std::stringstream ss;
    ss << "Requested destination [" << request.type << " " << destination << "] does not exist.\n\nQuery parameters:\n";

    for (const auto& it : request.query)
    {
        ss << it.first << " : " << it.second << std::endl;
    }

    ss << "\nHeaders:\n";
    for (const auto& it : request.header)
    {
        ss << it.first << " : " << it.second << std::endl;
    }

    std::string reply = ss.str();
    response.status = boost::beast::http::status::not_found;
    response.body = reply;
    response.header["Content-Type"] = "text/plain";
    return response;
}

ChainRouter ChainRouter::route(std::string path)
{
    this->path = path;
    return *this;
}

ChainRouter ChainRouter::addHandler(RequestType type, ChainRouter::Handler handler)
{
    request_handlers[type].push_back(handler);
    return *this;
}

ChainRouter ChainRouter::all(ChainRouter::Handler handler)
{
    return addHandler(ALL, handler);
}

ChainRouter ChainRouter::put(ChainRouter::Handler handler)
{
    return addHandler(PUT, handler);
}

ChainRouter ChainRouter::patch(ChainRouter::Handler handler)
{
    return addHandler(PATCH, handler);
}

ChainRouter ChainRouter::get(ChainRouter::Handler handler)
{
    return addHandler(GET, handler);
}

ChainRouter ChainRouter::post(ChainRouter::Handler handler)
{
    return addHandler(POST, handler);
}

ChainRouter ChainRouter::delete_(ChainRouter::Handler handler)
{
    return addHandler(DELETE, handler);
}

ChainRouter ChainRouter::head(ChainRouter::Handler handler)
{
    return addHandler(HEAD, handler);
}

ChainRouter ChainRouter::query(ChainRouter::Handler handler)
{
    return addHandler(QUERY, handler);
}

HTTPMessage ChainRouter::operator()(const HTTPMessage& request)
{
    HTTPMessage response, interim_request = request;
    bool isProcessed = false;

    switch(request.type)
    {
        case RequestType::HEAD:
        case RequestType::GET:
        case RequestType::POST:
        case RequestType::PUT:
        case RequestType::PATCH:
        case RequestType::DELETE:
        case RequestType::QUERY:
            for (const auto& handler : request_handlers[request.type])
            {
                isProcessed = true;
                interim_request = handler(interim_request);
                if (!interim_request.isRequest)
                {
                    response = interim_request;
                    break;
                }
            }
            break;
        case RequestType::OPTIONS:
            {
                isProcessed = true;
                std::vector<RequestType> allowed_methods = {OPTIONS};
                for (const auto& [type, handlers] : request_handlers)
                    if (type == ALL) {
                        // all methods are allowed if there is a catch-all handler except the default one
                        if(handlers.size() > 1){
                            allowed_methods = {HEAD, GET, POST, PUT, PATCH, DELETE, QUERY, OPTIONS};
                            break;
                        }
                        // else, skip default handler
                    }
                    else {
                        allowed_methods.push_back(type);
                    }

                // https://stackoverflow.com/a/9279620
                std::stringstream allowed_methods_ss;
                for (const auto& method : allowed_methods) {
                    if (&method != &allowed_methods[0]) {
                        allowed_methods_ss << ", ";
                    }
                    allowed_methods_ss << method;
                }

                response.header["Allow"] = allowed_methods_ss.str();
                response.header["Access-Control-Allow-Methods"] = allowed_methods_ss.str();
            }
            break;
        default:
            isProcessed = true;
            response.status = boost::beast::http::status::not_implemented;
    }

    if (!isProcessed)
    {
        for (const auto& handler : request_handlers[ALL])
        {
            isProcessed = true;
            interim_request = handler(interim_request);
            if (!interim_request.isRequest)
            {
                response = interim_request;
                break;
            }
        }
    }

    if (!isProcessed)
        response.status = boost::beast::http::status::not_found;

    return response;
}

RequestRouter RequestRouter::use(const ChainRouter& router)
{
    route_handler[destination(router)] = router;
    return *this;
}

RequestRouter RequestRouter::use(const std::vector<ChainRouter>& routers)
{
    for (const auto& router : routers)
    {
        route_handler[destination(router)] = router;
    }
    return *this;
}

HTTPMessage RequestRouter::run(const std::string& path, HTTPMessage& request)
{
    HTTPMessage response;

    if (this->pre_handler(path, request))
    {
        response = this->operator[](path)(request);
        this->post_handler(path, response);
    }
    else
    {
        response.status = boost::beast::http::status::unauthorized;
    }

    return response;
}

ChainRouter& RequestRouter::operator[](const std::string& destination)
{
    if (route_handler.find(destination) == route_handler.end())
    {
        auto handler = std::bind(this->default_handler, destination, std::placeholders::_1);
        ChainRouter router;
        router.route(destination);
        route_handler[destination] = router;
        route_handler[destination].all(handler);
    }

    return route_handler[destination];
}

RequestRouter::RequestRouter()
{
    this->default_handler = default_req_handler;
    this->pre_handler = default_invoke_handler;
    this->post_handler = default_invoke_handler;
}

RequestRouter::RequestRouter(std::function<HTTPMessage(const std::string&, const HTTPMessage&)> default_request_handler,
                             std::function<bool(const std::string &, HTTPMessage &)> pre_invoke_handler,
                             std::function<bool(const std::string &, HTTPMessage &)> post_invoke_handler)
{
    this->default_handler = std::move(default_request_handler);
    this->pre_handler = std::move(pre_invoke_handler);
    this->post_handler = std::move(post_invoke_handler);
}