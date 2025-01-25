#Homepage: https://github.com/hanickadot/compile-time-regular-expressions

project(
    'compile-time-regular-expressions',
    ['cpp'],
    version: '3.9.0',
    meson_version: '>=1.3.0',
    default_options :
    [
        'warning_level=3',
        'buildtype=release',
        'optimization=3',
        'cpp_std=c++latest',
    ],
)

sources = [
    './ctre-unicode.hpp',
    './unicode-db.hpp',
    './ctre.hpp',
]

ctre = declare_dependency (
    include_directories: include_directories('./'),
    sources: sources,
)