# Homepage : https://github.com/redduxi/SHA-224

project(
    'SHA224CryptoServiceProvider',
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
    './SHA224CryptoServiceProvider.h',
    './SHA224CryptoServiceProvider.cpp',
    './main.cpp',
]

if meson.get_compiler('cpp').get_id() == 'msvc'
    add_project_arguments(['/utf-8'], language: 'cpp')
else
    add_project_arguments(['-finput-charset=UTF-8'], language: 'cpp')
endif

SHA224CryptoServiceProvider = static_library (
    'SHA224CryptoServiceProvider',
    sources: sources,
    include_directories: include_directories('./'),
    pic: true,
    install: false,
)