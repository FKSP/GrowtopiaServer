cmake_minimum_required(VERSION 3.12)
project(GrowtopiaServer)

set(CMAKE_CXX_STANDARD 17)

set(LIBS ${CMAKE_CURRENT_SOURCE_DIR}/libs)

include_directories(GrowtopiaServer PRIVATE ${LIBS}/enet/include)
include_directories(GrowtopiaServer PRIVATE ${LIBS}/msgpack/include)
include_directories(GrowtopiaServer PRIVATE ${LIBS}/lzma)

add_subdirectory(${LIBS}/enet)
add_subdirectory(${LIBS}/msgpack)

add_executable(GrowtopiaServer src/main.cpp
        src/server/udp_server.cpp src/server/udp_server.h
        src/objects/player.cpp src/objects/player.h
        src/packets/gamepacket.cpp src/packets/gamepacket.h
        src/handlers/PacketHandler.cpp src/handlers/PacketHandler.h
        src/helpers/login_parser.cpp src/helpers/login_parser.h
        src/handlers/LoginHandler.cpp src/handlers/LoginHandler.h
        src/database/world_db.cpp src/database/world_db.h)

target_link_libraries(GrowtopiaServer enet)