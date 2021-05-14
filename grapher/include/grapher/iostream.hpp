#pragma once

#include <grapher/core.hpp>
#include <grapher/containers.hpp>

#include <ostream>

namespace grapher {

/// Outputs a bunch of categories as CSV.
std::ostream &to_csv(std::ostream &o, category_t const &cat);

} // namespace grapher
