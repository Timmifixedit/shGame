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

project(SecretHitlerGameLogic VERSION 0.1 DESCRIPTION "Game logic for the card game Secret Hitler")

set(SOURCES
        ${CMAKE_SOURCE_DIR}/src/shGameLogic.cpp
        ${CMAKE_SOURCE_DIR}/src/util.cpp
        ${CMAKE_SOURCE_DIR}/src/CardRange.cpp
        ${CMAKE_SOURCE_DIR}/src/GameEvent.cpp
        ${CMAKE_SOURCE_DIR}/src/rules.cpp
        ${CMAKE_SOURCE_DIR}/src/enumsToString.cpp
        )
include_directories(${CMAKE_SOURCE_DIR}/src)

add_library(${PROJECT_NAME} SHARED ${SOURCES})
target_link_libraries(${PROJECT_NAME} ${LIBS})

# Installation via make install
include(GNUInstallDirs)
set_target_properties(${PROJECT_NAME} PROPERTIES
        VERSION ${PROJECT_VERSION}
        SOVERSION 1
        PUBLIC_HEADER
        "src/shGameLogic.h;src/util.h;src/CardRange.h;src/GameEvent.h;src/GlobalTypes.h;src/rules.h")

install(TARGETS ${PROJECT_NAME}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/SecretHitlerGameLogic )

add_subdirectory(Tests)
