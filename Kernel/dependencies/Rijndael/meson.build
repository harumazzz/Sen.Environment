# Homepage : None

project(
    'Rijndael',
    ['cpp'],
    version: '1.0',
    default_options :
    [
        'warning_level=3',
        'buildtype=release',
        'optimization=3',
        'cpp_std=c++latest',
    ],
)

sources = [
    './rijndael.h',
    './rijndael.cpp',
]

if meson.get_compiler('cpp').get_id() == 'msvc'
    add_project_arguments(['/utf-8'], language: 'cpp')
else
    add_project_arguments(['-finput-charset=UTF-8'], language: 'cpp')
endif

Rijndael = static_library (
    'Rijndael',
    sources: sources,
    include_directories: include_directories('./'),
    pic: true,
    install: false,
)