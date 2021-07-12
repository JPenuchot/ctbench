#pragma once

#include <grapher/core.hpp>

#include <nlohmann/json.hpp>

#include <llvm/Support/CommandLine.h>

namespace grapher {

/// Builds a category from a list of string arguments that contains paths to
/// benchmarks.
grapher::category_t build_category(llvm::cl::list<std::string> const &benchmark_path_list);

} // namespace grapher
