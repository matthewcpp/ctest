#ifndef CTEST_UTIL_H
#define CTEST_UTIL_H

/*
Note: snprintf is not available in MSVC prior to Visual Studio 2015.
*/
#if defined(_MSC_VER) && _MSC_VER < 1900
	#define cutil_snprintf_func _snprintf
#else
	#define cutil_snprintf_func snprintf
#endif

char* _cutil_strdup(const char *src);
int _cutil_str_cmp(const void* a, const void* b);

#endif
