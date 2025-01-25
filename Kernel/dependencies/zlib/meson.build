#Homepage : https://www.zlib.net/

project (
    'zlib',
    ['c'],
    version: '1.3.1',
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
    './zconf.h',
    './zlib.h',
    './crc32.h',
    './deflate.h',
    './gzguts.h',
    './inffast.h',
    './inffixed.h',
    './inflate.h',
    './inftrees.h',
    './trees.h',
    './zutil.h',
    './adler32.c',
    './compress.c',
    './crc32.c',
    './deflate.c',
    './gzclose.c',
    './gzlib.c',
    './gzread.c',
    './gzwrite.c',
    './infback.c',
    './inffast.c',
    './inflate.c',
    './inftrees.c',
    './trees.c',
    './uncompr.c',
    './zutil.c',
]

if meson.get_compiler('c').get_id() == 'msvc'
    add_project_arguments(['/utf-8'], language: 'c')
else
    add_project_arguments(['-finput-charset=UTF-8'], language: 'c')
endif

platform = host_machine.system()

if platform == 'linux' or platform == 'macos' or platform == 'android' or platform == 'ios'
    add_project_arguments(['-DZLIB_HAVE_UNISTD_H', '-D_LARGEFILE64_SOURCE=1'], language: 'c')
endif

zlib = static_library(
    'zlib',
    sources,
    install: false,
    pic: true,
)