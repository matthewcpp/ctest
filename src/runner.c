#include "ctest/internal/runner.h"

#include <stdlib.h>
#include <stdio.h>

void ctest_runner_process_test_result(ctest_runner* runner);
void ctest_runner_print_results(ctest_runner* runner);

ctest_runner* _ctest_runner_create() {
	ctest_runner* runner = calloc(1, sizeof(ctest_runner));

	return runner;
}

void _ctest_runner_destroy(ctest_runner* runner) {
	free(runner);
}

int _ctest_runner_run_tests(ctest_runner* runner, ctest_test* first_test, ctest_filter* filter) {
	runner->current_test = first_test;

	while (runner->current_test) {
		runner->current_test_result = CTEST_RESULT_NONE;
		runner->tests_considered += 1;

		if (ctest_filter_should_run_test(filter, runner->current_test->test_name)) {
			printf("Test: %s\n", runner->current_test->test_name);
			ctest_test_run(runner->current_test);
			runner->tests_ran += 1;
		}
		else {
			runner->current_test_result = CTEST_RESULT_FILTERED;
		}

		ctest_runner_process_test_result(runner);
		runner->current_test = runner->current_test->next;
	}

	runner->current_test = NULL;

	ctest_runner_print_results(runner);

	return runner->tests_failed > 0;
}

void ctest_runner_process_test_result(ctest_runner* runner) {
	switch (runner->current_test_result) {
		case CTEST_RESULT_NONE:
			puts("Warning: No assertions during test");

		case CTEST_RESULT_PASSED:
		case CTEST_RESULT_UNCONDITIONAL_PASS:
			puts("-----> Result: PASSED");
			runner->tests_passed += 1;
			break;

		case CTEST_RESULT_FAIL:
		case CTEST_RESULT_UNCONDITIONAL_FAIL:
			puts("-----> Result: FAILED");
			runner->tests_failed += 1;
			break;

		case CTEST_RESULT_FILTERED:
			runner->tests_filtered += 1;
			break;
	}
}

void ctest_runner_print_results(ctest_runner* runner) {
	printf("\n-----------------------------------\n");
	printf("TestResults:\n");
	printf("Total Tests: %d\n", runner->tests_considered);
	printf("Tests Passed: %d\n", runner->tests_passed);
	printf("Tests Failed: %d\n", runner->tests_failed);
}

void ctest_runner_set_test_result(ctest_runner* runner, ctest_test_result test_result) {
	runner->current_test_result = test_result;
}

void ctest_runner_update_test_result(ctest_runner* runner, int predicate_result) {
	if (runner->current_test_result == CTEST_RESULT_UNCONDITIONAL_PASS || runner->current_test_result == CTEST_RESULT_FAIL) {
		return;
	}

    if (predicate_result && runner->current_test_result != CTEST_RESULT_FAIL) {
        runner->current_test_result = CTEST_RESULT_PASSED;
    }
    else if (!predicate_result) {
        runner->current_test_result = CTEST_RESULT_FAIL;
    }
}

int ctest_runner_predicate_true(ctest_runner* runner, const char* expression_str, int result) {
    if (!result) {
        printf("Expected: %s to be true\n", expression_str);
        ctest_runner_update_test_result(runner, 0);
        return 0;
    }
    else {
        ctest_runner_update_test_result(runner, 1);
        return 1;
    }
}

int ctest_runner_predicate_false(ctest_runner* runner, const char* expression_str, int result) {
    if (result) {
        printf("Expected: %s to be false\n", expression_str);
        ctest_runner_update_test_result(runner, 0);
        return 0;
    }
    else {
        ctest_runner_update_test_result(runner, 1);
        return 1;
    }
}

int ctest_runner_predicate_int_eq(ctest_runner* runner, const char* expression_str, int expected, int actual) {
    if (expected != actual) {
        printf("Expected: %s to be %i\nActual: %i\n", expression_str, expected, actual);
        ctest_runner_update_test_result(runner, 0);
        return 0;
    }
    else {
        ctest_runner_update_test_result(runner, 1);
        return 1;
    }
}

int ctest_runner_predicate_int_lt(ctest_runner* runner, const char* expression_str, int expected, int actual) {
    if (actual >= expected) {
        printf("Expected: %s < %i\nActual: %i\n", expression_str, expected, actual);
        ctest_runner_update_test_result(runner, 0);
        return 0;
    }
    else {
        ctest_runner_update_test_result(runner, 1);
        return 1;
    }
}

int ctest_runner_predicate_int_gt(ctest_runner* runner, const char* expression_str, int expected, int actual) {
    if (actual <= expected) {
        printf("Expected: %s > %i\nActual: %i\n", expression_str, expected, actual);
        ctest_runner_update_test_result(runner, 0);
        return 0;
    }
    else {
        ctest_runner_update_test_result(runner, 1);
        return 1;
    }
}

int ctest_runner_predicate_float_eq(ctest_runner* runner, const char* expression_str, float expected, float actual) {
    if (expected != actual) {
        printf("Expected: %s == %f\nActual: %f\n", expression_str, expected, actual);
        ctest_runner_update_test_result(runner, 0);
        return 0;
    }
    else {
        ctest_runner_update_test_result(runner, 1);
        return 1;
    }
}

int ctest_runner_predicate_ptr_eq(ctest_runner* runner, const char* expression_str, void* expected, void* actual) {
    if (expected != actual) {
        printf("Expected: %s to be %p\nActual: %p\n", expression_str, expected, actual);
        ctest_runner_update_test_result(runner, 0);
        return 0;
    }
    else {
        ctest_runner_update_test_result(runner, 1);
        return 1;
    }
}

int ctest_runner_predicate_ptr_null(ctest_runner* runner, const char* expression_str, void* ptr) {
    if (ptr != NULL) {
        printf("Expected: %s to be NULL\nActual: %p\n", expression_str, ptr);
        ctest_runner_update_test_result(runner, 0);
        return 0;
    }
    else {
        ctest_runner_update_test_result(runner, 1);
        return 1;
    }
}

int ctest_runner_predicate_ptr_not_null(ctest_runner* runner, const char* expression_str, void* ptr) {
    if (ptr == NULL) {
        printf("Expected: %s to be non-NULL\nActual: %p", expression_str, ptr);
        ctest_runner_update_test_result(runner, 0);
        return 0;
    }
    else {
        ctest_runner_update_test_result(runner, 1);
        return 1;
    }

}

int ctest_runner_predicate_ptr_neq(ctest_runner* runner, const char* expression_str, void* ptr1, void* ptr2) {
    if (ptr1 == ptr2) {
        printf("Expected: %s to not equal %p\n", expression_str, ptr2);
		ctest_runner_update_test_result(runner, 0);
        return 0;
    }
	else {
		ctest_runner_update_test_result(runner, 1);
		return 1;
	}
}
