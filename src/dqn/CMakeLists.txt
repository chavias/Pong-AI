# # Find GoogleTest package
# find_package(GTest REQUIRED)

# # Add the test executable
# add_executable(activations_test activations.test.cpp activations.cpp)

# # Include the directory containing activations.hpp
# target_include_directories(activations_test PUBLIC ${PROJECT_SOURCE_DIR}/src/dqn)

# # Link Google Test
# target_link_libraries(activations_test PRIVATE gtest gtest_main)

# # Add the test to CTest
# add_test(NAME ActivationsTest COMMAND activations_test)
