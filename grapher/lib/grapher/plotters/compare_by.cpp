#include <algorithm>
#include <execution>
#include <filesystem>
#include <fstream>
#include <string>
#include <vector>

#include <fmt/core.h>

#include <boost/container/small_vector.hpp>

#include <llvm/Demangle/Demangle.h>

#include <sciplot/sciplot.hpp>

#include <grapher/core.hpp>
#include <grapher/plotters/compare_by.hpp>
#include <grapher/utils/error.hpp>
#include <grapher/utils/json.hpp>
#include <grapher/utils/tracy.hpp>

namespace grapher::plotters {

// =============================================================================
// AUXILIARY

// Plot-friendly data structures

/// Point aggregate (multiple Y coordinates)
using point_data_t = std::vector<double>;

/// Curve: X -> vec<Y>
using benchmark_curve_t = map_t<std::size_t, point_data_t>;

/// Benchmark name -> Curve
using curve_aggregate_t = map_t<std::string, benchmark_curve_t>;

/// Value key type. Contains multiple values to group by a tuple of parameters
using key_t = boost::container::small_vector<std::string, 4>;

/// Feature -> Benchmark aggregate
using curve_aggregate_map_t = map_t<key_t, curve_aggregate_t>;

/// Wrangles data into a structure that's easier to work with for plotting.
curve_aggregate_map_t
get_bench_curves(benchmark_set_t const &bset,
                 std::vector<json_t::json_pointer> const &key_ptrs,
                 json_t::json_pointer const &val_ptr) {
  ZoneScoped;
  namespace fs = std::filesystem;

  curve_aggregate_map_t res;

  for (benchmark_case_t const &bench_case : bset) {
    for (benchmark_instance_t const &instance : bench_case.instances) {
      for (fs::path const &repetition : instance.repetitions) {
        grapher::json_t repetition_json;
        {
          std::ifstream repetition_ifstream(repetition);
          repetition_ifstream >> repetition_json;
        }

        for (grapher::json_t const &event : get_as_ref<json_t::array_t const &>(
                 repetition_json, "traceEvents")) {

          // Building key from JSON pointers
          key_t key;
          for (json_t::json_pointer const &key_ptr : key_ptrs) {
            if (event.contains(key_ptr) && event[key_ptr].is_string()) {
              key.push_back(event[key_ptr]);
            }
          }

          // Key/value presence and type checks
          if (check(event.contains(val_ptr),
                    fmt::format("No value at {}: {}", val_ptr.to_string(),
                                event.dump()),
                    info_v) &&
              check(event[val_ptr].is_number(),
                    fmt::format("Value at {} is not an integer: {}",
                                val_ptr.to_string(), event.dump()),
                    info_v)) {
            // Adding value
            res[key][bench_case.name][instance.size].push_back(event[val_ptr]);
          }
        }
      }
    }
  }

  return res;
}

/// Transforms a key into a string that's usable as a path.
std::string to_string(key_t const &key, bool demangle = true) {
  if (key.empty()) {
    return "empty";
  }

  std::string res = demangle ? llvm::demangle(key[0]) : key[0];
  std::for_each(key.begin() + 1, key.end(), [&](std::string const &part) {
    res += '/';
    for (char const c : demangle ? llvm::demangle(part) : part) {
      res += c == '/' ? '_' : c;
    }
  });

  return res;
}

// =============================================================================
// OVERRIDES

grapher::json_t plotter_compare_by_t::get_default_config() const {
  grapher::json_t res = grapher::base_default_config();

  // Plotter name
  res["plotter"] = "compare_by";

  res["key_ptrs"] = json_t::array({"/name", "/args/detail"});
  res["value_ptr"] = "/dur";
  res["draw_average"] = true;
  res["draw_points"] = true;
  res["demangle"] = true;

  return res;
}

void plotter_compare_by_t::plot(benchmark_set_t const &bset,
                                std::filesystem::path const &dest,
                                grapher::json_t const &config) const {
  ZoneScoped;
  namespace fs = std::filesystem;

  // Config reading

  std::vector<json_t::string_t> key_strs =
      config.value("key_ptrs", json_t::array({"/name", "/args/detail"}));

  json_t::json_pointer value_ptr(config.value("value_ptr", "/dur"));

  bool draw_average = config.value("draw_average", true);
  bool draw_points = config.value("draw_points", true);
  bool demangle = config.value("demangle", true);

  std::vector<json_t::json_pointer> key_ptrs;
  std::transform(key_strs.begin(), key_strs.end(), std::back_inserter(key_ptrs),
                 [](std::string const &s) -> json_t::json_pointer {
                   return json_t::json_pointer{s};
                 });

  // Wrangling
  curve_aggregate_map_t curve_aggregate_map =
      get_bench_curves(bset, key_ptrs, value_ptr);

  // Ensure the destination folder exists
  fs::create_directories(dest);

  // Drawing, ie. unwrapping the nested maps and drawing curves + saving plots

  std::for_each(
      std::execution::par_unseq, curve_aggregate_map.begin(),
      curve_aggregate_map.end(), [&](auto const &kv) {
        ZoneScoped;
        auto const &[key, curve_aggregate] = kv;

        // Plot init
        sciplot::Plot2D plot;

        for (auto const &[bench_name, benchmark_curve] : curve_aggregate) {
          // Average curve coord vectors
          std::vector<double> x_curve;
          std::vector<double> y_curve;

          // Point coord vectors
          std::vector<double> x_points;
          std::vector<double> y_points;

          // Build point & curve vectors
          for (auto const &[x, y_vec] : benchmark_curve) {
            // Building average curve vector
            if (draw_average && !y_vec.empty()) {
              double const sum = std::reduce(y_vec.begin(), y_vec.end());
              std::size_t const n = y_vec.size();

              double const y = sum / n;

              x_curve.push_back(x);
              y_curve.push_back(y);
            }

            // Building point vector
            if (draw_points) {
              for (double y : y_vec) {
                x_points.push_back(x);
                y_points.push_back(y);
              }
            }
          }

          // Plot drawing

          if (draw_average && !x_curve.empty()) {
            // Draw average curve
            plot.drawCurve(x_curve, y_curve).label(bench_name + " average");
          }

          if (draw_points && !x_points.empty()) {
            // Draw points
            plot.drawPoints(x_points, y_points).label(bench_name + " points");
          }
        }

        save_plot(std::move(plot), dest / to_string(key, demangle), config);
      });
}

} // namespace grapher::plotters
