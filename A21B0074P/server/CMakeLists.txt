cmake_minimum_required(VERSION 3.22)
project(server2)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(Threads REQUIRED)

add_executable(BlackjackServer
        src/BlackjackGame.cpp
        src/Card.cpp
        src/ClientGuard.cpp
        src/ClientInfo.cpp
        src/ClientManager.cpp
        src/Constants.cpp
        src/Lobby.cpp
        src/LobbyManager.cpp
        src/Main.cpp
        src/MessageProcessor.cpp
        src/Socket.cpp
        src/User.cpp
        src/Utils.cpp
)

target_include_directories(BlackjackServer PRIVATE src)

target_link_libraries(BlackjackServer PRIVATE Threads::Threads)