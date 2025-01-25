#Homepage: https://github.com/leethomason/cpp-base64

project (
    'cpp-base64',
    ['cpp'],
    version: '1.0.0',
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
    './base64.h',
    './base64.cpp',
]

if meson.get_compiler('cpp').get_id() == 'msvc'
    add_project_arguments(['/utf-8'], language: 'cpp')
else
    add_project_arguments(['-finput-charset=UTF-8'], language: 'cpp')
endif

cpp_base64 = static_library(
    'cpp-base64',
    sources: sources,
    include_directories: include_directories('.'),
    pic: true,
    install: false,
)