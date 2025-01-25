# Homepage: https://www.bzip.org/
project (
    'bzip2',
    ['c'],
    version: '1.0.8',
    meson_version: '>=1.3.0',
    default_options :
    [
        'warning_level=3',
        'buildtype=release',
        'optimization=3',
        'c_std=c17',
    ],
)

sources = [
    './bzlib.h',
    './bzlib_private.h',
    './blocksort.c',
    './huffman.c',
    './crctable.c',
    './randtable.c',
    './compress.c',
    './decompress.c',
    './bzlib.c',
    './bzip2.c',
    './bzip2recover.c',
]

if meson.get_compiler('c').get_id() == 'msvc'
    add_project_arguments(['/utf-8'], language: 'c')
else
    add_project_arguments(['-finput-charset=UTF-8'], language: 'c')
endif

bzip2 = static_library (
    'bzip2',
    sources,
    include_directories: include_directories('./'),
    pic: true,
    install: false,
)