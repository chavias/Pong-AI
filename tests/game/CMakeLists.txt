set(TEST_NAME "test_pong")  # Changed from test_game to test_pong

add_executable(${TEST_NAME}
    Game.test.cpp
)

target_link_libraries(${TEST_NAME}
    PRIVATE pong_lib
    PRIVATE test_main
    PRIVATE GTest::GTest
    PRIVATE GTest::Main
)


# Set include directories explicitly for the test executable
target_include_directories(${TEST_NAME}
    PRIVATE
        ${PROJECT_SOURCE_DIR}/src/game/
)

# Add test to CTest
add_test(
    NAME ${TEST_NAME}
    COMMAND ${TEST_NAME}
)