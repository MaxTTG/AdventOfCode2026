# Advent of Code 2026

A small, extensible C++20 template for solving Advent of Code puzzles. Each
day lives in its own source file and registers its solver with the application.
Reusable helpers are provided by the header-only `aoc::helpers` library in
`helpers/`.

## Input helpers

`helpers/parse.h` provides typed, row-by-row parsing. Template arguments
define the field types and their order; whitespace is the default separator.
For comma-separated records, pass the delimiter explicitly:

```cpp
#include "helpers/parse.h"

auto records = aoc::helpers::parse_lines<int, std::string>(input, ',');
```

Additional helpers cover common puzzle formats:

- `parse_values<T>(input)` for one value per line
- `parse_grid<T>(input)` for rectangular, token-separated grids
- `extract_integers<T>(input)` for numbers embedded in free-form text
- `read_lines(input)` for text or character grids
- `read_digit_grid(input)` for compact digit grids such as `123`
- `read_sections(input)` for groups separated by blank lines

Blank lines are skipped by the row and grid parsers. Use `read_sections` when
blank lines divide the input into meaningful groups. Invalid field counts,
values, or grid shapes produce an exception with the line number.

## Requirements

- CMake 3.20 or newer
- A C++20-compatible compiler

## Build and run

```powershell
cmake -S . -B build
cmake --build build --config Release
```

Run a day from the repository root; the default input path is
`inputs/day_XX.txt`:

```powershell
.\build\Release\aoc2026.exe 1
```

On single-configuration generators, the executable is usually at
`build/aoc2026` (or `build/aoc2026.exe` on Windows).
An alternate input file can be supplied as the second argument:

```text
aoc2026 1 path/to/input.txt
```

## Add a day

From the repository root, run:

```powershell
.\scripts\new-day.ps1 -Day 2
```

The script creates `days/day_02.cpp` and `inputs/day_02.txt`, and adds the
source file to the CMake target. Re-run the CMake build command to configure
and build the new solver. Days must be between 1 and 25.

Each day implements `aoc::Solution solve_day_XX(std::istream&)` and registers
it with `AOC_REGISTER_DAY`. It also defines separate `part1` and `part2`
functions, each receiving the input stream and returning its answer as a
string. Use `aoc::run_parts` to run both against the same input; it rewinds
the input between calls. Keep puzzle-specific parsing and logic in that day's
source file; put only genuinely reusable code in the helper library.

## Project layout

```text
days/                   One source file per puzzle day
include/aoc/             Solver and registry interfaces
helpers/                 Header-only reusable helper library
inputs/                  Local puzzle inputs (not committed)
scripts/                 Developer scripts
src/                     CLI and registry implementation
```