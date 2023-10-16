cmake_minimum_required(VERSION 3.1)
project(sb3)

add_subdirectory("libs/json")
set(CMAKE_CXX_STANDARD 17)

add_executable(sb3 main.cpp)

target_link_libraries(sb3 PRIVATE nlohmann_json::nlohmann_json)
