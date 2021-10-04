#pragma once

#include <functional>

#include <nlohmann/json.hpp>

namespace grapher {

using predicate_t = std::function<bool(nlohmann::json const &)>;

/// Builds predicate and stores it in an std::function object.
predicate_t get_predicate(nlohmann::json const &constraint);

} // namespace grapher
