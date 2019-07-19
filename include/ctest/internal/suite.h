#ifndef CTEST_SUITE_H
#define CTEST_SUITE_H

#include "ctest/internal/test.h"

/* _ctest_suite describes a collection of related unit tests. */
typedef struct _ctest_suite {
    char* name;
    ctest_test_func before_each;
    ctest_test_func after_each;
    _ctest_unit_test* _tests;

    struct _ctest_suite* next;
} _ctest_suite;

_ctest_suite *_cutil_testing_suite_create(const char *name);
void _cutil_testing_suite_destroy(_ctest_suite *test_suite);

#endif
