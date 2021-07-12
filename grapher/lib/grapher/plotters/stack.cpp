#include <algorithm>

#include <llvm/Support/raw_ostream.h>
#include <nlohmann/json_fwd.hpp>
#include <sciplot/sciplot.hpp>

#include "grapher/json-utils.hpp"
#include "grapher/plot-utils.hpp"
#include "grapher/plotters/stack.hpp"

namespace grapher {

std::string_view plotter_stack_t::get_help() const { return ""; }

nlohmann::json plotter_stack_t::get_default_config() const {
  nlohmann::json res = grapher::base_default_config();

  // Basic values, probably no need to change them
  res["value_json_pointer"] = "/dur";
  res["name_json_pointer"] = "/name";

  // Some matchers as an example...
  res["matchers"].push_back({{"name", "Total Frontend"}});
  res["matchers"].push_back({{"name", "Total Backend"}});

  return res;
}

void plotter_stack_t::plot(category_t const &cat,
                           std::filesystem::path const &dest,
                           nlohmann::json const &config) const {
  auto const default_config = this->get_default_config();

  std::vector<nlohmann::json> matcher_set;

  if (config.contains("matcher") && config.is_array()) {
    matcher_set = std::vector<nlohmann::json>(config["matcher"]);
  } else {
    llvm::errs() << "Warning: No matcher was specified in the configuration "
                    "file. Falling back to default matchers.\n";
    matcher_set = std::vector<nlohmann::json>(default_config["matcher"]);
  }

  nlohmann::json::json_pointer feature_value_jptr(
      config.value("value_json_pointer", "/dur"));

  nlohmann::json::json_pointer feature_name_jptr(
      config.value("name_json_pointer", "/name"));

  std::vector<sciplot::Plot> plots;

  // Saving max value
  double max_val = 0.;

  /// Draws a stacked curve graph for a given benchmark
  auto draw_plot = [&](benchmark_t const &bench) -> sciplot::Plot {
    namespace sp = sciplot;

    sp::Plot plot;
    apply_config(plot, config);

    auto const &[bench_name, bench_entries] = bench;

    // x axis
    std::vector<double> x;
    std::transform(bench_entries.begin(), bench_entries.end(),
                   std::back_inserter(x),
                   [](entry_t const &e) -> double { return e.size; });

    // Low y axis
    std::vector<double> y_low(x.size(), 0.);
    // High y axis
    std::vector<double> y_high(x.size());

    for (auto const &matcher : matcher_set) {
      // Storing previous value as we iterate
      double value = 0.;

      std::string curve_name("UNKNOWN");
      if (!bench_entries.empty() && !bench_entries.begin()->data.empty()) {
        curve_name = get_feature_name(*bench_entries.begin()->data.begin(),
                                      feature_name_jptr)
                         .value_or(std::move(curve_name));
      }

      for (std::size_t i = 0; i < bench_entries.size(); i++) {
        auto const &[entry_size, entry_iterations] = bench_entries[i];
        // TODO: Get better stats (standard deviation, etc...)
        value = get_average(entry_iterations, matcher, feature_value_jptr)
                    .value_or(value);
        y_high[i] = y_low[i] + value;
      }

      plot.drawCurvesFilled(x, y_low, y_high).label(std::move(curve_name));

      // Swapping
      std::swap(y_low, y_high);
    }

    return plot;
  };

  // Drwaing...
  std::transform(cat.begin(), cat.end(), std::back_inserter(plots), draw_plot);

  // Normalize & save
  for (std::size_t i = 0; i < cat.size(); i++) {
    plots[i].yrange(0., max_val);
    plots[i].save(dest / (cat[i].name + ".svg"));
  }
}

} // namespace grapher
