add_executable(unit_tests test_main.cpp)
target_link_libraries(unit_tests PRIVATE MahjongLib)
add_test(
    NAME unit_tests
    COMMAND $<TARGET_FILE:unit_tests>
)
