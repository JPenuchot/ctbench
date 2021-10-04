#pragma once

#include <experimental/source_location>
#include <llvm/Support/raw_ostream.h>
#include <optional>
#include <string>
#include <type_traits>

#include <llvm/ADT/SmallVector.h>

#include <nlohmann/json.hpp>

#include "grapher/core.hpp"

namespace grapher {

/// Finds a JSON element in a range, given a matcher.
/// find_matching will look at every element between begin and end,
/// and return a nlohmann::json::const_iterator pointing to an element
/// that contains all the elements in the matcher.
[[deprecated("Use grapher predicates instead.")]] nlohmann::json::const_iterator
find_matching(nlohmann::json::const_iterator begin,
              nlohmann::json::const_iterator end,
              nlohmann::json const &matcher);

/// Averages
std::optional<double> get_average(std::vector<nlohmann::json> const &data,
                                  nlohmann::json const &matcher,
                                  nlohmann::json::json_pointer value_jptr);

/// Gets feature name from a vector of json, given a JSON pointer
[[deprecated("Use group descriptor name instead.")]] std::optional<std::string>
get_feature_name(benchmark_t const &bench, nlohmann::json const &matcher,
                 nlohmann::json::json_pointer const &name_jptr);

/// Merges the contents of b into a, with items of a being overwritten if items
/// present in b share the same key.
nlohmann::json merge_into(nlohmann::json a, nlohmann::json const &b);

/// Wrapper for strict JSON type error management.
/// Exits program if value at given field is empty or isn't of the right type.
template <typename T>
inline T json_value(nlohmann::json const &j, std::string const &field,
                    const std::experimental::source_location loc =
                        std::experimental::source_location::current()) {
  // std::string
  if constexpr (std::is_same<std::string, T>::value) {
    if (!j.contains(field) || !j[field].is_string()) {
      llvm::errs() << loc.function_name() << " - Invalid field " << field
                   << ", expected string:\n"
                   << j.dump(2) << '\n';
      std::exit(1);
    } else {
      return j[field];
    }
  }

  // std::vector<nlohmann::json>
  if constexpr (std::is_same<std::vector<nlohmann::json>, T>::value) {
    if (!j.contains(field) || !j[field].is_array()) {
      llvm::errs() << loc.function_name() << " - Invalid field " << field
                   << ", expected std::vector<nlohmann::json>:\n"
                   << j.dump(2) << '\n';
      std::exit(1);
    } else {
      return j[field];
    }
  }

  // bool
  if constexpr (std::is_same<bool, T>::value) {
    if (!j.contains(field) || !j[field].is_boolean()) {
      llvm::errs() << loc.function_name() << " - Invalid field " << field
                   << ", expected bool:\n"
                   << j.dump(2) << '\n';
      std::exit(1);
    } else {
      return j[field];
    }
  }
}

} // namespace grapher
