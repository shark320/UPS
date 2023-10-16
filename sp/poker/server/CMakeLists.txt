
cmake_minimum_required(VERSION 3.10)
project(server)

add_subdirectory("libs/json")

# You don't need to manually include log4cxx headers or libraries

add_executable(server src/server.cpp
        src/api/type.hpp
        src/api/request/Request.hpp
        src/api/payload/payload.hpp)

target_link_libraries(server PRIVATE nlohmann_json::nlohmann_json)