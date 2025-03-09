cmake_minimum_required(VERSION 3.20)

project(
        kernel
        LANGUAGES CXX
)

add_library(
        ${PROJECT_NAME} SHARED

        ./subprojects/fmt.hpp
        ./subprojects/zlib.hpp
        ./subprojects/Rijndael.hpp
        ./subprojects/libpng.hpp
        ./subprojects/etc1.hpp
        ./subprojects/lzma.hpp
        ./subprojects/avir.hpp
        ./subprojects/open_vcdiff.hpp
        ./subprojects/bzip2.hpp
        ./subprojects/quickjs.hpp
        ./subprojects/zip.hpp
        ./subprojects/dirent.hpp
        ./subprojects/magic_enum.hpp
        ./subprojects/canvas_ity.hpp
        ./subprojects/compile-time-regular-expressions.hpp
        ./subprojects/parallel-hashmap.hpp
        ./subprojects/thread-pool.hpp
        ./subprojects/mscharconv.hpp
        ./subprojects/jsoncons.hpp
	    ./subprojects/decodeless.hpp

        ./utility/macro.hpp
        ./utility/range/forward.hpp
        ./utility/range/reverse.hpp
        ./utility/library.hpp
        ./utility/literal.hpp
        ./utility/assert.hpp
        ./utility/utility.hpp

        ./utility/algorithm/math.hpp
        ./utility/algorithm/string_helper.hpp

        ./utility/miscellaneous/language/language.hpp
        ./utility/algorithm/pointer.hpp
        ./utility/container/array/array.hpp
        ./utility/container/array/byte_array.hpp
        ./utility/container/list/common.hpp
        ./utility/container/list/list.hpp
        ./utility/container/list/byte_list.hpp

        ./utility/miscellaneous/system/system.hpp
        ./utility/container/string/common.hpp
        ./utility/container/string/compile_string.hpp
        ./utility/container/string/basic_string.hpp
        ./utility/container/string/string_parser.hpp
        ./utility/container/map/common.hpp

        ./utility/encoding/apng/common.hpp
        ./utility/encoding/apng/setting.hpp
        ./utility/encoding/apng/writer.hpp

        ./utility/algorithm/utility.hpp
        ./utility/miscellaneous/clock/clock.hpp

        ./utility/trait/trait.hpp

        ./utility/encryption/crc/common.hpp
        ./utility/encryption/crc/crc32/crc32.hpp
        ./utility/encryption/crc/crc32/bzip2.hpp
        ./utility/encryption/crc/crc32/jamcrc.hpp
        ./utility/encryption/crc/crc32/common.hpp
        ./utility/encryption/crc/crc32/c.hpp
        ./utility/encryption/crc/crc32/d.hpp
        ./utility/encryption/crc/crc32/mpeg2.hpp
        ./utility/encryption/crc/crc32/posix.hpp
        ./utility/encryption/crc/crc32/q.hpp
        ./utility/encryption/crc/crc32/sata.hpp
        ./utility/encryption/crc/crc32/xfer.hpp
        ./utility/encryption/crc/crc8/common.hpp
        ./utility/encryption/crc/crc8/crc8.hpp
        ./utility/encryption/crc/crc8/cdma2000.hpp
        ./utility/encryption/crc/crc8/darc.hpp
        ./utility/encryption/crc/crc8/dvbs2.hpp
        ./utility/encryption/crc/crc8/ebu.hpp
        ./utility/encryption/crc/crc8/icode.hpp
        ./utility/encryption/crc/crc8/itu.hpp
        ./utility/encryption/crc/crc8/maxim.hpp
        ./utility/encryption/crc/crc8/rohc.hpp
        ./utility/encryption/crc/crc8/wcdma.hpp
        ./utility/encryption/crc/crc16/common.hpp
        ./utility/encryption/crc/crc16/arc.hpp
        ./utility/encryption/crc/crc16/augccitt.hpp
        ./utility/encryption/crc/crc16/buypass.hpp
        ./utility/encryption/crc/crc16/cdma2000.hpp
        ./utility/encryption/crc/crc16/ccittfalse.hpp
        ./utility/encryption/crc/crc16/dectr.hpp
        ./utility/encryption/crc/crc16/dds110.hpp
        ./utility/encryption/crc/crc16/dectx.hpp
        ./utility/encryption/crc/crc16/dnp.hpp
        ./utility/encryption/crc/crc16/en13757.hpp
        ./utility/encryption/crc/crc16/genibus.hpp
        ./utility/encryption/crc/crc16/t10dif.hpp
        ./utility/encryption/crc/crc16/kermit.hpp
        ./utility/encryption/crc/crc16/mcrf4xx.hpp
        ./utility/encryption/crc/crc16/maxim.hpp
        ./utility/encryption/crc/crc16/riello.hpp
        ./utility/encryption/crc/crc16/tms37157.hpp
        ./utility/encryption/crc/crc16/teledisk.hpp
        ./utility/encryption/crc/crc16/modbus.hpp
        ./utility/encryption/crc/crc16/x25.hpp
        ./utility/encryption/crc/crc16/usb.hpp
        ./utility/encryption/crc/crc16/xmodem.hpp
        ./utility/encryption/crc/crc.hpp

        ./utility/encryption/fnv/common.hpp

        ./utility/encryption/base64/common.hpp
        ./utility/encryption/base64/encode.hpp
        ./utility/encryption/base64/decode.hpp
        ./utility/encryption/md5/common.hpp
        ./utility/encryption/md5/hash.hpp

        ./utility/filesystem/file_handler.hpp
        ./utility/filesystem/windows_file_handler.hpp
        ./utility/algorithm/shared.hpp
        ./utility/algorithm/image/max_rects/common.hpp
        ./utility/algorithm/image/max_rects/extended_image.hpp
        ./utility/algorithm/image/max_rects/max_rects_bin.hpp
        ./utility/algorithm/image/max_rects/max_rects_packer.hpp

        ./utility/compression/zlib/common.hpp
        ./utility/compression/lzma/common.hpp
        ./utility/compression/bzip2/common.hpp
        ./utility/compression/bzip2/compress.hpp
        ./utility/compression/bzip2/uncompress.hpp
        ./utility/compression/lzma/compress.hpp
        ./utility/compression/lzma/uncompress.hpp
        ./utility/compression/zlib/compress.hpp
        ./utility/compression/zlib/uncompress.hpp
        ./utility/compression/zip/compress.hpp
        ./utility/compression/zip/uncompress.hpp

        ./utility/filesystem/utility.hpp
        ./utility/filesystem/path.hpp
        ./utility/filesystem/posix_file_handler.hpp
        ./utility/container/base/base.hpp
        ./utility/range/iterator.hpp
        ./utility/container/base/common.hpp
        ./utility/algorithm/image/transformation/crop.hpp
        ./utility/algorithm/image/transformation/rotate.hpp
        ./utility/algorithm/image/transformation/scale.hpp
        ./utility/algorithm/image/transformation/resize.hpp
        ./utility/algorithm/image/transformation/merge.hpp
        ./utility/algorithm/image/transformation/common.hpp
        ./utility/diff/vcdiff/common.hpp
        ./utility/diff/vcdiff/decode.hpp
        ./utility/diff/vcdiff/encode.hpp
        ./utility/encryption/fnv/hash.hpp

        ./utility/stream/base_stream.hpp
        utility/stream/read_memory_stream.hpp
        utility/stream/write_memory_stream.hpp

        ./support/popcap/animation/common.hpp
        ./support/popcap/animation/decode.hpp
        ./support/popcap/animation/encode.hpp
        ./support/popcap/animation/model.hpp
        ./support/popcap/character_font_widget_2/decode.hpp
        ./support/popcap/character_font_widget_2/encode.hpp
        ./support/popcap/character_font_widget_2/model.hpp
        ./support/popcap/compiled_text/decode.hpp
        ./support/popcap/compiled_text/encode.hpp
        ./support/popcap/crypt_data/common.hpp
        ./support/popcap/crypt_data/decrypt.hpp
        ./support/popcap/crypt_data/encrypt.hpp
        ./support/popcap/new_type_object_notation/decode.hpp
        ./support/popcap/new_type_object_notation/encode.hpp
        ./support/popcap/package/common.hpp
        ./support/popcap/package/model.hpp
        ./support/popcap/package/pack.hpp
        ./support/popcap/package/unpack.hpp
        ./support/popcap/particles/common.hpp
        ./support/popcap/particles/decode.hpp
        ./support/popcap/particles/encode.hpp
        ./support/popcap/particles/model.hpp
        ./support/popcap/particles/to_xml.hpp
        ./support/popcap/particles/from_xml.hpp
        ./support/popcap/player_info/common.hpp
        ./support/popcap/player_info/decode.hpp
        support/popcap/player_info/model.hpp
        ./support/popcap/player_info/encode.hpp
        ./support/popcap/reflection_object_notation/common.hpp
        ./support/popcap/reflection_object_notation/decode.hpp
        ./support/popcap/reflection_object_notation/encode.hpp
        support/popcap/reflection_object_notation/encrypt.hpp
        ./support/popcap/render_effects/common.hpp
        ./support/popcap/render_effects/decode.hpp
        support/popcap/render_effects/model.hpp
        ./support/popcap/render_effects/encode.hpp
        ./support/popcap/resource_group/common.hpp
        ./support/popcap/resource_group/split.hpp
        ./support/popcap/resource_group/merge.hpp
        ./support/popcap/resource_group/convert.hpp
        ./support/popcap/res_info/common.hpp
        ./support/popcap/res_info/split.hpp
        ./support/popcap/res_info/merge.hpp
        ./support/popcap/res_info/convert.hpp
        support/popcap/resource_stream_bundle/model.hpp
        ./support/popcap/resource_stream_bundle/pack.hpp
        ./support/popcap/resource_stream_bundle/unpack.hpp
        ./support/popcap/resource_stream_bundle/common.hpp
        ./support/popcap/resource_stream_bundle/miscellaneous/unpack_resource.hpp
        ./support/popcap/resource_stream_bundle/miscellaneous/pack_resource.hpp
        ./support/popcap/resource_stream_bundle_patch/common.hpp
        ./support/popcap/resource_stream_bundle_patch/decode.hpp
        ./support/popcap/resource_stream_bundle_patch/encode.hpp
        ./support/popcap/resource_stream_group/common.hpp
        ./support/popcap/resource_stream_group/model.hpp
        ./support/popcap/resource_stream_group/pack.hpp
        ./support/popcap/resource_stream_group/unpack.hpp
        ./support/popcap/re_animation/decode.hpp
        ./support/popcap/re_animation/encode.hpp
        ./support/popcap/re_animation/common.hpp
        ./support/popcap/re_animation/to_xml.hpp
        ./support/popcap/re_animation/from_xml.hpp
        ./support/popcap/re_animation/model.hpp
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
        ./support/popcap/animation/miscellaneous/convert.hpp
        ./support/popcap/animation/miscellaneous/dump.hpp
        ./support/popcap/resource_stream_bundle/miscellaneous/unpack_cipher.hpp
        ./support/popcap/re_animation/convert/from_flash.hpp
        ./support/popcap/re_animation/convert/instance.hpp
        ./support/popcap/re_animation/convert/to_flash.hpp
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
        ./support/marmalade/dzip/pack.hpp
        ./support/marmalade/dzip/unpack.hpp
        ./support/marmalade/dzip/common.hpp
        ./support/utility.hpp
        ./support/miscellaneous/project/stream_compressed_group/common.hpp
        ./support/miscellaneous/project/stream_compressed_group/convert.hpp
        ./support/miscellaneous/project/stream_compressed_group/definition.hpp
        ./support/miscellaneous/project/stream_compressed_group/encode.hpp
        ./support/miscellaneous/project/stream_compressed_group/decode.hpp
        ./support/miscellaneous/project/resource_stream_bundle/common.hpp
        ./support/miscellaneous/project/resource_stream_bundle/definition.hpp
        ./support/miscellaneous/project/resource_stream_bundle/convert.hpp
        ./support/miscellaneous/project/resource_stream_bundle/pack.hpp
        ./support/miscellaneous/project/resource_stream_bundle/unpack.hpp

        ./interface/data.hpp
        interface/context.hpp
        ./interface/version.hpp
        ./interface/shell.hpp
        ./interface/api/method.hpp
        ./interface/api/class.hpp
        ./interface/interface.cpp
        utility/encryption/rijndael/encrypt.hpp
        utility/encryption/rijndael/decrypt.hpp
        utility/javascript/common.hpp
        utility/javascript/runtime.hpp
        utility/javascript/runtime.hpp
        utility/javascript/context.hpp
        utility/javascript/value.hpp
        utility/javascript/trait.hpp
        utility/javascript/js_string.hpp
        interface/runtime.hpp
        utility/javascript/proxy.hpp
        utility/stream/readable_stream.hpp
        utility/stream/writeable_stream.hpp
        utility/stream/read_file_stream.hpp
        utility/byte_stream/mapped_stream.hpp
        utility/byte_stream/common.hpp
        utility/byte_stream/writer_stream.hpp
        support/popcap/reflection_object_notation/decrypt.hpp
        utility/container/array_view/array_view.hpp
        utility/container/list_view/list_view.hpp
        utility/container/list_view/byte_view.hpp
        utility/container/array_view/byte_view.hpp
        support/popcap/new_type_object_notation/common.hpp
        utility/container/string_view/string_view.hpp
        support/popcap/text_table/common.hpp
        support/popcap/text_table/convert.hpp
        utility/container/map/linear_map.hpp
        interface/api/service.hpp
        utility/container/set/common.hpp
        utility/trait/pointer.hpp
        utility/trait/callable.hpp
        utility/trait/container.hpp
        utility/trait/utility.hpp
        utility/trait/numeric.hpp
        utility/container/variant/common.hpp
        utility/exception/common.hpp
        utility/stream/write_file_stream.hpp
        utility/stream/output/output_stream.hpp
        utility/stream/output/buffered_output_stream.hpp
        utility/container/allocator.hpp
        utility/encoding/xml/comment.hpp
        utility/encoding/xml/common.hpp
        utility/encoding/xml/element.hpp
        utility/encoding/xml/node.hpp
        utility/encoding/xml/text.hpp
        utility/encoding/xml/read.hpp
        utility/encoding/xml/write.hpp
        interface/executor.hpp

)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/subprojects/fmt)

set_target_properties(${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE CXX)

target_include_directories(
        ${PROJECT_NAME} PRIVATE
        ..
)

if (MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Zi")
else ()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")
endif ()


if (CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    target_compile_definitions(${PROJECT_NAME} PRIVATE MSVC_COMPILER)
endif ()
if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    target_compile_definitions(${PROJECT_NAME} PRIVATE CLANG_COMPILER)
endif ()

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
        ../subprojects/fmt
)

if (MSVC)
    target_link_libraries(
            ${PROJECT_NAME} PRIVATE
            dirent
    )
endif ()

target_link_libraries(
        ${PROJECT_NAME} PRIVATE
        zlib
        fmt
        Rijndael
        libpng
        etc1
        lzma
        avir
        open_vcdiff
        bzip2
        quickjs
        zip
        canvas_ity
        compile-time-regular-expressions
        parallel-hashmap
        thread-pool
        jsoncons
        pugixml
        pvrtc
	    decodeless
)

if (WIN32)
    target_compile_definitions(${PROJECT_NAME} PRIVATE "NOMINMAX")
    target_compile_definitions(${PROJECT_NAME} PRIVATE "_WINSOCKAPI_")
    target_compile_definitions(${PROJECT_NAME} PRIVATE "_UCRT_NOISY_NAN")
    # Disable MSVC Scream on WINSOCKAPI
    target_compile_options(${PROJECT_NAME} PRIVATE /wd4005)
endif ()

if (WIN32)
    target_compile_definitions(${PROJECT_NAME} PRIVATE UNICODE _UNICODE)
endif ()

add_definitions(-D_CRT_SECURE_NO_WARNINGS)

if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
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
        add_definitions(-DNOMINMAX)
    else ()
        add_compile_options(-Wa,-mbig-obj)
        # For some reason, Android doesn't support std=c++23 yet
        target_compile_options(${PROJECT_NAME} PRIVATE -std=c++20)
    endif ()


endif ()

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


if (CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-strict-prototypes")
endif ()

if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    target_compile_options(${PROJECT_NAME} PRIVATE
            -fmacro-prefix-map=${CMAKE_SOURCE_DIR}=.
    )
elseif (MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE
            /D "NDEBUG"
    )
endif ()

option(DEBUG_LOG "Enable debug log" ON)

if (DEBUG_LOG)
    add_definitions(-DDEBUG_LOG)
    message(STATUS "Enabled debug log")
endif ()

if (NOT MSVC)
    set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -s")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -s")
    set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -fvisibility=hidden")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -fvisibility=hidden")
endif ()

if (MSVC)
    target_compile_options(
            ${PROJECT_NAME} PRIVATE
            /W4
            /WX
            /permissive-
            /wd4100
            /wd4189
            /wd4324
            /wd4456
            /wd4458
            /wd4996
    )

    target_compile_options(
            ${PROJECT_NAME} PRIVATE
            /external:W0
            /external:anglebrackets
            /external:env:EXTERNAL
    )
endif ()