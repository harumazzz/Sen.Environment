cmake_minimum_required (VERSION 3.20)

project(
    SHA256 
    VERSION 0
    HOMEPAGE_URL https://github.com/System-Glitch/SHA256
    LANGUAGES CXX
)

add_library(
    ${PROJECT_NAME} STATIC
    ./SHA256.h
    ./SHA256.cpp
)

if (MSVC)
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Zi")
else ()
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")
endif ()

if (UNIX AND NOT APPLE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
endif()