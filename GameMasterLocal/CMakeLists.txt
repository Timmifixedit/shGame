cmake_minimum_required(VERSION 3.16)
set(CMAKE_CXX_STANDARD 17)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif(NOT CMAKE_BUILD_TYPE)

set(CMAKE_CXX_FLAGS "-Wall -Wextra -Wpedantic -Werror -march=native -mtune=native")
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address -fno-omit-frame-pointer")
    message("Building for debug")
else()
    set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -O3")
    message("Building for release, all librarys need to be compiled for release!")
endif ()

include_directories(SYSTEM lib/)
project(SecretHitlerGameMaster VERSION 0.1 DESCRIPTION "Game master application for the card game 'Secret Hitler'")
set(SOURCES
        ${CMAKE_SOURCE_DIR}/src/gameStateHandlers.cpp
        ${CMAKE_SOURCE_DIR}/src/util.cpp
        )

set(LIBS SecretHitlerGameLogic)
include_directories(${CMAKE_SOURCE_DIR}/src)
add_executable(${PROJECT_NAME} src/main.cpp ${SOURCES})
target_link_libraries(${PROJECT_NAME} ${LIBS})
