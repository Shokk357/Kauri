cmake_minimum_required(VERSION 3.15)
project(Kauri)

set(CMAKE_CXX_STANDARD 17)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread -Wall")

add_executable(ClientPart Client/client.cpp ASN/ASN.cpp)

add_executable(ServerPart Server/server.cpp ASN/ASN.cpp)