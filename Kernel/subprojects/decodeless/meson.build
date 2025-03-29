# ?
project (
    'decodeless',
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

if meson.get_compiler('cpp').get_id() == 'clang'
    add_project_arguments('-fPIC', '-D_GNU_SOURCE', '-fvisibility=hidden', language : ['cpp'])
endif

if meson.get_compiler('cpp').get_id() == 'msvc'
    add_project_arguments('/DNOMINMAX', language: 'cpp')
endif

headers = [
    'allocator.hpp',
    'allocator_concepts.hpp',
    'allocator_construction.hpp',
    'header.hpp',
    'mappedfile.hpp',
    'offset_ptr.hpp',
    'offset_span.hpp',
    'pmr_allocator.hpp',
    'pmr_writer.hpp',
    'writer.hpp',
    'detail/mappedfile_common.hpp',
    'detail/mappedfile_linux.hpp',
    'detail/mappedfile_windows.hpp',
]

install_headers(
    headers,
    subdir: 'decodeless',
)

decodeless_dep = declare_dependency(
    include_directories: include_directories('.'),
)

meson.override_dependency('decodeless', decodeless_dep)