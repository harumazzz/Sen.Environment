#Homepage : https://github.com/tronkko/dirent

project(
    'dirent',
    ['c'],
    version: '1.24',
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
    './dirent.h',
]

dirent = declare_dependency (
    sources: sources,
    include_directories: include_directories('./'),
)