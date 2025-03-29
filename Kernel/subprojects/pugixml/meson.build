# https://github.com/zeux/pugixml

project (
    'pugixml',
    'cpp',
    version: '1.15',
    default_options : [
        'warning_level=3',
        'c_std=c18',
        'cpp_std=c++latest',
        'buildtype=release',
        'b_ndebug=if-release',
        'optimization=3',
    ]
)

if meson.get_compiler('cpp').get_id() == 'clang'
    add_project_arguments('-fPIC', '-D_GNU_SOURCE', '-fvisibility=hidden', language : ['cpp'])
endif

pugixml_lib = static_library (
    'pugixml',
    sources: [
        'pugixml.cpp',
        'pugixml.hpp',
    ],
    install: true,
    pic: true,
)

headers = [
    'pugixml.hpp',
]

install_headers (
    headers,
    subdir: 'pugixml',
)

pugixml_dep = declare_dependency(
    link_with: pugixml_lib,
    include_directories: include_directories('.')
)

meson.override_dependency('pugixml', pugixml_dep)