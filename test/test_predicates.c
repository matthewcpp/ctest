#include <ctest/ctest.h>

#include <stddef.h>
#include <stdio.h>


void test_expect_true_passes_test() {
	CTEST_EXPECT_TRUE(1);
	CTEST_EXPECT_TRUE(-1);
}

void test_expect_true_fails_test() {
	CTEST_EXPECT_TRUE(0);
}

void test_assert_true_fails_test() {
	CTEST_ASSERT_TRUE(0);

	CTEST_PASS_TEST();  /* should not get here */
}

void test_expect_false_passes_test() {
	CTEST_EXPECT_FALSE(0);
}

void test_expect_false_fails_test() {
	CTEST_EXPECT_FALSE(1);
}

void test_assert_false_fails_test() {
	CTEST_ASSERT_FALSE(1);

	CTEST_PASS_TEST();  /* should not get here */
}

void test_expect_int_eq_passes_test() {
	CTEST_EXPECT_INT_EQ(1, 1);
}

void test_expect_int_eq_fails_test() {
	CTEST_EXPECT_INT_EQ(1, 2);
}

void test_assert_int_eq_fails_test() {
	CTEST_ASSERT_INT_EQ(1, 2);

	CTEST_PASS_TEST();  /* should not get here */
}

void test_expect_int_lt_passes_test() {
	CTEST_EXPECT_INT_LT(1, 2);
}

void test_expect_int_lt_fails_test() {
	CTEST_EXPECT_INT_LT(1, 0);
}

void test_assert_int_lt_fails_test() {
	CTEST_ASSERT_INT_LT(1, 0);

	CTEST_PASS_TEST();  /* should not get here */
}

void test_expect_int_gt_passes_test() {
	CTEST_EXPECT_INT_GT(2, 1);
}

void test_expect_int_gt_fails_test() {
	CTEST_EXPECT_INT_EQ(2, 3);
}

void test_assert_int_gt_fails_test() {
	CTEST_ASSERT_INT_EQ(2, 3);

	CTEST_PASS_TEST();  /* should not get here */
}

void test_expect_float_eq_passes_test() {
	CTEST_EXPECT_FLOAT_EQ(1.0f, 1.0f);
}

void test_expect_float_eq_fails_test() {
	CTEST_EXPECT_INT_EQ(1.0f, 2.0f);
}

void test_assert_float_eq_fails_test() {
	CTEST_ASSERT_INT_EQ(1.0f, 2.0f);

	CTEST_PASS_TEST();  /* should not get here */
}

void test_expect_ptr_eq_passes_test() {
	int i = 1;
	int* iptr = &i;
	CTEST_EXPECT_PTR_EQ(iptr, &i);
}

void test_expect_ptr_eq_fails_test() {
	int i1 = 1;
	int i2 = 2;
	CTEST_EXPECT_PTR_EQ(&i1, &i2);
}

void test_assert_ptr_eq_fails_test() {
	int i1 = 1;
	int i2 = 2;
	CTEST_ASSERT_PTR_EQ(&i1, &i2);

	CTEST_PASS_TEST();  /* should not get here */
}

void test_expect_ptr_neq_passes_test() {
	int i1 = 1;
	int i2 = 2;
	CTEST_EXPECT_PTR_NEQ(&i1, &i2);
}

void test_expect_ptr_neq_fails_test() {
	int i = 1;
	CTEST_EXPECT_PTR_NEQ(&i, &i);
}

void test_assert_ptr_neq_fails_test() {
	int i = 1;
	CTEST_ASSERT_PTR_NEQ(&i, &i);

	CTEST_PASS_TEST();  /* should not get here */
}

void test_expect_ptr_null_passes_test() {
	int* i1 = NULL;
	int* i2 = 0;
	CTEST_EXPECT_PTR_NULL(i1);
	CTEST_EXPECT_PTR_NULL(i2);
}

void test_expect_ptr_null_fails_test() {
	int i = 1;
	CTEST_EXPECT_PTR_NULL(&i);
}

void test_assert_ptr_null_fails_test() {
	int i = 1;
	CTEST_ASSERT_PTR_NULL(&i);

	CTEST_PASS_TEST();  /* should not get here */
}

void test_expect_ptr_not_null_passes_test() {
	int i = 0;
	CTEST_EXPECT_PTR_NOT_NULL(&i);
}

void test_expect_ptr_not_null_fails_test1() {
	int* i = NULL;
	CTEST_EXPECT_PTR_NOT_NULL(i);
}

void test_expect_ptr_not_null_fails_test2() {
	int* i = 0;
	CTEST_EXPECT_PTR_NOT_NULL(i);
}

void test_assert_ptr_not_null_fails_test() {
	int* i = NULL;
	CTEST_ASSERT_PTR_NOT_NULL(i);

	CTEST_PASS_TEST();  /* should not get here */
}

int run_test(const char* name, ctest_test_func test_func) {
	int test_result = 0;
	ctest_init();
	_ctest_add_test(name, test_func);

	test_result = ctest_run();
	ctest_destroy();

	return test_result;
}

#define TEST_SHOULD_PASS(TEST) \
	puts(#TEST); \
	if (run_test(#TEST, &TEST)){  \
		printf("Expected Test: %s to pass but it failed.\n", #TEST); \
		return 1; \
	}

#define TEST_SHOULD_FAIL(TEST) \
	puts(#TEST); \
	if (!run_test(#TEST, &TEST)) { \
		printf("Expected Test: %s to fail but it passed.\n", #TEST); \
		return 1; \
	}

int main(int argc, char** argv) {
	(void)argc;
	(void)argv;

	TEST_SHOULD_PASS(test_expect_true_passes_test)
	TEST_SHOULD_FAIL(test_expect_true_fails_test)
	TEST_SHOULD_FAIL(test_assert_true_fails_test)

	TEST_SHOULD_PASS(test_expect_false_passes_test)
	TEST_SHOULD_FAIL(test_expect_false_fails_test)
	TEST_SHOULD_FAIL(test_assert_false_fails_test)

	TEST_SHOULD_PASS(test_expect_int_eq_passes_test)
	TEST_SHOULD_FAIL(test_expect_int_eq_fails_test)
	TEST_SHOULD_FAIL(test_assert_int_eq_fails_test)

	TEST_SHOULD_PASS(test_expect_int_gt_passes_test)
	TEST_SHOULD_FAIL(test_expect_int_gt_fails_test)
	TEST_SHOULD_FAIL(test_assert_int_gt_fails_test)

	TEST_SHOULD_PASS(test_expect_int_lt_passes_test)
	TEST_SHOULD_FAIL(test_expect_int_lt_fails_test)
	TEST_SHOULD_FAIL(test_assert_int_lt_fails_test)

	TEST_SHOULD_PASS(test_expect_float_eq_passes_test)
	TEST_SHOULD_FAIL(test_expect_float_eq_fails_test)
	TEST_SHOULD_FAIL(test_assert_float_eq_fails_test)

	TEST_SHOULD_PASS(test_expect_ptr_eq_passes_test)
	TEST_SHOULD_FAIL(test_expect_ptr_eq_fails_test)
	TEST_SHOULD_FAIL(test_assert_ptr_eq_fails_test)

	TEST_SHOULD_PASS(test_expect_ptr_neq_passes_test)
	TEST_SHOULD_FAIL(test_expect_ptr_neq_fails_test)
	TEST_SHOULD_FAIL(test_assert_ptr_neq_fails_test)

	TEST_SHOULD_PASS(test_expect_ptr_null_passes_test)
	TEST_SHOULD_FAIL(test_expect_ptr_null_fails_test)
	TEST_SHOULD_FAIL(test_assert_ptr_null_fails_test)

	TEST_SHOULD_PASS(test_expect_ptr_not_null_passes_test)
	TEST_SHOULD_FAIL(test_expect_ptr_not_null_fails_test1)
	TEST_SHOULD_FAIL(test_expect_ptr_not_null_fails_test2)
	TEST_SHOULD_FAIL(test_assert_ptr_not_null_fails_test)

	puts("All tests passed");

    return 0;
}
