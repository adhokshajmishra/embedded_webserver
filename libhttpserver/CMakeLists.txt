cmake_minimum_required(VERSION 3.16)
project(libhttpserver)

set(CMAKE_CXX_STANDARD 17)

add_library(libhttpserver http_common.cpp http_common.h RequestRouter.cpp RequestRouter.h ssl_certificate.h WebServer.cpp WebServer.h)
target_link_libraries(libhttpserver -lboost_thread)
target_link_libraries(libhttpserver -lboost_system)
target_link_libraries(libhttpserver -lssl)
target_link_libraries(libhttpserver -lcrypto)
target_link_libraries(libhttpserver -lpthread)