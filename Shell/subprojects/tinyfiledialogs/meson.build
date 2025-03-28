project(
    'tinyfiledialogs',
    'c',
    version: '3.19.1',
    default_options : [
        'warning_level=3',
        'c_std=c18',
        'buildtype=release',
        'b_ndebug=if-release',
        'optimization=3',
    ]
)

tinyfiledialogs_lib = static_library (
    'tinyfiledialogs',
    sources: [
        'tinyfiledialogs.h',
        'tinyfiledialogs.c'
    ]
)

tinyfiledialogs_deps = declare_dependency(
    link_with: tinyfiledialogs_lib,
    include_directories: include_directories('.')
)

meson.override_dependency('tinyfiledialogs', tinyfiledialogs_deps)