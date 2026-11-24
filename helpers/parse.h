#pragma once

#include <array>
#include <cctype>
#include <charconv>
#include <concepts>
#include <cstddef>
#include <istream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <string_view>
#include <system_error>
#include <tuple>
#include <vector>

#include "helpers/input.h"

namespace aoc::helpers {
namespace detail {

inline std::string_view trim(std::string_view text) {
    while (!text.empty() && std::isspace(static_cast<unsigned char>(text.front()))) {
        text.remove_prefix(1);
    }
    while (!text.empty() && std::isspace(static_cast<unsigned char>(text.back()))) {
        text.remove_suffix(1);
    }
    return text;
}

template <typename Visitor>
std::size_t visit_fields(std::string_view line, char delimiter, Visitor&& visitor) {
    std::size_t field_count = 0;
    const auto visit        = [&](std::string_view field) {
        visitor(field, field_count);
        ++field_count;
    };

    if (delimiter == '\0') {
        std::size_t position = 0;
        while (position < line.size()) {
            while (position < line.size() && std::isspace(static_cast<unsigned char>(line[position])) != 0) {
                ++position;
            }
            const auto start = position;
            while (position < line.size() && std::isspace(static_cast<unsigned char>(line[position])) == 0) {
                ++position;
            }
            if (start < position) {
                visit(line.substr(start, position - start));
            }
        }
        return field_count;
    }

    std::size_t start = 0;
    while (true) {
        const auto end = line.find(delimiter, start);
        visit(trim(line.substr(start, end - start)));
        if (end == std::string_view::npos) {
            break;
        }
        start = end + 1;
    }
    return field_count;
}

template <typename T>
T parse_field(std::string_view text, std::size_t line_number, std::size_t field_number) {
    if constexpr (std::same_as<T, std::string>) {
        return std::string(text);
    } else if constexpr (std::same_as<T, char>) {
        if (text.size() != 1) {
            throw std::invalid_argument("Line " + std::to_string(line_number) + ", field " +
                                        std::to_string(field_number) + ": expected one character.");
        }
        return text.front();
    } else if constexpr (std::integral<T> && !std::same_as<T, bool>) {
        const auto original_text = text;
        if (!text.empty() && text.front() == '+') {
            text.remove_prefix(1);
        }

        T value{};
        const auto result = std::from_chars(text.data(), text.data() + text.size(), value);
        if (result.ec != std::errc{} || result.ptr != text.data() + text.size()) {
            throw std::invalid_argument("Line " + std::to_string(line_number) + ", field " +
                                        std::to_string(field_number) + ": invalid integer '" +
                                        std::string(original_text) + "'.");
        }
        return value;
    } else {
        std::istringstream field{std::string(text)};
        T value{};
        if (!(field >> value)) {
            throw std::invalid_argument("Line " + std::to_string(line_number) + ", field " +
                                        std::to_string(field_number) + ": invalid value '" + std::string(text) + "'.");
        }

        field >> std::ws;
        if (!field.eof()) {
            throw std::invalid_argument("Line " + std::to_string(line_number) + ", field " +
                                        std::to_string(field_number) + ": unexpected characters in '" +
                                        std::string(text) + "'.");
        }
        return value;
    }
}

template <typename... Types, std::size_t... Indices>
std::tuple<Types...> make_tuple(const std::array<std::string_view, sizeof...(Types)>& fields, std::size_t line_number,
                                std::index_sequence<Indices...>) {
    return {parse_field<Types>(fields[Indices], line_number, Indices + 1)...};
}

}  // namespace detail

// A '\0' delimiter means whitespace-separated fields.
template <typename... Types>
[[nodiscard]] std::vector<std::tuple<Types...>> parse_lines(std::istream& input, char delimiter = '\0') {
    static_assert(sizeof...(Types) > 0, "parse_lines requires at least one field type.");

    std::vector<std::tuple<Types...>> rows;
    std::string line;
    std::size_t line_number = 0;
    while (detail::read_normalized_line(input, line)) {
        ++line_number;
        if (detail::is_blank_line(line)) {
            continue;
        }

        std::array<std::string_view, sizeof...(Types)> fields{};
        const auto field_count = detail::visit_fields(line, delimiter, [&](std::string_view field, std::size_t index) {
            if (index < fields.size()) {
                fields[index] = field;
            }
        });
        if (field_count != fields.size()) {
            throw std::invalid_argument("Line " + std::to_string(line_number) + ": expected " +
                                        std::to_string(sizeof...(Types)) + " fields, got " +
                                        std::to_string(field_count) + ".");
        }
        rows.push_back(detail::make_tuple<Types...>(fields, line_number, std::index_sequence_for<Types...>{}));
    }
    return rows;
}

template <typename T>
[[nodiscard]] std::vector<T> parse_values(std::istream& input, char delimiter = '\0') {
    std::vector<T> values;
    std::string line;
    std::size_t line_number = 0;
    while (detail::read_normalized_line(input, line)) {
        ++line_number;
        if (detail::is_blank_line(line)) {
            continue;
        }

        std::string_view value;
        const auto field_count = detail::visit_fields(line, delimiter, [&](std::string_view field, std::size_t index) {
            if (index == 0) {
                value = field;
            }
        });
        if (field_count != 1) {
            throw std::invalid_argument("Line " + std::to_string(line_number) + ": expected one field, got " +
                                        std::to_string(field_count) + ".");
        }
        values.push_back(detail::parse_field<T>(value, line_number, 1));
    }
    return values;
}

template <typename T>
[[nodiscard]] std::vector<std::vector<T>> parse_grid(std::istream& input, char delimiter = '\0') {
    std::vector<std::vector<T>> grid;
    std::string line;
    std::size_t line_number = 0;
    while (detail::read_normalized_line(input, line)) {
        ++line_number;
        if (detail::is_blank_line(line)) {
            continue;
        }

        auto& row              = grid.emplace_back();
        const auto field_count = detail::visit_fields(line, delimiter, [&](std::string_view field, std::size_t index) {
            row.push_back(detail::parse_field<T>(field, line_number, index + 1));
        });
        if (grid.size() > 1 && field_count != grid.front().size()) {
            throw std::invalid_argument("Line " + std::to_string(line_number) +
                                        ": grid rows must have the same number of fields.");
        }
    }
    return grid;
}

template <std::integral T>
[[nodiscard]] std::vector<T> extract_integers(std::istream& input) {
    static_assert(!std::same_as<T, bool>, "extract_integers does not support bool.");

    std::vector<T> values;
    std::string line;
    std::size_t line_number = 0;
    while (detail::read_normalized_line(input, line)) {
        ++line_number;
        std::size_t match_number = 0;
        std::size_t position     = 0;
        while (position < line.size()) {
            const auto character = static_cast<unsigned char>(line[position]);
            const bool has_sign  = (line[position] == '+' || line[position] == '-') && position + 1 < line.size() &&
                                  std::isdigit(static_cast<unsigned char>(line[position + 1])) != 0;
            if (std::isdigit(character) == 0 && !has_sign) {
                ++position;
                continue;
            }

            const auto start = position;
            if (has_sign) {
                ++position;
            }
            while (position < line.size() && std::isdigit(static_cast<unsigned char>(line[position])) != 0) {
                ++position;
            }

            values.push_back(detail::parse_field<T>(std::string_view(line).substr(start, position - start), line_number,
                                                    ++match_number));
        }
    }
    return values;
}

}  // namespace aoc::helpers
