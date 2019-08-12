#include "ctest/ctest.h"

#include <stdio.h>

int SuiteSetupCallCount = 0;
int SuiteTeardownCallCount = 0;
int SuiteTestFuncCallCount = 0;

void suite_test_func() {
    SuiteTestFuncCallCount += 1;
}

void suite_test_setup() {
    SuiteSetupCallCount += 1;
    CTEST_PASS_TEST();
}

void suite_test_teardown() {
    SuiteTeardownCallCount += 1;
}

CTEST_SUITE(suite_test, suite_test_setup, suite_test_teardown)

#define ASSERT_RESULT(var, val) \
    if (var != val) { \
        printf("%s did not have expected value", #var); \
        return 1; \
    }

int main(int argc, char** argv) {
    int result;
    (void)argc;
    (void)argv;

    ctest_init();

    CTEST_ADD_TEST_S(suite_test, suite_test_func);

    result = ctest_run();

    ASSERT_RESULT(result, 0);
    ASSERT_RESULT(SuiteSetupCallCount, 1);
    ASSERT_RESULT(SuiteTeardownCallCount, 1);
    ASSERT_RESULT(SuiteTestFuncCallCount, 1);

    puts("All tests passed");

    return 0;
}
