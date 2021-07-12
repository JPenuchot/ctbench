#pragma once

#include <ios>
#include <string_view>

#include <nlohmann/json.hpp>

#include <sciplot/sciplot.hpp>

namespace grapher {

/// Apply config to plot.
sciplot::Plot &apply_config(sciplot::Plot &plot, nlohmann::json config);

/// Returns the default configuration
nlohmann::json base_default_config();

} // namespace grapher
