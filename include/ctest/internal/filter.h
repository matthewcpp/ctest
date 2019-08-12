#ifndef CTEST_FILTER_H
#define CTEST_FILTER_H

#include <stddef.h>

typedef struct {
	char** filters;
	int filter_count;
} ctest_filter;

ctest_filter* ctest_filter_create();
void ctest_filter_destroy(ctest_filter* filter);

void ctest_filter_parse_string(ctest_filter* filter, const char* filter_str);
int ctest_filter_should_run_test(ctest_filter* filter, const char* test_name);

#endif
