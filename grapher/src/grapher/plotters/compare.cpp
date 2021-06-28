#include <algorithm>

#include <sciplot/sciplot.hpp>

#include <grapher/json-utils.hpp>
#include <grapher/plot-utils.hpp>

#include <grapher/plotters/compare.hpp>

namespace grapher {

void comparative_graph_t::operator()(
    category_t const &cat, std::filesystem::path const &dest,
    std::vector<nlohmann::json> const &matcher_set,
    nlohmann::json::json_pointer feature_value_jptr,
    nlohmann::json::json_pointer feature_name_jptr,
    graph_config_t const &config) {

  namespace sp = sciplot;

  for (auto const &matcher : matcher_set) {
    sp::Plot plot;
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

} // namespace grapher
