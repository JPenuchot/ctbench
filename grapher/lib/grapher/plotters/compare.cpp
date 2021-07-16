#include <algorithm>

#include <llvm/Support/raw_ostream.h>

#include <nlohmann/detail/json_pointer.hpp>

#include <sciplot/sciplot.hpp>

#include "grapher/json-utils.hpp"
#include "grapher/plot-utils.hpp"
#include "grapher/plotters/compare.hpp"

namespace grapher {

std::string_view plotter_compare_t::get_help() const {
  return "For each matcher in the \'matchers\' JSON field, generates a graph "
         "comparing all benchmarks in the category. Each graph will be named "
         "after the feature they observe.";
}

nlohmann::json plotter_compare_t::get_default_config() const {
  nlohmann::json res = grapher::base_default_config();

  // Basic values, probably no need to change them
  res["value_json_pointer"] = "/dur";
  res["name_json_pointer"] = "/name";
  res["plot_file_extension"] = ".svg";

  // Some matchers as an example...
  res["matchers"].push_back({{"name", "Total Frontend"}});
  res["matchers"].push_back({{"name", "Total Backend"}});

  return res;
}

void plotter_compare_t::plot(category_t const &cat,
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
      // TODO: Add warning
      continue;
    }

    plot.save(dest / (std::move(filename) +
                      config.value("plot_file_extension", ".svg")));
  }
}

} // namespace grapher
