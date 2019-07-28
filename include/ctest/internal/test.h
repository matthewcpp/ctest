#ifndef CTEST_TEST_H
#define CTEST_TEST_H

#include "ctest/ctest.h"

/* _ctest_unit_test describes a single unit test. */
typedef struct _ctest_unit_test {
	_ctest_fixture_test_runner fixture_test_runner;
    _ctest_generic_fixture_test fixture_test_func;
    ctest_test_func test_func;
    char *test_name;

    struct _ctest_unit_test* next;
} _ctest_unit_test;

_ctest_unit_test *_ctest_unit_test_create_with_test(const char* test_name, ctest_test_func test_func);
_ctest_unit_test *_ctest_unit_test_create_with_fixture(const char* fixture_name, const char* test_name, _ctest_fixture_test_runner fixture_test_runner, _ctest_generic_fixture_test fixture_test_func);
void _ctest_unit_test_destroy(_ctest_unit_test* test);
void _ctest_unit_test_run(_ctest_unit_test* test);

#endif
