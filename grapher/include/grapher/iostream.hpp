#pragma once

/// \file
/// All things related to data output to various formats.

#include <grapher/core.hpp>

#include <ostream>

namespace grapher {

/// Outputs a bunch of categories as CSV.
std::ostream &to_csv(std::ostream &o, category_t const &cat);

} // namespace grapher
