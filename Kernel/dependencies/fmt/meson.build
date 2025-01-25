# Homepage : https://github.com/fmtlib/fmt

project(
    'fmt',
    ['cpp'],
    version: '11.1.2',
    meson_version: '>=1.3.0',
    default_options:
    [
        'warning_level=3',
        'buildtype=release',
        'optimization=3',
        'cpp_std=c++latest',
    ],
)

sources = [
    './include/fmt/args.h',
    './include/fmt/base.h',
    './include/fmt/chrono.h',
    './include/fmt/color.h',
    './include/fmt/compile.h',
    './include/fmt/core.h',
    './include/fmt/format-inl.h',
    './include/fmt/format.h',
    './include/fmt/os.h',
    './include/fmt/ostream.h',
    './include/fmt/printf.h',
    './include/fmt/ranges.h',
    './include/fmt/std.h',
    './include/fmt/xchar.h',
]

fmt = declare_dependency (
    sources: sources,
    include_directories: include_directories('./'),
)