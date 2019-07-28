#ifndef CTEST_UTIL_H
#define CTEST_UTIL_H

/* 
Note: snprintf is not available in MSVC prior to Visual Studio 2015.
Although the behavior of _snprintf is not identical to snprintf, it's implementation is sufficient for the use case in this library.
*/
#if defined(_MSC_VER) && _MSC_VER < 1900
	#define ctest_snprintf_func _snprintf
#else
	#define ctest_snprintf_func snprintf
#endif

typedef enum {
	CTEST_RESULT_NONE,
	CTEST_RESULT_PASSED,
	CTEST_RESULT_UNCONDITIONAL_PASS,
	CTEST_RESULT_FAIL,
	CTEST_RESULT_UNCONDITIONAL_FAIL,
	CTEST_RESULT_FILTERED
} ctest_test_result;

char* ctest_util_strdup(const char *src);
int ctest_util_str_cmp(const void* a, const void* b);

#endif
