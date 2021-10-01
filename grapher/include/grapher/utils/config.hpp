#pragma once

#include <llvm/ADT/SmallString.h>
#include <llvm/ADT/SmallVector.h>

#include <nlohmann/json.hpp>

/// \file
/// Common structure and function definitions that can be used for plotter
/// configuration.

namespace grapher {

/// Named set of constraint.
struct group_t {
  llvm::SmallString<16> name;
  llvm::SmallVector<nlohmann::json, 4> constraints;
};

} // namespace grapher
