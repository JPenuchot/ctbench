#include <filesystem>

#include <llvm/ADT/SmallString.h>
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

  llvm::SmallString<16> value_json_pointer;
  llvm::SmallString<16> name_json_pointer;
  llvm::SmallString<16> plot_file_extension;

  llvm::SmallVector<group_t, 8> groups;
};

compare_config_t get_default_compare_config() {
  return compare_config_t{
      .draw_average = true,
      .draw_points = true,

      .value_json_pointer{"/dur"},
      .name_json_pointer{"/name"},
      .plot_file_extension{".svg"},

      .groups = {{{"name", "Total ExecuteCompiler"}},
                 {{"name", "Total Frontend"}},
                 {{"name", "Total Source"}},
                 {{"name", "Total ParseClass"}},
                 {{"name", "Total InstantiateClass"}},
                 {{"name", "Total Backend"}},
                 {{"name", "Total ParseTemplate"}},
                 {{"name", "Total OptModule"}},
                 {{"name", "Total CodeGenPasses"}},
                 {{"name", "Total PerModulePasses"}},
                 {{"name", "Total PerFunctionPasses"}},
                 {{"name", "Total PerformPendingInstantiations"}}},
  };
}

nlohmann::json compare_config_to_json(compare_config_t const &c) {
  nlohmann::json res;

  res["draw_average"] = c.draw_average;
  res["draw_points"] = c.draw_points;
  res["value_json_pointer"] = c.value_json_pointer;
  res["name_json_pointer"] = c.name_json_pointer;
  res["plot_file_extension"] = c.plot_file_extension;
  // res["groups"] = c.groups;

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

      // .matchers = j.value("matchers", d.matchers),
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
  std::vector<nlohmann::json> matcher_set;

  if (config.contains("matchers") && config["matchers"].is_array()) {
    matcher_set = std::vector<nlohmann::json>(config["matchers"]);
  } else {
    llvm::errs() << "Warning: No matcher was specified in the configuration "
                    "file. Falling back to default matchers.\n";
    matcher_set =
        std::vector<nlohmann::json>(this->get_default_config()["matchers"]);
  }

  nlohmann::json::json_pointer feature_value_jptr(
      config.value("value_json_pointer", "/dur"));

  nlohmann::json::json_pointer feature_name_jptr(
      config.value("name_json_pointer", "/name"));

  for (auto const &matcher : matcher_set) {
    sciplot::Plot plot;
    apply_config(plot, config);

    std::string filename;

    for (auto const &bench : cat) {
      std::vector<double> x;
      std::vector<double> y;

      // Setting feature name if not already done
      if (filename.empty()) {
        filename = get_feature_name(bench, matcher, feature_name_jptr)
                       .value_or(std::move(filename));
      }

      for (auto const &[size, data] : bench.entries) {
        // TODO: Maybe get better stats (standard deviation, etc...) ?
        std::optional<double> const val =
            get_average(data, matcher, feature_value_jptr);

        if (val) {
          x.push_back(size);
          y.push_back(*val);
        }
      }

      plot.drawCurve(x, y).label(bench.name);
    }

    if (filename.empty()) {
      llvm::errs() << "[WARNING] Couldn't find filename for benchmark with "
                      "following matcher:\n"
                   << matcher.dump(2) << '\n';
      continue;
    }

    std::filesystem::create_directories(dest);
    plot.save(dest / (std::move(filename) +
                      config.value("plot_file_extension", ".svg")));
  }
}

} // namespace grapher::plotters
