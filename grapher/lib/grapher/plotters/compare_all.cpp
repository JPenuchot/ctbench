#include <filesystem>
#include <fstream>
#include <numeric>
#include <string>
#include <vector>

#include <llvm/Support/raw_ostream.h>

#include <nlohmann/json.hpp>

#include <sciplot/sciplot.hpp>

#include "grapher/core.hpp"
#include "grapher/plotters/compare_all.hpp"
#include "grapher/utils/config.hpp"
#include "grapher/utils/error.hpp"
#include "grapher/utils/json.hpp"
#include "grapher/utils/plot.hpp"

namespace grapher::plotters {

std::string_view plotter_compare_all_t::get_help() const {
  return "Compares all traceEvents that have a matching name.";
}

grapher::json_t plotter_compare_all_t::get_default_config() const {
  grapher::json_t res = grapher::base_default_config();

  res["plotter"] = "compare";

  res["value_json_pointer"] = "/dur";
  res["draw_average"] = true;
  res["draw_points"] = true;

  res["plot_file_extension"] = grapher::json_t::array({".svg", ".png"});

  return res;
}

// Point aggregate (multiple Y coordinates)
using point_data_t = std::vector<double>;

// Curve: X -> vec<Y>
using benchmark_curve_t = map_t<std::size_t, point_data_t>;

// Benchmark name -> Curve
using curve_aggregate_t = map_t<std::string, benchmark_curve_t>;

// Feature -> Benchmark aggregate
using curve_aggregate_map_t = map_t<std::string, curve_aggregate_t>;

/// Wrangles data into a structure that's easier to work with for plotting.
curve_aggregate_map_t
get_bench_curves(benchmark_set_t const &bset,
                 grapher::json_t::json_pointer const &key_ptr,
                 grapher::json_t::json_pointer const &value_ptr) {
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
          grapher::json_t::string_t const &key =
              json_at_ref<grapher::json_t::string_t const &>(event, key_ptr);

          double value = json_at(event, value_ptr);

          res[key][bench_case.name][iteration.size].push_back(value);
        } // sample_json["traceEvents"]
      }   // iteration.samples
    }     // bench_case.iterations
  }       // bset

  return res;
}

void plotter_compare_all_t::plot(benchmark_set_t const & /*bset*/,
                                 std::filesystem::path const & /*dest*/,
                                 grapher::json_t const &config) const {
  // Config

  grapher::json_t::json_pointer value_json_pointer(
      json_at_ref<json_t::string_t const &>(config, "value_json_pointer"));

  // bool draw_average = config.value("draw_average", true);
  // bool draw_points = config.value("draw_points", true);

  std::vector<std::string> plot_file_extensions = config.value(
      "plot_file_extensions", grapher::json_t::array({".svg", ".png"}));

  std::string key_ptr = config.value("key_ptr", "/name");

  std::vector<group_descriptor_t> group_descriptors = read_descriptors(
      json_at_ref<json_t::array_t const &>(config, "group_descriptors"));
}

} // namespace grapher::plotters
