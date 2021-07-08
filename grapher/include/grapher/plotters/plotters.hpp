#pragma once

#include <grapher/plotters/compare.hpp>
#include <grapher/plotters/stacked_graph.hpp>

#include <llvm/ADT/StringMap.h>
#include <llvm/ADT/StringRef.h>
#include <llvm/Support/CommandLine.h>

namespace grapher {

// TYPES

enum plotter_type_t {
  compare_v,
  stack_v,
};

// VARIABLES

/// String to plotter type map.
inline const llvm::StringMap<plotter_type_t> plotter_name_map = {
    {"compare", compare_v},
    {"stack", stack_v},
};

inline const llvm::cl::ValuesClass plotter_cl_values{
    llvm::cl::OptionEnumValue{"compare", compare_v,
                              "Compare benchmarks feature by feature."},
    llvm::cl::OptionEnumValue{"stack", stack_v,
                              "Stack features for each benchmark."}};

// FUNCTIONS

/// Converts a string to a plotter type.
plotter_type_t string_to_plotter_type(llvm::StringRef const &name);

/// Type to parser
plotter_t plotter_type_to_plotter(plotter_type_t type);

/// String to parser
plotter_t string_to_plotter(llvm::StringRef const &name);

} // namespace grapher
