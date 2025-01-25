# Homepage : https://github.com/System-Glitch/SHA256

project (
    'SHA256',
    ['cpp'],
    version: '0',
    default_options :
    [
        'warning_level=3',
        'buildtype=release',
        'optimization=3',
        'cpp_std=c++latest',
    ],
)

sources = [
    './SHA256.h',
    './SHA256.cpp',
]

if meson.get_compiler('cpp').get_id() == 'msvc'
    add_project_arguments(['/utf-8'], language: 'cpp')
else
    add_project_arguments(['-finput-charset=UTF-8'], language: 'cpp')
endif

SHA256 = static_library (
    'SHA256',
    sources: sources,
    include_directories: include_directories('./'),
    pic: true,
    install: false,
)