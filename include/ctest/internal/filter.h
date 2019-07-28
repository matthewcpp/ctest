#ifndef CTEST_FILTER_H
#define CTEST_FILTER_H

#include <stddef.h>

typedef struct {
	char** filters;
	int filter_count;
} _ctest_filter;

_ctest_filter* _ctest_filter_create();
void _ctest_filter_destroy(_ctest_filter* filter);

void ctest_filter_parse_string(_ctest_filter* filter, const char* filter_str);

int _ctest_filter_should_run_test(_ctest_filter* filter, const char* test_name);

#endif
