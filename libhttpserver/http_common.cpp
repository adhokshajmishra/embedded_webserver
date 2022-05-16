#include "http_common.h"

HTTPMessage::HTTPMessage()
{
    this->isRequest = false;
    this->status = boost::beast::http::status::ok;
}