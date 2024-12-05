# Find Google Test
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

# Add test executable
add_executable(test_dqn
    test_main.cpp 
)

find_package(OpenMP REQUIRED)

# set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -march=native -funroll-loops")

# add test sources
target_sources(test_dqn
PRIVATE ${PROJECT_SOURCE_DIR}/src/dqn/Agent.cpp
PRIVATE ${PROJECT_SOURCE_DIR}/src/dqn/Memory.cpp
PRIVATE ${PROJECT_SOURCE_DIR}/src/dqn/DQN.cpp
PRIVATE ${PROJECT_SOURCE_DIR}/src/dqn/Training.cpp
PRIVATE ${PROJECT_SOURCE_DIR}/src/pong/Game.cpp
PRIVATE ${PROJECT_SOURCE_DIR}/src/pong/Ball.cpp
PRIVATE ${PROJECT_SOURCE_DIR}/src/pong/Paddle.cpp
PRIVATE ${PROJECT_SOURCE_DIR}/src/pong/ScoreManager.cpp
# PRIVATE activations.test.cpp    
# PRIVATE Agent.test.cpp
# PRIVATE Memory.test.cpp
# PRIVATE DQN.test.cpp
PRIVATE Training.test.cpp
)

# Include Eigen library
target_include_directories(test_dqn
PRIVATE ${PROJECT_SOURCE_DIR}/src/pong
PRIVATE ${PROJECT_SOURCE_DIR}/src/dqn
PRIVATE ${PROJECT_SOURCE_DIR}/external/eigen
PRIVATE ${PROJECT_SOURCE_DIR}/external/raylib/src/
)


target_link_directories(test_dqn
PRIVATE ${PROJECT_SOURCE_DIR}/external/raylib/src/
)

# target_link_libraries(test_dqn PRIVATE OpenMP::OpenMP_CXX)

# Link the test executable with the main library and GTest
target_link_libraries(test_dqn
    ${GTEST_LIBRARIES}
    pthread
    raylib
    OpenMP::OpenMP_CXX
)

# set_target_properties(test_dqn PROPERTIES COMPILE_OPTIONS "-pg")
# set_target_properties(test_dqn PROPERTIES LINK_OPTIONS "-pg")

# Register the test executable with CTest
add_test(NAME TestDQN COMMAND test_dqn)


