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

/// Wraps json_t object access with error management.
template <typename LocType>
grapher::json_t::const_reference
json_at(grapher::json_t const &object, LocType const &field_location,
        const std::experimental::source_location loc =
            std::experimental::source_location::current()) {
  std::string const field_location_str = field_location;

  check(object.contains(field_location),
        fmt::format("Empty field {}:\n{}", field_location_str, object.dump(2)),
        error_v, -1, loc);

  return object[field_location];
}

/// Wraps reference access with type checking and error management.
template <typename ReferenceType, typename LocType>
inline ReferenceType
json_at_ref(grapher::json_t const &object, LocType const &field_location,
            const std::experimental::source_location loc =
                std::experimental::source_location::current()) {

  using ValueType = std::decay_t<ReferenceType>;

  std::string const field_location_str = field_location;

  check(object.contains(field_location),
        fmt::format("Empty field {}:\n{}", field_location_str, object.dump(2)),
        error_v, -1, loc);

  // number_unsigned_t
  if constexpr (std::is_same<grapher::json_t::number_unsigned_t,
                             ValueType>::value) {
    check(object[field_location].type() == json_t::value_t::number_unsigned,
          fmt::format("Invalid field {}, expected string:\n{}",
                      field_location_str, object.dump(2)),
          error_v, -1, loc);
  }

  // number_integer_t
  else if constexpr (std::is_same<grapher::json_t::number_integer_t,
                                  ValueType>::value) {
    check(object[field_location].type() == json_t::value_t::number_integer,
          fmt::format("Invalid field {}, expected number:\n{}",
                      field_location_str, object.dump(2)),
          error_v, -1, loc);
  }

  // number_float_t
  else if constexpr (std::is_same<grapher::json_t::number_float_t,
                                  ValueType>::value) {
    check(object[field_location].type() == json_t::value_t::number_float,
          fmt::format("Invalid field {}, expected number:\n{}",
                      field_location_str, object.dump(2)),
          error_v, -1, loc);
  }

  // array_t
  else if constexpr (std::is_same<grapher::json_t::array_t, ValueType>::value) {
    check(object[field_location].type() == json_t::value_t::array,
          fmt::format("Invalid field {}, expected array:\n{}",
                      field_location_str, object.dump(2)),
          error_v, -1, loc);
  }

  // boolean_t
  else if constexpr (std::is_same<grapher::json_t::boolean_t,
                                  ValueType>::value) {
    check(object[field_location].type() == json_t::value_t::boolean,
          fmt::format("Invalid field {}, expected bool:\n{}",
                      field_location_str, object.dump(2)),
          error_v, -1, loc);
  }

  // object_t
  else if constexpr (std::is_same<grapher::json_t::object_t,
                                  ValueType>::value) {
    check(object[field_location].type() == json_t::value_t::object,
          fmt::format("Invalid field {}, expected JSON object:\n{}",
                      field_location_str, object.dump(2)),
          error_v, -1, loc);
  }

  return object[field_location].template get_ref<ReferenceType>();
}

} // namespace grapher
