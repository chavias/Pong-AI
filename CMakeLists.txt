cmake_minimum_required(VERSION 3.14.4)

set(CMAKE_CXX_STANDARD 20)

project(pong_dqn VERSION 1.0)

option(BUILD_TESTS "BUILD_TESTS" ON)

find_package(OpenMP REQUIRED)


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

# Add external libraries
add_subdirectory(external/raylib)

# Add our libraries
add_subdirectory(src/dqn)
add_subdirectory(src/pong)

include_directories(${PROJECT_NAME} PUBLIC
    ${PROJECT_SOURCE_DIR}/external/raylib/src/
)

# Add the main executable
add_executable(${PROJECT_NAME} src/main.cpp)
target_link_libraries(${PROJECT_NAME}
    PRIVATE dqn_lib
    PRIVATE pong_lib
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE raylib
    PRIVATE pthread
    PRIVATE OpenMP::OpenMP_CXX
)

if (BUILD_TESTS)
    # Enable testing
    include(CTest) 
    add_subdirectory(tests)
endif()
