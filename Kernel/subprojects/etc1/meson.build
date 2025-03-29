# ?
project (
    'etc1',
    'cpp',
    version: '1.0.0',
    default_options : [
        'warning_level=3',
        'c_std=c18',
        'cpp_std=c++latest',
        'buildtype=release',
        'b_ndebug=if-release',
        'optimization=3',
    ]
)

if meson.get_compiler('cpp').get_id() == 'clang'
    add_project_arguments('-fPIC', '-D_GNU_SOURCE', '-fvisibility=hidden', language : ['cpp'])
endif

zlib_dep = dependency('zlib', required: true)

etc1_lib = static_library (
    'etc1',
    sources: [
        'etc1.cpp',
        'etc1.h',
    ],
    install: true,
    pic: true,
    dependencies: [zlib_dep],
    include_directories: include_directories('.'),
)

etc1_dep = declare_dependency(
    link_with: etc1_lib,
    include_directories: include_directories('.')
)

meson.override_dependency('etc1', etc1_dep)