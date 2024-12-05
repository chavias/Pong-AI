# Find Google Test
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

# Add test executable
add_executable(test_game
    test_main.cpp 
)

find_package(OpenMP REQUIRED)


# add test sources
target_sources(test_game
PRIVATE ${PROJECT_SOURCE_DIR}/src/pong/Game.cpp
PRIVATE ${PROJECT_SOURCE_DIR}/src/pong/Ball.cpp
PRIVATE ${PROJECT_SOURCE_DIR}/src/pong/Paddle.cpp
PRIVATE ${PROJECT_SOURCE_DIR}/src/pong/ScoreManager.cpp
PRIVATE Game.test.cpp    
)

# Include Eigen library
target_include_directories(test_game
PRIVATE ${PROJECT_SOURCE_DIR}/src/pong
PRIVATE ${PROJECT_SOURCE_DIR}/external/eigen
PRIVATE ${PROJECT_SOURCE_DIR}/external/raylib/src/
)


target_link_directories(test_game
PRIVATE ${PROJECT_SOURCE_DIR}/external/raylib/src/
)

# Link the test executable with the main library and GTest
target_link_libraries(test_game
    ${GTEST_LIBRARIES}
    pthread
    raylib
    OpenMP::OpenMP_CXX
)

# Register the test executable with CTest
add_test(NAME TestDQN COMMAND test_game)


