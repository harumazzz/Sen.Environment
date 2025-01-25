# Homepage : https://github.com/Tessil/ordered-map/

project(
    'tsl',
    ['cpp'],
    version: '1.1.0',
    default_options :
    [
        'warning_level=3',
        'buildtype=release',
        'optimization=3',
        'cpp_std=c++latest',
    ],
)

sources = [
    './include/tsl/ordered_hash.h',
    './include/tsl/ordered_map.h',
    './include/tsl/ordered_set.h',
]

tsl = declare_dependency (
    sources: sources,
    include_directories: include_directories('./'),
)