cmake_minimum_required(VERSION 3.11)

include (cmake/util_functions.cmake)

set(ctest_sources
	include/ctest/ctest.h
    src/ctest.c
)

add_library (ctest ${ctest_sources})
set_compiler_options(ctest)

target_include_directories(ctest PUBLIC include)