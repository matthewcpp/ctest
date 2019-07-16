#include "ctest/internal/test.h"
#include "ctest/internal/util.h"

#include <stdlib.h>

_cutil_test_entry *_cutil_testing_entry_create(const char *test_name, cutil_test_function test_func) {
    _cutil_test_entry *test_entry = malloc(sizeof(_cutil_test_entry));

    test_entry->test_func = test_func;
    test_entry->test_name = _str_cpy(test_name);
    test_entry->test_result = 0;
    test_entry->test_message = NULL;
    test_entry->next = NULL;

    return test_entry;
}

void _cutil_testing_entry_destroy(_cutil_test_entry *test_entry) {
    if (test_entry->test_name) {
        free(test_entry->test_name);
    }

    if (test_entry->test_message) {
        free(test_entry->test_message);
    }

    free(test_entry);
}
