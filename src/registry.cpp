#include "aoc/registry.hpp"

namespace aoc {

Registry& Registry::instance() {
    static Registry registry;
    return registry;
}

void Registry::add(unsigned day, Solver solver) {
    if (day < 1 || day > 25) {
        throw std::invalid_argument("Day number must be between 1 and 25.");
    }
    if (solver == nullptr) {
        throw std::invalid_argument("Cannot register an empty solver.");
    }
    if (!solvers_.emplace(day, solver).second) {
        throw std::logic_error("A solver is already registered for this day.");
    }
}

Solver Registry::find(unsigned day) const {
    const auto it = solvers_.find(day);
    return it == solvers_.end() ? nullptr : it->second;
}

Registrar::Registrar(unsigned day, Solver solver) {
    Registry::instance().add(day, solver);
}

}  // namespace aoc
