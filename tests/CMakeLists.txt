
add_executable(test_arena test_arena.cpp)
target_link_libraries(test_arena PRIVATE stz)

add_executable(test_arena_save_load test_arena_save_load.cpp)
target_link_libraries(test_arena_save_load PRIVATE stz)

add_executable(test_buf test_buf.cpp)
target_link_libraries(test_buf PRIVATE stz)

add_executable(test_cmd test_cmd.cpp)
target_link_libraries(test_cmd PRIVATE stz)

add_executable(test_file test_file.cpp)
target_link_libraries(test_file PRIVATE stz)

add_executable(test_gui test_gui.cpp)
target_link_libraries(test_gui PRIVATE stz)

add_executable(test_index test_index.cpp)
target_link_libraries(test_index PRIVATE stz)

add_executable(test_map test_map.cpp)
target_link_libraries(test_map PRIVATE stz)

add_executable(test_str test_str.cpp)
target_link_libraries(test_str PRIVATE stz)

add_executable(test_slice test_slice.cpp)
target_link_libraries(test_slice PRIVATE stz)
