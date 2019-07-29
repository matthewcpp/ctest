#include "ctest/ctest.h"

#include <stdio.h>
#include <string.h>

#define TEST_FUNC_COUNT 4
int actual_call_counts[TEST_FUNC_COUNT];
int expected_call_counts[TEST_FUNC_COUNT];

#define ZERO_OUT(var) \
    memset((var), 0, sizeof((var)))

typedef struct {
	int i;
} test_fixture;

void test_fixture_func(test_fixture* test_fixture) {
	(void)test_fixture;
}

CTEST_FIXTURE(TestFixture1, test_fixture, test_fixture_func, test_fixture_func)
CTEST_FIXTURE(TestFixture2, test_fixture, test_fixture_func, test_fixture_func)

void test_0() {
    actual_call_counts[0] += 1;
    CTEST_PASS_TEST();
}

void test_1(test_fixture* tf) {
    tf->i = 1;
    actual_call_counts[1] += 1;
    CTEST_PASS_TEST();
}

void test_2(test_fixture* tf) {
    tf->i = 1;
    actual_call_counts[2] += 1;
    CTEST_PASS_TEST();
}

void test_3(test_fixture* tf) {
    tf->i = 1;
    actual_call_counts[3] += 1;
    CTEST_PASS_TEST();
}

int run_tests_with_filter(const char* filter) {
    int result, i;
    ZERO_OUT(actual_call_counts);

    ctest_init();
    ctest_config_set_filter(filter);

    CTEST_ADD_TEST(test_0);
    CTEST_ADD_TESTF(TestFixture1, test_1);
    CTEST_ADD_TESTF(TestFixture1, test_2);
    CTEST_ADD_TESTF(TestFixture2, test_3);

    result = ctest_run();

    ctest_destroy();

    if (result){
        return result;
    }

    for (i = 0; i < TEST_FUNC_COUNT; i++) {
        if (expected_call_counts[i] != actual_call_counts[i]) {
            printf("Filter: %s: Test %d: expected: %d, actual: %d", filter, i, expected_call_counts[i], actual_call_counts[i]);
            return 1;
        }
    }

    return 0;
}

int main(int argc, char** argv) {
    int i = 0;
	(void)argc;
	(void)argv;

    ZERO_OUT(expected_call_counts);
    for (i = 0; i < TEST_FUNC_COUNT; i++) { expected_call_counts[i] = 1; }
    if (run_tests_with_filter(NULL)) return 1;

    ZERO_OUT(expected_call_counts);
    for (i = 1; i <= 2; i++) { expected_call_counts[i] = 1; }
    if (run_tests_with_filter("TestFixture1")) return 1;

    ZERO_OUT(expected_call_counts);
    expected_call_counts[2] = 1;
    if (run_tests_with_filter("TestFixture1.test_2")) return 1;

    ZERO_OUT(expected_call_counts);
    for (i = 1; i <= 3; i++) { expected_call_counts[i] = 1; }
    if (run_tests_with_filter("TestFixture1;TestFixture2")) return 1;

    puts("All Tests Passed");
	return 0;
}
