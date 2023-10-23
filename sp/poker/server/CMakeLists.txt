
cmake_minimum_required(VERSION 3.10)
project(server)

add_subdirectory("libs/json")

# You don't need to manually include log4cxx headers or libraries

add_executable(server src/server.cpp
        src/connection/request/request.hpp
        src/connection/payload/payload.hpp
        src/connection/payload/payload.cpp
        src/connection/status.hpp
        src/connection/type.cpp
        src/connection/subtype.cpp
)

target_link_libraries(server PRIVATE nlohmann_json::nlohmann_json pthread)