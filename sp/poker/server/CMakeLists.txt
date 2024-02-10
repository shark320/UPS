
cmake_minimum_required(VERSION 3.10)
project(server)

set(LOG4CXX_INCLUDE_DIRS "/usr/include/log4cxx")
set(LOG4CXX_LIBRARIES "/usr/lib/x86_64-linux-gnu/liblog4cxx.so")
set(CONFIG_DIR "config")
set(CONFIG_LOG_DIR "${CONFIG_DIR}/logging")
set(SRC_RES_DIR "src/main/resources")
set(CONFIG_SRC_DIR "${SRC_RES_DIR}/config")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/assembly)

find_package(log4cxx REQUIRED)
# You don't need to manually include log4cxx headers or libraries

message("LOG4CXX_INCLUDE_DIRS: ${LOG4CXX_INCLUDE_DIRS}")
message("LOG4CXX_LIBRARIES: ${LOG4CXX_LIBRARIES}")

add_executable(server src/main/cpp/server.cpp
        src/main/cpp/connection/message/message.hpp
        src/main/cpp/connection/message/message.cpp
        src/main/cpp/connection/message/payload/payload.hpp
        src/main/cpp/connection/message/payload/payload.cpp
        src/main/cpp/base/base.cpp
        src/main/cpp/base/base.hpp
        src/main/cpp/connection/message/header/header.cpp
        src/main/cpp/connection/message/header/header.hpp
        src/main/cpp/connection/message/enums/type.cpp
        src/main/cpp/connection/message/enums/type.hpp
        src/main/cpp/connection/message/enums/status.cpp
        src/main/cpp/connection/message/enums/status.hpp
        src/main/cpp/utils/utils.cpp
        src/main/cpp/utils/utils.hpp
        src/main/cpp/connection/consts/consts.cpp
        src/main/cpp/connection/consts/consts.hpp

)

file(COPY ${CONFIG_SRC_DIR}/logging/log4cxx.xml DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${CONFIG_LOG_DIR})


include_directories(${LOG4CXX_INCLUDE_DIRS})
target_link_libraries(server PRIVATE pthread ${LOG4CXX_LIBRARIES})