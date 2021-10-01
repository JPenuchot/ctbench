#include "grapher/utils/plot.hpp"

#include <nlohmann/json.hpp>
#include <sciplot/sciplot.hpp>

#include "grapher/plotters/plotter_i.hpp"

namespace grapher {

/// Default config file for plots
nlohmann::json const default_config = {
    {"width", 1500},
    {"height", 500},
    {"legend_title", "Timings"},
    {"xlabel", "Benchmark size factor"},
    {"ylabel", "Time (µs)"},
};

sciplot::Plot &apply_config(sciplot::Plot &plot, nlohmann::json config) {
  // Dimensions
  if (config.contains("width") && config.contains("height")) {
    plot.size(config["width"], config["height"]);
  }

  // Labels
  if (config.contains("legend_title")) {
    plot.legend().atOutsideRightTop().title(config["legend_title"]);
  }

  if (config.contains("xlabel")) {
    plot.xlabel(config["xlabel"]);
  }

  if (config.contains("ylabel")) {
    plot.ylabel(config["ylabel"]);
  }

  return plot;
}

nlohmann::json base_default_config() { return default_config; }

} // namespace grapher
