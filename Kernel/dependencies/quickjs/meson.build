# Homepage : https://github.com/quickjs-ng/quickjs

project(
    'quickjs',
    ['c'],
    version: '0.8.0',
    default_options :
    [
        'warning_level=3',
        'buildtype=release',
        'optimization=3',
        'c_std=c11',
    ],
)

sources = [
    './cutils.h',
    './dirent_compat.h',
    './getopt_compat.h',
    './libbf.h',
    './libregexp-opcode.h',
    './libregexp.h',
    './libunicode-table.h',
    './libunicode.h',
    './list.h',
    './quickjs-atom.h',
    './quickjs-c-atomics.h',
    './quickjs-libc.h',
    './quickjs-opcode.h',
    './quickjs.h',
    './unicode_gen_def.h',
    './cutils.c',
    './libbf.c',
    './libunicode.c',
    './libregexp.c',
    './qjs.c',
    './qjsc.c',
    './quickjs-libc.c',
    './quickjs.c',
    './unicode_gen.c',
]

if meson.get_compiler('c').get_id() == 'msvc'
    add_project_arguments(['/utf-8'], language: 'c')
else
    add_project_arguments(['-fPIC'], language: 'c')
endif

if meson.get_compiler('c').get_id() == 'msvc'
    add_compile_definitions('NOMINMAX', '_WINSOCKAPI_', '_UCRT_NOISY_NAN', '_HAS_C11=1')
    add_project_arguments('/wd4005', '/wd4018', '/wd4061', '/wd4100', '/wd4200', '/wd4242', '/wd4244', '/wd4245',
                          '/wd4267', '/wd4388', '/wd4389', '/wd4710', '/wd4711', '/wd4820', '/wd4996', '/wd5045',
                          language: 'c')
    add_project_arguments('/std:c11', '/experimental:c11atomics', language: 'c')
endif

quickjs = static_library (
    'quickjs',
    sources: sources,
    include_directories: include_directories('./'),
    install: false,
    pic: true,
)