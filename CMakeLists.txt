cmake_minimum_required(VERSION 3.11)

project (ctest C)

include (cmake/util_functions.cmake)

set(ctest_sources
	include/ctest/ctest.h
    src/ctest.c
	src/suite.c include/ctest/internal/suite.h include/ctest/internal/system.h include/ctest/internal/test.h src/test.c include/ctest/internal/util.h src/util.c src/system.c)

add_library (ctest ${ctest_sources})
set_compiler_options(ctest)

target_include_directories(ctest PUBLIC include)