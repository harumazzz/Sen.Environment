cmake_minimum_required (VERSION 3.20)

project(
  dirent #MSVC
  LANGUAGES C
  VERSION 1.24
  HOMEPAGE_URL https://github.com/tronkko/dirent
)

add_library(
  ${PROJECT_NAME} INTERFACE
  ./dirent.h
)

if (UNIX)
    add_compile_options(-fPIC)
    add_compile_options(_GNU_SOURCE)
    add_compile_options(-fvisibility=hidden)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -D_GNU_SOURCE")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_GNU_SOURCE")
endif()

set(CMAKE_POSITION_INDEPENDENT_CODE ON)
