cmake_minimum_required(VERSION 3.15)

find_package(GTest REQUIRED)


add_library(test_main STATIC test_main.cpp)
target_link_libraries(test_main PUBLIC GTest::GTest)

add_subdirectory(dqn)
add_subdirectory(game)

enable_testing()