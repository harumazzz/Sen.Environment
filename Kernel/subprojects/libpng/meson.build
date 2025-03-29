# http://www.libpng.org/

project(
    'libpng',
    'c',
    version: '1.6.46',
    default_options : [
        'warning_level=0',
        'c_std=c11',
        'buildtype=release',
        'b_ndebug=if-release',
        'optimization=3',
    ]
)

if meson.get_compiler('c').get_id() == 'clang'
    add_project_arguments('-fPIC', '-D_GNU_SOURCE', '-fvisibility=hidden', language : ['c'])
endif

if host_machine.cpu_family().startswith('arm') or host_machine.system() == 'android'
    add_project_arguments('-DPNG_ARM_NEON_OPT=0', language : 'c')
endif

zlib_dep = dependency('zlib', required: true)

libpng_lib = static_library (
    'libpng',
    sources: [
        'png.h',
        'pngconf.h',
        'pnglibconf.h',
        'pngpriv.h',
        'pngdebug.h',
        'pnginfo.h',
        'pngstruct.h',
        'png.c',
        'pngerror.c',
        'pngget.c',
        'pngmem.c',
        'pngpread.c',
        'pngread.c',
        'pngrio.c',
        'pngrtran.c',
        'pngrutil.c',
        'pngset.c',
        'pngtrans.c',
        'pngwio.c',
        'pngwrite.c',
        'pngwtran.c',
        'pngwutil.c',
    ],
    include_directories: [
        '.',
    ],
    install: true,
    pic: true,
    dependencies: [zlib_dep],
)

headers = [
    'png.h',
    'pngconf.h',
    'pnglibconf.h',
    'pngpriv.h',
    'pngdebug.h',
    'pnginfo.h',
    'pngstruct.h',
]

install_headers(
    headers,
    subdir: 'libpng',
)

libpng_dep = declare_dependency(
    link_with: libpng_lib,
    include_directories: include_directories('.')
)

meson.override_dependency('libpng', libpng_dep)