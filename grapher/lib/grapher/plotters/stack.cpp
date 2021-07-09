#include <algorithm>

#include <nlohmann/json_fwd.hpp>
#include <sciplot/sciplot.hpp>

#include "grapher/json-utils.hpp"
#include "grapher/plot-utils.hpp"
#include "grapher/plotters/stack.hpp"

namespace grapher {

std::string_view plotter_stack_t::get_help() const { return ""; }

nlohmann::json plotter_stack_t::get_default_config() const {
  nlohmann::json res = grapher::get_default_config();

  res["value_json_pointer"] = "/dur";
  res["name_json_pointer"] = "/name";

  res["matchers"].push_back({ {"name", "Total Source"} });

  return res;
}

void plotter_stack_t::plot(category_t const &cat,
                           std::filesystem::path const &dest,
                           nlohmann::json const &config) const {
  // TODO: Error management
  std::vector<nlohmann::json> matcher_set = config["matchers"];

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

    auto const &[name, entries] = bench;

    std::vector<double> x;
    std::transform(entries.begin(), entries.end(), std::back_inserter(x),
                   [](entry_t const &e) -> double { return e.size; });

    std::vector<double> y_low(x.size(), 0.);
    std::vector<double> y_high(x.size());

    for (auto const &matcher : matcher_set) {
      // Storing previous value as we iterate
      double value = 0.;

      std::string curve_name("UNKNOWN");
      if (!entries.empty() && !entries.begin()->data.empty()) {
        curve_name =
            get_feature_name(*entries.begin()->data.begin(), feature_name_jptr)
                .value_or(std::move(curve_name));
      }

      for (std::size_t i = 0; i < entries.size(); i++) {
        auto const &[size, data] = entries[i];
        // TODO: Get better stats (standard deviation, etc...)
        value = get_average(data, matcher, feature_value_jptr).value_or(value);
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
