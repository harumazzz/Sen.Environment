cmake_minimum_required(VERSION 3.20)

project(
    stringzilla
    VERSION 3.11.3
    HOMEPAGE_URL https://github.com/ashvardanian/StringZilla
    LANGUAGES CXX C
)

add_library(${PROJECT_NAME} INTERFACE)

if(CMAKE_SYSTEM_PROCESSOR MATCHES "x86_64|AMD64|amd64")
  SET(SZ_PLATFORM_X86 TRUE)
  message(STATUS "Platform: x86")
elseif(CMAKE_SYSTEM_PROCESSOR MATCHES "aarch64|AARCH64|arm64|ARM64")
  SET(SZ_PLATFORM_ARM TRUE)
  message(STATUS "Platform: ARM")
endif()


target_compile_definitions(${PROJECT_NAME} INTERFACE 
    "SZ_AVOID_LIBC=1"
    "SZ_OVERRIDE_LIBC=0"
)

target_include_directories(${PROJECT_NAME} INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
	${CMAKE_CURRENT_SOURCE_DIR}
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_EXTENSIONS OFF)
set(CMAKE_CXX_EXTENSIONS OFF)

if(CMAKE_C_BYTE_ORDER STREQUAL "BIG_ENDIAN")
    target_compile_definitions(${PROJECT_NAME} INTERFACE "SZ_DETECT_BIG_ENDIAN=1")
else()
    target_compile_definitions(${PROJECT_NAME} INTERFACE "SZ_DETECT_BIG_ENDIAN=0")
endif()

target_compile_options(${PROJECT_NAME} INTERFACE
    "$<$<CXX_COMPILER_ID:GNU,Clang,AppleClang>:-fno-builtin-memcmp;-fno-builtin-memchr;-fno-builtin-memcpy;-fno-builtin-memset>"
    "$<$<CXX_COMPILER_ID:MSVC>:/Oi->"
    "$<$<CXX_COMPILER_ID:GNU,Clang,AppleClang>:-Wall;-Wextra;-pedantic;-Werror;-Wfatal-errors;-Wno-unknown-pragmas>"
    "$<$<CXX_COMPILER_ID:MSVC>:/Bt;/wd4068;/wd4146;/utf-8;/WX>"
)

if(MSVC)
    target_compile_options(${PROJECT_NAME} INTERFACE /utf-8)
else()
    target_compile_options(${PROJECT_NAME} INTERFACE -finput-charset=UTF-8)
endif()

set_target_properties(${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE CXX)
