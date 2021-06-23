#pragma once

/// \file
/// Defining functions for graphing

#include <filesystem>

#include <grapher/core.hpp>

namespace grapher {

/// Data structure for plotting parameters (will be added in the future)
using plot_params_t = nlohmann::json;

/// Given a category, draws all the features as a stacked filled curve graph for
/// each benchmark at the given path. Each item in matcher_set is used to find
/// which event to plot in the time-trace event array. feature_value_ptr is then
/// used to select the desired feature to plot, and feature_name_ptr is used to
/// name the filled curve.
void stacked_graph(category_t const &cat, std::filesystem::path const &dest,
                   std::vector<nlohmann::json> const &matcher_set,
                   nlohmann::json::json_pointer feature_value_ptr,
                   nlohmann::json::json_pointer feature_name_ptr);

/// Given a category, draws a comparative curve between all benchmarks for each
/// matcher.
void comparative_graph(category_t const &cat, std::filesystem::path const &dest,
                       std::vector<nlohmann::json> const &matcher_set,
                       nlohmann::json::json_pointer feature_value_jptr,
                       nlohmann::json::json_pointer feature_name_jptr);

} // namespace grapher
