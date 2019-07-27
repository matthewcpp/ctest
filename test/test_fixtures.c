#include <ctest/ctest.h>

#include <stdio.h>

typedef struct {
   int* buffer;
} TestFixture;

void TestFixture1Setup(TestFixture* fixture) {
	puts("TestFixture1Setup");
	fixture->buffer = malloc(sizeof(int));
	*fixture->buffer = 10;
}

void TestFixture1Teardown(TestFixture* fixture) {
	puts("TestFixture1Teardown");
	free(fixture->buffer);
}

void TestFixture2Setup(TestFixture* fixture) {
	puts("TestFixture2Setup");
	fixture->buffer = malloc(sizeof(int));
	*fixture->buffer = 20;
}

void TestFixture2Teardown(TestFixture* fixture) {
	puts("TestFixture2Teardown");
	free(fixture->buffer);
}

void test_fixture1_test(TestFixture* fixture) {
	puts("test_fixture1_test");
	CTEST_ASSERT_INT_EQ(*fixture->buffer, 10);
}

void test_fixture2_test(TestFixture* fixture) {
	puts("test_fixture2_test");
	CTEST_ASSERT_INT_EQ(*fixture->buffer, 20);
}

void free_function_test() {
	puts("free_function_test");
	CTEST_ASSERT_TRUE(111);
}

CTEST_FIXTURE(TestFixture1, TestFixture, TestFixture1Setup, TestFixture1Teardown)
CTEST_FIXTURE(TestFixture2, TestFixture, TestFixture2Setup, TestFixture2Teardown)

int main (int argc, char** argv) {
	int result = 0;
	(void)argc;
	(void)argv;

    CTEST_ADD_TESTF(TestFixture1, test_fixture1_test);
	CTEST_ADD_TESTF(TestFixture2, test_fixture2_test);
	CTEST_ADD_TEST(free_function_test);

    result = ctest_run();

	if (result) {
		puts("tests did not pass");
	}
	else {
		puts("Tests passed");
	}

	return result;
}
