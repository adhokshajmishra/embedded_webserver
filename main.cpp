#include <iostream>
#include "./libhttpserver/RequestRouter.h"
#include "./libhttpserver/WebServer.h"

int main() {
    RequestRouter router;
    router["/hello"].get([](const HTTPMessage& req) -> HTTPMessage
                         {
                            HTTPMessage response;
                            response.body = "hello world!";
                            response.status = boost::beast::http::status::ok;
                            return response;
                         });
    WebServer server(router, "localhost", 8888);
    server.setTlsCertificates("/tmp/ssl/localhost_certificate.crt",
                              "/tmp/ssl/localhost_private.key",
                              "/tmp/ssl/diffey_hellman.pem",
                              "private_key_password");
    server.run();
    return 0;
}
