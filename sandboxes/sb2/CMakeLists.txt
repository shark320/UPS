set(CMAKE_CXX_STANDARD 17)
set(SOURCE_DIR ${CMAKE_SOURCE_DIR})
set(BUILD_DIR ${CMAKE_BINARY_DIR})
set(LOG4CXX_PROPERTIES_SOURCE ${SOURCE_DIR}/config/log4cxx.properties)
set(LOG4CXX_PROPERTIES_DEST ${BUILD_DIR}/config/log4cxx.properties)
cmake_minimum_required(VERSION 3.10)
project(sb2)

# Locate and configure the log4cxx package

# You don't need to manually include log4cxx headers or libraries

add_executable(testapp main.cpp)

add_custom_command(
    TARGET testapp POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy ${LOG4CXX_PROPERTIES_SOURCE} ${LOG4CXX_PROPERTIES_DEST}
)

target_link_libraries(testapp PRIVATE pthread log4cxx)
