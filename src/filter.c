#include "ctest/internal/filter.h"
#include "ctest/internal/util.h"

#include <string.h>
#include <stdlib.h>

char** _ctest_filter_split_filter_string(const char* str, int* count);
int _ctest_filter_has_str(const char* needle, char** haystack, int start, int end);
void ctest_filter_clear(ctest_filter* filter);

ctest_filter* ctest_filter_create() {
	ctest_filter* filter = calloc(1, sizeof(ctest_filter));

	return filter;
}

void ctest_filter_destroy(ctest_filter* filter) {
	ctest_filter_clear(filter);

	free(filter);
}

void ctest_filter_parse_string(ctest_filter* filter, const char* filter_str) {
	ctest_filter_clear(filter);

	if (filter_str == NULL) {
		filter->filters = NULL;
		filter->filter_count = 0;
	}
	else {
		char* filter_str_cpy = _cutil_strdup(filter_str);
		filter->filters = _ctest_filter_split_filter_string(filter_str_cpy, &filter->filter_count);
		free(filter_str_cpy);
	}
}

void ctest_filter_clear(ctest_filter* filter) {
	if (filter->filters) {
		int i;
		for (i = 0; i < filter->filter_count; i++) {
			free(filter->filters[i]);
		}

		free(filter->filters);
	}
}

char** _ctest_filter_split_filter_string(const char* str, int* count) {
	int token_count = 0;
	int token_size = 1;

	char** tokens = malloc(sizeof(char*) * token_size);
	char* search_str = _cutil_strdup(str);

	char *token = strtok(search_str, ";");
	while (token) {
		tokens[token_count] = _cutil_strdup(token);

		token_count += 1;

		if (token_count == token_size) {
			token_size *= 2;
			tokens = realloc(tokens, sizeof(char*) * token_size);
		}

		token = strtok(NULL, ";");
	}

	free(search_str);

	qsort(tokens, token_count, sizeof(char*), _cutil_str_cmp);

	*count = token_count;
	return tokens;
}

int ctest_filter_should_run_test(ctest_filter* filter, const char* test_name) {
	if (filter->filters) {
		return _ctest_filter_has_str(test_name, filter->filters, 0, filter->filter_count - 1);
	}
	else {
		return 1;
	}
}

int _ctest_filter_has_str(const char* needle, char** haystack, int start, int end) {
	int middle = start + ((end - start) / 2);
	int result = strcmp(needle, haystack[middle]);

	if (end <= start) {
		if (result == 0) {
			return 1;
		}
		else {
			return 0;
		}
	}

	if (result > 0) {
		return _ctest_filter_has_str(needle, haystack, middle + 1, end);
	}
	else if (result < 0) {
		return _ctest_filter_has_str(needle, haystack, start, middle - 1);
	}
	else {
		return 1;
	}
}
