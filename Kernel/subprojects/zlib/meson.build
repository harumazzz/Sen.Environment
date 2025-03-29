# https://www.zlib.net/

project(
    'zlib',
    'c',
    version: '1.3.1',
    default_options : [
        'warning_level=0',
        'c_std=c11',
        'buildtype=release',
        'b_ndebug=if-release',
        'optimization=3',
    ]
)

if meson.get_compiler('c').get_id() == 'clang'
    add_project_arguments('-fPIC', '-D_GNU_SOURCE', '-fvisibility=hidden', language : ['c'])
endif

if meson.get_compiler('c').get_id() == 'clang'
    add_project_arguments('-DZ_HAVE_UNISTD_H', '-D_LARGEFILE64_SOURCE=1', language : ['c'])
endif

zlib_lib = static_library (
    'zlib',
    sources: [
        './adler32.c',
        './adler32_p.h',
        './arch_functions.h',
        './chunkset_tpl.h',
        './compare256_rle.h',
        './compress.c',
        './cpu_features.c',
        './cpu_features.h',
        './crc32.c',
        './crc32.h',
        './crc32_braid_comb.c',
        './crc32_braid_comb_p.h',
        './crc32_braid_p.h',
        './crc32_braid_tbl.h',
        './deflate.c',
        './deflate.h',
        './deflate_fast.c',
        './deflate_huff.c',
        './deflate_medium.c',
        './deflate_p.h',
        './deflate_quick.c',
        './deflate_rle.c',
        './deflate_slow.c',
        './deflate_stored.c',
        './fallback_builtins.h',
        './functable.c',
        './functable.h',
        './gzguts.h',
        './gzlib.c',
        './gzread.c',
        './gzwrite.c',
        './infback.c',
        './inffast_tpl.h',
        './inffixed_tbl.h',
        './inflate.c',
        './inflate.h',
        './inflate_p.h',
        './inftrees.c',
        './inftrees.h',
        './insert_string.c',
        './insert_string_roll.c',
        './insert_string_tpl.h',
        './match_tpl.h',
        './trees.c',
        './trees.h',
        './trees_emit.h',
        './trees_tbl.h',
        './uncompr.c',
        './zbuild.h',
        './zconf.h',
        './zendian.h',
        './zlib.h',
        './zlib_name_mangling.h',
        './zmemory.h',
        './zutil.c',
        './zutil.h',
        './zutil_p.h',
        './arch/generic/adler32_c.c',
        './arch/generic/adler32_fold_c.c',
        './arch/generic/chunkset_c.c',
        './arch/generic/chunk_permute_table.h',
        './arch/generic/compare256_c.c',
        './arch/generic/compare256_p.h',
        './arch/generic/crc32_braid_c.c',
        './arch/generic/crc32_fold_c.c',
        './arch/generic/generic_functions.h',
        './arch/generic/slide_hash_c.c',
    ],
    install: true,
    pic: true,
    include_directories: include_directories('.'),
)

headers = [
    './zlib.h',
    './zconf.h',
    './zlib_name_mangling.h',
]

install_headers(
    headers,
    subdir: 'zlib',
)

zlib_dep = declare_dependency(
    link_with: zlib_lib,
    include_directories: include_directories('.')
)

meson.override_dependency('zlib', zlib_dep)