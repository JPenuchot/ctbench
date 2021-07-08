#pragma once

#include <optional>
#include <string>

#include <nlohmann/json.hpp>

namespace grapher {

/// Finds an element that includes a given JSON object.
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
get_feature_name(std::vector<nlohmann::json> const &data,
                 nlohmann::json::json_pointer const &name_jptr);

} // namespace grapher
