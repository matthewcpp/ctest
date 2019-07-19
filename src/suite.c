#include "ctest/internal/suite.h"
#include "ctest/internal/util.h"

#include <stdlib.h>

_ctest_suite *_cutil_testing_suite_create(const char *name) {
    _ctest_suite * suite = malloc(sizeof(_ctest_suite));

    suite->name = _cutil_strdup(name);
    suite->_tests = NULL;
    suite->before_each = NULL;
    suite->after_each = NULL;
    suite->next = NULL;
    return suite;
}
void _cutil_testing_suite_destroy(_ctest_suite *test_suite) {
    _ctest_unit_test *current_test = test_suite->_tests;
    _ctest_unit_test *free_ptr = NULL;

    if (test_suite->name) {
        free(test_suite->name);
    }

    while (current_test) {
        free_ptr = current_test;
        current_test = current_test->next;
        _cutil_testing_entry_destroy(free_ptr);
    }

    free(test_suite);
}


