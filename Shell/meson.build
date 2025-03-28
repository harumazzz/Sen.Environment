project (
    'Shell',
    'cpp',
    version: '1.0.0',
    default_options : [
        'warning_level=3',
        'c_std=c18',
        'cpp_std=c++latest',
        'buildtype=release',
        'b_ndebug=if-release',
        'debug=false',
        'optimization=3',
    ],
)

tinyfiledialogs_deps = dependency('tinyfiledialogs', required: true)

library_directory = include_directories([
    'bridge',
    'library',
])

sources = ['shell.cpp']

if meson.get_compiler('cpp').get_id() == 'msvc'
    resource_compiler = find_program('rc', required: true)
    add_project_arguments(
        '-DUNICODE', '-D_UNICODE', '-DNOMINMAX', '-DWIN32_LEAN_AND_MEAN',
        '-D_WINSOCKAPI_', '-D_UCRT_NOISY_NAN',
        language: 'cpp'
    )
    manifest_tool = find_program('mt', required: true)
    resource = custom_target(
        'resource',
        output: 'shell.res',
        input: 'shell.rc',
        command: [resource_compiler, '/fo', '@OUTPUT@', '@INPUT@']
    )
    sources += resource
    exe = executable(
        'Shell',
        sources,
        include_directories: library_directory,
        dependencies: [tinyfiledialogs_deps],
        win_subsystem: 'console',
        link_args: [
            '/MACHINE:x64',
            '/SUBSYSTEM:CONSOLE',
            'kernel32.lib', 'user32.lib', 'gdi32.lib', 'winspool.lib',
            'shell32.lib', 'ole32.lib', 'oleaut32.lib', 'uuid.lib',
            'comdlg32.lib', 'advapi32.lib'
        ]
    )
    manifest_embed = custom_target(
        'embed_manifest',
        input: 'resource/windows/application.manifest',
        output: 'Shell.exe.manifest',
        command: [
            manifest_tool, '-nologo',
            '-manifest', '@INPUT@',
            '-outputresource:@OUT@:1'
        ],
        depends: exe
    )
else
    add_project_arguments([
      '-Weverything',
      '-Wno-c++98-compat',
      '-Wno-c++98-compat-pedantic',
      '-Wno-pre-c++14-compat',
      '-Wno-pre-c++17-compat',
      '-Wno-c++20-compat',
      '-Wno-c99-extensions',
      '-Wno-padded',
      '-Wno-redundant-parens',
      '-Wno-missing-field-initializers',
      '-Wno-unused-parameter',
      '-Wno-unused-variable',
      '-Wno-missing-noreturn',
      '-Wno-gnu-zero-variadic-macro-arguments',
      '-Wno-ctad-maybe-unsupported',
      '-Wno-global-constructors',
      '-Wno-exit-time-destructors',
      '-Wno-weak-vtables',
      '-Wno-self-assign',
      '-Wno-switch-enum',
      '-Wno-shadow-field',
      '-Wno-shadow-field-in-constructor',
      '-Wno-shadow-uncaptured-local',
      '-Wno-unsafe-buffer-usage',
      '-Wno-float-equal',
      '-Wno-gnu-line-marker',
    ], language: 'cpp')
    add_project_arguments('-D_GNU_SOURCE', language: 'c')
    add_project_arguments('-D_GNU_SOURCE', language: 'cpp')
    executable(
        'Shell',
        sources,
        include_directories: library_directory,
        dependencies: [tinyfiledialogs_deps]
    )
endif