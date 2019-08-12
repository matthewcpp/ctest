#include "ctest/internal/test.h"
#include "ctest/internal/util.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>

char* _test_name(const char* prefix, const char* test_name ) {
    size_t fixture_name_length = strlen(prefix);
    size_t test_name_length = strlen(test_name);
    size_t test_name_buffer_size = fixture_name_length + test_name_length + 2;
    char* name_buffer = malloc(test_name_buffer_size);

    ctest_snprintf_func(name_buffer, test_name_buffer_size, "%s.%s", prefix, test_name);

    return name_buffer;
}

ctest_test* ctest_test_create_with_function(const char* test_name, ctest_test_func test_func) {
	ctest_test* test = malloc(sizeof(ctest_test));
    test->next = NULL;
    test->test_name = ctest_util_strdup(test_name);

    test->type = CTEST_FUNCTION_TEST;
    test->data.function.func = test_func;

    return test;
}

ctest_test* ctest_test_create_with_fixture(const char* fixture_name, const char* test_name, _ctest_fixture_test_runner fixture_test_runner, _ctest_generic_fixture_test fixture_test_func) {
	ctest_test* test = malloc(sizeof(ctest_test));
    test->next = NULL;
    test->test_name = _test_name(fixture_name, test_name);

    test->type = CTEST_FIXTURE_TEST;
    test->data.fixture.runner = fixture_test_runner;
    test->data.fixture.func = fixture_test_func;

    return test;
}

ctest_test* ctest_test_create_with_suite(const char* suite_name, const char* test_name, _ctest_suite_test_runner test_runner, ctest_test_func test_func) {
    ctest_test* test = malloc(sizeof(ctest_test));
    test->next = NULL;
    test->test_name = _test_name(suite_name, test_name);

    test->type = CTEST_SUITE_TEST;
    test->data.suite.runner = test_runner;
    test->data.suite.func = test_func;

    return test;
}

void ctest_test_destroy(ctest_test* test) {
    if (test->test_name) {
        free(test->test_name);
    }

    free(test);
}

void ctest_test_run(ctest_test* test) {
    switch (test->type) {
        case CTEST_FUNCTION_TEST:
            test->data.function.func();
            break;
        case CTEST_FIXTURE_TEST:
            test->data.fixture.runner(test->data.fixture.func);
            break;
        case CTEST_SUITE_TEST:
            test->data.suite.runner(test->data.suite.func);
            break;
    }
}
