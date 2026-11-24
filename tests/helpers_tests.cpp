#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <tuple>
#include <vector>

#include "helpers/input.h"
#include "helpers/parse.h"

namespace {

void require(bool condition, const char* message) {
    if (!condition) {
        throw std::runtime_error(message);
    }
}

template <typename Function>
void require_invalid_argument(Function&& function, const char* message) {
    try {
        function();
    } catch (const std::invalid_argument&) {
        return;
    }
    throw std::runtime_error(message);
}

}  // namespace

int main() {
    try {
        std::istringstream whitespace_rows("12 north\n34 south\n");
        const auto rows = aoc::helpers::parse_lines<int, std::string>(whitespace_rows);
        require(rows.size() == 2, "Expected two parsed rows.");
        require(std::get<0>(rows[0]) == 12, "Expected the first integer field.");
        require(std::get<1>(rows[1]) == "south", "Expected the second string field.");

        std::istringstream comma_rows("7, 2.5,ok\n");
        const auto comma_values = aoc::helpers::parse_lines<int, double, std::string>(comma_rows, ',');
        require(std::get<0>(comma_values[0]) == 7, "Expected an integer CSV field.");
        require(std::get<1>(comma_values[0]) == 2.5, "Expected a floating-point CSV field.");
        require(std::get<2>(comma_values[0]) == "ok", "Expected a string CSV field.");

        std::istringstream scalar_lines("-3\n9\n");
        const auto scalars = aoc::helpers::parse_values<int>(scalar_lines);
        require(scalars == std::vector<int>{-3, 9}, "Expected one value per line.");

        std::istringstream embedded_numbers("x=-12, y=+8\nmove 5 steps\n");
        const auto numbers = aoc::helpers::extract_integers<int>(embedded_numbers);
        require(numbers == std::vector<int>{-12, 8, 5}, "Expected integers extracted from text.");

        std::istringstream digit_rows("123\n456\n");
        const auto digit_grid = aoc::helpers::read_digit_grid(digit_rows);
        require(digit_grid.size() == 2, "Expected two digit-grid rows.");
        require(digit_grid[1] == std::vector<int>{4, 5, 6}, "Expected parsed grid digits.");

        std::istringstream token_rows("1 2\n3 4\n");
        const auto token_grid = aoc::helpers::parse_grid<int>(token_rows);
        require(token_grid[0] == std::vector<int>{1, 2}, "Expected a token-separated grid.");

        std::istringstream grouped("alpha\nbeta\n\n gamma \n\n");
        const auto sections = aoc::helpers::read_sections(grouped);
        require(sections.size() == 2, "Expected two sections.");
        require(sections[0].size() == 2, "Expected two lines in the first section.");

        std::istringstream malformed_row("1\n");
        require_invalid_argument([&] { (void)aoc::helpers::parse_lines<int, int>(malformed_row); },
                                 "Expected a field-count error.");

        std::istringstream malformed_grid("1 2\n3\n");
        require_invalid_argument([&] { (void)aoc::helpers::parse_grid<int>(malformed_grid); },
                                 "Expected a ragged-grid error.");
    } catch (const std::exception& error) {
        std::cerr << "Helper test failed: " << error.what() << '\n';
        return 1;
    }

    return 0;
}
