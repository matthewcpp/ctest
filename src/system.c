#include "ctest/internal/system.h"

#include <stdlib.h>

void _cutil_testing_system_init(_ctest_system* test_system) {
    test_system->_suites = NULL;
    test_system->_current_test = NULL;
    test_system->_current_suite = NULL;

    test_system->test_filters = NULL;
    test_system->test_filter_count = 0;
}

void _cutil_testing_system_destroy(_ctest_system* test_system) {
    _ctest_suite *current_suite = test_system->_suites;
    _ctest_suite *free_ptr = NULL;

    if (test_system->test_filters) {
        int i;
        for (i = 0; i < test_system->test_filter_count; i++) {
            free(test_system->test_filters[i]);
        }

        free(test_system->test_filters);
    }

    while (current_suite) {
        free_ptr = current_suite;
        current_suite = current_suite->next;
        _cutil_testing_suite_destroy(free_ptr);
    }

    free(test_system);
}
