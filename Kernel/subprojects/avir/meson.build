# https://github.com/avaneev/avir

project (
    'avir',
    'cpp',
    version: '3.0.11',
    default_options : [
        'warning_level=3',
        'c_std=c18',
        'cpp_std=c++latest',
        'buildtype=release',
        'b_ndebug=if-release',
        'optimization=3',
    ]
)

if meson.get_compiler('cpp').get_id() == 'msvc'
    add_project_arguments('/Zi', language: 'cpp')
else
    add_project_arguments('-g', language: 'cpp')
endif

if meson.get_compiler('cpp').get_id() == 'msvc'
    add_project_arguments(['/wd4100'], language: 'cpp')
    add_project_arguments(['/WX-'], language: 'cpp')
endif


if meson.get_compiler('cpp').get_id() == 'clang'
    add_project_arguments('-fPIC', '-D_GNU_SOURCE', '-fvisibility=hidden', language : ['c'])
endif

if meson.get_compiler('cpp').get_id() == 'msvc'
    add_project_arguments('/DNOMINMAX', language: 'cpp')
endif

headers = [
    'avir.h',
    'avir_dil.h',
    'lancir.h',
    'avir_float4_sse.h',
    'avir_float8_avx.h',
]

install_headers(
    headers,
    subdir: 'avir'
)

avir_dep = declare_dependency(
    include_directories: include_directories('.')
)

meson.override_dependency('avir', avir_dep)