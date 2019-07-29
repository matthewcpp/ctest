#include "ctest/internal/filter.h"
#include "ctest/internal/util.h"

#include <string.h>
#include <stdlib.h>

char** _ctest_filter_split_filter_string(const char* str, int* count);
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
		char* filter_str_cpy = ctest_util_strdup(filter_str);
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
	char* search_str = ctest_util_strdup(str);

	char *token = strtok(search_str, ";");
	while (token) {
		tokens[token_count] = ctest_util_strdup(token);

		token_count += 1;

		if (token_count == token_size) {
			token_size *= 2;
			tokens = realloc(tokens, sizeof(char*) * token_size);
		}

		token = strtok(NULL, ";");
	}

	free(search_str);

	*count = token_count;
	return tokens;
}

int ctest_filter_should_run_test(ctest_filter* filter, const char* test_name) {
	if (filter->filters) {
		int i;

		for (i = 0; i < filter->filter_count; i++) {
			const char* filter_str = filter->filters[i];
			const char* result = strstr(test_name, filter_str);

			if (result == test_name) {
				return 1;
			}
		}

		return 0;
	}
	else {
		return 1;
	}
}

