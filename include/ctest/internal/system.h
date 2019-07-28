#ifndef CTEST_SYSTEM_H
#define CTEST_SYSTEM_H

#include "ctest/internal/test.h"
#include "ctest/internal/filter.h"
#include "ctest/internal/runner.h"

typedef struct {
    _ctest_unit_test* unit_test_front;
    _ctest_unit_test* unit_test_back;
    size_t _unit_test_count;

	_ctest_filter* filter;
	_ctest_runner* runner;
} ctest_system;

void ctest_system_init(ctest_system* test_system);
void ctest_system_destroy(ctest_system* test_system);

void ctest_system_add_test(ctest_system* test_system, _ctest_unit_test* test_entry);

#endif
