#include "ctest/internal/util.h"

#include <stdlib.h>
#include <string.h>

char* ctest_util_strdup(const char *src) {
    size_t len = strlen(src) + 1;
    char *s = malloc(len);
    if (s == NULL)
        return NULL;
    return (char *)memcpy(s, src, len);
}

int ctest_util_str_cmp(const void* a, const void* b) {
    return strcmp(*(const char**)a, *(const char**)b);
}
