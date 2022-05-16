#pragma once

#include <nlohmann/json.hpp>

#include <sciplot/sciplot.hpp>

#include "grapher/core.hpp"

namespace grapher {

/// Apply config to plot.
sciplot::Plot &apply_config(sciplot::Plot &plot, grapher::json_t config);

/// Returns the default configuration for apply_config.
grapher::json_t base_default_config();

/// Returns help for base default config.
std::string base_default_config_help();

/// Plot saving helper function.
void save_plot(sciplot::Plot const &plot, std::string const &dest,
               grapher::json_t const &config);

} // namespace grapher
