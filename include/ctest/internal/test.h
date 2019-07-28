#ifndef CTEST_TEST_H
#define CTEST_TEST_H

#include "ctest/ctest.h"

/* ctest_test describes a single unit test. */
typedef struct ctest_test {
	_ctest_fixture_test_runner fixture_test_runner;
    _ctest_generic_fixture_test fixture_test_func;
    ctest_test_func test_func;
    char* test_name;

    struct ctest_test* next;
} ctest_test;

ctest_test* ctest_test_create_with_function(const char* test_name, ctest_test_func test_func);
ctest_test* ctest_test_create_with_fixture(const char* fixture_name, const char* test_name, _ctest_fixture_test_runner fixture_test_runner, _ctest_generic_fixture_test fixture_test_func);
void ctest_test_destroy(ctest_test* test);
void ctest_test_run(ctest_test* test);

#endif
