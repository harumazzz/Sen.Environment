cmake_minimum_required (VERSION 3.20)

project(
	kernel
	LANGUAGES CXX
)

add_library(
	${PROJECT_NAME} SHARED
	./definition/macro.hpp
	./definition/range.hpp
	./definition/library.hpp
	./definition/literal.hpp
	./definition/assert.hpp
	./definition/utility.hpp
	./definition/basic/filesystem.hpp
	./definition/basic/string.hpp
	./definition/basic/converter.hpp
	./definition/basic/math.hpp
	./definition/basic/language.hpp
	./definition/basic/path.hpp
	./definition/basic/pointer.hpp
	./definition/basic/timer.hpp
	./definition/basic/image.hpp
	./definition/basic/json.hpp
	./definition/basic/buffer.hpp
	./definition/basic/array.hpp
	./definition/basic/system.hpp
	./definition/basic/thread.hpp
	./definition/basic/json_writer.hpp
	./definition/basic/apng_maker.hpp
	./definition/basic/utility.hpp

	./definition/encryption/md5.hpp
	./definition/encryption/Rijndael.hpp
	./definition/encryption/sha224.hpp
	./definition/encryption/sha256.hpp
	./definition/encryption/base64.hpp
	./definition/encryption/xor.hpp
	./definition/encryption/crc.hpp
	./definition/encryption/fnv.hpp
	./definition/compression/zlib.hpp
	./definition/compression/lzma.hpp
	./definition/compression/bzip2.hpp
	./definition/javascript/common.hpp
	./definition/javascript/value.hpp
	./definition/javascript/runtime.hpp
	./definition/javascript/converter.hpp
	./definition/javascript/value_adapter.hpp
	./definition/diff/vcdiff.hpp

	./dependencies/fmt.hpp
	./dependencies/json.hpp
	./dependencies/md5.hpp
	./dependencies/zlib.hpp
	./dependencies/SHA256.hpp
	./dependencies/Rijndael.hpp
	./dependencies/SHA224CryptoServiceProvider.hpp
	./dependencies/cpp-base64.hpp
	./dependencies/libpng.hpp
	./dependencies/etc1.hpp
	./dependencies/tinyxml2.hpp
	./dependencies/SHA.hpp
	./dependencies/lzma.hpp
	./dependencies/avir.hpp
	./dependencies/open_vcdiff.hpp
	./dependencies/bzip2.hpp
	./dependencies/quickjs.hpp
	./dependencies/zip.hpp
	./dependencies/dirent.hpp
	./dependencies/simdjson.hpp
	./dependencies/magic_enum.hpp
	./dependencies/canvas_ity.hpp
	./dependencies/tsl.hpp

	./support/popcap/animation/common.hpp
	./support/popcap/animation/decode.hpp
	./support/popcap/animation/definition.hpp
	./support/popcap/animation/encode.hpp
	./support/popcap/character_font_widget_2/decode.hpp
	./support/popcap/character_font_widget_2/definition.hpp
	./support/popcap/character_font_widget_2/encode.hpp
	./support/popcap/compiled_text/decode.hpp
	./support/popcap/compiled_text/encode.hpp
	./support/popcap/crypt_data/decrypt.hpp
	./support/popcap/crypt_data/definition.hpp
	./support/popcap/crypt_data/encrypt.hpp
	./support/popcap/new_type_object_notation/decode.hpp
	./support/popcap/new_type_object_notation/encode.hpp
	./support/popcap/package/common.hpp
	./support/popcap/package/definition.hpp
	./support/popcap/package/pack.hpp
	./support/popcap/package/unpack.hpp
	./support/popcap/particles/common.hpp
	./support/popcap/particles/decode.hpp
	./support/popcap/particles/definition.hpp
	./support/popcap/particles/encode.hpp
	./support/popcap/particles/to_xml.hpp
	./support/popcap/particles/from_xml.hpp
	./support/popcap/player_info/common.hpp
	./support/popcap/player_info/decode.hpp
	./support/popcap/player_info/definition.hpp
	./support/popcap/player_info/encode.hpp
	./support/popcap/reflection_object_notation/common.hpp
	./support/popcap/reflection_object_notation/decode.hpp
	./support/popcap/reflection_object_notation/encode.hpp
	./support/popcap/reflection_object_notation/instance.hpp
	./support/popcap/render_effects/common.hpp
	./support/popcap/render_effects/decode.hpp
	./support/popcap/render_effects/definition.hpp
	./support/popcap/render_effects/encode.hpp
	./support/popcap/resource_group/common.hpp
	./support/popcap/resource_group/convert.hpp
	./support/popcap/res_info/common.hpp
	./support/popcap/res_info/convert.hpp
	./support/popcap/resource_stream_bundle/definition.hpp
	./support/popcap/resource_stream_bundle/pack.hpp
	./support/popcap/resource_stream_bundle/unpack.hpp
	./support/popcap/resource_stream_bundle/miscellaneous/unpack_resource.hpp
	./support/popcap/resource_stream_bundle/miscellaneous/pack_resource.hpp
	./support/popcap/resource_stream_bundle_patch/common.hpp
	./support/popcap/resource_stream_bundle_patch/decode.hpp
	./support/popcap/resource_stream_bundle_patch/encode.hpp
	./support/popcap/resource_stream_group/common.hpp
	./support/popcap/resource_stream_group/definition.hpp
	./support/popcap/resource_stream_group/pack.hpp
	./support/popcap/resource_stream_group/unpack.hpp
	./support/popcap/re_animation/decode.hpp
	./support/popcap/re_animation/definition.hpp
	./support/popcap/re_animation/encode.hpp
	./support/popcap/re_animation/common.hpp
	./support/popcap/re_animation/to_xml.hpp
	./support/popcap/re_animation/from_xml.hpp
	./support/popcap/zlib/common.hpp
	./support/popcap/zlib/compress.hpp
	./support/popcap/zlib/uncompress.hpp
	./support/popcap/animation/convert/definition.hpp
	./support/popcap/animation/convert/from_flash.hpp
	./support/popcap/animation/convert/common.hpp
	./support/popcap/animation/convert/instance.hpp
	./support/popcap/animation/convert/resize.hpp
	./support/popcap/animation/convert/to_flash.hpp
	./support/popcap/animation/miscellaneous/common.hpp
	./support/popcap/animation/miscellaneous/dump.hpp
	./support/popcap/resource_stream_bundle/miscellaneous/unpack_cipher.hpp
	./support/popcap/re_animation/convert/from_flash.hpp
	./support/popcap/re_animation/convert/instance.hpp
	./support/popcap/re_animation/convert/to_flash.hpp
	./support/miscellaneous/shared.hpp
	./support/texture/compression/pvrtc/color.hpp
	./support/texture/compression/pvrtc/packet.hpp
	./support/texture/compression/pvrtc/pvrtc.hpp
	./support/texture/common.hpp
	./support/texture/encode.hpp
	./support/texture/decode.hpp
	./support/texture/invoke.hpp
	./support/wwise/sound_bank/definition.hpp
	./support/wwise/sound_bank/encode.hpp
	./support/wwise/sound_bank/decode.hpp
	./support/wwise/sound_bank/common.hpp
	./support/wwise/sound_bank/miscellaneous/support.hpp
	./support/marmalade/dzip/pack.hpp
	./support/marmalade/dzip/unpack.hpp
	./support/marmalade/dzip/common.hpp
	./support/utility.hpp
	./support/miscellaneous/custom/stream_compressed_group/common.hpp
	./support/miscellaneous/custom/stream_compressed_group/definition.hpp
	./support/miscellaneous/custom/stream_compressed_group/encode.hpp
	./support/miscellaneous/custom/stream_compressed_group/decode.hpp
	./support/miscellaneous/custom/resource_stream_bundle/common.hpp
	./support/miscellaneous/custom/resource_stream_bundle/definition.hpp
	./support/miscellaneous/custom/resource_stream_bundle/pack.hpp
	./support/miscellaneous/custom/resource_stream_bundle/unpack.hpp

	./interface/data.hpp
	./interface/cli.hpp
	./interface/callback.hpp
	./interface/version.hpp
	./interface/shell.hpp
	./interface/script.hpp
	./interface/interface.cpp
)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/dependencies/fmt)

set_target_properties(${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE CXX)

target_include_directories(
	${PROJECT_NAME} PRIVATE
	..
)

if (MSVC)
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Zi")
else ()
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")
endif ()

if(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
	target_compile_definitions(${PROJECT_NAME} PRIVATE MSVC_COMPILER)
endif()
if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
	target_compile_definitions(${PROJECT_NAME} PRIVATE CLANG_COMPILER)
endif()

target_compile_definitions(
	${PROJECT_NAME} PRIVATE
  	CANVAS_ITY_IMPLEMENTATION
)

target_compile_definitions( 
    ${PROJECT_NAME} PRIVATE 
    _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING 
) 
  
target_compile_definitions( 
    ${PROJECT_NAME} PRIVATE 
    CONFIG_BIGNUM 
)

target_include_directories(
	${PROJECT_NAME} PRIVATE
	../dependencies/fmt
)

if (MSVC)
	target_link_libraries(
		${PROJECT_NAME} PRIVATE
		dirent
	)
endif()

target_link_libraries(
	${PROJECT_NAME} PRIVATE
	md5
	zlib
	json
	fmt
	Rijndael
	SHA224CryptoServiceProvider
	SHA256
	cpp-base64
	libpng
	etc1
	tinyxml2
	SHA
	lzma
	avir
	open_vcdiff
	bzip2
	quickjs
	zip
	simdjson
	canvas_ity
	tsl
)

if (WIN32)
    target_compile_definitions(${PROJECT_NAME} PRIVATE "NOMINMAX")
    target_compile_definitions(${PROJECT_NAME} PRIVATE "_WINSOCKAPI_")
    target_compile_definitions(${PROJECT_NAME} PRIVATE "_UCRT_NOISY_NAN")
	# Disable MSVC Scream on WINSOCKAPI
	target_compile_options(${PROJECT_NAME} PRIVATE /wd4005)
endif()

if(WIN32)
    target_compile_definitions(${PROJECT_NAME} PRIVATE UNICODE _UNICODE)
endif()


if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    target_compile_options(
      ${PROJECT_NAME} PRIVATE
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
        -Wno-missing-noreturn
        -Wno-gnu-zero-variadic-macro-arguments
        -Wno-ctad-maybe-unsupported
        -Wno-global-constructors
        -Wno-exit-time-destructors
        -Wno-weak-vtables
        -Wno-self-assign
        -Wno-switch-enum
        -Wno-shadow-field 
        -Wno-shadow-field-in-constructor 
        -Wno-shadow-uncaptured-local 
        -Wno-unsafe-buffer-usage 
        -Wno-float-equal 
        -Wno-gnu-line-marker 
	)

if (MSVC)
    add_compile_options(/bigobj)
	set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} -std=c++23)
else ()
    add_compile_options(-Wa,-mbig-obj)
	# For some reason, Android doesn't support std=c++23 yet
	target_compile_options(${PROJECT_NAME} PRIVATE -std=c++20)
endif ()



endif()

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(MSVC)
	target_compile_definitions(
		${PROJECT_NAME} PRIVATE
		JS_STRICT_NAN_BOXING
	)
endif()


if(CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-strict-prototypes")
endif()

if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    target_compile_options(${PROJECT_NAME} PRIVATE
        -fmacro-prefix-map=${CMAKE_SOURCE_DIR}=.
    )
elseif (MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE
        /D "NDEBUG"
    )
endif()

if (NOT MSVC)
	set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -s")
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -s")
	set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -fvisibility=hidden")
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -fvisibility=hidden")
endif()