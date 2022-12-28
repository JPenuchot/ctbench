#include <grapher/plotters/plotter_base.hpp>
#include <grapher/plotters/plotters.hpp>

namespace grapher {

plotter_type_t string_to_plotter_type(std::string const &name) {
  if (name.empty()) {
    return compare_v;
  }

  if (auto const name_iterator = plotter_name_map.find(name);
      name_iterator != plotter_name_map.end()) {
    return name_iterator->second;
  }

  return compare_v;
}

plotter_t string_to_plotter(std::string const &name) {
  return plotter_type_to_plotter(string_to_plotter_type(name));
}

} // namespace grapher
