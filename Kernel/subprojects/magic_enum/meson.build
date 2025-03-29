# https://github.com/Neargye/magic_enum

project (
    'magic_enum',
    'cpp',
    version: '0.9.7',
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

fmt_dep = dependency('fmt', required: true)

headers = [
    'magic_enum_all.hpp',
    'magic_enum_containers.hpp',
    'magic_enum_flags.hpp',
    'magic_enum_format.hpp',
    'magic_enum_iostream.hpp',
    'magic_enum_fuse.hpp',
    'magic_enum_switch.hpp',
    'magic_enum_utility.hpp',
    'magic_enum.hpp',
]

magic_enum_dep = declare_dependency(
    include_directories: include_directories('.'),
    dependencies: [fmt_dep],
)

install_headers(
    headers,
    subdir: 'magic_enum',
)

meson.override_dependency('magic_enum', magic_enum_dep)