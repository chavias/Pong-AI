set(TEST_NAME "test_dqn")

add_executable(${TEST_NAME}
    activations.test.cpp
    Agent.test.cpp
    DQN.test.cpp
    Memory.test.cpp
    Training.test.cpp
)

target_link_libraries(${TEST_NAME}
    PRIVATE test_main
    PRIVATE dqn_lib
    PRIVATE pong_lib
    PRIVATE GTest::GTest
    PRIVATE GTest::Main
)


# Set include directories explicitly for the test executable
target_include_directories(${TEST_NAME}
    PRIVATE
        ${PROJECT_SOURCE_DIR}/src
        ${PROJECT_SOURCE_DIR}/src/dqn/
)

# Add test to CTest
add_test(
    NAME ${TEST_NAME}
    COMMAND ${TEST_NAME}
)