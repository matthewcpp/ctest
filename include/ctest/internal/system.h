#ifndef CTEST_SYSTEM_H
#define CTEST_SYSTEM_H

#include "ctest/internal/test.h"
#include "ctest/internal/filter.h"
#include "ctest/internal/runner.h"

/* _ctest_suite top level object containing a collection of suites. */
typedef struct _ctest_system {
    _ctest_unit_test* _unit_test_front;
    _ctest_unit_test* _unit_test_back;
    size_t _unit_test_count;

	_ctest_filter* filter;
	_ctest_runner* runner;
} _ctest_system;

void _cutil_testing_system_init(_ctest_system* test_system);
void _cutil_testing_system_destroy(_ctest_system* test_system);

void ctest_system_add_test(_ctest_system* test_system, _ctest_unit_test* test_entry);

#endif
