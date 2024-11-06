cmake_minimum_required(VERSION 3.9)

project(
    quickjs 
    VERSION 0.6.1 
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

add_library(${PROJECT_NAME} STATIC ${QUICKJS_HEADERS} ${QUICKJS_SOURCES})

set(CMAKE_C_VISIBILITY_PRESET hidden)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)

if(MSVC)
    target_compile_definitions(${PROJECT_NAME} PRIVATE "_HAS_C11=1")
    target_compile_options(${PROJECT_NAME} PRIVATE /experimental:c11atomics)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /std:c11")
else()
    target_compile_options(${PROJECT_NAME} PRIVATE -std=c11)
endif()

if (WIN32)
    target_compile_definitions(${PROJECT_NAME} PRIVATE NOMINMAX _WINSOCKAPI_ _UCRT_NOISY_NAN)
    
    if (MSVC)
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
endif()
