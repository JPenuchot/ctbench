#pragma once

#include <filesystem>

#include <grapher/core.hpp>

namespace grapher {

/// Generates graphs for a collection of categories
/// and outputs them the given directory.
void graph(category_t const &, std::filesystem::path const &p);

} // namespace grapher
