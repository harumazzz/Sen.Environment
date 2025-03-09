cmake_minimum_required (VERSION 3.20)

project(
	zlib
	VERSION 1.3.1
	HOMEPAGE_URL https://www.zlib.net/
	LANGUAGES C
)

if (MSVC)
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Zi")
else ()
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")
endif ()

add_library(
	${PROJECT_NAME} STATIC
	./adler32.c
	./adler32_p.h
	./arch_functions.h
	./chunkset_tpl.h
	./compare256_rle.h
	./compress.c
	./cpu_features.c
	./cpu_features.h
	./crc32.c
	./crc32.h
	./crc32_braid_comb.c
	./crc32_braid_comb_p.h
	./crc32_braid_p.h
	./crc32_braid_tbl.h
	./deflate.c
	./deflate.h
	./deflate_fast.c
	./deflate_huff.c
	./deflate_medium.c
	./deflate_p.h
	./deflate_quick.c
	./deflate_rle.c
	./deflate_slow.c
	./deflate_stored.c
	./fallback_builtins.h
	./functable.c
	./functable.h
	./gzguts.h
	./gzlib.c
	./gzread.c
	./gzwrite.c
	./infback.c
	./inffast_tpl.h
	./inffixed_tbl.h
	./inflate.c
	./inflate.h
	./inflate_p.h
	./inftrees.c
	./inftrees.h
	./insert_string.c
	./insert_string_roll.c
	./insert_string_tpl.h
	./match_tpl.h
	./trees.c
	./trees.h
	./trees_emit.h
	./trees_tbl.h
	./uncompr.c
	./zbuild.h
	./zconf.h
	./zendian.h
	./zlib.h
	./zlib_name_mangling.h
	./zmemory.h
	./zutil.c
	./zutil.h
	./zutil_p.h
	./arch/generic/adler32_c.c
	./arch/generic/adler32_fold_c.c
	./arch/generic/chunkset_c.c
	./arch/generic/chunk_permute_table.h
	./arch/generic/compare256_c.c
	./arch/generic/compare256_p.h
	./arch/generic/crc32_braid_c.c
	./arch/generic/crc32_fold_c.c
	./arch/generic/generic_functions.h
	./arch/generic/slide_hash_c.c
)

if(UNIX OR APPLE OR ANDROID)
    add_definitions(
		-DZ_HAVE_UNISTD_H 
		-D_LARGEFILE64_SOURCE=1
	)
endif()


# UTF8 Support
if(MSVC)
  add_compile_options(/utf-8)
  add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
  add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")
else()
  add_compile_options(-finput-charset=UTF-8)
endif()

set_target_properties(zlib PROPERTIES POSITION_INDEPENDENT_CODE ON)
