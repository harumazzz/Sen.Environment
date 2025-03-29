# https://github.com/google/open-vcdiff

project (
    'open_vcdiff',
    'cpp',
    version: '0.8.4',
    default_options : [
        'warning_level=3',
        'c_std=c18',
        'cpp_std=c++17',
        'buildtype=release',
        'b_ndebug=if-release',
        'optimization=3',
    ]
)

if meson.get_compiler('cpp').get_id() == 'clang'
    add_project_arguments('-fPIC', '-D_GNU_SOURCE', '-fvisibility=hidden', language : ['cpp'])
endif

zlib_dep = dependency('zlib', required: true)

open_vcdiff_lib = static_library (
    'open_vcdiff',
    sources: [
        'google/format_extension_flags.h',
        'google/output_string.h',
        'google/vcdecoder.h',
        'google/vcencoder.h',
        'addrcache.h',
        'blockhash.h',
        'checksum.h',
        'codetable.h',
        'codetablewriter_interface.h',
        'compile_assert.h',
        'config.h',
        'decodetable.h',
        'encodetable.h',
        'headerparser.h',
        'instruction_map.h',
        'jsonwriter.h',
        'logging.h',
        'rolling_hash.h',
        'unique_ptr.h',
        'varint_bigendian.h',
        'vcdiff_defs.h',
        'vcdiffengine.h',
        'addrcache.cc',
        'blockhash.cc',
        'codetable.cc',
        'decodetable.cc',
        'encodetable.cc',
        'headerparser.cc',
        'instruction_map.cc',
        'jsonwriter.cc',
        'logging.cc',
        'varint_bigendian.cc',
        'vcdecoder.cc',
        'vcdiffengine.cc',
        'vcencoder.cc',
    ],
    install: true,
    pic: true,
    dependencies: [zlib_dep],
)

headers = [
    'google/format_extension_flags.h',
    'google/output_string.h',
    'google/vcdecoder.h',
    'google/vcencoder.h',
    'addrcache.h',
    'blockhash.h',
    'checksum.h',
    'codetable.h',
    'codetablewriter_interface.h',
    'compile_assert.h',
    'config.h',
    'decodetable.h',
]

install_headers(
    headers,
    subdir: 'open_vcdiff',
)

open_vcdiff_dep = declare_dependency(
    link_with: open_vcdiff_lib,
    include_directories: include_directories('.'),
)

meson.override_dependency('open_vcdiff', open_vcdiff_dep)