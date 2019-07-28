#ifndef CTEST_RUNNER_H
#define CTEST_RUNNER_H

#include "ctest/internal/util.h"
#include "ctest/internal/test.h"
#include "ctest/internal/filter.h"

#include <stddef.h>

typedef struct {
	int tests_considered;
    int tests_ran;
    int tests_passed;
    int tests_failed;
    int tests_filtered;

	ctest_test* current_test;
	ctest_test_result current_test_result;
} ctest_runner;

ctest_runner* _ctest_runner_create();
void _ctest_runner_destroy(ctest_runner* runner);
int _ctest_runner_run_tests(ctest_runner* runner, ctest_test* first_test, ctest_filter* filter);

void ctest_runner_update_test_result(ctest_runner* runner, int predicate_result);
void ctest_runner_set_test_result(ctest_runner* runner, ctest_test_result test_result);

int ctest_runner_predicate_true(ctest_runner* runner, const char* expression_str, int result);
int ctest_runner_predicate_false(ctest_runner* runner, const char* expression_str, int result);
int ctest_runner_predicate_int_eq(ctest_runner* runner, const char* expression_str, int expected, int actual);
int ctest_runner_predicate_int_lt(ctest_runner* runner, const char* expression_str, int expected, int actual);
int ctest_runner_predicate_int_gt(ctest_runner* runner, const char* expression_str, int expected, int actual);
int ctest_runner_predicate_float_eq(ctest_runner* runner, const char* expression_str, float expected, float actual);
int ctest_runner_predicate_ptr_eq(ctest_runner* runner, const char* expression_str, void* expected, void* actual);
int ctest_runner_predicate_ptr_null(ctest_runner* runner, const char* expression_str, void* ptr);
int ctest_runner_predicate_ptr_not_null(ctest_runner* runner, const char* expression_str, void* ptr);
int ctest_runner_predicate_ptr_neq(ctest_runner* runner, const char* expression_str, void* ptr1, void* ptr2);

#endif
