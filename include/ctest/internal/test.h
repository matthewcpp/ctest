#ifndef CTEST_TEST_H
#define CTEST_TEST_H

typedef void(*cutil_test_function)();

/* _cutil_test_entry describes a single unit test. */
typedef struct _cutil_test_entry {
    cutil_test_function test_func;
    char *test_name;
    unsigned int test_result;
    char *test_message;

    struct _cutil_test_entry* next;
} _cutil_test_entry;

_cutil_test_entry *_cutil_testing_entry_create(const char *test_name, cutil_test_function test_func);
void _cutil_testing_entry_destroy(_cutil_test_entry *test_entry);

#endif
