# Homepage : https://github.com/greg7mdp/parallel-hashmap

project (
    'parallel-hashmap',
    ['cpp'],
    version: '1.4.1',
    default_options :
    [
        'warning_level=3',
        'buildtype=release',
        'optimization=3',
        'cpp_std=c++latest',
    ],
)

sources = [
    './meminfo.h',
    './phmap_base.h',
    './phmap_bits.h',
    './btree.h',
    './phmap_config.h',
    './phmap_dump.h',
    './phmap_fwd_decl.h',
    './phmap_utils.h',
    './phmap.h',
]

if meson.get_compiler('cpp').get_id() == 'msvc'
    add_project_arguments(['/utf-8'], language: 'cpp')
else
    add_project_arguments(['-finput-charset=UTF-8'], language: 'cpp')
endif

phmap = declare_dependency (
    sources: sources,
    include_directories: include_directories('./'),
)