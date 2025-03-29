# https://github.com/danielaparker/jsoncons

project(
    'jsoncons',
    'cpp',
    version : '1.3.0',
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

jsoncons_include_dir = include_directories('include')

jsoncons_dep = declare_dependency(
    include_directories : jsoncons_include_dir,
    link_with : []
)

meson.override_dependency('jsoncons', jsoncons_dep)