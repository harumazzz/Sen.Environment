# https://sourceware.org/bzip2

project (
    'bzip2',
    'c',
    version: '1.0.8',
    default_options : [
        'warning_level=0',
        'c_std=c11',
        'buildtype=release',
        'b_ndebug=if-release',
        'optimization=3',
    ]
)

if meson.get_compiler('cpp').get_id() == 'msvc'
    add_project_arguments('/Zi', language: 'cpp')
else
    add_project_arguments('-g', language: 'cpp')
endif

if meson.get_compiler('c').get_id() == 'clang'
    add_project_arguments('-fPIC', '-D_GNU_SOURCE', '-fvisibility=hidden', language : ['c'])
endif

bzip2_lib = static_library (
    'bzip2',
    sources: [
        'bzlib.h',
        'bzlib_private.h',
        'blocksort.c',
        'huffman.c',
        'crctable.c',
        'randtable.c',
        'compress.c',
        'decompress.c',
        'bzlib.c',
        'bzip2.c',
        'bzip2recover.c',
    ],
    install: true,
    pic: true,
    include_directories: include_directories('.'),
)

bzip2_dep = declare_dependency(
    link_with: bzip2_lib,
    include_directories: include_directories('.')
)

meson.override_dependency('bzip2', bzip2_dep)