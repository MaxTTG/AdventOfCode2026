#pragma once

#include <iosfwd>
#include <map>
#include <stdexcept>

#include "aoc/solution.hpp"

namespace aoc {

using Solver = Solution (*)(std::istream&);

class Registry {
public:
    static Registry& instance();

    void add(unsigned day, Solver solver);
    [[nodiscard]] Solver find(unsigned day) const;

private:
    std::map<unsigned, Solver> solvers_;
};

class Registrar {
public:
    Registrar(unsigned day, Solver solver);
};

}  // namespace aoc

#define AOC_REGISTER_DAY(day_number, solver_function)                               \
    namespace {                                                                     \
    const ::aoc::Registrar registrar_day_##day_number{day_number, solver_function}; \
    }
