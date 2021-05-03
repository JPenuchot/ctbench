#include "grapher/core.hpp"

#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <string>

#include <grapher/display.hpp>
#include <sciplot/sciplot.hpp>

// Feature list:
// - execute_compiler
// - frontend
// - source
// - instantiate_function
// - parse_class
// - instantiate_class
// - backend
// - opt_module
// - parse_template
// - opt_function
// - run_pass
// - per_module_passes
// - perform_pending_instantiations
// - run_loop_pass
// - code_gen_passes
// - code_gen_function
// - per_function_passes

namespace grapher {

/// Returns plot and max measurement for plot normalization across a category
std::tuple<sciplot::Plot, measure_t> make_plot(benchmark_t const &cat) {
  namespace sp = sciplot;

  // Select what to plot:
  auto const measures = {
      execute_compiler_v,
      // frontend_v,
      // backend_v,
      // run_pass_v,
      // source_v,
      // instantiate_function_v,
      // parse_class_v,
      // instantiate_class_v,
      // opt_module_v,
      // parse_template_v,
      // opt_function_v,
      // per_module_passes_v,
      // perform_pending_instantiations_v,
      // run_loop_pass_v,
      // code_gen_passes_v,
      // code_gen_function_v,
      // per_function_passes_v,
  };

  auto const &[name, entries] = cat;
  sp::Plot plot;

  // Adjust if graph doesn't fit or looks weird
  constexpr std::size_t plot_w = 1000;
  constexpr std::size_t plot_h = 500;

  plot.legend().atOutsideRightTop().title("Time scopes:");
  plot.size(plot_w, plot_h);
  plot.xlabel("Benchmark Size Factor");
  plot.ylabel("Time (µs)");

  using vec = std::vector<measure_t>;

  vec x;

  for (auto const &e : entries) {
    x.push_back(e.size);
  }

  // No entries ? No plot
  if (x.empty()) {
    std::cerr << "Empty plot for category " << name << ".\n";
    std::exit(1);
  }

  // Initializing vectors for incremental curve drawing
  vec ylow(x.size());
  vec yhigh(x.size(), 0.);
  measure_t y_max = 0.;

  for (measure_kind_t measure_kind : measures) {
    // Previous high becomes new low
    std::swap(ylow, yhigh);

    // Summing up measurements
    std::transform(entries.begin(), entries.end(), ylow.begin(), yhigh.begin(),
                   [&](auto const &ehigh, auto const &mlow) {
                     const measure_t val =
                         mlow + get_measure(ehigh, measure_kind);
                     y_max = std::max(y_max, val);
                     return val;
                   });

    // Draw
    plot.drawWithVecs("filledcurves", x, ylow, yhigh)
        .label(std::string(get_measure_name(measure_kind)));
  }

  return {std::move(plot), y_max};
}

void graph(category_t const &cat, std::filesystem::path const &p) {
  namespace sp = sciplot;

  std::vector<std::tuple<sp::Plot, std::string>> plots;
  measure_t category_max = 0.;

  for (benchmark_t const &bench : cat) {
    auto const &[name, benchmark] = bench;
    if (benchmark.empty()) {
      std::cout << "Warning: category " << name << " is empty.\n";
      continue;
    }

    auto [plot, plot_max] = make_plot(bench);

    category_max = std::max(category_max, plot_max);

    plots.push_back({plot, name});
  }

  for (auto &[plot, name] : plots) {
    plot.yrange(0., double(category_max));
    std::filesystem::create_directories(p);
    plot.save(p / (name + ".svg"));
  }
}

} // namespace grapher
