# https://github.com/codywatts/PVRTCCompressor

project (
    'PVRTCCompressor',
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

PVRTCCompressor_lib = static_library (
    'PVRTCCompressor',
    sources: [
        'AlphaBitmap.h',
        'Bitmap.h',
        'BitScale.cpp',
        'BitScale.h',
        'BitUtility.h',
        'ColorRgba.h',
        'Interval.h',
        'MortonTable.cpp',
        'MortonTable.h',
        'Point2.h',
        'PvrTcDecoder.cpp',
        'PvrTcDecoder.h',
        'PvrTcEncoder.cpp',
        'PvrTcEncoder.h',
        'PvrTcPacket.cpp',
        'PvrTcPacket.h',
        'RgbaBitmap.h',
        'RgbBitmap.h',
    ],
    install: true,
    pic: true,
    include_directories: include_directories('.'),
)

headers = [
    'AlphaBitmap.h',
    'Bitmap.h',
    'BitScale.h',
    'BitUtility.h',
    'ColorRgba.h',
    'Interval.h',
    'MortonTable.h',
    'Point2.h',
    'PvrTcDecoder.h',
    'PvrTcEncoder.h',
    'PvrTcPacket.h',
    'RgbaBitmap.h',
    'RgbBitmap.h',
]

install_headers(
    headers,
    subdir: 'PVRTCCompressor',
)

PVRTCCompressor_dep = declare_dependency(
    link_with: PVRTCCompressor_lib,
    include_directories: include_directories('.')
)

meson.override_dependency('PVRTCCompressor', PVRTCCompressor_dep)