cmake_minimum_required(VERSION 3.20)

if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project(
        explorer
        LANGUAGES CXX
)

set(VCPKG_MANIFEST_MODE ON)

add_library(
        ${PROJECT_NAME} SHARED
        explorer.cpp
        explorer.h
        command/base_command.hpp
        command/seperator_command.hpp
        utility/system.hpp
        command/method_invoke.hpp
        command/method_list.hpp
        explorer.def
        resources/app_icon.rc
)

set_target_properties(${PROJECT_NAME} PROPERTIES CXX_STANDARD 20 CXX_STANDARD_REQUIRED YES)

target_include_directories(${PROJECT_NAME} PRIVATE ..)

find_package(wil CONFIG REQUIRED)

target_link_libraries(${PROJECT_NAME} PRIVATE WIL::WIL)

target_link_libraries(
        ${PROJECT_NAME} PRIVATE
        ole32
        oleaut32
        uuid
        shlwapi
        shell32
        runtimeobject
)

set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")