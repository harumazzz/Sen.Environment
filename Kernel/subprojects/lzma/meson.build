# https://www.7-zip.org/sdk.html

project(
    'lzma',
    'c',
    version: '5.2.5',
    default_options : [
        'warning_level=0',
        'c_std=c18',
        'buildtype=release',
        'b_ndebug=if-release',
        'optimization=3',
    ]
)

if meson.get_compiler('c').get_id() == 'clang'
    add_project_arguments('-fPIC', '-D_GNU_SOURCE', '-fvisibility=hidden', language : ['c'])
endif

if meson.get_compiler('c').get_id() != 'msvc'
    add_project_arguments('-DLZMA_API_NAMESPACE=7z', language : ['c'])
    add_project_arguments('-D_7ZIP_ST', language : ['c'])
endif

lzma_lib = static_library (
    'lzma',
    sources: [
        './7zTypes.h',
        './7zWindows.h',
        './Alloc.h',
        './Alloc.c',
        './Compiler.h',
        './CpuArch.h',
        './CpuArch.c',
        './LzFind.h',
        './LzFind.c',
        './LzFindMt.h',
        './LzFindMt.c',
        './LzHash.h',
        './LzmaDec.h',
        './LzmaDec.c',
        './LzmaEnc.h',
        './LzmaEnc.c',
        './LzmaLib.h',
        './Precomp.h',
        './Threads.h',
        './LzFindOpt.c',
        './LzmaLib.c',
        './Threads.c',
    ],
    install: true,
    pic: true,
    include_directories: include_directories('.'),
)

headers = [
    '7zTypes.h',
    '7zWindows.h',
    'Alloc.h',
    'Compiler.h',
    'CpuArch.h',
    'LzFind.h',
    'LzFindMt.h',
    'LzHash.h',
    'LzmaDec.h',
    'LzmaEnc.h',
    'LzmaLib.h',
    'Precomp.h',
    'Threads.h',
]

install_headers(
    headers,
    subdir: 'lzma',
)

lzma_dep = declare_dependency(
    link_with: lzma_lib,
    include_directories: include_directories('.')
)

meson.override_dependency('lzma', lzma_dep)

