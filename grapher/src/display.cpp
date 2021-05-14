#include <grapher/core.hpp>
#include <grapher/containers.hpp>

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
sciplot::Plot graph(benchmark_t const &bench) {
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

  sp::Plot plot;

  // Adjust if graph doesn't fit or looks weird
  constexpr std::size_t plot_w = 1000;
  constexpr std::size_t plot_h = 500;

  plot.legend().atOutsideRightTop().title("Time scopes:");
  plot.size(plot_w, plot_h);
  plot.xlabel("Benchmark Size Factor");
  plot.ylabel("Time (µs)");

  // For each size
  for (std::size_t s = 0, s_end = bench.size(); s < s_end; s++) {
    // ...
  }

  //  plot.drawWithVecs("filledcurves", x, ylow, yhigh)
  //      .label(std::string(get_measure_name(measure_kind)));

  return plot;
}

void graph(category_t const &cat, std::filesystem::path const &p) {
  namespace sp = sciplot;

  /*
  std::vector<std::tuple<sp::Plot, std::string>> plots;
  measure_t category_max = 0.;

  // This will need a remake...

  for (benchmark_t const &bench : cat) {
    auto const &[name, benchmarks] = bench;
    if (benchmarks.empty()) {
      std::cout << "Warning: category " << name << " is empty.\n";
      continue;
    }

    auto [plot, plot_max] = graph(bench);

    category_max = std::max(category_max, plot_max);

    plots.push_back({plot, name});
  }

  for (auto &[plot, name] : plots) {
    plot.yrange(0., double(category_max));
    std::filesystem::create_directories(p);
    plot.save(p / (name + ".svg"));
  }
  */
}

} // namespace grapher
