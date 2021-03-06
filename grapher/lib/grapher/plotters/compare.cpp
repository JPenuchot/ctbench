#include <filesystem>
#include <numeric>
#include <string>
#include <vector>

#include <llvm/Support/raw_ostream.h>

#include <nlohmann/json.hpp>

#include <sciplot/sciplot.hpp>

#include "grapher/core.hpp"
#include "grapher/plotters/compare.hpp"
#include "grapher/predicates.hpp"
#include "grapher/utils/config.hpp"
#include "grapher/utils/json.hpp"
#include "grapher/utils/plot.hpp"

namespace grapher::plotters {

std::string_view plotter_compare_t::get_help() const {
  return "For each group descriptor, generates a graph comparing all "
         "benchmark cases in the set.";
}

nlohmann::json plotter_compare_t::get_default_config() const {
  nlohmann::json res = grapher::base_default_config();

  res["plotter"] = "compare";

  res["value_json_pointer"] = "/dur";
  res["draw_average"] = true;
  res["draw_points"] = true;

  res["plot_file_extension"] = ".svg";

  res["group_descriptors"] =
      write_descriptors({get_default_group_descriptor()});

  return res;
}

void plotter_compare_t::plot(benchmark_set_t const &bset,
                             std::filesystem::path const &dest,
                             nlohmann::json const &config) const {
  // Config

  nlohmann::json::json_pointer value_json_pointer(
      json_value<std::string>(config, "value_json_pointer"));

  bool draw_average = config.value("draw_average", true);
  bool draw_points = config.value("draw_points", true);

  std::string plot_file_extension = config.value("plot_file_extension", ".svg");

  std::vector<group_descriptor_t> group_descriptors = read_descriptors(
      json_value<std::vector<nlohmann::json>>(config, "group_descriptors"));

  // Drawing

  for (group_descriptor_t const &descriptor : group_descriptors) {
    // Plot init
    sciplot::Plot plot;
    apply_config(plot, config);

    // Generating predicates
    std::vector<predicate_t> predicates = get_predicates(descriptor);

    for (benchmark_case_t const &bench : bset) {
      std::vector<double> x_points;
      std::vector<double> y_points;

      std::vector<double> x_average;
      std::vector<double> y_average;

      for (benchmark_iteration_t const &iteration : bench.iterations) {
        if (iteration.samples.empty()) {
          llvm::errs() << "[WARNING] No data in benchmark " << bench.name
                       << " for iteration size " << iteration.size << "\n";
          continue;
        }

        std::vector<double> const values =
            get_values(iteration, predicates, value_json_pointer);

        if (values.empty()) {
          llvm::errs() << "[WARNING] No event in benchmark " << bench.name
                       << " at size " << iteration.size
                       << " matched by group descriptor " << descriptor.name
                       << ".\n";
          continue;
        }

        // Drawing points
        if (draw_points) {
          for (double value : values) {
            x_points.push_back(iteration.size);
            y_points.push_back(value);
          }
        }

        // Drawing average
        if (draw_average) {
          x_average.push_back(iteration.size);
          y_average.push_back(std::reduce(values.begin(), values.end()) /
                              values.size());
        }
      }

      if (draw_points) {
        plot.drawPoints(x_points, y_points).label(bench.name);
      }

      if (draw_average) {
        plot.drawCurve(x_average, y_average).label(bench.name);
      }
    }

    // Saving plot
    std::filesystem::create_directories(dest);
    plot.save(dest / (std::move(descriptor.name) + plot_file_extension));
  }
}

} // namespace grapher::plotters
