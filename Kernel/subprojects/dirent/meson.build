# https://github.com/tronkko/dirent

project (
    'dirent',
    'c',
    version: '1.24',
    default_options : [
        'warning_level=3',
        'c_std=c18',
        'buildtype=release',
        'b_ndebug=if-release',
        'optimization=3',
    ]
)

if meson.get_compiler('cpp').get_id() == 'clang'
    add_project_arguments('-fPIC', '-D_GNU_SOURCE', '-fvisibility=hidden', language : ['cpp'])
endif

headers = [
    'dirent.h',
]

install_headers(
    headers,
    subdir: 'dirent',
)

dirent_dep = declare_dependency(
    include_directories: include_directories('.'),
)

meson.override_dependency(
    'dirent',
    dirent_dep,
)