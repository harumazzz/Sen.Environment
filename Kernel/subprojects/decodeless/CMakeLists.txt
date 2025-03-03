cmake_minimum_required (VERSION 3.20)

project(
	decodeless
	HOMEPAGE_URL https://github.com/vimpunk/mio
	LANGUAGES CXX
)

add_library(
	${PROJECT_NAME} INTERFACE
	./allocator.hpp
./allocator_concepts.hpp
./allocator_construction.hpp
./header.hpp
./mappedfile.hpp
./offset_ptr.hpp
./offset_span.hpp
./pmr_allocator.hpp
./pmr_writer.hpp
./writer.hpp
./detail/mappedfile_common.hpp
./detail/mappedfile_linux.hpp
./detail/mappedfile_windows.hpp
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
	add_definitions(-DNOMINMAX)
else ()
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")
endif ()

set_target_properties(${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE CXX)