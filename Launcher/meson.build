project('Launcher', ['rust', 'cpp'],
  version : '1.0', meson_version: '>=1.3.0',
  default_options : 
  [
      'rust_std=2021', 
      'warning_level=3',
      'buildtype=release',   
      'optimization=3',
      'cpp_std=c++latest',
  ],
)

resource = custom_target(
  'launcher_obj',
  input : 'launcher.rc',
  output : 'launcher.res',
  command : ['rc', '/fo@OUTPUT@', '@INPUT@'],
  build_by_default : true,
)

icon_library = static_library(
  'icon_library',
  sources : [resource],
  cpp_args : ['/EHsc'],
)

exe = executable(
  'Launcher',
  sources : ['src/main.rs'],
  link_whole : [icon_library],
  install : true,
)

test('basic', exe)
