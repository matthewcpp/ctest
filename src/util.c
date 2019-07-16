#include "ctest/internal/util.h"

#include <stdlib.h>
#include <string.h>

char *_str_cpy(const char *src) {
    size_t len = strlen(src) + 1;
    char *s = malloc(len);
    if (s == NULL)
        return NULL;
    return (char *)memcpy(s, src, len);
}

int _cutil_testing_string_cmp(const void* a, const void* b) {
    return strcmp(*(const char**)a, *(const char**)b);
}
