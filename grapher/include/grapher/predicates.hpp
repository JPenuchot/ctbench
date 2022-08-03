#pragma once

#include <grapher/core.hpp>

#include <functional>

#include <nlohmann/json.hpp>

namespace grapher {

/// \defgroup predicates Predicates
/// Predicates for group descriptors.

/// \ingroup predicates
/// Dynamic representation of a predicate.
using predicate_t = std::function<bool(grapher::json_t const &)>;

/// \ingroup predicates
/// Builds predicate and stores it in an std::function object.
predicate_t get_predicate(grapher::json_t const &constraint);

} // namespace grapher
