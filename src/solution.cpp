#include "aoc/solution.hpp"

#include <iterator>
#include <sstream>
#include <stdexcept>
#include <utility>

namespace aoc {

Solution run_parts(std::istream& input, PartSolver part1, PartSolver part2) {
    if (part1 == nullptr || part2 == nullptr) {
        throw std::invalid_argument("Both puzzle parts must have a solver.");
    }

    std::string input_contents{std::istreambuf_iterator<char>(input), std::istreambuf_iterator<char>()};
    std::istringstream part_input{std::move(input_contents)};

    auto part1_result = part1(part_input);
    part_input.clear();
    part_input.seekg(0);
    if (!part_input) {
        throw std::runtime_error("Could not rewind input before running part 2.");
    }

    return {std::move(part1_result), part2(part_input)};
}

}  // namespace aoc
