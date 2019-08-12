#include <ctest/ctest.h>

#include <stdio.h>

typedef struct {
   int* buffer;
} TestFixture;

int TestFixture1SetupCallCount = 0;
int TestFixture1TeardownCallCount = 0;

int TestFixture2SetupCallCount = 0;
int TestFixture2TeardownCallCount = 0;

void TestFixture1Setup(TestFixture* fixture) {
	fixture->buffer = malloc(sizeof(int));
	*fixture->buffer = 10;

	TestFixture1SetupCallCount += 1;
}

void TestFixture1Teardown(TestFixture* fixture) {
	free(fixture->buffer);

	TestFixture1TeardownCallCount += 1;
}

void TestFixture2Setup(TestFixture* fixture) {
	fixture->buffer = malloc(sizeof(int));
	*fixture->buffer = 20;

	TestFixture2SetupCallCount += 1;
}

void TestFixture2Teardown(TestFixture* fixture) {
	free(fixture->buffer);

	TestFixture2TeardownCallCount += 1;
}

void TestFixture1Test(TestFixture* fixture) {
	CTEST_ASSERT_INT_EQ(*fixture->buffer, 10);
}

void TestFixture2Test(TestFixture* fixture) {
	CTEST_ASSERT_INT_EQ(*fixture->buffer, 20);
}

CTEST_FIXTURE(TestFixture1, TestFixture, TestFixture1Setup, TestFixture1Teardown)
CTEST_FIXTURE(TestFixture2, TestFixture, TestFixture2Setup, TestFixture2Teardown)

#define ASSERT_CALL_COUNT_EQ(call_count_var, expected_val) \
	if (call_count_var != expected_val) { \
		printf("expected %s to be %d , but actual value was: %d\n", #call_count_var, expected_val, call_count_var); \
		return 1; \
	} \



int main (int argc, char** argv) {
	int result = 0;
	(void)argc;
	(void)argv;

	ctest_init();

    CTEST_ADD_TEST_F(TestFixture1, TestFixture1Test);
	CTEST_ADD_TEST_F(TestFixture2, TestFixture2Test);

    result = ctest_run();
	if (result) {
		return result;
	}

	ctest_destroy();

	ASSERT_CALL_COUNT_EQ(TestFixture1SetupCallCount, 1)
	ASSERT_CALL_COUNT_EQ(TestFixture1TeardownCallCount, 1)

	ASSERT_CALL_COUNT_EQ(TestFixture2SetupCallCount, 1)
	ASSERT_CALL_COUNT_EQ(TestFixture2TeardownCallCount, 1)

	puts("Fixture Tests Passed");

	return 0;
}
