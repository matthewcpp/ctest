#ifndef CTEST_SYSTEM_H
#define CTEST_SYSTEM_H

#include "ctest/internal/suite.h"

/* _cutil_test_suite top level object containing a collection of suites. */
typedef struct _cutil_test_system {
    _cutil_test_suite *_suites;
    _cutil_test_entry *_current_test;
    _cutil_test_suite *_current_suite;

    char** test_filters;
    int test_filter_count;
} _cutil_test_system;

void _cutil_testing_system_init(_cutil_test_system* test_system);
void _cutil_testing_system_destroy(_cutil_test_system* test_system);

#endif
