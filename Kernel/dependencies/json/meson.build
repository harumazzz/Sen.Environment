# Homepage: https://github.com/nlohmann/json

project (
    'json',
    ['cpp'],
    version: '3.11.3',
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
    './json.hpp',
    './json_fwd.hpp',
]

json = declare_dependency (
    sources: sources,
    include_directories: include_directories('./'),
)