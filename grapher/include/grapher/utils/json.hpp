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

/// Returns the average value of
std::vector<double> get_values(entry_t const &entry,
                               group_descriptor_t const &descriptor,
                               nlohmann::json::json_pointer value_jptr);

/// Merges the contents of b into a, with items of a being overwritten if items
/// present in b share the same key.
nlohmann::json merge_into(nlohmann::json a, nlohmann::json const &b);

/// Wrapper for strict JSON type error management.
/// Exits program if value at given field location is empty or isn't of the
/// right type.
template <typename ValueType, typename LocType>
inline ValueType json_value(nlohmann::json const &object,
                            LocType const &field_location,
                            const std::experimental::source_location loc =
                                std::experimental::source_location::current()) {
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

  // std::vector<nlohmann::json>
  if constexpr (std::is_same<std::vector<nlohmann::json>, ValueType>::value) {
    if (!object.contains(field_location) ||
        !object[field_location].is_array()) {
      llvm::errs() << loc.function_name() << " - Invalid field "
                   << field_location
                   << ", expected std::vector<nlohmann::json>:\n"
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
