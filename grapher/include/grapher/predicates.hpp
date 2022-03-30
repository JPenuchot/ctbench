#pragma once

#include <functional>

#include <nlohmann/json.hpp>

namespace grapher {

/// \defgroup predicates Predicates
/// Predicates for group descriptors.

/// \ingroup predicates
/// Dynamic representation of a predicate.
using predicate_t = std::function<bool(nlohmann::json const &)>;

/// \ingroup predicates
/// Builds predicate and stores it in an std::function object.
predicate_t get_predicate(nlohmann::json const &constraint);

} // namespace grapher
