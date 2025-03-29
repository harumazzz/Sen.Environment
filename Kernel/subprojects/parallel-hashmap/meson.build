# https://github.com/greg7mdp/parallel-hashmap

project (
    'parallel-hashmap',
    'cpp',
    version: '2.0.0',
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
    'meminfo.h',
    'phmap_base.h',
    'phmap_bits.h',
    'btree.h',
    'phmap_config.h',
    'phmap_dump.h',
    'phmap_fwd_decl.h',
    'phmap_utils.h',
    'phmap.h',
]

install_headers (
    headers,
    subdir: 'parallel-hashmap',
)

parallel_hashmap_dep = declare_dependency(
    include_directories: include_directories('.'),
)

meson.override_dependency('parallel-hashmap', parallel_hashmap_dep)