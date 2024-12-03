# Find Google Test
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

# Add test executable
add_executable(test_dqn
    test_main.cpp 
)

# add test sources
target_sources(test_dqn
    PRIVATE ${PROJECT_SOURCE_DIR}/src/dqn/activations.cpp
    PRIVATE ${PROJECT_SOURCE_DIR}/src/dqn/Agent.cpp
    PRIVATE ${PROJECT_SOURCE_DIR}/src/dqn/Memory.cpp
    PRIVATE activations.test.cpp    
    PRIVATE Agent.test.cpp
    PRIVATE Memory.test.cpp
    # PRIVATE Training.test.cpp
)

# Include Eigen library
target_include_directories(test_dqn
    PRIVATE ${PROJECT_SOURCE_DIR}/src/pong
    PRIVATE ${PROJECT_SOURCE_DIR}/src/dqn
    PRIVATE ${PROJECT_SOURCE_DIR}/external/eigen
    PRIVATE ${PROJECT_SOURCE_DIR}/external/raylib/src/
)

# Link the test executable with the main library and GTest
target_link_libraries(test_dqn
    ${GTEST_LIBRARIES}
    pthread
)

# Register the test executable with CTest
add_test(NAME TestDQN COMMAND test_dqn)


