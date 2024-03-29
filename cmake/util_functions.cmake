function(set_compiler_options target)
    set_property(TARGET ${target} PROPERTY C_STANDARD 90)
    set_property(TARGET ${target} PROPERTY C_STANDARD_REQUIRED ON)
    
    if ( MSVC )
        target_compile_definitions(${target} PUBLIC _CRT_SECURE_NO_WARNINGS)
        target_compile_options(${target} PUBLIC /W4 /WX)
    else()
        target_compile_options(${target} PUBLIC -Wall -Werror -pedantic)
    endif()

endfunction()

function(add_test_executable name sources)
    add_executable(${name} ${sources})
    set_compiler_options(${name})
    target_link_libraries(${name} PUBLIC ctest)

    set_target_properties (${name} PROPERTIES FOLDER "Tests")

    add_test (NAME ${name} COMMAND ${name})

endfunction()