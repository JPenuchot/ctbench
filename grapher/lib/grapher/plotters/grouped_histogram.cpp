#include <sciplot/sciplot.hpp>

#include "grapher/plotters/grouped_histogram.hpp"
#include "grapher/plotters/plotters.hpp"
#include "grapher/utils/json.hpp"
#include "grapher/utils/plot.hpp"

namespace grapher::plotters {

std::string_view plotter_grouped_histogram_t::get_help() const {
  // TODO: Add doc
  return "TODO";
}

nlohmann::json plotter_grouped_histogram_t::get_default_config() const {
  // TODO
  return grapher::base_default_config();
}

void plotter_grouped_histogram_t::plot(const benchmark_set_t &cat,
                                       const std::filesystem::path &dest,
                                       const nlohmann::json &config) const {
  // TODO
}

} // namespace grapher::plotters
