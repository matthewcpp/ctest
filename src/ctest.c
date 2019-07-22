#include "ctest/ctest.h"

#include "ctest/internal/util.h"
#include "ctest/internal/system.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>



_ctest_system* test_system = NULL;


/*******************************************/
/* Implementation for main testing interface. */

void ctest_init() {
	if (!test_system) {
		test_system = malloc(sizeof(_ctest_system));
		_cutil_testing_system_init(test_system);
	}
}

void ctest_destroy() {
	if (test_system) {
        _cutil_testing_system_destroy(test_system);
		test_system = NULL;
	}
}

void ctest_suite(const char *name) {
	_ctest_suite* new_suite;

    ctest_init();

	new_suite = _cutil_testing_suite_create(name);
	if (test_system->_current_suite) {
		test_system->_current_suite->next = new_suite;
	}
	else {
		test_system->_suites = new_suite;
	}

	test_system->_current_suite = new_suite;
	test_system->_current_test = NULL;
}

void _ctest_add_test(const char *test_name, ctest_test_func test_func) {
	_ctest_unit_test* test_entry;
    ctest_init();

	if (!test_system->_current_suite) {
        ctest_suite("Default");
	}

	test_entry = _cutil_testing_entry_create(test_name, test_func);
	if (test_system->_current_test) {
		test_system->_current_test->next = test_entry;
	}
	else {
		test_system->_current_suite->_tests = test_entry;
	}

	test_system->_current_test = test_entry;
}

int ctest_suite_before_each(ctest_test_func func) {
	if (test_system->_current_suite) {
		test_system->_current_suite->before_each = func;

		return 1;
	}
	else {
		return 0;
	}
}

int ctest_suite_after_each(ctest_test_func func) {
	if (test_system->_current_suite) {
		test_system->_current_suite->after_each = func;

		return 1;
	}
	else {
		return 0;
	}
}

char** _cutil_testing_get_suite_names(const char* str, int* count) {
	int token_count = 0;
	int token_size = 1;

	char** tokens = malloc(sizeof(char*) * token_size);
	char* search_str = _cutil_strdup(str);

	char *token = strtok(search_str, ";");
	while (token) {
		tokens[token_count] = _cutil_strdup(token);

		token_count += 1;

		if (token_count == token_size) {
			token_size *= 2;
			tokens = realloc(tokens, sizeof(char*) * token_size);
		}

		token = strtok(NULL, ";");
	}

	free(search_str);

	qsort(tokens, token_count, sizeof(char*), _cutil_str_cmp);

	*count = token_count;
	return tokens;
}

void ctest_config_set_filter(const char *filter_str)
{
	test_system->test_filters = _cutil_testing_get_suite_names(filter_str, &test_system->test_filter_count);
}

int _cutil_testing_str_find(const char* needle, char** haystack, int start, int end) {
	int middle = start + ((end - start) / 2);
	int result = strcmp(needle, haystack[middle]);

	if (end <= start) {
		if (result == 0) {
			return 1;
		}
		else {
			return 0;
		}
	}

	if (result > 0) {
		return _cutil_testing_str_find(needle, haystack, middle + 1, end);
	}
	else if (result < 0) {
		return _cutil_testing_str_find(needle, haystack, start, middle - 1);
	}
	else {
		return 1;
	}
}

int _cutil_testing_should_run_test_suite(_ctest_suite * test_suite) {
	if (test_system->test_filters) {
		return _cutil_testing_str_find(test_suite->name, test_system->test_filters, 0, test_system->test_filter_count - 1);
	}
	else {
		return 1;
	}
}

int _cutil_testing_process_suite(_ctest_suite *current_suite, int* out_pass_count, int* out_fail_count) {
	int test_pass_count = 0;
	int test_fail_count = 0;

	printf("Test Suite: %s\n", current_suite->name);
	printf("-----------------------------------\n");

	test_system->_current_test = current_suite->_tests;
	while (test_system->_current_test) {
		printf("Test: %s\n", test_system->_current_test->test_name);
		if (current_suite->before_each) {
			current_suite->before_each();
		}

		test_system->_current_test->test_func();

		if (current_suite->after_each) {
			current_suite->after_each();
		}

		if (test_system->_current_test->test_result == 0) {
			printf("Result: PASSED\n");
			test_pass_count += 1;
		}
		else {
			printf("Result: FAILED\n");
			test_fail_count += 1;
		}

		test_system->_current_test = test_system->_current_test->next;
	}

	*out_pass_count = test_pass_count;
	*out_fail_count = test_fail_count;

	if (test_fail_count > 0)
		return 1;
	else
		return 0;
}

void _cutil_print_results(int total_pass_count, int total_fail_count) {
	printf("\n-----------------------------------\n");
	printf("TestResults:\n");
	printf("Total Tests: %d\n", total_pass_count + total_fail_count);
	printf("Tests Passed: %d\n", total_pass_count);
	printf("Tests Failed: %d\n", total_fail_count);
}

int ctest_run() {
	int total_pass_count = 0;
	int total_fail_count = 0;

	test_system->_current_suite = test_system->_suites;

	while (test_system->_current_suite) {
		int suite_pass_count = 0;
		int suite_fail_count = 0;

		if (_cutil_testing_should_run_test_suite(test_system->_current_suite)) {
			_cutil_testing_process_suite(test_system->_current_suite, &suite_pass_count, &suite_fail_count);
		}

		total_pass_count += suite_pass_count;
		total_fail_count += suite_fail_count;

		test_system->_current_suite = test_system->_current_suite->next;
	}

	_cutil_print_results(total_pass_count, total_fail_count);

	if (total_fail_count > 0)
		return 1;
	else
		return 0;
}

void _ctest_unconditional_test_result(int result) {
	test_system->_current_test->test_result = result;
}


int _ctest_predicate_true(const char *exppression_str, int result) {
	if (!result) {
		printf("Expected: %s to be true\n", exppression_str);
		test_system->_current_test->test_result = 1;
		return 0;
	}
	else {
		return 1;
	}
}

int _ctest_predicate_false(const char *exppression_str, int result) {
	if (result) {
		printf("Expected: %s to be false\n", exppression_str);
		test_system->_current_test->test_result = 1;
		return 0;
	}
	else {
		return 1;
	}
}

int _ctest_predicate_int_eq(const char *exppression_str, int expected, int actual) {
	if (expected != actual) {
		printf("Expected: %s to be %i\nActual: %i\n", exppression_str, expected, actual);
		test_system->_current_test->test_result = 1;
		return 0;
	}
	else {
		return 1;
	}
}

int _ctest_predicate_int_lt(const char *exppression_str, int expected, int actual) {
	if (actual >= expected) {
		printf("Expected: %s < %i\nActual: %i\n", exppression_str, expected, actual);
		test_system->_current_test->test_result = 1;
		return 0;
	}
	else {
		return 1;
	}
}

int _ctest_predicate_int_gt(const char *exppression_str, int expected, int actual) {
	if (actual <= expected) {
		printf("Expected: %s > %i\nActual: %i\n", exppression_str, expected, actual);
		test_system->_current_test->test_result = 1;
		return 0;
	}
	else {
		return 1;
	}
}

int _ctest_predicate_float_eq(const char *exppression_str, float expected, float actual) {
	if (expected != actual) {
		printf("Expected: %s == %f\nActual: %f\n", exppression_str, expected, actual);
		test_system->_current_test->test_result = 1;
		return 0;
	}
	else {
		return 1;
	}
}

int _ctest_predicate_ptr_eq(const char *exppression_str, void* expected, void* actual) {
	if (expected != actual) {
		printf("Expected: %s to be %p\nActual: %p\n", exppression_str, expected, actual);
		test_system->_current_test->test_result = 1;
		return 0;
	}
	else {
		return 1;
	}
}

int _ctest_predicate_ptr_null(const char *exppression_str, void* ptr) {
	if (ptr != NULL) {
		printf("Expected: %s to be NULL\nActual: %p\n", exppression_str, ptr);
		test_system->_current_test->test_result = 1;
		return 0;
	}

	return 1;
}

int _ctest_predicate_ptr_not_null(const char *exppression_str, void* ptr) {
	if (ptr == NULL) {
		printf("Expected: %s to be non-NULL\nActual: %p", exppression_str, ptr);
		test_system->_current_test->test_result = 1;
		return 0;
	}

	return 1;
}

int _ctest_predicate_ptr_neq(const char *ptr1_str, void* ptr1, void* ptr2) {
	if (ptr1 == ptr2) {
		printf("Expected: %s to not equal %p\n", ptr1_str, ptr2);
		test_system->_current_test->test_result = 1;
		return 0;
	}

	return 1;
}
