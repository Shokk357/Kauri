cmake_minimum_required(VERSION 3.15)
project(Kauri)

set(CMAKE_CXX_STANDARD 17)

include_directories(ACE_wrappers)

link_directories(ACE_wrappers/lib)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread -Wall")

add_executable(ClientPart Client/client.cpp ASN/ASN.cpp)

add_executable(ServerPart Server/server.cpp ASN/ASN.cpp)

target_link_libraries(ClientPart ACE)
target_link_libraries(ServerPart ACE)
