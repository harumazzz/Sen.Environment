project (
    'test',
    'cpp',
    version: '1.0.0',
    default_options : [
        'warning_level=3',
        'c_std=c18',
        'cpp_std=c++latest',
        'buildtype=release',
        'b_ndebug=if-release',
        'optimization=3',
    ]
)

sources = [
    'main.cpp',
]

executable(
    'test-bench',
    sources,
    include_directories: include_directories('.'),
)