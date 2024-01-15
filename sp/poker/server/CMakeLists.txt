
cmake_minimum_required(VERSION 3.10)
project(server)

# You don't need to manually include log4cxx headers or libraries

add_executable(server src/server.cpp
        src/connection/message/message.hpp
        src/connection/message/message.cpp
        src/connection/payload/payload.hpp
        src/connection/payload/payload.cpp
        src/connection/status.hpp
        src/base/base.cpp
        src/base/base.hpp
        src/connection/header/header.cpp
        src/connection/header/header.hpp
        src/connection/enums/type.cpp
        src/connection/enums/type.hpp
        src/connection/enums/status.cpp
        src/connection/enums/status.hpp
        src/utils/utils.cpp
        src/utils/utils.hpp
        src/connection/consts/consts.cpp
        src/connection/consts/consts.hpp

)

target_link_libraries(server PRIVATE pthread)