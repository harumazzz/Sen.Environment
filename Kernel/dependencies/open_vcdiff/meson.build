# Homepage : https://github.com/google/open-vcdiff

project(
    'open_vcdiff',
    ['cpp'],
    version: '0.8.4',
    default_options :
    [
        'warning_level=3',
        'buildtype=release',
        'optimization=3',
        'cpp_std=c++17',
    ],
)

sources = [
    './google/format_extension_flags.h',
    './google/output_string.h',
    './addrcache.h',
    './blockhash.h',
    './checksum.h',
    './codetable.h',
    './codetablewriter_interface.h',
    './compile_assert.h',
    './config.h',
    './decodetable.h',
    './encodetable.h',
    './headerparser.h',
    './instruction_map.h',
    './jsonwriter.h',
    './logging.h',
    './rolling_hash.h',
    './unique_ptr.h',
    './varint_bigendian.h',
    './vcdiff_defs.h',
    './vcdiffengine.h',
    './addrcache.cc',
    './blockhash.cc',
    './codetable.cc',
    './decodetable.cc',
    './encodetable.cc',
    './headerparser.cc',
    './instruction_map.cc',
    './jsonwriter.cc',
    './logging.cc',
    './varint_bigendian.cc',
    './vcdecoder.cc',
    './vcdiffengine.cc',
    './vcencoder.cc',
]

if meson.get_compiler('cpp').get_id() == 'msvc'
    add_project_arguments(['/utf-8'], language: 'cpp')
else
    add_project_arguments(['-finput-charset=UTF-8'], language: 'cpp')
endif

open_vcdiff = static_library (
    'open_vcdiff',
    sources: sources,
    include_directories: [
        include_directories('./'),
        include_directories('../zlib'),
    ],
    install: false,
    pic: true,
)