# Homepage : https://github.com/avaneev/avir
project (
	'avir',
	['c', 'cpp'],
	version: '3.0.11',
	meson_version: '>=1.3.0',
	default_options :
	[
		'warning_level=3',
		'buildtype=release',
		'optimization=3',
		'cpp_std=c++latest',
	],
)

sources = [
	'./avir.h',
	'./avir_dil.h',
	'./avir_float4_sse.h',
	'./avir_float8_avx.h',
	'./lancir.h',
]

avir = declare_dependency (
	include_directories: include_directories('./'),
	sources: sources,
)