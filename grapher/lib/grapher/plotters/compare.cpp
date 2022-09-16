#include <filesystem>
#include <numeric>
#include <string>
#include <vector>

#include <llvm/Support/raw_ostream.h>

#include <fmt/core.h>

#include <nlohmann/json.hpp>

#include <sciplot/sciplot.hpp>

#include <grapher/core.hpp>
#include <grapher/plotters/compare.hpp>
#include <grapher/predicates.hpp>
#include <grapher/utils/error.hpp>
#include <grapher/utils/json.hpp>

namespace grapher::plotters {

grapher::json_t plotter_compare_t::get_default_config() const {
  grapher::json_t res = grapher::base_default_config();

  res["plotter"] = "compare";

  res["value_json_pointer"] = "/dur";
  res["draw_average"] = true;
  res["draw_points"] = true;

  res["group_descriptors"] =
      write_descriptors({get_default_group_descriptor()});

  return res;
}

void plotter_compare_t::plot(benchmark_set_t const &bset,
                             std::filesystem::path const &dest,
                             grapher::json_t const &config) const {
  // Config

  grapher::json_t::json_pointer value_json_pointer(
      json_at_ref<json_t::string_t const &>(config, "value_json_pointer"));

  bool draw_average = config.value("draw_average", true);
  bool draw_points = config.value("draw_points", true);

  std::vector<group_descriptor_t> group_descriptors = read_descriptors(
      json_at_ref<json_t::array_t const &>(config, "group_descriptors"));

  // Drawing

  for (group_descriptor_t const &descriptor : group_descriptors) {
    // Plot init
    sciplot::Plot2D plot;
    apply_config(plot, config);

    // Generating predicates
    std::vector<predicate_t> predicates = get_predicates(descriptor);

    for (benchmark_case_t const &bench : bset) {
      std::vector<double> x_points;
      std::vector<double> y_points;

      std::vector<double> x_average;
      std::vector<double> y_average;

      for (benchmark_iteration_t const &iteration : bench.iterations) {
        check(!iteration.samples.empty(),
              fmt::format("No data in benchmark {} for iteration size {}.",
                          bench.name, iteration.size),
              error_level_t::warning_v);

        std::vector<double> const values =
            get_values(iteration, predicates, value_json_pointer);

        check(!values.empty(),
              fmt::format("No event in benchmark {} at size {} matched by "
                          "group descriptor {}.\n",
                          bench.name, iteration.size, descriptor.name),
              error_level_t::warning_v);

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
    save_plot(plot, dest / std::move(descriptor.name), config);
  }
}

} // namespace grapher::plotters
