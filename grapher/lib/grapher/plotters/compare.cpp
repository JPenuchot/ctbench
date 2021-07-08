#include <algorithm>

#include <ios>
#include <nlohmann/detail/json_pointer.hpp>
#include <sciplot/sciplot.hpp>

#include <grapher/json-utils.hpp>
#include <grapher/plot-utils.hpp>

#include <grapher/plotters/compare.hpp>

namespace grapher {

void plotter_compare_t::plot(category_t const &cat,
                               std::filesystem::path const &dest,
                               nlohmann::json const &config) const {

  // TODO: Error management
  std::vector<nlohmann::json> matcher_set = config["matchers"];

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

      for (auto const &[size, data] : bench.entries) {
        // TODO: Maybe get better stats (standard deviation, etc...) ?
        std::optional<double> const val =
            get_average(data, matcher, feature_value_jptr);

        if (val) {
          x.push_back(size);
          y.push_back(*val);
        }

        // Setting feature name if not already done
        if (filename.empty()) {
          filename = get_feature_name(data, feature_name_jptr)
                         .value_or(std::move(filename));
        }
      }

      plot.drawCurve(x, y).label(bench.name);
    }

    if (filename.empty()) {
      // TODO: Add warning
      continue;
    }

    plot.save(dest / (std::move(filename) + ".svg"));
  }
}

std::string_view plotter_compare_t::get_help() const { return ""; }

nlohmann::json plotter_compare_t::get_default_config() const {
  return grapher::get_default_config();
}

} // namespace grapher
