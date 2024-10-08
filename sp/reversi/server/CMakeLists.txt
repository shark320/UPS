cmake_minimum_required(VERSION 3.28)
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
find_package(Threads REQUIRED)

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

add_executable(server
        src/main/cpp/main.cpp
        src/main/cpp/base/base.hpp
        src/main/cpp/base/base.cpp
        src/main/cpp/config/configuration.cpp
        src/main/cpp/config/configuration.hpp
        src/main/cpp/utils/utils.cpp
        src/main/cpp/utils/utils.hpp
        src/main/cpp/utils/global.cpp
        src/main/cpp/utils/global.hpp
        src/main/cpp/reversi/board/board.hpp
        src/main/cpp/reversi/board/board.cpp
        src/main/cpp/reversi/config/game_config.cpp
        src/main/cpp/reversi/config/game_config.hpp
        src/main/cpp/reversi/consts/game_consts.hpp
        src/main/cpp/reversi/consts/game_consts.cpp
        src/main/cpp/reversi/engine/reversi_engine.cpp
        src/main/cpp/reversi/engine/reversi_engine.hpp
        src/main/cpp/reversi/reversi_game.cpp
        src/main/cpp/reversi/reversi_game.hpp
        src/main/cpp/server/connection/consts/consts.hpp
        src/main/cpp/server/connection/consts/consts.cpp
        src/main/cpp/server/connection/message/enums/type.hpp
        src/main/cpp/server/connection/message/enums/type.cpp
        src/main/cpp/server/connection/message/enums/status.hpp
        src/main/cpp/server/connection/message/enums/status.cpp
        src/main/cpp/server/connection/message/header/header.hpp
        src/main/cpp/server/connection/message/header/header.cpp
        src/main/cpp/server/connection/message/payload/payload.hpp
        src/main/cpp/server/connection/message/payload/payload.cpp
        src/main/cpp/server/connection/message/message.hpp
        src/main/cpp/server/connection/message/message.cpp

        #[[Unit Tests]]
        src/test/cpp/test_utils/test_utils.hpp
        src/test/cpp/test_utils/test_utils.cpp
        src/test/cpp/test.hpp
        src/test/cpp/test.cpp
        src/test/cpp/server/connection/message/message_test.hpp
        src/test/cpp/server/connection/message/header/header_test.hpp
        src/test/cpp/server/connection/message/header/header_test.cpp
        src/main/cpp/server/server.cpp
        src/main/cpp/server/server.hpp
        src/main/cpp/server/managers/client_manager.cpp
        src/main/cpp/server/managers/client_manager.hpp
        src/main/cpp/server/managers/lobby_manager.cpp
        src/main/cpp/server/managers/lobby_manager.hpp
        src/main/cpp/server/connection/client_info.cpp
        src/main/cpp/server/connection/client_info.hpp
        src/main/cpp/game/lobby/lobby.cpp
        src/main/cpp/game/lobby/lobby.hpp
        src/main/cpp/server/connection/client/client.cpp
        src/main/cpp/server/connection/client/client.hpp
        src/main/cpp/server/config/server_config.cpp
        src/main/cpp/server/config/server_config.hpp
)

add_custom_target(clean-assembly
        COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
        COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
        COMMENT "Removing assembly directory"
)
add_dependencies(server clean-assembly)

add_custom_command(
        TARGET server PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/${CONFIG_SRC_DIR}
        ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${CONFIG_DIR}
        COMMENT "Copying config files to assembly directory"
)

include_directories(${LOG4CXX_INCLUDE_DIRS})
target_link_libraries(server PRIVATE pthread ${LOG4CXX_LIBRARIES} SimpleIni fmt::fmt Threads::Threads)
