cmake_minimum_required (VERSION 3.20)

project(
	parallel-hashmap
	VERSION 1.4.1
	HOMEPAGE_URL https://github.com/greg7mdp/parallel-hashmap
	LANGUAGES CXX
)

add_library(
	${PROJECT_NAME} INTERFACE
	./meminfo.h
	./phmap_base.h
	./phmap_bits.h
	./btree.h
	./phmap_config.h
	./phmap_dump.h
	./phmap_fwd_decl.h
	./phmap_utils.h
	./phmap.h
)

# Set Kind

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# UTF8 Support
if(MSVC)
  add_compile_options(/utf-8)
  add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
  add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")
else()
  add_compile_options(-finput-charset=UTF-8)
endif()

if (MSVC)
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Zi")
else ()
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")
endif ()

set_target_properties(${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE CXX)