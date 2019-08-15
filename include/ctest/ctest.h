#ifndef CTEST_CTEST_H
#define CTEST_CTEST_H

/** \file ctest.h */

#include <stdlib.h>

/** 
Callback for a function test.  
A function test does not require any parameters and does not need assoicated setup or teardown functionality.
*/
typedef void(*ctest_test_func)();

/**
Function definition for a suite setup or teardown function.
A suite setup function is called before each test in a suite.
A suite teardown function is called after each test in a suite.
*/
typedef void(*ctest_suite_func)();



/**
Initializes the testting framework.  
This method should be called once before any other library function. 
*/
void ctest_init();

/**
Releases all resources used by the testing library.
After calling this methid, \ref ctest_init should be called again before any subsequent called to library methods.
*/
void ctest_destroy();

/**
Sets the filter string which is used to determine which tests should be run.
When a filter is active, a test will only be ran if its name begins with at least one active filter.
Multiple filters may be specified as a ';' separated list.
Calling this method multiple times will clear out any previously set filters.
\param filter_str list of filters to apply to all tests.  Multiple filters are spearated by ';'.
*/
void ctest_config_set_filter(const char* filter_str);

/**
Runs all tests.
\returns zero if all tests passed, otherwise a non zero value.
*/
int ctest_run();

/** \cond PRIVATE */
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

typedef void(*_ctest_suite_test_runner)(ctest_test_func);
void _ctest_add_suite_test(_ctest_suite_test_runner test_runner, const char* suite_name, const char* test_name, ctest_test_func test_func);

/** \endcond */


/**
Defines a new text fixture.
A test fixture is useful for tests which require non trivial setup and tear down as well as operating on non global data.
The system will allocate a test data struct, as well as call the appropriate setup and teardown methods for each test that uses the fixture.
For a test to use a fixture with type T.  It's function signature should have one parameter of type T*.
A test fixture is also a good way to avoid using global data in all of your tests.
\param name The name of the test fixture.  Each fixture name must be unique.
\param type The type name of the struct which contains the fixture data.  A new struct will be allocated and used for each test in the fixture
\param setup_func The name of a function which will be called before each fixture test.  This function will receive a pointer to a freshly allocated struct of the type named by the name parameter.  This function should initialize the members of the fixture struct to default values.
\param teardown_func The name of a function which will be called after each fixture test.  This function will recieve a pointer to a struct of the type named by the name parameter.  This function should free all resources used by the object, but not the object itself.
*/
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

/**
Adds a fixture test to the system.
A Fixture tests runs after the appropriate setup method has been called and receives a freshly allocated and initialized test data structure.
If a test is being added to a fixture whose type is T, the test should take a single parameter of T*.
After the test returns, the fixture's teardown method will be called on the structure that was passed to the test.
\param fixture_name The name of the fixture that this test will be added to.
\param test name of the function serving as the test method to be added to the system.  Note that for filtering purposes, the test name in the system will be <fixture_name>.<test>
*/
#define CTEST_ADD_TEST_F(fixture_name, test) \
    _ctest_add_fixture_test_##fixture_name(#test, &test)


/**
Defines a new test suite.
A test suite is useful for tests which require non trivial setup and tear down but do not need any test specific data, or use global data.
The system will call the appropriate setup and teardown methods for each test that is in the suite.
\param name the name of this suite.  Each Suite name must be unique.
\param setup_func The name of a function which will be called before each test in this suite.  This function should have a signature matching \ref ctest_suite_func
\param teardown_func The name of a function which will be called after each test in this suite.  This function should have a signature matching \ref ctest_suite_func
*/
#define CTEST_SUITE(name, setup_func, teardown_func) \
void _ctest_suite_runner##name(ctest_test_func test_func) { \
	setup_func(); \
	test_func(); \
	teardown_func(); \
} \
void _ctest_add_suite_test_##name(const char* test_name, ctest_test_func test_func) { \
    _ctest_add_suite_test(_ctest_suite_runner##name, #name, test_name, test_func); \
} \

/**
Adds a suite test to the system
A suite test runs in between the setup and teardown functions defined by the suite it is a part of.
The test function being added should have a signature matching \ref ctest_test_func
\param suite_name The name of the suite this test will be added to.
\param test name of the function serving as the test method to be added to the system.  Note that for filtering purposes, the test name in the system will be <suite_name>.<test>
*/
#define CTEST_ADD_TEST_S(suite_name, test) \
    _ctest_add_suite_test_##suite_name(#test, &test);

/**
Unconditionally passes the current test.  Assert and expect macros that fail after this method is called will not affect the status of the test.
*/
#define CTEST_PASS_TEST() \
	_ctest_unconditional_test_result(1)

/**
Unconditionally fails the current test.
*/
#define CTEST_FAIL_TEST() \
	_ctest_unconditional_test_result(0)

/**
Adds a test function to the system.
The test function being added should have a signature matching \ref ctest_test_func
\param suite_name The function that will be added. Note that for filtering purposes, the test name in the system will be the same as the added function's name.
*/
#define CTEST_ADD_TEST(test_func) \
	_ctest_add_test(#test_func, &test_func)

/**
Tests that an expression evaluates to true.
If it does not, the currently running test is marked as failed.
The expression is true if it evaluates to a non-zero value.
\param EXP The expression to test.
*/
#define CTEST_EXPECT_TRUE(EXP) \
	_ctest_predicate_true(#EXP, (EXP))

/**
Tests that an expression evaluates to false.
If it does not, the currently running test is marked as failed.
The expression is false if it evaluates to zero.
\param EXP The expression to test.
*/
#define CTEST_EXPECT_FALSE(EXP) \
	_ctest_predicate_false(#EXP, (EXP))

/**
Tests that an expression evaluates to an expected integer value.
If it does not, the currently running test is marked as failed.
\param EXP The expression to test.
\param VAL the expected integer value.
*/
#define CTEST_EXPECT_INT_EQ(EXP, VAL) \
	_ctest_predicate_int_eq(#EXP, (int)VAL, (int)(EXP))

/**
Tests that an expression evaluates to less than an expected integer value.
If it does not, the currently running test is marked as failed.
\param EXP The expression to test.
\param VAL the expected integer value that EXP should be less than.
*/
#define CTEST_EXPECT_INT_LT(EXP, VAL) \
	_ctest_predicate_int_lt(#EXP, (int)VAL, (int)(EXP))

/**
Tests that an expression evaluates to greater than an expected integer value.
If it does not, the currently running test is marked as failed.
\param EXP The expression to test.
\param VAL the expected integer value that EXP should be greater than.
*/
#define CTEST_EXPECT_INT_GT(EXP, VAL) \
	_ctest_predicate_int_gt(#EXP, (int)VAL, (int)(EXP))

/**
Tests that an expression evaluates to an expected floating point value.
If it does not, the currently running test is marked as failed.
Note this test performs a strict equality check.
\param EXP The expression to test.
\param VAL the expected float value.
*/
#define CTEST_EXPECT_FLOAT_EQ(EXP, VAL) \
	_ctest_predicate_float_eq(#EXP, VAL, (EXP))

/**
Tests two pointers are equal.
If they are not, the currently running test is marked as failed.
\param EXPECTED The expected value of the pointer.
\param ACTUAL The actual value of the pointer.
*/
#define CTEST_EXPECT_PTR_EQ(EXPECTED, ACTUAL) \
	_ctest_predicate_ptr_eq(#EXPECTED, ACTUAL, (EXPECTED))

/**
Tests two pointers are not equal.
If they are equal, the currently running test is marked as failed.
\param PTR1 The first pointer.
\param PTR2 The second pointer.
*/
#define CTEST_EXPECT_PTR_NEQ(PTR1, PTR2) \
	_ctest_predicate_ptr_neq(#PTR1, (PTR1), (PTR2))

/**
Tests that a pointer is NULL.
If it is not, the currently running test is marked as failed.
\param PTR The pointer to test.
*/
#define CTEST_EXPECT_PTR_NULL(PTR) \
	_ctest_predicate_ptr_null(#PTR, (PTR))

/**
Tests that a pointer is not NULL.
If it is, the currently running test is marked as failed.
\param PTR The pointer to test.
*/
#define CTEST_EXPECT_PTR_NOT_NULL(EXP) \
	_ctest_predicate_ptr_not_null(#EXP, (EXP))


/**
Asserts that an expression evaluates to true.
If it does not, the currently running test is marked as failed and the current function will return.
The expression is true if it evaluates to a non-zero value.
\param EXP The expression to test.
*/
#define CTEST_ASSERT_TRUE(EXP) \
	if (!_ctest_predicate_true(#EXP, (EXP))) return

/**
Asserts that an expression evaluates to false.
If it does not, the currently running test is marked as failed and the current function will return.
The expression is false if it evaluates to zero.
\param EXP The expression to test.
*/
#define CTEST_ASSERT_FALSE(EXP) \
	if (!_ctest_predicate_false(#EXP, (EXP))) return

/**
Asserts that an expression evaluates to an expected integer value.
If it does not, the currently running test is marked as failed and the current function will return.
\param EXP The expression to test.
\param VAL the expected integer value.
*/
#define CTEST_ASSERT_INT_EQ(EXP, VAL) \
	if (!_ctest_predicate_int_eq(#EXP, (int)VAL, (int)(EXP))) return

/**
Asserts that an expression evaluates to less than an expected integer value.
If it does not, the currently running test is marked as failed and the current function will return.
\param EXP The expression to test.
\param VAL the expected integer value that EXP should be less than.
*/
#define CTEST_ASSERT_INT_LT(EXP, VAL) \
	if (!_ctest_predicate_int_lt(#EXP, (int)VAL, (int)(EXP))) return

/**
Asserts that an expression evaluates to greater than an expected integer value.
If it does not, the currently running test is marked as failed and the current function will return.
\param EXP The expression to test.
\param VAL the expected integer value that EXP should be greater than.
*/
#define CTEST_ASSERT_INT_GT(EXP, VAL) \
	if (!_ctest_predicate_int_gt(#EXP, (int)VAL, (int)(EXP))) return

/**
Asserts that an expression evaluates to an expected floating point value.
If it does not, the currently running test is marked as failed and the current function will return.
Note this test performs a strict equality check.
\param EXP The expression to test.
\param VAL the expected float value.
*/
#define CTEST_ASSERT_FLOAT_EQ(EXP, VAL) \
	if (!_ctest_predicate_float_eq(#EXP, VAL, (EXP))) return

/**
Asserts two pointers are equal.
If they are not, the currently running test is marked as failed and the current function will return.
\param EXPECTED The expected value of the pointer.
\param ACTUAL The actual value of the pointer.
*/
#define CTEST_ASSERT_PTR_EQ(EXPECTED, ACTUAL) \
	if (!_ctest_predicate_ptr_eq(#EXPECTED, ACTUAL, (EXPECTED))) return

/**
Asserts two pointers are not equal.
If they are equal, the currently running test is marked as failed and the current function will return.
\param PTR1 The first pointer.
\param PTR2 The second pointer.
*/
#define CTEST_ASSERT_PTR_NEQ(PTR1, PTR2) \
	if (!_ctest_predicate_ptr_neq(#PTR1, (PTR1), (PTR2))) return

/**
Asserts that a pointer is NULL.
If it is not, the currently running test is marked as failed and the current function will return.
\param PTR The pointer to test.
*/
#define CTEST_ASSERT_PTR_NULL(PTR) \
	if (!_ctest_predicate_ptr_null(#PTR, (PTR))) return

/**
Asserts that a pointer is not NULL.
If it is, the currently running test is marked as failed and the current function will return.
\param PTR The pointer to test.
*/
#define CTEST_ASSERT_PTR_NOT_NULL(PTR) \
	if (!_ctest_predicate_ptr_not_null(#PTR, (PTR))) return

#endif
