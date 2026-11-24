#pragma once

#include <cctype>
#include <istream>
#include <stdexcept>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

namespace aoc::helpers {

namespace detail {

inline bool is_blank_line(std::string_view line) {
    for (const unsigned char character : line) {
        if (std::isspace(character) == 0) {
            return false;
        }
    }
    return true;
}

inline bool read_normalized_line(std::istream& input, std::string& line) {
    if (!std::getline(input, line)) {
        return false;
    }
    if (!line.empty() && line.back() == '\r') {
        line.pop_back();
    }
    return true;
}

}  // namespace detail

[[nodiscard]] inline std::vector<std::string> read_lines(std::istream& input) {
    std::vector<std::string> lines;
    std::string line;
    while (detail::read_normalized_line(input, line)) {
        lines.push_back(std::move(line));
    }
    return lines;
}

[[nodiscard]] inline std::vector<std::vector<std::string>> read_sections(std::istream& input) {
    std::vector<std::vector<std::string>> sections;
    std::vector<std::string> section;
    std::string line;
    while (detail::read_normalized_line(input, line)) {
        if (detail::is_blank_line(line)) {
            if (!section.empty()) {
                sections.push_back(std::move(section));
                section.clear();
            }
        } else {
            section.push_back(std::move(line));
        }
    }
    if (!section.empty()) {
        sections.push_back(std::move(section));
    }
    return sections;
}

[[nodiscard]] inline std::vector<std::vector<int>> read_digit_grid(std::istream& input) {
    std::vector<std::vector<int>> grid;
    std::string line;
    std::size_t line_number = 0;
    while (detail::read_normalized_line(input, line)) {
        ++line_number;
        if (line.empty()) {
            continue;
        }

        std::vector<int> row;
        row.reserve(line.size());
        for (const unsigned char character : line) {
            if (character < '0' || character > '9') {
                throw std::invalid_argument("Line " + std::to_string(line_number) +
                                            ": digit grid contains a non-digit character.");
            }
            row.push_back(character - '0');
        }
        if (!grid.empty() && row.size() != grid.front().size()) {
            throw std::invalid_argument("Line " + std::to_string(line_number) +
                                        ": digit grid rows must have the same width.");
        }
        grid.push_back(std::move(row));
    }
    return grid;
}

}  // namespace aoc::helpers
