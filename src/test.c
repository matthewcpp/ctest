#include "ctest/internal/test.h"
#include "ctest/internal/util.h"

#include <stdlib.h>

_ctest_unit_test *_cutil_testing_entry_create(const char *test_name, ctest_test_func test_func) {
    _ctest_unit_test *test_entry = malloc(sizeof(_ctest_unit_test));

    test_entry->test_func = test_func;
    test_entry->test_name = _cutil_strdup(test_name);
    test_entry->test_result = 0;
    test_entry->test_message = NULL;
    test_entry->next = NULL;

    return test_entry;
}

void _cutil_testing_entry_destroy(_ctest_unit_test *test_entry) {
    if (test_entry->test_name) {
        free(test_entry->test_name);
    }

    if (test_entry->test_message) {
        free(test_entry->test_message);
    }

    free(test_entry);
}
