cmake_minimum_required(VERSION 3.9)

project(
	quickjs
	VERSION 0.6.1
	HOMEPAGE_URL https://github.com/quickjs-ng/quickjs
	LANGUAGES C
)

add_library(
	${PROJECT_NAME} STATIC
	./cutils.h
	./dirent_compat.h
	./getopt_compat.h
	./libbf.h
	./libregexp-opcode.h
	./libregexp.h
	./libunicode-table.h
	./libunicode.h
	./list.h
	./quickjs-atom.h
	./quickjs-c-atomics.h
	./quickjs-libc.h
	./quickjs-opcode.h
	./quickjs.h
	./unicode_gen_def.h
	./cutils.c
	./libbf.c
	./libunicode.c
	./libregexp.c
	./qjs.c
	./qjsc.c
	./quickjs-libc.c
	./quickjs.c
	./unicode_gen.c
)
target_compile_definitions(
	${PROJECT_NAME} PRIVATE
	CONFIG_BIGNUM
	CONFIG_VERSION="2021-03-27"
)