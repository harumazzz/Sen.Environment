project('Shell', 'rust',
        version : '0.1', meson_version: '>=1.3.0',
        default_options : ['rust_std=2021', 'warning_level=3']
)

rust = import('rust')

exe = executable(
    'Shell',
    'src/main.rs',
    install : true,
    rust_crate_type : 'bin',
)

test('basic', exe)
