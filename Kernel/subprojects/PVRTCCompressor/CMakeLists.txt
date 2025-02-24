cmake_minimum_required (VERSION 3.20)

project(
	pvrtc
	HOMEPAGE_URL https://github.com/codywatts/PVRTCCompressor
	LANGUAGES CXX
)

# Set Kind

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (MSVC)
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Zi")
else ()
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")
endif ()

add_library(
	${PROJECT_NAME} STATIC
	./AlphaBitmap.h
./Bitmap.h
./BitScale.cpp
./BitScale.h
./BitUtility.h
./ColorRgba.h
./Interval.h
        ./MortonTable.cpp
./MortonTable.h
./Point2.h
./PvrTcDecoder.cpp
./PvrTcDecoder.h
./PvrTcEncoder.cpp
./PvrTcEncoder.h
./PvrTcPacket.cpp
./PvrTcPacket.h
./RgbaBitmap.h
./RgbBitmap.h
)

# UTF8 Support
if(MSVC)
  add_compile_options(/utf-8)
  add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
  add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")
else()
  add_compile_options(-finput-charset=UTF-8)
endif()

set_target_properties(${PROJECT_NAME} PROPERTIES POSITION_INDEPENDENT_CODE ON)

