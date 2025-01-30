cmake_minimum_required(VERSION 3.20)

project(
    quickjs
    VERSION 0.8.0
    HOMEPAGE_URL https://github.com/quickjs-ng/quickjs
    LANGUAGES C
)

set(
    QUICKJS_HEADERS
    cutils.h
    dirent_compat.h
    getopt_compat.h
    libbf.h
    libregexp-opcode.h
    libregexp.h
    libunicode-table.h
    libunicode.h
    list.h
    quickjs-atom.h
    quickjs-c-atomics.h
    quickjs-libc.h
    quickjs-opcode.h
    quickjs.h
    unicode_gen_def.h
)

set(
    QUICKJS_SOURCES
    cutils.c
    libbf.c
    libunicode.c
    libregexp.c
    qjs.c
    qjsc.c
    quickjs-libc.c
    quickjs.c
    unicode_gen.c
)

add_library(
    ${PROJECT_NAME} STATIC
    ${QUICKJS_HEADERS}
    ${QUICKJS_SOURCES}
)

set(CMAKE_C_VISIBILITY_PRESET hidden)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)

include(CheckCCompilerFlag)
include(GNUInstallDirs)

function(xcheck_add_c_compiler_flag flag)
    if(NOT MSVC)
        target_compile_options(${PROJECT_NAME} PRIVATE ${flag})
    endif()
endfunction()

# Add GCC/Clang-specific flags
if(NOT MSVC)
    xcheck_add_c_compiler_flag(-Wno-implicit-fallthrough)
    xcheck_add_c_compiler_flag(-Wno-sign-compare)
    xcheck_add_c_compiler_flag(-Wno-missing-field-initializers)
    xcheck_add_c_compiler_flag(-Wno-unused-parameter)
    xcheck_add_c_compiler_flag(-Wno-unused-but-set-variable)
    xcheck_add_c_compiler_flag(-Wno-array-bounds)
    xcheck_add_c_compiler_flag(-Wno-format-truncation)
    xcheck_add_c_compiler_flag(-funsigned-char)
endif()

# MSVC-specific configuration
if(MSVC)
    target_compile_definitions(${PROJECT_NAME} PRIVATE "_HAS_C11=1")
    target_compile_options(${PROJECT_NAME} PRIVATE /experimental:c11atomics)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /std:c11")
    target_compile_options(
        ${PROJECT_NAME} PRIVATE
        /experimental:c11atomics
        /wd4018
        /wd4061
        /wd4100
        /wd4200
        /wd4242
        /wd4244
        /wd4245
        /wd4267
        /wd4388
        /wd4389
        /wd4710
        /wd4711
        /wd4820
        /wd4996
        /wd5045
    )
endif()

if(APPLE AND CMAKE_C_COMPILER_ID STREQUAL "GNU" AND CMAKE_C_COMPILER_VERSION VERSION_GREATER_EQUAL 11)
    xcheck_add_c_compiler_flag(-Wno-maybe-uninitialized)
endif()

if(CMAKE_SYSTEM_NAME STREQUAL "WASI")
    add_compile_definitions(
        _WASI_EMULATED_PROCESS_CLOCKS
        _WASI_EMULATED_SIGNAL
    )
    add_link_options(
        -lwasi-emulated-process-clocks
        -lwasi-emulated-signal
    )
endif()

set_source_files_properties(
    ${QUICKJS_SOURCES}
    PROPERTIES LANGUAGE C
)

if (WIN32)
    target_compile_definitions(${PROJECT_NAME} PRIVATE NOMINMAX _WINSOCKAPI_ _UCRT_NOISY_NAN)
    target_compile_options(${PROJECT_NAME} PRIVATE /wd4005)
else()
    if(NOT DEFINED CLOCK_MONOTONIC)
        add_definitions(-DCLOCK_MONOTONIC=1)
    endif()
    add_definitions(-D_POSIX_C_SOURCE=200809L)
    add_definitions(-Dtm_gmtoff=__tm_gmtoff)
endif()

if(UNIX AND NOT APPLE)
    add_definitions(-D_POSIX_C_SOURCE=200809L)

    if(DEFINED CMAKE_SYSTEM_NAME AND (CMAKE_SYSTEM_NAME STREQUAL "FreeBSD" OR
                                       CMAKE_SYSTEM_NAME STREQUAL "OpenBSD" OR
                                       CMAKE_SYSTEM_NAME STREQUAL "NetBSD"))
        add_definitions(-D_POSIX_C_SOURCE=200809L)
        target_compile_definitions(${PROJECT_NAME} PRIVATE "ENVRION_DEFINED")
        target_compile_definitions(${PROJECT_NAME} PRIVATE "SIGHANDLER_T_DEFINED")
    endif()
endif()

if(UNIX AND NOT APPLE)
    add_compile_options(-fPIC)
endif()

set(CMAKE_POSITION_INDEPENDENT_CODE ON)
