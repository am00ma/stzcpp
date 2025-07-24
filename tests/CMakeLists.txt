function(add_test_stz TNAME)
    add_executable(test_${TNAME} test_${TNAME}.cpp)
    target_link_libraries(test_${TNAME} PRIVATE stz)
endfunction()

function(add_test_util TNAME)
    add_executable(test_${TNAME} util/test_${TNAME}.cpp)
    target_link_libraries(test_${TNAME} PRIVATE stz)
endfunction()


add_test_stz(arena)
add_test_stz(str)
add_test_stz(buf)
add_test_stz(slice)
add_test_stz(map)
add_test_stz(trie)
add_test_stz(ltrie)

add_test_util(file)
add_test_util(cmd)
add_test_util(gui)
