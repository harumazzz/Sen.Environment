cmake_minimum_required (VERSION 3.20)

project(
	jsoncons
	VERSION 1.3.0
	HOMEPAGE_URL https://github.com/danielaparker/jsoncons
	LANGUAGES CXX
)
set(JSONCONS_PROJECT_DIR ${PROJECT_SOURCE_DIR})
set(JSONCONS_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)


add_library(jsoncons INTERFACE)
target_include_directories(jsoncons INTERFACE $<BUILD_INTERFACE:${JSONCONS_INCLUDE_DIR}>
		$<INSTALL_INTERFACE:include>)

# UTF8 Support
if(MSVC)
  add_compile_options(/utf-8)
  add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
  add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")
else()
  add_compile_options(-finput-charset=UTF-8)
endif()

if(MSVC)
    target_compile_options(${PROJECT_NAME} INTERFACE /utf-8)
else()
    target_compile_options(${PROJECT_NAME} INTERFACE -finput-charset=UTF-8)
endif()

if (MSVC)
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Zi")
else ()
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")
endif ()

set_target_properties(${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE CXX)