#pragma once

/// \file
/// All things related to data output to various formats.

#include <ostream>

#include "grapher/core.hpp"

namespace grapher {

/// Outputs a bunch of categories as CSV.
std::ostream &to_csv(std::ostream &o, category_t const &cat);

} // namespace grapher
