# meson.build

project(
    'quickjs',
    'c',
    version : '0.9.0',
    default_options : [
        'warning_level=3',
        'c_std=c11',
        'buildtype=release',
        'b_ndebug=if-release',
        'optimization=3',
    ]
)

if meson.get_compiler('c').get_id() == 'clang'
    add_project_arguments('-fPIC', '-D_GNU_SOURCE', '-fvisibility=hidden', language : ['c'])
endif

if meson.get_compiler('cpp').get_id() == 'msvc'
    add_project_arguments([
      '/DWIN32',
      '/DNOMINMAX',
      '/D_WINSOCKAPI_',
      '/D_UCRT_NOISY_NAN',
      '/wd4005',
      '/experimental:c11atomics',
      '/J',
    ], language: 'c')
endif

quickjs_headers = files(
    'cutils.h',
    'dirent_compat.h',
    'getopt_compat.h',
    'libbf.h',
    'libregexp-opcode.h',
    'libregexp.h',
    'libunicode-table.h',
    'libunicode.h',
    'list.h',
    'quickjs-atom.h',
    'quickjs-c-atomics.h',
    'quickjs-libc.h',
    'quickjs-opcode.h',
    'quickjs.h',
    'unicode_gen_def.h'
)

quickjs_sources = files(
    'cutils.c',
    'libbf.c',
    'libunicode.c',
    'libregexp.c',
    'qjs.c',
    'qjsc.c',
    'quickjs-libc.c',
    'quickjs.c',
    'unicode_gen.c'
)

c_options = []

if meson.get_compiler('c').get_id() == 'msvc'
    c_options += [
        '/wd4018',
        '/wd4061',
        '/wd4100',
        '/wd4200',
        '/wd4242',
        '/wd4244',
        '/wd4245',
        '/wd4267',
        '/wd4388',
        '/wd4389',
        '/wd4710',
        '/wd4711',
        '/wd4820',
        '/wd4996',
        '/wd5045',
        '/wd4146',
        '/wd4456',
        '/wd4457',
        '/wd4132',
        '/wd4127',
        '/wd4702',
        '/wd4701',
        '/wd4459',
        '/wd4334',
        '/wd4006',
        '/D_HAS_C11=1',
    ]
endif

if host_machine.system() == 'darwin' or host_machine.system() == 'ios'
    c_options += ['-Wno-maybe-uninitialized']
endif

if meson.get_compiler('c').get_id() == 'clang'
    c_options += ['-fPIC', '-D_GNU_SOURCE', '-fvisibility=hidden']
    c_options += [
        '-Wno-unsafe-buffer-usage',
        '-Wno-sign-conversion',
        '-Wno-nonportable-system-include-path',
        '-Wno-implicit-int-conversion',
        '-Wno-shorten-64-to-32',
        '-Wno-reserved-macro-identifier',
        '-Wno-reserved-identifier',
        '-Wdeprecated-declarations',
        '-Wno-implicit-fallthrough',
        '-Wno-sign-compare',
        '-Wno-missing-field-initializers',
        '-Wno-unused-parameter',
        '-Wno-unused-but-set-variable',
        '-Wno-array-bounds',
        '-Wno-format-truncation'
    ]
endif

add_project_arguments(c_options, language : ['c'])

if meson.get_compiler('c').get_id() == 'msvc'
    stack_size = 8 * 1024 * 1024
    add_project_link_arguments(f'/STACK:@stack_size@', language: 'c')
endif

quickjs_lib = static_library(
    'quickjs',
    quickjs_sources,
    include_directories: include_directories('.'),
    install: true,
)

headers = [
    'quickjs-atom.h',
    'quickjs-c-atomics.h',
    'quickjs-libc.h',
    'quickjs-opcode.h',
    'quickjs.h',
]

install_headers(
    headers,
    subdir: 'quickjs',
)

quickjs_dep = declare_dependency(
    link_with: quickjs_lib,
    include_directories: include_directories('.')
)

meson.override_dependency('quickjs', quickjs_dep)