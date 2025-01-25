# Homepage : https://github.com/leethomason/tinyxml2

project(
    'tinyxml2',
    ['cpp'],
    version: '10.0.0',
    default_options :
    [
        'warning_level=3',
        'buildtype=release',
        'optimization=3',
        'cpp_std=c++latest',
    ],
)

sources = [
    './tinyxml2.h',
    './tinyxml2.cpp',
]

if meson.get_compiler('cpp').get_id() == 'msvc'
    add_project_arguments(['/utf-8'], language: 'cpp')
else
    add_project_arguments(['-finput-charset=UTF-8'], language: 'cpp')
endif

tinyxml2 = static_library (
    'tinyxml2',
    sources: sources,
    include_directories: include_directories('./'),
    install: false,
    pic: true,
)