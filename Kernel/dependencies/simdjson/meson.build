# Homepage : https://simdjson.org/

project(
    'simdjson',
    ['cpp'],
    version: '3.11.6',
    default_options :
    [
        'warning_level=3',
        'buildtype=release',
        'optimization=3',
        'cpp_std=c++latest',
    ],
)

sources = [
    './simdjson.h',
    './simdjson.cpp',
]

if meson.get_compiler('cpp').get_id() == 'msvc'
    add_project_arguments(['/utf-8'], language: 'cpp')
else
    add_project_arguments(['-finput-charset=UTF-8'], language: 'cpp')
endif

simdjson = static_library (
    'simdjson',
    sources: sources,
    include_directories: include_directories('./'),
    pic: true,
    install: false,
)