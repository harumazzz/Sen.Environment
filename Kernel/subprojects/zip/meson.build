# https://github.com/kuba--/zip

project (
    'zip',
    'c',
    version: '0.3.3',
    default_options : [
        'warning_level=3',
        'c_std=c99',
        'buildtype=release',
        'b_ndebug=if-release',
        'optimization=3',
    ]
)

if meson.get_compiler('c').get_id() == 'clang'
    add_project_arguments('-fPIC', '-D_GNU_SOURCE', '-fvisibility=hidden', language : ['c'])
endif

if meson.get_compiler('c').get_id() == 'msvc'
    add_project_arguments(
        '-D_CRT_SECURE_CPP_OVERLOAD_STANDARD_NAMES',
        '-D_CRT_SECURE_CPP_OVERLOAD_STANDARD_NAMES_COUNT',
        '-D_CRT_NONSTDC_NO_WARNINGS',
        '-D_CRT_SECURE_NO_WARNINGS',
        '/wd4127',
        '/wd4267',
        language: 'c'
    )
endif

zip_lib = static_library (
    'zip',
    sources: [
        'zip.h',
        'miniz.h',
        'zip.c',
    ],
    install: true,
    pic: true,
    include_directories: include_directories('.'),
)

headers = [
    'zip.h',
    'miniz.h',
]

install_headers (
    headers,
    subdir: 'zip',
)

zip_dep = declare_dependency(
    link_with: zip_lib,
    include_directories: include_directories('.'),
)


meson.override_dependency('zip', zip_dep)