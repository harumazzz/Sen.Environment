# Homepage : https://github.com/kuba--/zip

project(
    'zip',
    ['c'],
    version: '0.3.3',
    default_options :
    [
        'warning_level=3',
        'buildtype=release',
        'optimization=3',
        'c_std=c17',
    ],
)

sources = [
    './miniz.h',
    './zip.h',
    './zip.c',
]

add_project_arguments(['-DZIP_ENABLE_SHARABLE_FILE_OPEN'], language: 'c')

platform = host_machine.system()

if platform == 'windows'
    add_project_arguments(['-D_LARGEFILE_SOURCE', '-D_FILE_OFFSET_BITS=64'], language: 'c')
endif

zip = static_library (
    'zip',
    sources: sources,
    include_directories: include_directories('./'),
    install: false,
    pic: true,
)