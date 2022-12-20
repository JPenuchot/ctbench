#include <grapher/plotters/plotter_base.hpp>
#include <grapher/plotters/plotters.hpp>

namespace grapher {

plotter_type_t string_to_plotter_type(std::string const &name) {
  if (name.empty()) {
    return compare_v;
  }

  if (auto const it = plotter_name_map.find(name);
      it != plotter_name_map.end()) {
    return it->second;
  }

  return compare_v;
}

plotter_t string_to_plotter(std::string const &name) {
  return plotter_type_to_plotter(string_to_plotter_type(name));
}

} // namespace grapher
