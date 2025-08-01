function(stz_add_test STZ_TEST_NAME)
    add_executable(${STZ_TEST_NAME} ${STZ_TEST_NAME}.cpp backward.cpp)
    target_link_libraries(
        ${STZ_TEST_NAME}
            PRIVATE stz
            PUBLIC unwind bfd dl)
    add_test(NAME ${STZ_TEST_NAME} COMMAND ${STZ_TEST_NAME})
endfunction()

stz_add_test(test_01_inheritence)
stz_add_test(test_02_boundscheck)
stz_add_test(test_03_allocation)
stz_add_test(test_04_lssb)
stz_add_test(test_05_arena)
stz_add_test(test_06_list)
stz_add_test(test_07_slice)
stz_add_test(test_08_str)
stz_add_test(test_09_dict)

stz_add_test(test_util_01_file)
