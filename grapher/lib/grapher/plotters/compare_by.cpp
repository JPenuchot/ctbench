#include "grapher/predicates.hpp"
#include <algorithm>
#include <bits/ranges_algo.h>
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
#include <grapher/utils/math.hpp>
#include <grapher/utils/tracy.hpp>

namespace grapher::plotters {

// =============================================================================
// AUXILIARY

// Plot-friendly data structures

/// Value key type. Contains multiple values to group by a tuple of parameters
using key_t = boost::container::small_vector<std::string, 4>;

/// Point aggregate (multiple Y coordinates)
using point_data_t = std::vector<grapher::value_t>;

/// Curve: X -> vec<Y>
using benchmark_curve_t = grapher::map_t<std::size_t, point_data_t>;

/// Benchmark name -> Curve
using curve_aggregate_t = grapher::map_t<std::string, benchmark_curve_t>;

/// Feature -> Benchmark aggregate
using curve_aggregate_map_t = grapher::map_t<key_t, curve_aggregate_t>;

struct process_event_parameters_t {
  std::vector<json_t::json_pointer> const &key_pointers;
  json_t::json_pointer const &value_pointer;
  benchmark_case_t const &bench_case;
  benchmark_instance_t const &instance;
};

/// Generate a curve for a given time-trace event and stores it in output_map.
inline void process_event(curve_aggregate_map_t &output_map,
                          grapher::json_t const &event,
                          process_event_parameters_t const &parameters) {
  // Building key from JSON pointers
  key_t key;
  for (json_t::json_pointer const &key_ptr : parameters.key_pointers) {
    if (event.contains(key_ptr) && event[key_ptr].is_string()) {
      key.push_back(event[key_ptr]);
    }
  }

  // Key/value presence and type checks
  if (check(event.contains(parameters.value_pointer),
            fmt::format("No value at {}: {}",
                        parameters.value_pointer.to_string(), event.dump()),
            info_v) &&
      check(event[parameters.value_pointer].is_number(),
            fmt::format("Value at {} is not an integer: {}",
                        parameters.value_pointer.to_string(), event.dump()),
            info_v)) {
    // Storing value
    output_map[key][parameters.bench_case.name][parameters.instance.size]
        .push_back(event[parameters.value_pointer]);
  }
}

/// Scans event data at value_pointer and generates curves for each key
/// generated from key_pointers. The curves are stored in a nested map
/// structure which is far from optimal but we're limited by gnuplot's
/// performance anyway.
curve_aggregate_map_t
get_bench_curves(benchmark_set_t const &input,
                 std::vector<json_t::json_pointer> const &key_pointers,
                 json_t::json_pointer const &value_pointer,
                 std::vector<predicate_t> filters = {}) {
  ZoneScoped;
  namespace fs = std::filesystem;

  curve_aggregate_map_t res;

  // Unfolding the benchmark set data structure
  for (benchmark_case_t const &bench_case : input) {
    for (benchmark_instance_t const &instance : bench_case.instances) {
      for (fs::path const &repetition : instance.repetitions) {
        // JSON file extraction
        grapher::json_t repetition_json;
        {
          std::ifstream repetition_ifstream(repetition);
          repetition_ifstream >> repetition_json;
        }

        // Time-trace event reading
        for (grapher::json_t const &current_event :
             get_as_ref<json_t::array_t const &>(repetition_json,
                                                 "traceEvents")) {
          // Applying filter
          if (std::ranges::all_of(
                  filters,
                  [&current_event](predicate_t const &predicate) -> bool {
                    return predicate(current_event);
                  })) {
            // Event processing, ie. building the key and storing the value in
            // the imbricated map data structure
            process_event(res, current_event,
                          {.key_pointers = key_pointers,
                           .value_pointer = value_pointer,
                           .bench_case = bench_case,
                           .instance = instance});
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

  std::string result = demangle ? llvm::demangle(key[0]) : key[0];
  std::for_each(key.begin() + 1, key.end(), [&](std::string const &part) {
    result += '/';
    for (char const name_character : demangle ? llvm::demangle(part) : part) {
      result += name_character == '/' ? '_' : name_character;
    }
  });

  return result;
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
  res["draw_median"] = true;
  res["demangle"] = true;

  // Simple default filter as an example
  res["filters"] = json_t::array({grapher::json_t{
      {"type", "regex"},
      {"pointer", "/name"},
      {"regex", "*"},
  }});

  return res;
}

/// Parameter list for the generate_plot function
/// extracted into a struct for readability
struct generate_plot_parameters_t {
  std::filesystem::path const &plot_output_folder;
  grapher::json_t const &plotter_config;
  bool draw_average;
  bool draw_points;
  bool draw_median;
  bool demangle;
};

/// Function to generate one plot.
/// NB: This function must remain free of config reading logic.
inline void generate_plot(
    curve_aggregate_map_t::const_iterator::value_type aggregate_key_value,
    generate_plot_parameters_t const &parameters) {
  ZoneScoped; // Used for profiling with Tracy

  auto const &[key, curve_aggregate] = aggregate_key_value;

  // Plot init
  sciplot::Plot2D plot;

  for (auto const &[bench_name, benchmark_curve] : curve_aggregate) {
    // Average curve coord vectors
    std::vector<grapher::value_t> x_average_curve;
    std::vector<grapher::value_t> y_average_curve;

    // Median curve coord vectors
    std::vector<grapher::value_t> x_median_curve;
    std::vector<grapher::value_t> y_median_curve;

    // Point coord vectors
    std::vector<grapher::value_t> x_points;
    std::vector<grapher::value_t> y_points;

    // Build point & curve vectors
    for (auto const &[x_value, y_values] : benchmark_curve) {
      // Building average curve vector
      if (parameters.draw_average && !y_values.empty()) {
        x_average_curve.push_back(x_value);
        y_average_curve.push_back(math::average(y_values));
      }

      // Building median curve vector
      if (parameters.draw_median && !y_values.empty()) {
        x_average_curve.push_back(x_value);
        y_average_curve.push_back(math::median(y_values));
      }

      // Building point vector
      if (parameters.draw_points) {
        for (grapher::value_t y_value : y_values) {
          x_points.push_back(x_value);
          y_points.push_back(y_value);
        }
      }
    }

    // Plot drawing

    if (parameters.draw_average && !x_average_curve.empty()) {
      // Draw average curve
      plot.drawCurve(x_average_curve, y_average_curve)
          .label(bench_name + " average");
    }

    if (parameters.draw_median && !x_median_curve.empty()) {
      // Draw median curve
      plot.drawCurve(x_median_curve, y_median_curve)
          .label(bench_name + " median");
    }

    if (parameters.draw_points && !x_points.empty()) {
      // Draw points
      plot.drawPoints(x_points, y_points).label(bench_name + " points");
    }
  }

  save_plot(std::move(plot),
            parameters.plot_output_folder / to_string(key, parameters.demangle),
            parameters.plotter_config);
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
  bool draw_median = config.value("draw_median", true);
  bool demangle = config.value("demangle", true);

  std::vector<json_t::json_pointer> key_ptrs;
  std::transform(key_strs.begin(), key_strs.end(), std::back_inserter(key_ptrs),
                 [](std::string const &pointer) -> json_t::json_pointer {
                   return json_t::json_pointer{pointer};
                 });

  // Optional: filter extraction
  std::vector<predicate_t> filters;
  if (config.contains("filters") && config["filters"].is_array()) {
    grapher::json_t::array_t const &filter_json_array =
        grapher::get_as_ref<grapher::json_t::array_t const &>(config,
                                                              "filters");
    filters.reserve(filter_json_array.size());
    std::ranges::transform(filter_json_array, std::back_inserter(filters),
                           &get_predicate);
  }

  // Wrangling happens there
  curve_aggregate_map_t curve_aggregate_map =
      get_bench_curves(bset, key_ptrs, value_ptr, filters);

  // Ensure the destination folder exists
  fs::create_directories(dest);

  // Drawing, ie. unwrapping the nested maps and drawing curves + saving plots
  std::for_each(
      std::execution::par_unseq, curve_aggregate_map.begin(),
      curve_aggregate_map.end(), [&](auto const &aggregate_key_value) {
        generate_plot(aggregate_key_value, {.plot_output_folder = dest,
                                            .plotter_config = config,
                                            .draw_average = draw_average,
                                            .draw_points = draw_points,
                                            .draw_median = draw_median,
                                            .demangle = demangle});
      });
}

} // namespace grapher::plotters
