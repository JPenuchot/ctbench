#pragma once

#include "nlohmann/detail/json_pointer.hpp"
#include <ios>
#include <string_view>

#include <nlohmann/json.hpp>

#include <sciplot/sciplot.hpp>

namespace grapher {

/// Apply config to plot.
sciplot::Plot &apply_config(sciplot::Plot &plot, nlohmann::json config);

/// Returns the default configuration
nlohmann::json get_default_config();

} // namespace grapher
