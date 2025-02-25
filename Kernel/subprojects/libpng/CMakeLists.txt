cmake_minimum_required (VERSION 3.20)

project(
	libpng
	VERSION 1.6.46
	HOMEPAGE_URL http://www.libpng.org/
	LANGUAGES C
)

add_library(
	${PROJECT_NAME} STATIC
	./png.h
	./pngconf.h
	./pnglibconf.h
	./pngpriv.h
	./pngdebug.h
	./pnginfo.h
	./pngstruct.h
	./png.c
	./pngerror.c
	./pngget.c
	./pngmem.c
	./pngpread.c
	./pngread.c
	./pngrio.c
	./pngrtran.c
	./pngrutil.c
	./pngset.c
	./pngtrans.c
	./pngwio.c
	./pngwrite.c
	./pngwtran.c
	./pngwutil.c
)

target_include_directories(
	${PROJECT_NAME} PUBLIC
	../zlib
)

target_link_libraries(
	${PROJECT_NAME} PRIVATE
	zlib
)

if(CMAKE_SYSTEM_PROCESSOR MATCHES "arm")
    target_compile_definitions(${PROJECT_NAME} PUBLIC PNG_ARM_NEON_OPT=0)
endif()

if (ANDROID)
	target_compile_definitions(${PROJECT_NAME} PUBLIC PNG_ARM_NEON_OPT=0)
endif()

set(CMAKE_POSITION_INDEPENDENT_CODE ON)
