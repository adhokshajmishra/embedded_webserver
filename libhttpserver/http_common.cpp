#include "http_common.h"
#include <iostream>

std::ostream& operator<<(std::ostream& lhs, RequestType type) {
    switch(type) {
        case ALL: lhs << "ALL"; break;
        case HEAD: lhs << "HEAD"; break;
        case GET: lhs << "GET"; break;
        case POST: lhs << "POST"; break;
        case PUT: lhs << "PUT"; break;
        case PATCH: lhs << "PATCH"; break;
        case DELETE: lhs << "DELETE"; break;
        case QUERY: lhs << "QUERY"; break;
        case OPTIONS: lhs << "OPTIONS"; break;
        default: lhs << "unknown request type (" << static_cast<int>(type) << ")";
    }
    return lhs;
}

HTTPMessage::HTTPMessage()
{
    this->isRequest = false;
    this->status = boost::beast::http::status::ok;
}