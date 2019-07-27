#include "ctest/internal/test.h"
#include "ctest/internal/util.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

_ctest_unit_test* _create_base_unit_test() {
    _ctest_unit_test *test_entry = malloc(sizeof(_ctest_unit_test));
    test_entry->test_result = 0;
    test_entry->test_message = NULL;
    test_entry->next = NULL;

    return test_entry;
}

_ctest_unit_test* _ctest_unit_test_create_with_test(const char* test_name, ctest_test_func test_func) {
    _ctest_unit_test* test_entry = _create_base_unit_test();

	test_entry->test_name = _cutil_strdup(test_name);
    test_entry->test_func = test_func;
    test_entry->fixture_test_runner = NULL;
    test_entry->fixture_test_func = NULL;

    return test_entry;
}

_ctest_unit_test *_ctest_unit_test_create_with_fixture(const char* fixture_name, const char *test_name, _ctest_fixture_test_runner fixture_test_runner, _ctest_generic_fixture_test fixture_test_func) {
    _ctest_unit_test* test_entry = _create_base_unit_test();
	size_t fixture_name_length = strlen(fixture_name);
	size_t test_name_length = strlen(test_name);
	size_t test_name_buffer_size = fixture_name_length + test_name_length + 2;

	test_entry->test_name = malloc(test_name_buffer_size);
	cutil_snprintf_func(test_entry->test_name, test_name_buffer_size, "%s.%s", fixture_name, test_name);

    test_entry->test_func = NULL;
    test_entry->fixture_test_runner = fixture_test_runner;
    test_entry->fixture_test_func = fixture_test_func;

    return test_entry;
}

void _ctest_unit_test_destroy(_ctest_unit_test *test_entry) {
    if (test_entry->test_name) {
        free(test_entry->test_name);
    }

    if (test_entry->test_message) {
        free(test_entry->test_message);
    }

    free(test_entry);
}
