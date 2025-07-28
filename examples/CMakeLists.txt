function(add_example_stz TNAME FNAME)
    add_executable(example_${TNAME} ${FNAME}/example_${TNAME}.cpp)
    target_link_libraries(example_${TNAME} PRIVATE stz)
endfunction()

add_example_stz(range_1 definitions)
add_example_stz(log_1 definitions)

# add_example_stz(arena, fundamentals)
# add_example_stz(str, fundamentals)
# add_example_stz(slice, fundamentals)

# add_test_stz(map)
# add_test_stz(trie)
# add_test_stz(ltrie)
# add_test_stz(table)

# add_test_util(file)
# add_test_util(cmd)
# add_test_util(gui)
# add_test_util(path)
# add_test_util(db)
