#pragma once

#include <experimental/source_location>
#include <optional>
#include <string>
#include <type_traits>

#include <llvm/Support/raw_ostream.h>

#include <fmt/core.h>

#include "grapher/core.hpp"
#include "grapher/utils/config.hpp"
#include "grapher/utils/error.hpp"

namespace grapher {

/// For each iteration in entry, returns the sum of the values pointed by
/// value_jptr in the events matching the descriptor's predicates.
std::vector<double> get_values(benchmark_iteration_t const &iteration,
                               std::vector<predicate_t> const &predicates,
                               grapher::json_t::json_pointer value_jptr);

/// Merges the contents of b into a, with items of a being overwritten if items
/// present in b share the same key.
grapher::json_t merge_into(grapher::json_t a, grapher::json_t const &b);

/// Wraps value access with type checking and error reporting.
template <typename ReturnType, typename LocType>
inline ReturnType
json_value(grapher::json_t const &object, LocType const &field_location,
           const std::experimental::source_location loc =
               std::experimental::source_location::current()) {

  using ValueType = std::decay_t<ReturnType>;

  std::string const field_location_str = field_location;

  check(object.contains(field_location),
        fmt::format("Empty field {}:\n{}", field_location_str, object.dump(2)),
        error_v, -1, loc);

  if constexpr (std::is_same<grapher::json_t::string_t, ValueType>::value) {
    check(object[field_location].is_string(),
          fmt::format("Invalid field {}, expected string:\n{}",
                      field_location_str, object.dump(2)),
          error_v, -1, loc);

    return object[field_location];
  }

  else if constexpr (std::is_same<grapher::json_t::array_t, ValueType>::value) {
    check(object[field_location].is_array(),
          fmt::format("Invalid field {}, expected array:\n{}",
                      field_location_str, object.dump(2)),
          error_v, -1, loc);

    return object[field_location];
  }

  else if constexpr (std::is_same<grapher::json_t::boolean_t,
                                  ValueType>::value) {
    check(object.contains(field_location) &&
              object[field_location].is_boolean(),
          fmt::format("Invalid field {}, expected bool:\n{}",
                      field_location_str, object.dump(2)),
          error_v, -1, loc);

    return object[field_location];
  }

  else if constexpr (std::is_same<grapher::json_t::number_integer_t,
                                  ValueType>::value) {
    check(object[field_location].is_number(),
          fmt::format("Invalid field {}, expected number:\n{}",
                      field_location_str, object.dump(2)),
          error_v, -1, loc);

    return object[field_location];
  }

  // double
  else if constexpr (std::is_same<grapher::json_t::number_float_t,
                                  ValueType>::value) {
    check(object[field_location].is_number(),
          fmt::format("Invalid field {}, expected number:\n{}",
                      field_location_str, object.dump(2)),
          error_v, -1, loc);

    return object[field_location];
  }

  // Anything else
  else {
    return object[field_location];
  }
}

} // namespace grapher
