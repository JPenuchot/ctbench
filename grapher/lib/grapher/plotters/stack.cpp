#include <algorithm>
#include <filesystem>
#include <numeric>

#include <llvm/Support/raw_ostream.h>

#include <fmt/core.h>

#include <sciplot/Plot2D.hpp>
#include <sciplot/sciplot.hpp>

#include <grapher/core.hpp>
#include <grapher/plotters/stack.hpp>
#include <grapher/predicates.hpp>
#include <grapher/utils/error.hpp>
#include <grapher/utils/json.hpp>

namespace grapher::plotters {

grapher::json_t plotter_stack_t::get_default_config() const {
  grapher::json_t res = grapher::base_default_config();

  res["plotter"] = "stack";
  res["value_json_pointer"] = "/dur";
  res["group_descriptors"] =
      write_descriptors({get_default_group_descriptor()});

  return res;
}

void plotter_stack_t::plot(benchmark_set_t const &bset,
                           std::filesystem::path const &dest,
                           grapher::json_t const &config) const {
  // Config reading

  grapher::json_t::json_pointer feature_value_jptr(
      config.value("value_json_pointer", "/dur"));

  std::vector<group_descriptor_t> descriptors = read_descriptors(
      get_as_ref<json_t::array_t const &>(config, "group_descriptors"));

  // Drawing

  std::vector<sciplot::Plot2D> plots;

  // Storing max y value for normalization
  grapher::value_t max_y_val = 0.;

  /// Draws a stacked curve graph for a given benchmark
  auto draw_plot = [&](benchmark_case_t const &bench) -> sciplot::Plot2D {
    sciplot::Plot2D plot;
    apply_config(plot, config);

    // x axis
    std::vector<grapher::value_t> x_axis;
    std::transform(bench.instances.begin(), bench.instances.end(),
                   std::back_inserter(x_axis),
                   [](benchmark_instance_t const &element) -> grapher::value_t {
                     return element.size;
                   });

    // Low y axis
    std::vector<grapher::value_t> y_low(x_axis.size(), 0.);
    // High y axis
    std::vector<grapher::value_t> y_high(x_axis.size());

    for (group_descriptor_t const &descriptor : descriptors) {
      // Storing previous value as we iterate
      std::string curve_name = descriptor.name;

      std::vector<predicate_t> extracted_predicates =
          get_predicates(descriptor);

      for (std::size_t i = 0; i < bench.instances.size(); i++) {
        benchmark_instance_t const &instance = bench.instances[i];
        std::vector<grapher::value_t> const values = filtered_values_sums(
            instance, extracted_predicates, feature_value_jptr);

        check(values.empty(),
              fmt::format("No event matching descriptor {} in benchmark {} "
                          "with instance size {}.\n",
                          descriptor.name, bench.name, instance.size));

        // TODO: Get better stats (standard deviation, etc...)
        grapher::value_t const y_val =
            y_low[i] +
            std::reduce(values.begin(), values.end()) / values.size();

        y_high[i] = y_val;

        // Update max_val for y-azis normalization
        max_y_val = std::max(max_y_val, y_val);
      }

      plot.drawCurvesFilled(x_axis, y_low, y_high).label(std::move(curve_name));

      // Swapping
      std::swap(y_low, y_high);
    }

    plot.legend().displayStartFromLast();
    return plot;
  };

  // Drwaing...
  std::transform(bset.begin(), bset.end(), std::back_inserter(plots),
                 draw_plot);

  // Normalize & save
  std::filesystem::create_directories(dest);
  for (std::size_t i = 0; i < bset.size(); i++) {
    plots[i].yrange(0., double(max_y_val));
    save_plot(plots[i], dest / bset[i].name, config);
  }
}

} // namespace grapher::plotters
