#include <algorithm>
#include <filesystem>

#include <llvm/Support/raw_ostream.h>

#include <nlohmann/json.hpp>

#include <numeric>
#include <sciplot/sciplot.hpp>

#include "grapher/core.hpp"
#include "grapher/plotters/stack.hpp"
#include "grapher/utils/config.hpp"
#include "grapher/utils/json.hpp"
#include "grapher/utils/plot.hpp"

namespace grapher::plotters {

std::string_view plotter_stack_t::get_help() const {
  return "For each benchmark in the category, generates a stakcked curve graph "
         "where each curve corresponds to a matcher in the \'matchers\' JSON "
         "field.";
}

nlohmann::json plotter_stack_t::get_default_config() const {
  nlohmann::json res = grapher::base_default_config();

  res["plotter"] = "stack";

  // Basic values, probably no need to change them
  res["value_json_pointer"] = "/dur";
  res["name_json_pointer"] = "/name";
  res["plot_file_extension"] = ".svg";

  // Some matchers as an example...
  res["group_descriptors"] =
      write_descriptors({get_default_group_descriptor()});

  return res;
}

void plotter_stack_t::plot(benchmark_set_t const &bset,
                           std::filesystem::path const &dest,
                           nlohmann::json const &config) const {
  // Config

  nlohmann::json::json_pointer feature_value_jptr(
      config.value("value_json_pointer", "/dur"));

  nlohmann::json::json_pointer feature_name_jptr(
      config.value("name_json_pointer", "/name"));

  std::string plot_file_extension = config.value("plot_file_extension", ".svg");

  std::vector<group_descriptor_t> descriptors = read_descriptors(
      json_value<std::vector<nlohmann::json>>(config, "group_descriptors"));

  // Drawing

  std::vector<sciplot::Plot> plots;

  // Saving max value
  double max_val = 0.;

  /// Draws a stacked curve graph for a given benchmark
  auto draw_plot = [&](benchmark_case_t const &bench) -> sciplot::Plot {
    namespace sp = sciplot;

    sp::Plot plot;
    apply_config(plot, config);

    // x axis
    std::vector<double> x;
    std::transform(
        bench.iterations.begin(), bench.iterations.end(), std::back_inserter(x),
        [](benchmark_iteration_t const &i) -> double { return i.size; });

    // Low y axis
    std::vector<double> y_low(x.size(), 0.);
    // High y axis
    std::vector<double> y_high(x.size());

    for (group_descriptor_t const &descriptor : descriptors) {
      // Storing previous value as we iterate
      double value = 0.;

      std::string curve_name = descriptor.name;

      for (std::size_t i = 0; i < bench.iterations.size(); i++) {
        benchmark_iteration_t const &iteration = bench.iterations[i];
        std::vector<double> const values =
            get_values(iteration, descriptor, feature_value_jptr);

        if (values.empty()) {
          llvm::errs() << "[ERROR] No event matching descriptor "
                       << descriptor.name << " in benchmark " << bench.name
                       << " with size = " << iteration.size << ".\n";
          std::exit(1);
        }

        // TODO: Get better stats (standard deviation, etc...)
        value = std::reduce(values.begin(), values.end()) / values.size();
        double const new_y = y_low[i] + value;
        y_high[i] = new_y;

        // Update max_val for y-azis normalization
        max_val = std::max(max_val, new_y);
      }

      plot.drawCurvesFilled(x, y_low, y_high).label(std::move(curve_name));

      // Swapping
      std::swap(y_low, y_high);
    }

    plot.legend().displayStartFromLast();
    return plot;
  };

  // Drwaing...
  std::transform(bset.begin(), bset.end(), std::back_inserter(plots),
                 draw_plot);

  // Normalize & save
  std::filesystem::create_directories(dest);
  for (std::size_t i = 0; i < bset.size(); i++) {
    plots[i].yrange(0., max_val);
    plots[i].save(dest / (bset[i].name + plot_file_extension));
  }
}

} // namespace grapher::plotters
