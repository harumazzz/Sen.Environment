cmake_minimum_required (VERSION 3.20)

project(
    mscharconv
    VERSION 1.2.3
    HOMEPAGE_URL https://github.com/iboB/mscharconv
    LANGUAGES CXX
)

add_library(
    ${PROJECT_NAME} INTERFACE
    ./include/msstl/converted/charconv.inl
	./include/msstl/converted/m_floating_type_traits.inl
	./include/msstl/converted/xbit_ops.h.inl
	./include/msstl/converted/xcharconv.h.inl
	./include/msstl/converted/xcharconv_ryu.h.inl
	./include/msstl/converted/xcharconv_ryu_tables.h.inl
	./include/msstl/charconv.hpp
	./include/msstl/charconv_fwd.hpp
	./include/msstl/charconv_impl.inl
	./include/msstl/util.hpp
)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
target_compile_features(${PROJECT_NAME} INTERFACE cxx_std_20)

if(MSVC)
    target_compile_options(${PROJECT_NAME} INTERFACE /utf-8)
else()
    target_compile_options(${PROJECT_NAME} INTERFACE -finput-charset=UTF-8)
endif()

set_target_properties(${PROJECT_NAME} PROPERTIES POSITION_INDEPENDENT_CODE ON)
