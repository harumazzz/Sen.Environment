# Homepage : https://github.com/JieweiWei/md5

project(
    'md5',
    ['cpp'],
    version : '1.0',
    default_options :
    [
        'warning_level=3',
        'buildtype=release',
        'optimization=3',
        'cpp_std=c++latest',
    ],
)

sources = [
    './md5.h',
    './md5.cpp',
]

if meson.get_compiler('cpp').get_id() == 'msvc'
    add_project_arguments(['/utf-8'], language: 'cpp')
else
    add_project_arguments(['-finput-charset=UTF-8'], language: 'cpp')
endif

md5 = static_library (
    'md5',
    sources: sources,
    include_directories: include_directories('./'),
    pic: true,
    install: false,
)