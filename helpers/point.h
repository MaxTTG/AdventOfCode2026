#pragma once

namespace aoc::helpers {

struct Point {
    int x{};
    int y{};

    friend constexpr Point operator+(Point left, Point right) { return {left.x + right.x, left.y + right.y}; }

    friend constexpr bool operator==(Point, Point) = default;
};

}  // namespace aoc::helpers
