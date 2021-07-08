#pragma once

#include <llvm/ADT/StringMap.h>
#include <llvm/ADT/StringRef.h>
#include <llvm/Support/CommandLine.h>

#include "grapher/plotters/compare.hpp"
#include "grapher/plotters/stack.hpp"

namespace grapher {

/// Plotter type enumeration. One per plotter.
enum plotter_type_t {
  compare_v,
  stack_v,
};

/// String to plotter type map.
inline const llvm::StringMap<plotter_type_t> plotter_name_map = {
    {"compare", compare_v},
    {"stack", stack_v},
};

/// ValuesClass object to use for CLI parsing.
inline const llvm::cl::ValuesClass plotter_cl_values{
    llvm::cl::OptionEnumValue{"compare", compare_v,
                              "Compare benchmarks feature by feature."},
    llvm::cl::OptionEnumValue{"stack", stack_v,
                              "Stack features for each benchmark."}};

/// Converts a plotter type to a plotter.
inline plotter_t plotter_type_to_plotter(plotter_type_t type) {
  switch (type) {
  case compare_v:
    return std::make_unique<grapher::plotter_compare_t>();
  case stack_v:
    return std::make_unique<grapher::plotter_stack_t>();
  }
}

/// Converts a string to a plotter type.
plotter_type_t string_to_plotter_type(llvm::StringRef const &name);

/// String to parser
plotter_t string_to_plotter(llvm::StringRef const &name);

} // namespace grapher
