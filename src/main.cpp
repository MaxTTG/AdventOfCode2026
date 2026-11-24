#include <charconv>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string_view>

#include "aoc/registry.hpp"

namespace {

void print_usage(std::ostream& output) {
    output << "Usage: aoc2026 <day 1-25> [input-file]\n";
}

bool parse_day(std::string_view text, unsigned& day) {
    const auto result = std::from_chars(text.data(), text.data() + text.size(), day);
    return result.ec == std::errc{} && result.ptr == text.data() + text.size() && day >= 1 && day <= 25;
}

}  // namespace

int main(int argc, char* argv[]) {
    if (argc == 2 && std::string_view(argv[1]) == "--help") {
        print_usage(std::cout);
        return 0;
    }
    if (argc < 2 || argc > 3) {
        print_usage(std::cerr);
        return 2;
    }

    unsigned day = 0;
    if (!parse_day(argv[1], day)) {
        std::cerr << "Invalid day: expected a number between 1 and 25.\n";
        print_usage(std::cerr);
        return 2;
    }

    const auto solver = aoc::Registry::instance().find(day);
    if (solver == nullptr) {
        std::cerr << "No solver is registered for day " << day << ".\n";
        return 1;
    }

    const auto input_path =
        argc == 3 ? std::filesystem::path(argv[2])
                  : std::filesystem::path("inputs") /
                        ("day_" + (day < 10 ? std::string("0") : std::string()) + std::to_string(day) + ".txt");
    std::ifstream input(input_path);
    if (!input) {
        std::cerr << "Could not open input file: " << input_path.string() << '\n';
        return 1;
    }

    const auto solution = solver(input);
    std::cout << "Part 1: " << solution.part1 << '\n' << "Part 2: " << solution.part2 << '\n';
    return 0;
}
