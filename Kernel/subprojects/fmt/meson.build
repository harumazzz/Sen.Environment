project (
    'fmt',
    'cpp',
    version: '11.1.4',
    default_options : [
        'warning_level=3',
        'c_std=c18',
        'cpp_std=c++latest',
        'buildtype=release',
        'b_ndebug=if-release',
        'optimization=3',
    ]
)

add_project_arguments('-DFMT_HEADER_ONLY', language: ['cpp', 'c'])

if meson.get_compiler('cpp').get_id() == 'clang'
    add_project_arguments('-fPIC', '-D_GNU_SOURCE', '-fvisibility=hidden', language : ['cpp'])
endif

headers = [
    'include/fmt/args.h',
    'include/fmt/base.h',
    'include/fmt/chrono.h',
    'include/fmt/color.h',
    'include/fmt/compile.h',
    'include/fmt/core.h',
    'include/fmt/format-inl.h',
    'include/fmt/format.h',
    'include/fmt/os.h',
    'include/fmt/ostream.h',
    'include/fmt/printf.h',
    'include/fmt/ranges.h',
    'include/fmt/std.h',
    'include/fmt/xchar.h',
]

fmt_dep = declare_dependency(
    include_directories: [
        include_directories('include'),
        include_directories('.'),
        include_directories('include/fmt')
    ],
)

meson.override_dependency('fmt', fmt_dep)