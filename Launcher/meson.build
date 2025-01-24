project('Launcher', 'rust',
  version : '0.1', meson_version: '>=1.3.0',
  default_options : ['rust_std=2021', 'warning_level=3'])

exe = executable(
  'Launcher',
  'src/main.rs',
  install : true,
)

test('basic', exe)
