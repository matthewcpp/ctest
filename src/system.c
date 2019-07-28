#include "ctest/internal/system.h"

#include <stdlib.h>

void _cutil_testing_system_init(_ctest_system* test_system) {
    test_system->_unit_test_front = NULL;
    test_system->_unit_test_back = NULL;
    test_system->_unit_test_count = 0;
	test_system->filter = _ctest_filter_create();
	test_system->runner = _ctest_runner_create();
}

void _cutil_testing_system_destroy(_ctest_system* test_system) {
    _ctest_unit_test* current_test = test_system->_unit_test_front;

    while (current_test) {
        _ctest_unit_test* delete_ptr = current_test;
        current_test = current_test->next;

        _ctest_unit_test_destroy(delete_ptr);
    }

	_ctest_filter_destroy(test_system->filter);
	_ctest_runner_destroy(test_system->runner);

	free(test_system);
}

void ctest_system_add_test(_ctest_system* test_system, _ctest_unit_test* test_entry) {
	if (test_system->_unit_test_count == 0) {
		test_system->_unit_test_front = test_entry;
		test_system->_unit_test_back = test_entry;
	}
	else {
		test_system->_unit_test_back->next = test_entry;
		test_system->_unit_test_back = test_entry;
	}

	test_system->_unit_test_count += 1;
}
