add_library(aoc_helpers INTERFACE)

set(AOC_HELPERS_HEADERS
    "${CMAKE_CURRENT_SOURCE_DIR}/input.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/parse.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/point.h"
)

target_include_directories(aoc_helpers INTERFACE
    "${PROJECT_SOURCE_DIR}"
)

target_sources(aoc_helpers INTERFACE ${AOC_HELPERS_HEADERS})
source_group(TREE "${PROJECT_SOURCE_DIR}" FILES ${AOC_HELPERS_HEADERS})

add_library(aoc::helpers ALIAS aoc_helpers)
