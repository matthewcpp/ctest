# ctest
[![Build Status](https://travis-ci.org/matthewcpp/ctest.svg?branch=master)](https://travis-ci.org/matthewcpp/ctest) [![Build status](https://ci.appveyor.com/api/projects/status/dxqm86e15soryrdi/branch/master?svg=true)](https://ci.appveyor.com/project/matthewcpp/ctest/branch/master) [![Documentation Status](https://readthedocs.org/projects/ctest/badge/?version=master)](https://ctest.readthedocs.io/en/latest/?badge=master)

### About
This is a small testing framework inspired by gtest for use in C 89/90 projects.  The library provides facilities for running and filtering tests.

### Test Functions
In order to test your code with ctest, you write functions that have expectations or assertions about how your code should behave.  For example, a test for a global function _fibonacci_ might look like this:

```C
void test_fib() {
	CTEST_ASSERT_INT_EQ(fibonacci(1), 1);
	CTEST_ASSERT_INT_EQ(fibonacci(5), 5);
	CTEST_ASSERT_INT_EQ(fibonacci(10), 55);
}
```

In order to run the test, it must be added to the test system:


```C
int main(int argc, char** argv) {
	ctest_init();

	CTEST_ADD_TEST(test_fib);

	return ctest_run();
}
```

### Test Fixtures
You can also make use of fixtures to reduce amount of setup and teardown code that would be shared by numerous tests.  A fixture has a unique name and consists of a structure to hold the test data along with a setup and teardown method.  The system will handle calling the correct setup and teardown method when running each test belonging to the fixture.

```C
typedef struct {
	list* test_list;
} list_test_data;

void list_test_setup(list_test_data* fixture) {
	fixture->test_list = list_create();
}

void list_test_teardown(list_test_data* fixture) {
	list_destroy(fixture->test_list);
}

CTEST_FIXTURE(list_test, list_test_data, list_test_setup, list_test_teardown)
```

A test using a fixture receives a pointer to the fixture's structure when it is ran:

```C
void test_list_push(list_test_data* fixture) {
	list_push_back(fixture->test_list, 42);

	CTEST_ASSERT_INT_EQ(list_size(fixture->test_list), 1)
}
```

Fixture tests are added in a similar way to test functions:

```C
int main(int argc, char** argv) {
	ctest_init();

	CTEST_ADD_TEST_F(list_test, test_list_push);

	return ctest_run();
}
```