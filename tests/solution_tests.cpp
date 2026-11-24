#include <iostream>
#include <iterator>
#include <sstream>
#include <string>

#include "aoc/solution.hpp"

namespace {

std::string read_all(std::istream& input) {
    return {std::istreambuf_iterator<char>(input), std::istreambuf_iterator<char>()};
}

std::string solve_part1(std::istream& input) {
    return read_all(input) + "|part1";
}

std::string solve_part2(std::istream& input) {
    return read_all(input) + "|part2";
}

}  // namespace

int main() {
    std::istringstream input{"shared puzzle input"};
    const auto solution = aoc::run_parts(input, solve_part1, solve_part2);
    if (solution.part1 != "shared puzzle input|part1" || solution.part2 != "shared puzzle input|part2") {
        std::cerr << "Each part must receive the full input and return its own result.\n";
        return 1;
    }
    return 0;
}
