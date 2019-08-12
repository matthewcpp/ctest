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
