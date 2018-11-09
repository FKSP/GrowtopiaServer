cmake_minimum_required(VERSION 3.10.2)
project(GrowtopiaServer)

set(CMAKE_CXX_STANDARD 17)

set(LIBS ${CMAKE_CURRENT_SOURCE_DIR}/libs)

include_directories(GrowtopiaServer PRIVATE ${LIBS}/enet/include)
include_directories(GrowtopiaServer PRIVATE ${LIBS}/msgpack/include)
include_directories(GrowtopiaServer PRIVATE ${LIBS}/json)

add_subdirectory(${LIBS}/enet)
add_subdirectory(${LIBS}/msgpack)

set(INCLUDED_LIBS enet)

set(Boost_USE_MULTITHREADED ON)
find_package(Boost 1.46.0 COMPONENTS iostreams)
if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
    set(INCLUDED_LIBS ${INCLUDED_LIBS} ${Boost_LIBRARIES})
endif()


add_executable(GrowtopiaServer src/main.cpp
        src/server/udp_server.cpp src/server/udp_server.h
        src/objects/player.cpp src/objects/player.h
        src/packets/gamepacket.cpp src/packets/gamepacket.h
        src/handlers/PacketHandler.cpp src/handlers/PacketHandler.h
        src/helpers/login_parser.cpp src/helpers/login_parser.h
        src/handlers/login_handler.cpp src/handlers/login_handler.h
        src/database/world_db.cpp src/database/world_db.h src/helpers/random.h src/helpers/string.cpp src/helpers/string.h src/helpers/system.cpp src/helpers/system.h src/enums/block_types.h src/helpers/compression.cpp src/helpers/compression.h src/helpers/encoding.cpp src/helpers/encoding.h)

target_link_libraries(GrowtopiaServer ${INCLUDED_LIBS})
