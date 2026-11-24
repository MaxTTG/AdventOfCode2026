cmake_minimum_required(VERSION 3.20)

project(AdventOfCode2026
    VERSION 1.0.0
    DESCRIPTION "Advent of Code 2026 solutions"
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_subdirectory(helpers)

set(AOC_CORE_SOURCES
    src/registry.cpp
    src/solution.cpp
    include/aoc/registry.hpp
    include/aoc/solution.hpp
)
add_library(aoc_core STATIC ${AOC_CORE_SOURCES})
target_include_directories(aoc_core PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

set(AOC_DAY_SOURCES
    # BEGIN AOC DAY SOURCES
    days/day_01.cpp
    # END AOC DAY SOURCES
)

source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES
    ${AOC_CORE_SOURCES}
    src/main.cpp
    ${AOC_DAY_SOURCES}
    tests/helpers_tests.cpp
    tests/solution_tests.cpp
)

add_executable(aoc2026
    src/main.cpp
    ${AOC_DAY_SOURCES}
)
target_link_libraries(aoc2026 PRIVATE aoc_core aoc::helpers)

include(CTest)
if(BUILD_TESTING)
    add_executable(aoc_helpers_tests tests/helpers_tests.cpp)
    target_link_libraries(aoc_helpers_tests PRIVATE aoc::helpers)
    add_test(NAME aoc_helpers_tests COMMAND aoc_helpers_tests)

    add_executable(aoc_solution_tests tests/solution_tests.cpp)
    target_link_libraries(aoc_solution_tests PRIVATE aoc_core)
    add_test(NAME aoc_solution_tests COMMAND aoc_solution_tests)
endif()

if(MSVC)
    target_compile_options(aoc_core PRIVATE /W4 /permissive-)
    target_compile_options(aoc2026 PRIVATE /W4 /permissive-)
    if(BUILD_TESTING)
        target_compile_options(aoc_helpers_tests PRIVATE /W4 /permissive-)
        target_compile_options(aoc_solution_tests PRIVATE /W4 /permissive-)
    endif()
else()
    target_compile_options(aoc_core PRIVATE -Wall -Wextra -Wpedantic)
    target_compile_options(aoc2026 PRIVATE -Wall -Wextra -Wpedantic)
    if(BUILD_TESTING)
        target_compile_options(aoc_helpers_tests PRIVATE -Wall -Wextra -Wpedantic)
        target_compile_options(aoc_solution_tests PRIVATE -Wall -Wextra -Wpedantic)
    endif()
endif()
