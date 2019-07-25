include (CTest)

add_executable(test_predicates test_predicates.c)
set_compiler_options(test_predicates)
target_link_libraries(test_predicates PUBLIC ctest)

set_target_properties (test_predicates PROPERTIES FOLDER "Tests")

add_test (NAME test_predicates COMMAND test_predicates)
