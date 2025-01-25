# Homepage : https://www.7-zip.org/sdk.html

project (
    'lzma',
    ['c'],
    version: '24.09',
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
    './Threads.c',
    './LzFindOpt.c',
    './LzmaLib.c',
]

if meson.get_compiler('c').get_id() == 'msvc'
    add_project_arguments(['/utf-8'], language: 'c')
else
    add_project_arguments(['-finput-charset=UTF-8'], language: 'c')
endif

lzma = static_library(
    'lzma',
    sources: sources,
    include_directories: include_directories('./'),
    pic: true,
    install: false,
)

if host_machine.system() != 'windows'
    add_project_arguments(['-D_7ZIP_ST'], 'c')
endif