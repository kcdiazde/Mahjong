macro(add_mahjong_test TEST)
    add_executable(${TEST} ${TEST}.cpp)
    target_link_libraries(${TEST} PRIVATE MahjongLib)
    target_include_directories(${TEST} PRIVATE ${PROJECT_SOURCE_DIR}/src)
    add_test( NAME ${TEST} COMMAND $<TARGET_FILE:${TEST}>)
endmacro()

add_mahjong_test(test_tiles)
add_mahjong_test(test_player)
