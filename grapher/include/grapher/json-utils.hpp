#pragma once

#include <nlohmann/detail/json_pointer.hpp>
#include <optional>
#include <string>

#include <nlohmann/json.hpp>

#include "grapher/core.hpp"

namespace grapher {

/// Finds a JSON element in a range, given a matcher.
/// find_matching will look at every element between begin and end,
/// and return a nlohmann::json::const_iterator pointing to an element
/// that contains all the elements in the matcher.
nlohmann::json::const_iterator
find_matching(nlohmann::json::const_iterator begin,
              nlohmann::json::const_iterator end,
              nlohmann::json const &matcher);

/// Averages
std::optional<double> get_average(std::vector<nlohmann::json> const &data,
                                  nlohmann::json const &matcher,
                                  nlohmann::json::json_pointer value_jptr);

/// Gets feature name from a vector of json, given a JSON pointer
std::optional<std::string>
get_feature_name(benchmark_t const &bench, nlohmann::json const &matcher,
                 nlohmann::json::json_pointer const &name_jptr);

/// Merges the contents of b into a, with items of a being overwritten if items
/// present in b share the same key.
nlohmann::json merge_into(nlohmann::json a, nlohmann::json const &b);

} // namespace grapher
