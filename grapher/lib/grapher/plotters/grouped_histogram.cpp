#include "grapher/plotters/grouped_histogram.hpp"

#include <llvm/Support/raw_ostream.h>
#include <string_view>

namespace grapher::plotters {

std::string_view plotter_grouped_histogram_t::get_help() const {
  return "TODO";
}

nlohmann::json plotter_grouped_histogram_t::get_default_config() const {
  return {};
}

void plotter_grouped_histogram_t::plot(const category_t &cat,
                                       const std::filesystem::path &dest,
                                       const nlohmann::json &config) const {
  // ...
}

} // namespace grapher::plotters
