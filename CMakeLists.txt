cmake_minimum_required(VERSION 3.14)
project(SudokuSolver LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_options(-Wall -Wextra -Wpedantic)

add_executable(BasicSudoku BasicSudoku.cpp)
add_executable(BreadthFSSudoku BreadthFSSudoku.cpp)
add_executable(BestFSSudoku BestFSSudoku.cpp)

enable_testing()
add_test(NAME BasicSudoku COMMAND BasicSudoku)
add_test(NAME BreadthFSSudoku COMMAND BreadthFSSudoku)
add_test(NAME BestFSSudoku COMMAND BestFSSudoku)

# Google Test
include(FetchContent)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.14.0
)
FetchContent_MakeAvailable(googletest)

add_executable(SudokuTests tests/SudokuTests.cpp)
target_link_libraries(SudokuTests GTest::gtest_main)
target_include_directories(SudokuTests PRIVATE ${CMAKE_SOURCE_DIR})

include(GoogleTest)
gtest_discover_tests(SudokuTests)
