cmake_minimum_required (VERSION 3.10)

if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project (Launcher)

if(MSVC)
    add_compile_options(/utf-8)
else()
    add_compile_options(-finput-charset=UTF-8 -fexec-charset=UTF-8)
endif()

add_definitions(-w)

if (WIN32)
  add_compile_options(/w)
endif()

add_executable (
    ${PROJECT_NAME} 
    ./library/utility.hpp
    ./library/string.hpp
    ./library/version.hpp
    ./library/platform/windows.hpp
    ./library/platform/unix.hpp
    ./launcher.cpp 
    $<$<PLATFORM_ID:Windows>:./launcher.rc>
)

if (WIN32)
  set(MY_MANIFEST_FILE ./resource/windows/application.manifest)
  set(CMAKE_SHARED_LINKER_FLAGS "/MANIFEST:NO")
endif()

target_include_directories(
	${PROJECT_NAME} PRIVATE
    .
)

set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Choose the type of build." FORCE)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 23)
endif()
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
