#ifndef CTEST_SUITE_H
#define CTEST_SUITE_H

#include "ctest/internal/test.h"

/* _cutil_test_suite describes a collection of related unit tests. */
typedef struct _cutil_test_suite {
    char* name;
    cutil_test_function before_each;
    cutil_test_function after_each;
    _cutil_test_entry* _tests;

    struct _cutil_test_suite* next;
} _cutil_test_suite;

_cutil_test_suite *_cutil_testing_suite_create(const char *name);
void _cutil_testing_suite_destroy(_cutil_test_suite *test_suite);

#endif //CTEST_SUITE_H
