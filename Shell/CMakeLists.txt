cmake_minimum_required (VERSION 3.10)

if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project (Shell)

set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Build can only be Release" FORCE)

if(MSVC)
    add_compile_options(/utf-8)
else()
    add_compile_options(-finput-charset=UTF-8 -fexec-charset=UTF-8)
endif()

add_definitions(-w)

if (WIN32)
  add_compile_options(/w)
endif()

if(NOT CMAKE_SIZEOF_VOID_P EQUAL 8)
    message(FATAL_ERROR "This project only supports x64 builds.")
endif()

add_executable (
    ${PROJECT_NAME} 
    ./library/color.hpp
    ./library/dialog.hpp
    ./library/string.hpp
    ./library/macro.hpp
    ./library/callback.hpp
    ./shell.cpp 
    $<$<PLATFORM_ID:Windows>:./shell.rc>
)


if (WIN32)
  set(MY_MANIFEST_FILE ./resource/windows/application.manifest)
  set(CMAKE_SHARED_LINKER_FLAGS "/MANIFEST:NO")
endif()

target_include_directories(
	${PROJECT_NAME} PRIVATE
	..
)

if(NOT (CMAKE_SYSTEM_NAME MATCHES "iOS" OR CMAKE_SYSTEM_NAME MATCHES "Android"))
    add_subdirectory(./deps/tinyfiledialogs)
    target_link_libraries(${PROJECT_NAME} PRIVATE tinyfiledialogs)
endif()

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if (MSVC)
    add_compile_options(/std:c++latest)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++latest")
endif()

if (MSVC)
    target_compile_definitions(${PROJECT_NAME} PUBLIC _WIN32_WINNT=0x0A00)
    target_link_libraries(${PROJECT_NAME} PRIVATE Kernel32)
endif()

if(WIN32)
    target_compile_definitions(${PROJECT_NAME} PRIVATE UNICODE _UNICODE)
endif()