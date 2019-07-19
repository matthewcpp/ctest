#ifndef CTEST_SYSTEM_H
#define CTEST_SYSTEM_H

#include "ctest/internal/suite.h"

/* _ctest_suite top level object containing a collection of suites. */
typedef struct _ctest_system {
    _ctest_suite *_suites;
    _ctest_unit_test *_current_test;
    _ctest_suite *_current_suite;

    char** test_filters;
    int test_filter_count;
} _ctest_system;

void _cutil_testing_system_init(_ctest_system* test_system);
void _cutil_testing_system_destroy(_ctest_system* test_system);

#endif
