#pragma once

#include <iosfwd>
#include <string>

namespace aoc {

using PartSolver = std::string (*)(std::istream&);

struct Solution {
    std::string part1;
    std::string part2;
};

[[nodiscard]] Solution run_parts(std::istream& input, PartSolver part1, PartSolver part2);

}  // namespace aoc
