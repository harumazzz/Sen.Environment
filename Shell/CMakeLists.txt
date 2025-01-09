cmake_minimum_required(VERSION 3.22)

project(Shell LANGUAGES CXX)

set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Build can only be Release" FORCE)

if(CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
    add_compile_options(-std=c++23 -fmodules-ts -fPIC -fvisibility=hidden)
endif()

if(MSVC)
    add_compile_options(/utf-8)
else()
    add_compile_options(-finput-charset=UTF-8 -fexec-charset=UTF-8)
endif()

file(GLOB_RECURSE HEADER_FILES "./**.hpp")
file(GLOB_RECURSE SOURCE_FILES "./**.cppm" "./**.cpp")

add_executable(
    ${PROJECT_NAME} 
    $<$<PLATFORM_ID:Windows>:./shell.rc>
    ${HEADER_FILES} 
    ${SOURCE_FILES}
)

target_include_directories(
    ${PROJECT_NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}
)

if(WIN32)
    set(MY_MANIFEST_FILE ./resource/windows/application.manifest)
    set(CMAKE_SHARED_LINKER_FLAGS "/MANIFEST:NO")
    target_compile_definitions(${PROJECT_NAME} PRIVATE UNICODE _UNICODE)
    add_compile_options(/w) 
endif()

if(CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
    target_compile_options(${PROJECT_NAME} PRIVATE
        -Weverything
        -Wno-c++98-compat
        -Wno-c++98-compat-pedantic
        -Wno-pre-c++14-compat
        -Wno-pre-c++17-compat
        -Wno-c++20-compat
        -Wno-c99-extensions
        -Wno-padded
        -Wno-redundant-parens
        -Wno-missing-field-initializers
        -Wno-unused-parameter
        -Wno-unused-variable
        -Wno-unused-local-typedef
        -Wno-missing-noreturn
        -Wno-gnu-zero-variadic-macro-arguments
        -Wno-ctad-maybe-unsupported
        -Wno-global-constructors
        -Wno-exit-time-destructors
        -Wno-weak-vtables
        -Wno-self-assign
        -Wno-switch-enum
        -Wno-switch-default
        -Wno-covered-switch-default
        -Wno-shadow-field
        -Wno-shadow-field-in-constructor
        -Wno-shadow-uncaptured-local
        -Wno-unsafe-buffer-usage
        -Wno-gnu-line-marker
        -Wno-disabled-macro-expansion
        -Wno-language-extension-token
        -Wno-shadow
        -Wno-float-equal
        -Wno-deprecated-declarations
        -Wno-dollar-in-identifier-extension
        -Wno-reserved-user-defined-literal
        -Wno-unreachable-code-return
        -Wno-unreachable-code-break
    )
endif()

set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME "Shell" RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")

if(NOT (CMAKE_SYSTEM_NAME MATCHES "iOS" OR CMAKE_SYSTEM_NAME MATCHES "Android"))
    add_subdirectory(./deps/tinyfiledialogs)
    target_link_libraries(${PROJECT_NAME} PRIVATE tinyfiledialogs)
endif()

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)
set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)

if (MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /std:c++latest)
    target_compile_definitions(${PROJECT_NAME} PUBLIC _WIN32_WINNT=0x0A00)
    target_link_libraries(${PROJECT_NAME} PRIVATE Kernel32)
endif()

if (MSVC)
    target_link_libraries(${PROJECT_NAME} PRIVATE Ole32 User32)
endif()

if (MSVC)
    target_compile_definitions(${PROJECT_NAME} PRIVATE UNICODE _UNICODE)
    target_link_libraries(${PROJECT_NAME} PRIVATE Ole32 User32 Kernel32)
    set_property(TARGET ${PROJECT_NAME} PROPERTY WIN32_EXECUTABLE OFF)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:CONSOLE /ENTRY:wmainCRTStartup")
endif()


