#pragma once

/// \file
/// Display/draw graphs.

#include <filesystem>

#include <grapher/core.hpp>

namespace grapher {

/// Generates graphs for a collection of categories
/// and outputs them the given directory.
void graph(category_t const &cat,
           std::vector<std::string_view> const &feature_set,
           std::filesystem::path const &dest);

} // namespace grapher
