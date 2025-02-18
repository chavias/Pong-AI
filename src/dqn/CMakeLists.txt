add_library(dqn_lib
    Agent.cpp
    DQN.cpp
    Memory.cpp
    Training.cpp
)

target_include_directories(dqn_lib
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${PROJECT_SOURCE_DIR}/external/raylib/src
        ${PROJECT_SOURCE_DIR}/external/eigen/
)

target_link_libraries(dqn_lib
    PUBLIC OpenMP::OpenMP_CXX
)

# Add compile options
if(MSVC)
    target_compile_options(dqn_lib PRIVATE /W4)
else()
    target_compile_options(dqn_lib PRIVATE -Wall -Wextra -Wpedantic)
endif()