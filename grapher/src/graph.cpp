#include <algorithm>
#include <iterator>
#include <nlohmann/detail/json_pointer.hpp>
#include <string_view>

#include <nlohmann/json.hpp>

#include <sciplot/sciplot.hpp>

#include <grapher/core.hpp>
#include <grapher/graph.hpp>

namespace grapher {

nlohmann::json::const_iterator
find_matching(nlohmann::json::const_iterator begin,
              nlohmann::json::const_iterator end,
              nlohmann::json const &matcher) {
  auto const flat_matcher = matcher.flatten();

  return std::find_if(begin, end, [&](nlohmann::json const &j) -> bool {
    auto const flat_j = j.flatten();

    for (auto const &i : flat_matcher.items()) {
      if (auto it = flat_j.find(i.key());
          it == flat_j.end() || *it != i.value()) {
        return false;
      }
    }
    return true;
  });
}

/// Given a category, draws all the features as a stacked filled curve graph
/// for each benchmark at the given path
void stacked_graph(category_t const &cat,
                   std::vector<nlohmann::json> const &matcher_set,
                   std::filesystem::path const &dest) {

  std::vector<sciplot::Plot> plots;

  // Saving max value
  double max_val = 0.;

  /// Draws a stacked curve graph for a given benchmark
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

    for (auto const &matcher : matcher_set) {
      // Storing previous value as we iterate
      double prev = 0.;

      for (std::size_t i = 0; i < entries.size(); i++) {
        auto const &[size, data] = entries[i];
        nm::json::const_iterator j_it =
            find_matching(data.begin(), data.end(), matcher);
        double dur = prev;

        // Ensuring the event exists and is valid
        if (j_it != data.end() || j_it->find("duration") != j_it->end()) {
          // Setting dur & updating previous value
          dur = (*j_it)["dur"];
          prev = dur;
        }

        // Doing the trace thing...
        y_high[i] = y_low[i] + dur;
      }

      // Do the thing
      plot.drawCurvesFilled(x, y_low, y_high).label(std::string(matcher));

      // Swapping
      std::swap(y_low, y_high);
    }

    return plot;
  };

  // Drwaing...
  std::transform(cat.begin(), cat.end(), std::back_inserter(plots), draw_plot);

  // Normalize & save
  for (std::size_t i = 0; i < cat.size(); i++) {
    plots[i].yrange(0., max_val);
    plots[i].save(dest / (cat[i].name + ".svg"));
  }
}

void comparative_graph(category_t const &cat,
                       std::vector<nlohmann::json> const &matcher_set,
                       nlohmann::json::pointer const &feature_pointer,
                       std::filesystem::path const &dest) {
  namespace sp = sciplot;
  for (auto const &matcher : matcher_set) {
    sp::Plot plot;

    // Adjust if graph doesn't fit or looks weird
    constexpr std::size_t plot_w = 1000;
    constexpr std::size_t plot_h = 500;

    plot.legend().atOutsideRightTop().title("Benchmark:");
    plot.size(plot_w, plot_h);
    plot.xlabel("Benchmark Size Factor");
    plot.ylabel("Time (µs)");

    for (auto const &bench : cat) {
      std::vector<double> x;
      std::vector<double> y;

      for (auto const &[size, data] : bench.entries) {
        auto const it = find_matching(data.begin(), data.end(), matcher);
        // Data integrity checks

        if (it == data.end()) {
          // TODO: Add warning
          continue;
        }

        if (!it->contains(feature_pointer)) {
          // TODO: Add warning
          continue;
        }

        if (!(*it)[feature_pointer].is_number()) {
          // TODO: Add warning
          continue;
        }

        x.push_back(size);
        y.push_back((*it)[feature_pointer]);
      }

      plot.drawCurve(x, y).label(bench.name);
    }

    // Saveeee
  }
}

} // namespace grapher
