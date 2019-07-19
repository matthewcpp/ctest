#ifndef CTEST_TEST_H
#define CTEST_TEST_H

#include "ctest/ctest.h"

/* _ctest_unit_test describes a single unit test. */
typedef struct _ctest_unit_test {
    ctest_test_func test_func;
    char *test_name;
    unsigned int test_result;
    char *test_message;

    struct _ctest_unit_test* next;
} _ctest_unit_test;

_ctest_unit_test *_cutil_testing_entry_create(const char *test_name, ctest_test_func test_func);
void _cutil_testing_entry_destroy(_ctest_unit_test *test_entry);

#endif
