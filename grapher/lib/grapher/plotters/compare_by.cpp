#include <filesystem>
#include <fstream>
#include <numeric>
#include <sciplot/Plot.hpp>
#include <string>
#include <string_view>
#include <vector>

#include <llvm/Support/raw_ostream.h>

#include <nlohmann/json.hpp>

#include <sciplot/sciplot.hpp>

#include "grapher/core.hpp"
#include "grapher/plotters/compare_by.hpp"
#include "grapher/utils/config.hpp"
#include "grapher/utils/error.hpp"
#include "grapher/utils/json.hpp"
#include "grapher/utils/plot.hpp"

namespace grapher::plotters {

std::string_view plotter_compare_by_t::get_help() const {
  return "Compares all traceEvents with a matching feature.";
}

grapher::json_t plotter_compare_by_t::get_default_config() const {
  grapher::json_t res = grapher::base_default_config();

  res["plotter"] = "compare_by";

  res["key_ptr"] = "/name";
  res["value_ptr"] = "/dur";

  res["draw_average"] = true;
  res["draw_points"] = true;

  res["plot_file_extension"] = grapher::json_t::array({".svg", ".png"});

  return res;
}

// Plot-friendly data structures

/// Point aggregate (multiple Y coordinates)
using point_data_t = std::vector<double>;

/// Curve: X -> vec<Y>
using benchmark_curve_t = map_t<std::size_t, point_data_t>;

/// Benchmark name -> Curve
using curve_aggregate_t = map_t<std::string, benchmark_curve_t>;

/// Feature -> Benchmark aggregate
using curve_aggregate_map_t = map_t<std::string, curve_aggregate_t>;

/// Wrangles data into a structure that's easier to work with for plotting.
curve_aggregate_map_t
get_bench_curves(benchmark_set_t const &bset,
                 grapher::json_t::json_pointer const &key_ptr,
                 grapher::json_t::json_pointer const &val_ptr) {
  namespace fs = std::filesystem;

  curve_aggregate_map_t res;

  for (benchmark_case_t const &bench_case : bset) {

    // std::size_t const iteration_count = bench_case.iterations.size();

    for (benchmark_iteration_t const &iteration : bench_case.iterations) {
      for (fs::path const &sample : iteration.samples) {
        grapher::json_t sample_json;
        {
          std::ifstream sample_ifstream(sample);
          sample_ifstream >> sample_json;
        }

        for (grapher::json_t const &event :
             json_at_ref<json_t::array_t const &>(sample_json, "traceEvents")) {

          // Key/value presence and type checks
          if (check(event.contains(key_ptr),
                    fmt::format("No matching key at {}:\n{}",
                                key_ptr.to_string(), event.dump(2)),
                    warning_v) &&
              check(event[key_ptr].is_string(),
                    fmt::format("Key at {} is not a string:\n{}",
                                key_ptr.to_string(), event.dump(2)),
                    warning_v) &&
              check(event.contains(val_ptr),
                    fmt::format("No matching value at {}:\n{}",
                                val_ptr.to_string(), event.dump(2)),
                    warning_v) &&
              check(event[val_ptr].is_number(),
                    fmt::format("Value at {} is not an integer:\n{}",
                                val_ptr.to_string(), event.dump(2)),
                    warning_v)) {
            // Adding value
            res[event[key_ptr].get_ref<grapher::json_t::string_t const &>()]
               [bench_case.name][iteration.size]
                   .push_back(event[val_ptr]);
          }
        }
      }
    }
  }

  return res;
}

void plotter_compare_by_t::plot(benchmark_set_t const &bset,
                                std::filesystem::path const &dest,
                                grapher::json_t const &config) const {
  namespace sp = sciplot;
  namespace fs = std::filesystem;

  // Config

  bool draw_average = config.value("draw_average", true);
  bool draw_points = config.value("draw_points", true);

  std::vector<std::string> plot_file_extensions = config.value(
      "plot_file_extensions", grapher::json_t::array({".svg", ".png"}));

  json_t::json_pointer key_ptr(config.value("key_ptr", "/name"));
  json_t::json_pointer value_ptr(config.value("value_ptr", "/dur"));

  // Wrangling

  curve_aggregate_map_t curve_aggregate_map =
      get_bench_curves(bset, key_ptr, value_ptr);

  fs::create_directories(dest);

  // Drawing

  for (auto const &[feature_name, curve_aggregate] : curve_aggregate_map) {
    // Configure + draw + save plots
    sp::Plot plot;
    apply_config(plot, config);

    for (auto const &[bench_name, benchmark_curve] : curve_aggregate) {
      std::vector<double> x_curve;
      std::vector<double> y_curve;
      std::vector<double> x_points;
      std::vector<double> y_points;

      // Build point & curve vectors
      for (auto const &[x_value, y_values] : benchmark_curve) {
        // ...
        if (draw_average && !y_values.empty()) {
          x_curve.push_back(x_value);
          y_curve.push_back(std::reduce(y_values.begin(), y_values.end()) /
                            y_values.size());
        }

        if (draw_points) {
          for (double y_value : y_values) {
            x_points.push_back(x_value);
            y_points.push_back(y_value);
          }
        }
      }

      if (draw_average && !x_curve.empty()) {
        plot.drawCurve(x_curve, y_curve).label(bench_name + " average");
      }

      if (draw_points && !x_points.empty()) {
        plot.drawPoints(x_points, y_points).label(bench_name + " points");
      }
    }

    for (std::string const &extension : plot_file_extensions) {
      plot.save(fs::path{dest} / (feature_name + extension));
    }
  }
}

} // namespace grapher::plotters