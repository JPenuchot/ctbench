#include "grapher/plotters/compare.hpp"
#include "grapher/plotters/stacked_graph.hpp"
#include <grapher/plotters/plotters.hpp>

namespace grapher {

plotter_type_t string_to_plotter_type(llvm::StringRef const &name) {
  if (name.empty()) {
    return compare_v;
  }

  if (auto const it = plotter_name_map.find(name);
      it != plotter_name_map.end()) {
    return it->getValue();
  }

  return compare_v;
}

plotter_t plotter_type_to_plotter(plotter_type_t type) {
  switch (type) {
  case compare_v:
    return std::make_unique<grapher::comparative_graph_t>();
  case stack_v:
    return std::make_unique<grapher::stacked_graph_t>();
  }
}

plotter_t string_to_plotter(llvm::StringRef const &name) {
  return plotter_type_to_plotter(string_to_plotter_type(name));
}

} // namespace grapher
