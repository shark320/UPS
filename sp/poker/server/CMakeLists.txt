
cmake_minimum_required(VERSION 3.22)
project(server)

set(CMAKE_CXX_STANDARD 20)
set(LOG4CXX_INCLUDE_DIRS "/usr/include/log4cxx")
set(LOG4CXX_LIBRARIES "/usr/lib/x86_64-linux-gnu/liblog4cxx.so")
set(CONFIG_DIR "config")
set(CONFIG_LOG_DIR "${CONFIG_DIR}/logging")
set(SRC_RES_DIR "src/main/resources")
set(CONFIG_SRC_DIR "${SRC_RES_DIR}/config")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/assembly)

find_package(log4cxx REQUIRED)
# You don't need to manually include log4cxx headers or libraries

include(FetchContent)

FetchContent_Declare(
        fmt
        GIT_REPOSITORY https://github.com/fmtlib/fmt.git
        GIT_TAG master
)
FetchContent_MakeAvailable(fmt)

FetchContent_Declare(
        SimpleIni
        GIT_REPOSITORY https://github.com/brofield/simpleini
        GIT_TAG master
)

FetchContent_MakeAvailable(SimpleIni)



message("LOG4CXX_INCLUDE_DIRS: ${LOG4CXX_INCLUDE_DIRS}")
message("LOG4CXX_LIBRARIES: ${LOG4CXX_LIBRARIES}")

add_executable(server src/main/cpp/server-main.cpp
        src/main/cpp/server/connection/message/message.hpp
        src/main/cpp/server/connection/message/message.cpp
        src/main/cpp/server/connection/message/payload/payload.hpp
        src/main/cpp/server/connection/message/payload/payload.cpp
        src/main/cpp/base/base.cpp
        src/main/cpp/base/base.hpp
        src/main/cpp/server/connection/message/header/header.cpp
        src/main/cpp/server/connection/message/header/header.hpp
        src/main/cpp/server/connection/message/enums/type.cpp
        src/main/cpp/server/connection/message/enums/type.hpp
        src/main/cpp/server/connection/message/enums/status.cpp
        src/main/cpp/server/connection/message/enums/status.hpp
        src/main/cpp/utils/utils.cpp
        src/main/cpp/utils/utils.hpp
        src/main/cpp/server/connection/consts/consts.cpp
        src/main/cpp/server/connection/consts/consts.hpp
        src/main/cpp/server/connection/connection.cpp
        src/main/cpp/server/connection/connection.hpp
        src/main/cpp/server/connection/connection.hpp
        src/main/cpp/config/configuration.cpp
        src/main/cpp/config/configuration.hpp
        src/main/cpp/server/server.cpp
        src/main/cpp/server/server.hpp
        src/main/cpp/async/blocking_queue.hpp
        src/main/cpp/utils/global.cpp
        src/main/cpp/utils/global.hpp
        src/main/cpp/async/safe_queue.hpp
)

####Clean assembly dir before build
add_custom_target(clean-assembly
        COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
        COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
        COMMENT "Removing assembly directory"
)
add_dependencies(server clean-assembly)
####

add_custom_command(
        TARGET server PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/${CONFIG_SRC_DIR}
        ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${CONFIG_DIR}
        COMMENT "Copying config files to assembly directory"
)

#file(COPY ${CONFIG_SRC_DIR} DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})


include_directories(${LOG4CXX_INCLUDE_DIRS})
target_link_libraries(server PRIVATE pthread ${LOG4CXX_LIBRARIES} fmt::fmt SimpleIni)