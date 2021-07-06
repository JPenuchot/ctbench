#pragma once

#include <grapher/plotters/compare.hpp>
#include <grapher/plotters/stacked_graph.hpp>

#include <llvm/ADT/SmallVector.h>
#include <llvm/ADT/StringMap.h>
#include <llvm/ADT/StringRef.h>

namespace grapher {

// TYPES

enum plotter_type_t {
  compare_v,
  stack_v,
};

// VARIABLES

namespace plotters_detail {
/// \internal
/// String to plotter type map. Edit this if you're adding a plotter.
inline const llvm::StringMap<plotter_type_t> plotter_name_map = {
    {"compare", compare_v},
    {"stack", stack_v},
};
} // namespace plotters_detail

// FUNCTIONS

/// Converts a string to a plotter type.
plotter_type_t string_to_plotter_type(llvm::StringRef const &name);

/// Type to parser
plotter_t plotter_type_to_plotter(plotter_type_t type);

/// String to parser
plotter_t string_to_plotter(llvm::StringRef const &name);

} // namespace grapher
