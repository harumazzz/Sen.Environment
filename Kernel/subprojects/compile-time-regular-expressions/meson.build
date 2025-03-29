# https://github.com/hanickadot/compile-time-regular-expressions

project (
    'compile-time-regular-expressions',
    'cpp',
    version: '3.9.0',
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

headers = [
    'ctre-unicode.hpp',
    'unicode-db.hpp',
    'ctre.hpp',
]

install_headers(
    headers,
    subdir: 'compile-time-regular-expressions'
)

ctre_dep = declare_dependency(
    include_directories: include_directories('.')
)

meson.override_dependency('compile-time-regular-expressions', ctre_dep)