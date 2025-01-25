# Homepage : https://github.com/pr0f3ss/SHA

project (
    'SHA',
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
    './SHA384.h',
    './SHA384.cpp',
    './SHA512.h',
    './SHA512.cpp',
]

if meson.get_compiler('cpp').get_id() == 'msvc'
    add_project_arguments(['/utf-8'], language: 'cpp')
else
    add_project_arguments(['-finput-charset=UTF-8'], language: 'cpp')
endif

SHA = static_library (
    'SHA',
    sources: sources,
    include_directories: include_directories('./'),
    pic: true,
    install: false,
)