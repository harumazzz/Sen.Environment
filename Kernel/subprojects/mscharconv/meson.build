project(
    'mscharconv',
    'cpp',
    version: '1.2.3',
    default_options: [
        'warning_level=3',
        'c_std=c18',
        'cpp_std=c++latest',
        'buildtype=release',
        'b_ndebug=if-release',
        'optimization=3',
    ]
)

if meson.get_compiler('cpp').get_id() == 'msvc'
    add_project_arguments(['/wd4100'], language: 'cpp')
    add_project_arguments(['/WX-'], language: 'cpp')
endif

if meson.get_compiler('cpp').get_id() == 'msvc'
    add_project_arguments('/Zi', language: 'cpp')
else
    add_project_arguments('-g', language: 'cpp')
endif

if meson.get_compiler('cpp').get_id() == 'clang'
    add_project_arguments('-fPIC', '-D_GNU_SOURCE', '-fvisibility=hidden', language: ['cpp'])
endif

headers = [
    'include/msstl/charconv.hpp',
    'include/msstl/charconv_fwd.hpp',
    'include/msstl/util.hpp',
    'include/msstl/charconv_impl.inl',
    'include/msstl/converted/charconv.inl',
    'include/msstl/converted/m_floating_type_traits.inl',
    'include/msstl/converted/xbit_ops.h.inl',
    'include/msstl/converted/xcharconv.h.inl',
    'include/msstl/converted/xcharconv_ryu.h.inl',
    'include/msstl/converted/xcharconv_ryu_tables.h.inl',
]

install_headers(headers, subdir: 'mscharconv')

mscharconv_dep = declare_dependency(
    include_directories: include_directories('include'),
)

meson.override_dependency('mscharconv', mscharconv_dep)