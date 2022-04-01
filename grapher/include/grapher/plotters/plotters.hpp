#pragma once

#include <llvm/ADT/StringMap.h>
#include <llvm/ADT/StringRef.h>
#include <llvm/Support/CommandLine.h>

#include "grapher/plotters/compare.hpp"
#include "grapher/plotters/debug.hpp"
#include "grapher/plotters/grouped_histogram.hpp"
#include "grapher/plotters/stack.hpp"

namespace grapher {

/// Plotter type enumeration. One per plotter.
enum plotter_type_t {
  compare_v,
  debug_v,
  grouped_histogram_v,
  stack_v,
};

/// String to plotter type map.
inline const std::map<std::string, plotter_type_t> plotter_name_map = {
    {"compare", compare_v},
    {"debug", debug_v},
    {"grouped_histogram", grouped_histogram_v},
    {"stack", stack_v},
};

/// ValuesClass object to use for CLI parsing.
inline const llvm::cl::ValuesClass plotter_cl_values{
    llvm::cl::OptionEnumValue{"compare", compare_v,
                              "Compare benchmarks feature by feature."},
    llvm::cl::OptionEnumValue{"debug", debug_v,
                              "Output category stats for debug."},
    llvm::cl::OptionEnumValue{"grouped_histogram", grouped_histogram_v,
                              "Stack features for groups of symbols."},
    llvm::cl::OptionEnumValue{"stack", stack_v,
                              "Stack features for each benchmark."}};

/// Converts a plotter type to a plotter.
inline plotter_t plotter_type_to_plotter(plotter_type_t type) {
  switch (type) {
  case compare_v:
    return std::make_unique<grapher::plotters::plotter_compare_t>();
  case debug_v:
    return std::make_unique<grapher::plotters::plotter_debug_t>();
  case grouped_histogram_v:
    return std::make_unique<grapher::plotters::plotter_grouped_histogram_t>();
  case stack_v:
    return std::make_unique<grapher::plotters::plotter_stack_t>();
  }

  return std::make_unique<grapher::plotters::plotter_debug_t>();
}

/// Converts a string to a plotter type.
plotter_type_t string_to_plotter_type(std::string const &name);

/// String to parser
plotter_t string_to_plotter(std::string const &name);

} // namespace grapher
