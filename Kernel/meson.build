project('Kernel', 'cpp',
        version : '10',
        license : 'MIT',
        default_options : [
                'cpp_std=c++latest',
                'warning_level=0',
                'buildtype=release',
                'b_ndebug=if-release',
                'optimization=3',
        ]
)

if get_option('debug_log')
        add_project_arguments('-DDEBUG_LOG', language: 'cpp')
        message('Enabled debug log')
endif


subdir('kernel')
