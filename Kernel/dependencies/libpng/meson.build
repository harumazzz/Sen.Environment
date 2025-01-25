# Homepage : http://www.libpng.org/

project (
    'libpng',
    ['c'],
    version: '1.6.45',
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
    './png.h',
    './pngconf.h',
    './pnglibconf.h',
    './pngpriv.h',
    './pngdebug.h',
    './pnginfo.h',
    './pngstruct.h',
    './png.c',
    './pngerror.c',
    './pngget.c',
    './pngmem.c',
    './pngpread.c',
    './pngread.c',
    './pngrio.c',
    './pngrtran.c',
    './pngrutil.c',
    './pngset.c',
    './pngmem.c',
    './pngtrans.c',
    './pngwio.c',
    './pngwrite.c',
    './pngwtran.c',
    './pngwutil.c',
]

if meson.get_compiler('c').get_id() == 'msvc'
    add_project_arguments(['/utf-8'], language: 'c')
else
    add_project_arguments(['-finput-charset=UTF-8'], language: 'c')
endif

zlib_inc = include_directories('../zlib')

libpng = static_library(
    'png',
    sources: sources,
    include_directories: [include_directories('./'), zlib_inc],
    pic: true,
    install: false,
)

if host_machine.cpu_family() == 'arm' or (meson.is_cross_build() and meson.get_cross_property('android'))
    libpng.add_project_compile_definitions('PNG_ARM_NEON_OPT=0')
endif