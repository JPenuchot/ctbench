#include <filesystem>
#include <string>
#include <vector>

#include <llvm/Support/raw_ostream.h>

#include <nlohmann/json.hpp>

#include <sciplot/sciplot.hpp>

#include "grapher/plotters/compare.hpp"
#include "grapher/utils/config.hpp"
#include "grapher/utils/json.hpp"
#include "grapher/utils/plot.hpp"

namespace grapher::plotters {

/// Config type for plotter_compare_t
struct compare_config_t {
  bool draw_average;
  bool draw_points;

  std::string value_json_pointer;
  std::string name_json_pointer;
  std::string plot_file_extension;

  std::vector<group_descriptor_t> groups;
};

compare_config_t get_default_compare_config() {
  return compare_config_t{
      .draw_average = true,
      .draw_points = true,

      .value_json_pointer{"/dur"},
      .name_json_pointer{"/name"},
      .plot_file_extension{".svg"},

      .groups = {}, // TODO
  };
}

nlohmann::json compare_config_to_json(compare_config_t const &c) {
  nlohmann::json res;

  res["draw_average"] = c.draw_average;
  res["draw_points"] = c.draw_points;
  res["value_json_pointer"] = c.value_json_pointer;
  res["name_json_pointer"] = c.name_json_pointer;
  res["plot_file_extension"] = c.plot_file_extension;
  // res["group_descriptors"] = c.groups;

  return res;
}

compare_config_t json_to_compare_config(nlohmann::json const &j) {
  if (j.empty()) {
    return {};
  }

  compare_config_t d = get_default_compare_config();

  return compare_config_t{
      .draw_average = j.value("draw_average", d.draw_average),
      .draw_points = j.value("draw_points", d.draw_points),

      .value_json_pointer{j.value("value_json_pointer", d.value_json_pointer)},
      .name_json_pointer{j.value("name_json_pointer", d.name_json_pointer)},
      .plot_file_extension{
          j.value("plot_file_extension", d.plot_file_extension)},

      // .matchers = j.value("group_descriptors", d.matchers),
  };
}

std::string_view plotter_compare_t::get_help() const {
  return "For each matcher in the \'matchers\' JSON field, generates a graph "
         "comparing all benchmarks in the category. Each graph will be named "
         "after the feature they observe.";
}

nlohmann::json plotter_compare_t::get_default_config() const {
  nlohmann::json res = grapher::base_default_config();

  return merge_into(grapher::base_default_config(),
                    compare_config_to_json(get_default_compare_config()));
}

void plotter_compare_t::plot(benchmark_set_t const &cat,
                             std::filesystem::path const &dest,
                             nlohmann::json const &config) const {
  std::vector<group_descriptor_t> group_descriptors = read_descriptors(
      json_value<std::vector<nlohmann::json>>(config, "group_descriptors"));

  nlohmann::json::json_pointer feature_value_jptr(
      json_value<std::string>(config, "value_json_pointer"));

  nlohmann::json::json_pointer feature_name_jptr(
      json_value<std::string>(config, "name_json_pointer"));

  for (group_descriptor_t const &descriptor : group_descriptors) {
    sciplot::Plot plot;
    apply_config(plot, config);

    for (auto const &bench : cat) {
      std::vector<double> x;
      std::vector<double> y;

      for (auto const &[size, data] : bench.entries) {
        std::vector<nlohmann::basic_json<>> events =
            extract_group(descriptor, data);

        std::optional<double> const val =
            get_average(data, events, feature_value_jptr);

        if (val) {
          x.push_back(size);
          y.push_back(*val);
        }
      }

      plot.drawCurve(x, y).label(bench.name);
    }

    std::filesystem::create_directories(dest);
    plot.save(dest / (std::move(descriptor.name) +
                      config.value("plot_file_extension", ".svg")));
  }
}

} // namespace grapher::plotters
