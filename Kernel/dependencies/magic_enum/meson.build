# Homepage : https://github.com/Neargye/magic_enum

project (
    'magic_enum',
    ['cpp'],
    version: '0.9.7',
    default_options :
    [
        'warning_level=3',
        'buildtype=release',
        'optimization=3',
        'cpp_std=c++latest',
    ],
)

sources = [
    './magic_enum_all.hpp',
    './magic_enum_containers.hpp',
    './magic_enum_flags.hpp',
    './magic_enum_format.hpp',
    './magic_enum_iostream.hpp',
    './magic_enum_fuse.hpp',
    './magic_enum_switch.hpp',
    './magic_enum_utility.hpp',
    './magic_enum.hpp',
]

if meson.get_compiler('cpp').get_id() == 'msvc'
    add_project_arguments(['/utf-8'], language: 'cpp')
else
    add_project_arguments(['-finput-charset=UTF-8'], language: 'cpp')
endif

magic_enum_dep = declare_dependency (
    sources: sources,
    include_directories: [
        include_directories('./'),
        include_directories('../fmt'),
    ],
)