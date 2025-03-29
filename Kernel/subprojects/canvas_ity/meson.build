# https://github.com/a-e-k/canvas_ity/tree/main

project (
    'canvas_ity',
    'cpp',
    version: '1.0.0',
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

headers = [
    'canvas_ity.hpp',
]

install_headers(
    headers,
    subdir: 'canvas_ity'
)

canvas_ity_dep = declare_dependency(
    include_directories: include_directories('.')
)

meson.override_dependency('canvas_ity', canvas_ity_dep)