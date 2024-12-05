cmake_minimum_required(VERSION 3.14.4)

set(CMAKE_CXX_STANDARD 20)

project(pong_dqn VERSION 1.0)

option(TEST_DQN "Test DQN" ON)

# add_executable(${PROJECT_NAME} ${CMAKE_SOURCE_DIR}/src/pong/main.cpp)
set(SOURCE_FILES 
    src/pong/Ball.cpp 
    src/pong/Paddle.cpp 
    src/pong/Game.cpp 
    src/pong/main.cpp
    src/pong/ScoreManager.cpp)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})


# Download all submodules
find_package(Git QUIET)
if(GIT_FOUND AND EXISTS "${PROJECT_SOURCE_DIR}/.git")
    # Update submodules as needed
    option(GIT_SUBMODULE "Check submodules during build" ON)
    if(GIT_SUBMODULE)
        message(STATUS "Submodule update")
        execute_process(COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive
                        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} 
                        RESULT_VARIABLE GIT_SUBMOD_RESULT)
        if(NOT GIT_SUBMOD_RESULT EQUAL "0")
            message(FATAL_ERROR "git submodule update --init failed with ${GIT_SUBMOD_RESULT}")
        endif()
    endif()
endif()
# Check all submodules
if(NOT EXISTS "${PROJECT_SOURCE_DIR}/external/raylib/CMakeLists.txt")
    message(FATAL_ERROR "The raylib submodule was not downloaded! GIT_SUBMODULE was turned off or failed")
endif()



find_package(OpenMP REQUIRED)

# target_link_libraries(solver PRIVATE OpenMP::OpenMP_CXX)



add_subdirectory(external/raylib)

target_include_directories(${PROJECT_NAME}
            PUBLIC ${PROJECT_SOURCE_DIR}/external/raylib/src/
            PUBLIC ${PROJECT_SOURCE_DIR}/external/eigen/
)

target_link_directories(${PROJECT_NAME}
            PRIVATE ${PROJECT_SOURCE_DIR}/external/raylib/src/
)

target_link_libraries(${PROJECT_NAME}
                        PRIVATE raylib
                        PRIVATE OpenMP::OpenMP_CXX)

if (TEST_DQN)
    # Enable testing
    include(CTest)
    enable_testing()      
    add_subdirectory(tests)
    # Ensure the tests also have profiling and debugging enabled
endif()
