# ?
project (
    'Rijndael',
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

rijndael_lib = static_library (
    'rijndael',
    sources: [
        'Rijndael.cpp',
        'Rijndael.h',
    ],
    pic: true,
    include_directories: include_directories('.'),
)

rijndael_dep = declare_dependency(
    link_with: rijndael_lib,
    include_directories: include_directories('.'),
)

meson.override_dependency('Rijndael', rijndael_dep)