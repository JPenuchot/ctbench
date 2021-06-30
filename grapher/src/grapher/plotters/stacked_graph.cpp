#include <algorithm>

#include <sciplot/sciplot.hpp>

#include <grapher/plot-utils.hpp>
#include <grapher/json-utils.hpp>

#include <grapher/plotters/stacked_graph.hpp>

namespace grapher {

void stacked_graph_t::operator()(
    category_t const &cat, std::filesystem::path const &dest,
    std::vector<nlohmann::json> const &matcher_set,
    nlohmann::json::json_pointer feature_value_jptr,
    nlohmann::json::json_pointer feature_name_jptr,
    graph_config_t const &config) {
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