#Homepage: https://github.com/avaneev/canvas_ity
project (
    'canvas_ity',
    ['cpp'],
    version: '1.0.0',
    meson_version: '>=1.3.0',
    default_options :
    [
        'warning_level=3',
        'buildtype=release',
        'optimization=3',
        'cpp_std=c++latest',
    ],
)

sources = [
    './canvas_ity.hpp',
]

canvas_ity = declare_dependency (
    include_directories: include_directories('./'),
    sources: sources,
)