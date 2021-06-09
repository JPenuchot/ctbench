#include <grapher/core.hpp>
#include <grapher/display.hpp>

#include <algorithm>
#include <iterator>
#include <string_view>

#include <nlohmann/json.hpp>

#include <sciplot/sciplot.hpp>

namespace grapher {
nlohmann::json::const_iterator find_feature(nlohmann::json const &j,
                                            std::string_view const &feature) {
  if (auto tev_it = j.find("traceEvents"); tev_it != j.end()) {
    for (auto const &e : *tev_it) {
      if (auto ename_it = e.find("name");
          ename_it != e.end() && e["name"] == feature) {
        return tev_it;
      }
    }
  }

  return j.end();
}

void graph(category_t const &cat, std::filesystem::path const &p,
           std::vector<std::string_view> const &feature_set) {

  std::vector<sciplot::Plot> plots;

  double max_val = 0.;

  /// Draws a plot for a given benchmark
  auto draw_plot = [&](benchmark_t const &bench) -> sciplot::Plot {
    namespace sp = sciplot;
    namespace nm = nlohmann;

    sp::Plot plot;

    auto const &[name, entries] = bench;

    // Adjust if graph doesn't fit or looks weird
    constexpr std::size_t plot_w = 1000;
    constexpr std::size_t plot_h = 500;

    plot.legend().atOutsideRightTop().title("Time scopes:");
    plot.size(plot_w, plot_h);
    plot.xlabel("Benchmark Size Factor");
    plot.ylabel("Time (µs)");

    std::vector<double> x;
    std::transform(entries.begin(), entries.end(), std::back_inserter(x),
                   [](entry_t const &e) -> double { return e.size; });

    std::vector<double> y_low(x.size(), 0.);
    std::vector<double> y_high(x.size());

    for (auto const &feature : feature_set) {
      // Storing previous value as we iterate
      double prev = 0.;

      for (std::size_t i = 0; i < entries.size(); i++) {
        auto const &[size, data] = entries[i];
        nm::json::const_iterator j_it = find_feature(data, feature);
        double dur = prev;

        // Ensuring the event exists and is valid
        if (j_it != data.end() || j_it->find("duration") != j_it->end()) {
          // Setting dur & updating previous value
          dur = (*j_it)["duration"];
          prev = dur;
        }

        // Doing the trace thing...
        y_high[i] = y_low[i] + dur;
      }

      // Do the thing
      plot.drawCurvesFilled(x, y_low, y_high).label(std::string(feature));

      // Swapping
      std::swap(y_low, y_high);
    }

    return plot;
  };

  std::transform(cat.begin(), cat.end(), std::back_inserter(plots), draw_plot);

  // Normalize & save
  for (auto &p : plots) {
    p.yrange(0., max_val);
    // p.save(const std::string &filename);
  }
}

} // namespace grapher
