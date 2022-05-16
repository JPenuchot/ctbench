#include <sciplot/sciplot.hpp>

#include "grapher/plotters/grouped_histogram.hpp"
#include "grapher/plotters/plotters.hpp"
#include "grapher/utils/json.hpp"
#include "grapher/utils/plot.hpp"

namespace grapher::plotters {

grapher::json_t plotter_grouped_histogram_t::get_default_config() const {
  // TODO
  return grapher::base_default_config();
}

void plotter_grouped_histogram_t::plot(
    const benchmark_set_t & /* bset */,
    const std::filesystem::path & /* dest */,
    const grapher::json_t & /* config */) const {
  // TODO
}

} // namespace grapher::plotters
