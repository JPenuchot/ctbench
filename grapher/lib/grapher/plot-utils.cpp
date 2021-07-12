#include "grapher/plot-utils.hpp"
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
  // Merging defaults into config
  for (auto const &[key, value] : default_config.items()) {
    auto key_jptr = nlohmann::json::json_pointer(key);
    if (!config.contains(key_jptr)) {
      config[key_jptr] = value;
    }
  }

  // Dimensions
  plot.size(config["width"], config["height"]);

  // Labels
  plot.legend().atOutsideRightTop().title(config["legend_title"]);
  plot.xlabel(config["xlabel"]);
  plot.ylabel(config["ylabel"]);

  return plot;
}

nlohmann::json base_default_config() { return default_config; }

} // namespace grapher
