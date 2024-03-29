cmake_minimum_required(VERSION 3.11)

project (ctest C)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

SET(BUILD_TESTS OFF CACHE BOOL "Build Tests")

include (cmake/util_functions.cmake)

set(ctest_sources
	include/ctest/ctest.h src/ctest.c
    include/ctest/internal/filter.h src/filter.c
    include/ctest/internal/runner.h src/runner.c
	include/ctest/internal/system.h src/system.c
    include/ctest/internal/test.h src/test.c
    include/ctest/internal/util.h src/util.c )

add_library (ctest ${ctest_sources})
set_compiler_options(ctest)

target_include_directories(ctest PUBLIC include)

if (BUILD_TESTS)
    message("Building Test Projects")
    enable_testing()
    add_subdirectory (test)
endif()
