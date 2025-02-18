add_library(pong_lib
    Ball.cpp
    Engine.cpp
    Game.cpp
    Paddle.cpp
    ScoreManager.cpp
)

target_include_directories(pong_lib
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${PROJECT_SOURCE_DIR}/external/raylib/src
        ${PROJECT_SOURCE_DIR}/external/eigen/
)

target_link_libraries(pong_lib
    PUBLIC raylib
)

# Add compile options
if(MSVC)
    target_compile_options(pong_lib PRIVATE /W4)
else()
    target_compile_options(pong_lib PRIVATE -Wall -Wextra -Wpedantic)
endif()
