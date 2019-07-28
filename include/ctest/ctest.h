#ifndef CTEST_CTEST_H
#define CTEST_CTEST_H

#include <stdlib.h>

typedef void(*ctest_test_func)();


void ctest_init();
void ctest_destroy();

void ctest_config_set_filter(const char *filter_str);
int ctest_run();

void _ctest_add_test(const char *test_name, ctest_test_func test_func);

int _ctest_predicate_true(const char* expression_str, int result);
int _ctest_predicate_false(const char* expression_str, int result);
int _ctest_predicate_int_eq(const char* expression_str, int expected, int actual);
int _ctest_predicate_int_gt(const char* expression_str, int expected, int actual);
int _ctest_predicate_int_lt(const char* expression_str, int expected, int actual);
int _ctest_predicate_float_eq(const char* expression_str, float expected, float actual);
int _ctest_predicate_ptr_eq(const char* expression_str, void* expected, void* actual);
int _ctest_predicate_ptr_neq(const char* expression_str, void* ptr1, void* ptr2);
int _ctest_predicate_ptr_null(const char* expression_str, void* ptr);
int _ctest_predicate_ptr_not_null(const char* expression_str, void* ptr);

void _ctest_unconditional_test_result(int result);


/*
A Generic fixture test is an internal pointer used to store fixtre tests that are added to the system.
These typeless function pointers are stored alongside a fixture runner function whose job it is to cast
them to the appropriate type before running them.
*/
typedef void(*_ctest_generic_fixture_test)(void*);

/*
A Fixture test runner function receives pointer to the fixture test and cast it the correct function pointer type
It will create the fixture, call the setup function, run the fixture test, call the teardown function, then delete the fixture
Runner functions are defined automatically by the CTEST_FIXTURE macro.
*/
typedef void(*_ctest_fixture_test_runner)(_ctest_generic_fixture_test);

void _ctest_add_fixture_test(_ctest_fixture_test_runner test_runner, const char* fixture_name, const char* test_name, _ctest_generic_fixture_test test);

#define CTEST_FIXTURE(name, type, setup_func, teardown_func) \
typedef void(*_ctest_fixture_test_func_##name)(type*); \
void _ctest_fixture_test_runner_##name(_ctest_generic_fixture_test test_func) { \
    _ctest_fixture_test_func_##name fixture_test = (_ctest_fixture_test_func_##name)test_func; \
    type* fixture = malloc(sizeof(type)); \
    setup_func(fixture); \
    fixture_test(fixture); \
    teardown_func(fixture); \
    free(fixture); \
} \
void _ctest_add_fixture_test_##name(const char* test_name, _ctest_fixture_test_func_##name test_func){ \
    _ctest_add_fixture_test(_ctest_fixture_test_runner_##name, #name, test_name, (_ctest_generic_fixture_test)test_func); \
}

#define CTEST_ADD_TESTF(name, test) \
    _ctest_add_fixture_test_##name(#test, &test)


/**
Unconditionally passes the current test.  Assert and expect macros that fail after this method is called will not affect the status of the test.
*/
#define CTEST_PASS_TEST() \
	_ctest_unconditional_test_result(0)

/**
Unconditionally fails the current test.
*/
#define CTEST_FAIL_TEST() \
	_ctest_unconditional_test_result(1)

#define CTEST_ADD_TEST(FUNC) \
	_ctest_add_test(#FUNC, &FUNC)

#define CTEST_EXPECT_TRUE(EXP) \
	_ctest_predicate_true(#EXP, (EXP))

#define CTEST_EXPECT_FALSE(EXP) \
	_ctest_predicate_false(#EXP, (EXP))

#define CTEST_EXPECT_INT_EQ(EXP, VAL) \
	_ctest_predicate_int_eq(#EXP, (int)VAL, (int)(EXP))

#define CTEST_EXPECT_INT_LT(EXP, VAL) \
	_ctest_predicate_int_lt(#EXP, (int)VAL, (int)(EXP))

#define CTEST_EXPECT_INT_GT(EXP, VAL) \
	_ctest_predicate_int_gt(#EXP, (int)VAL, (int)(EXP))

#define CTEST_EXPECT_FLOAT_EQ(EXP, VAL) \
	_ctest_predicate_float_eq(#EXP, VAL, (EXP))

#define CTEST_EXPECT_PTR_EQ(EXP, VAL) \
	_ctest_predicate_ptr_eq(#EXP, VAL, (EXP))

#define CTEST_EXPECT_PTR_NEQ(PTR1, PTR2) \
	_ctest_predicate_ptr_neq(#PTR1, (PTR1), (PTR2))

#define CTEST_EXPECT_PTR_NULL(EXP) \
	_ctest_predicate_ptr_null(#EXP, (EXP))

#define CTEST_EXPECT_PTR_NOT_NULL(EXP) \
	_ctest_predicate_ptr_not_null(#EXP, (EXP))


#define CTEST_ASSERT_TRUE(EXP) \
	if (!_ctest_predicate_true(#EXP, (EXP))) return

#define CTEST_ASSERT_FALSE(EXP) \
	if (!_ctest_predicate_false(#EXP, (EXP))) return

#define CTEST_ASSERT_INT_EQ(EXP, VAL) \
	if (!_ctest_predicate_int_eq(#EXP, (int)VAL, (int)(EXP))) return

#define CTEST_ASSERT_INT_LT(EXP, VAL) \
	if (!_ctest_predicate_int_lt(#EXP, (int)VAL, (int)(EXP))) return

#define CTEST_ASSERT_INT_GT(EXP, VAL) \
	if (!_ctest_predicate_int_gt(#EXP, (int)VAL, (int)(EXP))) return

#define CTEST_ASSERT_FLOAT_EQ(EXP, VAL) \
	if (!_ctest_predicate_float_eq(#EXP, VAL, (EXP))) return

#define CTEST_ASSERT_PTR_EQ(EXP, VAL) \
	if (!_ctest_predicate_ptr_eq(#EXP, VAL, (EXP))) return

#define CTEST_ASSERT_PTR_NEQ(PTR1, PTR2) \
	if (!_ctest_predicate_ptr_neq(#PTR1, (PTR1), (PTR2))) return

#define CTEST_ASSERT_PTR_NULL(EXP) \
	if (!_ctest_predicate_ptr_null(#EXP, (EXP))) return

#define CTEST_ASSERT_PTR_NOT_NULL(EXP) \
	if (!_ctest_predicate_ptr_not_null(#EXP, (EXP))) return

#endif
