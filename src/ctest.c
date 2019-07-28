#include "ctest/ctest.h"

#include "ctest/internal/util.h"
#include "ctest/internal/system.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

ctest_system* test_system = NULL;

void ctest_init() {
	if (!test_system) {
		test_system = malloc(sizeof(ctest_system));
		ctest_system_init(test_system);
	}
}

void ctest_destroy() {
	if (test_system) {
        ctest_system_destroy(test_system);
		test_system = NULL;
	}
}

void _ctest_add_test(const char *test_name, ctest_test_func test_func) {
	ctest_test* test_entry = ctest_test_create_with_function(test_name, test_func);
	ctest_system_add_test(test_system, test_entry);
}

void _ctest_add_fixture_test(_ctest_fixture_test_runner test_runner, const char* fixture_name, const char* test_name, _ctest_generic_fixture_test test) {
	ctest_test* test_entry = ctest_test_create_with_fixture(fixture_name, test_name, test_runner, test);
	ctest_system_add_test(test_system, test_entry);
}

void ctest_config_set_filter(const char *filter_str) {
	ctest_filter_parse_string(test_system->filter, filter_str);
}

int ctest_run() {
	return _ctest_runner_run_tests(test_system->runner, test_system->unit_test_front, test_system->filter);
}

void _ctest_unconditional_test_result(int result) {
	if (result) {
		ctest_runner_set_test_result(test_system->runner, CTEST_RESULT_UNCONDITIONAL_PASS);
	}
	else {
		ctest_runner_set_test_result(test_system->runner, CTEST_RESULT_UNCONDITIONAL_FAIL);
	}
}


int _ctest_predicate_true(const char* expression_str, int result) {
	return ctest_runner_predicate_true(test_system->runner, expression_str, result);
}

int _ctest_predicate_false(const char* expression_str, int result) {
    return ctest_runner_predicate_false(test_system->runner, expression_str, result);
}

int _ctest_predicate_int_eq(const char* expression_str, int expected, int actual) {
	return ctest_runner_predicate_int_eq(test_system->runner, expression_str, expected, actual);
}

int _ctest_predicate_int_lt(const char* expression_str, int expected, int actual) {
	return ctest_runner_predicate_int_lt(test_system->runner, expression_str, expected, actual);
}

int _ctest_predicate_int_gt(const char *exppression_str, int expected, int actual) {
    return ctest_runner_predicate_int_gt(test_system->runner, exppression_str, expected, actual);
}

int _ctest_predicate_float_eq(const char* expression_str, float expected, float actual) {
	return ctest_runner_predicate_float_eq(test_system->runner, expression_str, expected, actual);
}

int _ctest_predicate_ptr_eq(const char* expression_str, void* expected, void* actual) {
	return ctest_runner_predicate_ptr_eq(test_system->runner, expression_str, expected, actual);
}

int _ctest_predicate_ptr_null(const char* expression_str, void* ptr) {
	return ctest_runner_predicate_ptr_null(test_system->runner, expression_str, ptr);
}

int _ctest_predicate_ptr_not_null(const char* expression_str, void* ptr) {
	return ctest_runner_predicate_ptr_not_null(test_system->runner, expression_str, ptr);
}

int _ctest_predicate_ptr_neq(const char* expression_str, void* ptr1, void* ptr2) {
	return ctest_runner_predicate_ptr_neq(test_system->runner, expression_str, ptr1, ptr2);
}
