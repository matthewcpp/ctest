#ifndef CTEST_TEST_H
#define CTEST_TEST_H

#include "ctest/ctest.h"

typedef struct {
	ctest_test_func func;
} ctest_function_test;

typedef struct {
	_ctest_fixture_test_runner runner;
	_ctest_generic_fixture_test func;
} ctest_fixture_test;

typedef struct {
	_ctest_suite_test_runner runner;
	ctest_test_func func;
} ctest_suite_test;

typedef union {
    ctest_function_test function;
	ctest_fixture_test fixture;
	ctest_suite_test suite;
} ctest_test_data;

typedef enum {
	CTEST_FUNCTION_TEST,
	CTEST_FIXTURE_TEST,
	CTEST_SUITE_TEST
} ctest_test_type;


/* ctest_test describes a single unit test. */
typedef struct ctest_test {
    char* test_name;
    struct ctest_test* next;
    ctest_test_type type;
    ctest_test_data data;
} ctest_test;

ctest_test* ctest_test_create_with_function(const char* test_name, ctest_test_func test_func);
ctest_test* ctest_test_create_with_fixture(const char* fixture_name, const char* test_name, _ctest_fixture_test_runner fixture_test_runner, _ctest_generic_fixture_test fixture_test_func);
ctest_test* ctest_test_create_with_suite(const char* suite_name, const char* test_name, _ctest_suite_test_runner runner, ctest_test_func test_func);

void ctest_test_destroy(ctest_test* test);
void ctest_test_run(ctest_test* test);

#endif
