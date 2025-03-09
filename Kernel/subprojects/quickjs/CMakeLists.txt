cmake_minimum_required(VERSION 3.20)

project(
    quickjs
    VERSION 0.9.0
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

if(MSVC)
    add_link_options("/FORCE:MULTIPLE")
endif()

add_library(
    ${PROJECT_NAME} STATIC
    ${QUICKJS_HEADERS}
    ${QUICKJS_SOURCES}
)

set(CMAKE_C_VISIBILITY_PRESET hidden)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)

function(xcheck_add_c_compiler_flag flag)
    if(NOT MSVC)
        target_compile_options(${PROJECT_NAME} PRIVATE ${flag})
    endif()
endfunction()

xcheck_add_c_compiler_flag(-Wno-unsafe-buffer-usage)
xcheck_add_c_compiler_flag(-Wno-sign-conversion)
xcheck_add_c_compiler_flag(-Wno-nonportable-system-include-path)
xcheck_add_c_compiler_flag(-Wno-implicit-int-conversion)
xcheck_add_c_compiler_flag(-Wno-shorten-64-to-32)
xcheck_add_c_compiler_flag(-Wno-reserved-macro-identifier)
xcheck_add_c_compiler_flag(-Wno-reserved-identifier)
xcheck_add_c_compiler_flag(-Wdeprecated-declarations)
xcheck_add_c_compiler_flag(-Wno-implicit-fallthrough)
xcheck_add_c_compiler_flag(-Wno-sign-compare)
xcheck_add_c_compiler_flag(-Wno-missing-field-initializers)
xcheck_add_c_compiler_flag(-Wno-unused-parameter)
xcheck_add_c_compiler_flag(-Wno-unused-but-set-variable)
xcheck_add_c_compiler_flag(-Wno-array-bounds)
xcheck_add_c_compiler_flag(-Wno-format-truncation)

# MSVC-specific configuration
if(MSVC)
    target_compile_definitions(${PROJECT_NAME} PRIVATE "_HAS_C11=1")
    target_compile_options(${PROJECT_NAME} PRIVATE /experimental:c11atomics)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /std:c11")
    target_compile_options(
        ${PROJECT_NAME} PRIVATE
        /experimental:c11atomics
        /J
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

if(WIN32)
    set(STACK_SIZE 8388608)
    if(MSVC)
        add_link_options("/STACK:${STACK_SIZE}")
    else()
        add_link_options("-Wl,--stack,${STACK_SIZE}")
    endif()
endif()


if(APPLE AND CMAKE_C_COMPILER_ID STREQUAL "GNU" AND CMAKE_C_COMPILER_VERSION VERSION_GREATER_EQUAL 11)
    xcheck_add_c_compiler_flag(-Wno-maybe-uninitialized)
endif()

set_source_files_properties(
    ${QUICKJS_SOURCES}
    PROPERTIES LANGUAGE C
)

if (WIN32)
    target_compile_definitions(${PROJECT_NAME} PRIVATE NOMINMAX _WINSOCKAPI_ _UCRT_NOISY_NAN)
    target_compile_options(${PROJECT_NAME} PRIVATE /wd4005)
endif()

if(UNIX AND NOT APPLE)
    add_compile_options(-fPIC)
endif()

set(CMAKE_POSITION_INDEPENDENT_CODE ON)
