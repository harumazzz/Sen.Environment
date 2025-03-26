cmake_minimum_required (VERSION 3.20)

project(
	avir
  	VERSION 3.0.11
	HOMEPAGE_URL https://github.com/avaneev/avir
	LANGUAGES CXX
)

add_library(
	${PROJECT_NAME} INTERFACE
	./avir.h
	./avir_dil.h
	./avir_float4_sse.h
	./avir_float8_avx.h
	./lancir.h
)

if (UNIX)
    add_compile_options(-fPIC)
    add_compile_options(_GNU_SOURCE)
    add_compile_options(-fvisibility=hidden)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -D_GNU_SOURCE")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_GNU_SOURCE")
endif()

set_target_properties(${PROJECT_NAME} PROPERTIES POSITION_INDEPENDENT_CODE ON)
