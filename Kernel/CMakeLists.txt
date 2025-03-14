cmake_minimum_required (VERSION 3.20)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

project (
  Kernel
  VERSION 10
  HOMEPAGE_URL https://github.com/Haruma-VN/Sen.Environment
  LANGUAGES CXX
)

add_definitions(-w)

if (WIN32)
  add_compile_options(/w)
endif()


if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

if (MSVC)
    message(STATUS "MSVC version: ${MSVC_VERSION}")
endif()

set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Build can only be Release" FORCE)

add_subdirectory("./subprojects/zlib")
add_subdirectory("./subprojects/fmt")
add_subdirectory("./subprojects/Rijndael")
add_subdirectory("./subprojects/libpng")
add_subdirectory("./subprojects/etc1")
add_subdirectory("./subprojects/lzma")
add_subdirectory("./subprojects/avir")
add_subdirectory("./subprojects/bzip2")
add_subdirectory("./subprojects/open_vcdiff")
add_subdirectory("./subprojects/zip")
add_subdirectory("./subprojects/magic_enum")
add_subdirectory("./subprojects/canvas_ity")
add_subdirectory("./subprojects/compile-time-regular-expressions")
add_subdirectory("./subprojects/parallel-hashmap")
add_subdirectory("./subprojects/thread-pool")
add_subdirectory("./subprojects/jsoncons")
add_subdirectory("./subprojects/pugixml")
add_subdirectory("./subprojects/PVRTCCompressor")
add_subdirectory("./subprojects/decodeless")

if (MSVC)
  add_subdirectory("./subprojects/dirent")
endif()

if (MSVC)
    add_compile_options(/bigobj)
endif ()

add_subdirectory("./subprojects/quickjs")



if(WIN32 OR UNIX OR APPLE)
    add_subdirectory("./test")
endif()

add_subdirectory("./kernel")

if(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /Z7")
    set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} /DEBUG:NONE")
    set(CMAKE_SHARED_LINKER_FLAGS_RELEASE "${CMAKE_SHARED_LINKER_FLAGS_RELEASE} /NOIMPLIB")
    set(CMAKE_MODULE_LINKER_FLAGS_RELEASE "${CMAKE_MODULE_LINKER_FLAGS_RELEASE} /NOIMPLIB")
endif()

if(ANDROID)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -stdlib=libc++ -lc++abi")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++23 -stdlib=libc++")
    set_target_properties(${TARGET_NAME} PROPERTIES
        ANDROID_STL c++_shared
        CMAKE_ANDROID_ARCH_ABI "arm64-v8a"  # or "armeabi-v7a", "x86", etc.
        CMAKE_ANDROID_NDK 27
        ANDROID_PLATFORM android-29 #  (Android 10) is minimum
    )
endif()

include(InstallRequiredSystemLibraries)

if(MSVC)
    set(CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS_NO_WARNINGS TRUE)
    include(InstallRequiredSystemLibraries)
    
    install(PROGRAMS ${CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS} DESTINATION bin)
endif()

# Symbol visibility
set(CMAKE_C_VISIBILITY_PRESET hidden)
set(CMAKE_CXX_VISIBILITY_PRESET hidden)

# Pos Independent
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

if (MSVC)
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Zi")
else ()
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")
endif ()

if(CMAKE_COMPILER_IS_GNUCC OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -march=znver3 -mtune=znver3")
endif()

if (MSVC)
	set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} -std=c++23)
endif()

set_target_properties(kernel PROPERTIES OUTPUT_NAME "Kernel")

include(InstallRequiredSystemLibraries)
install(PROGRAMS ${CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS} DESTINATION bin)

set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /Zl")

add_compile_definitions(NDEBUG)
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -flto")

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

if(ANDROID AND NOT ANDROID_ABI)
    set(ANDROID_ABI "arm64-v8a")
endif()

if(ANDROID AND NOT ANDROID_PLATFORM)
    set(ANDROID_PLATFORM 30)
endif()

if(ANDROID AND NOT ANDROID_STL)
    set(ANDROID_STL "c++_shared")
endif()

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    add_compile_options(
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
    )
endif()