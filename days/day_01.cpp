#include <istream>

#include "aoc/registry.hpp"
#include "aoc/solution.hpp"

namespace {

std::string part1(std::istream& input) {
    (void)input;
    return "TODO";
}

std::string part2(std::istream& input) {
    (void)input;
    return "TODO";
}

aoc::Solution solve_day_01(std::istream& input) {
    return aoc::run_parts(input, part1, part2);
}

AOC_REGISTER_DAY(1, solve_day_01)

}  // namespace
