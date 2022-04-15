#pragma once

#include <experimental/source_location>
#include <optional>
#include <string>
#include <type_traits>

#include <llvm/Support/raw_ostream.h>

#include <nlohmann/json.hpp>

#include "grapher/core.hpp"
#include "grapher/utils/config.hpp"

namespace grapher {

/// For each iteration in entry, returns the sum of the values pointed by
/// value_jptr in the events matching the descriptor's predicates.
std::vector<double> get_values(benchmark_iteration_t const &iteration,
                               std::vector<predicate_t> const &predicates,
                               grapher::json_t::json_pointer value_jptr);

/// Merges the contents of b into a, with items of a being overwritten if items
/// present in b share the same key.
grapher::json_t merge_into(grapher::json_t a, grapher::json_t const &b);

/// Wrapper for strict JSON type error management.
/// Exits program if value at given field location is empty or isn't of the
/// right type.
template <typename ValueType, typename LocType>
inline ValueType json_value(grapher::json_t const &object,
                            LocType const &field_location,
                            const std::experimental::source_location loc =
                                std::experimental::source_location::current()) {
  // grapher::json_t
  if constexpr (std::is_same<grapher::json_t, ValueType>::value) {
    if (!object.contains(field_location)) {
      llvm::errs() << loc.function_name() << " - Empty field " << field_location
                   << ":\n"
                   << object.dump(2) << '\n';
      std::exit(1);
    } else {
      return object[field_location];
    }
  }

  // std::string
  if constexpr (std::is_same<std::string, ValueType>::value) {
    if (!object.contains(field_location) ||
        !object[field_location].is_string()) {
      llvm::errs() << loc.function_name() << " - Invalid field "
                   << field_location << ", expected string:\n"
                   << object.dump(2) << '\n';
      std::exit(1);
    } else {
      return object[field_location];
    }
  }

  // std::vector<grapher::json_t>
  if constexpr (std::is_same<std::vector<grapher::json_t>, ValueType>::value) {
    if (!object.contains(field_location) ||
        !object[field_location].is_array()) {
      llvm::errs() << loc.function_name() << " - Invalid field "
                   << field_location
                   << ", expected std::vector<grapher::json_t>:\n"
                   << object.dump(2) << '\n';
      std::exit(1);
    } else {
      return object[field_location];
    }
  }

  // bool
  if constexpr (std::is_same<bool, ValueType>::value) {
    if (!object.contains(field_location) ||
        !object[field_location].is_boolean()) {
      llvm::errs() << loc.function_name() << " - Invalid field "
                   << field_location << ", expected bool:\n"
                   << object.dump(2) << '\n';
      std::exit(1);
    } else {
      return object[field_location];
    }
  }

  // int
  if constexpr (std::is_same<int, ValueType>::value) {
    if (!object.contains(field_location) ||
        !object[field_location].is_number()) {
      llvm::errs() << loc.function_name() << " - Invalid field "
                   << field_location << ", expected number:\n"
                   << object.dump(2) << '\n';
      std::exit(1);
    } else {
      return object[field_location];
    }
  }

  // double
  if constexpr (std::is_same<double, ValueType>::value) {
    if (!object.contains(field_location) ||
        !object[field_location].is_number()) {
      llvm::errs() << loc.function_name() << " - Invalid field "
                   << field_location << ", expected number:\n"
                   << object.dump(2) << '\n';
      std::exit(1);
    } else {
      return object[field_location];
    }
  }
}

} // namespace grapher
