#include "ctest/internal/suite.h"
#include "ctest/internal/util.h"

#include <stdlib.h>

_cutil_test_suite *_cutil_testing_suite_create(const char *name) {
    _cutil_test_suite * suite = malloc(sizeof(_cutil_test_suite));

    suite->name = _str_cpy(name);
    suite->_tests = NULL;
    suite->before_each = NULL;
    suite->after_each = NULL;
    suite->next = NULL;
    return suite;
}
void _cutil_testing_suite_destroy(_cutil_test_suite *test_suite) {
    _cutil_test_entry *current_test = test_suite->_tests;
    _cutil_test_entry *free_ptr = NULL;

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


