# Homepage : https://github.com/bshoshany/thread-pool

project(
    'thread-pool',
    ['cpp'],
    version: '5.0.0',
    default_options :
    [
        'warning_level=3',
        'buildtype=release',
        'optimization=3',
        'cpp_std=c++latest',
    ],
)

sources = [
    './BS_thread_pool.hpp',
]

if meson.get_compiler('cpp').get_id() == 'msvc'
    add_project_arguments(['/utf-8'], language: 'cpp')
else
    add_project_arguments(['-finput-charset=UTF-8'], language: 'cpp')
endif

thread_pool = declare_dependency (
    sources: sources,
    include_directories: include_directories('./'),
)