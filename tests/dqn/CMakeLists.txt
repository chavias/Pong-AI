# Find Google Test
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

# Add test executable
add_executable(test_dqn
    activations.test.cpp # Add test source files here
    test_main.cpp 
)

target_sources(test_dqn PRIVATE ${PROJECT_SOURCE_DIR}/src/dqn/activations.cpp)

# Include Eigen library
target_include_directories(test_dqn
    PRIVATE ${PROJECT_SOURCE_DIR}/external/eigen
)

# Link the test executable with the main library and GTest
target_link_libraries(test_dqn
    ${GTEST_LIBRARIES}
    pthread
)

# Register the test executable with CTest
add_test(NAME TestDQN COMMAND test_dqn)


