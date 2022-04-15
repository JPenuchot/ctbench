#pragma once

#include <nlohmann/json.hpp>

#include <sciplot/sciplot.hpp>

#include "grapher/core.hpp"

namespace grapher {

/// Apply config to plot.
sciplot::Plot &apply_config(sciplot::Plot &plot, grapher::json_t config);

/// Returns the default configuration for apply_config.
grapher::json_t base_default_config();

} // namespace grapher
