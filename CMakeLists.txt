cmake_minimum_required(VERSION 3.22)
project(embedded_webserver)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(${PROJECT_SOURCE_DIR}/libhttpserver)
add_executable(embedded_webserver main.cpp)
target_link_libraries(embedded_webserver libhttpserver)
