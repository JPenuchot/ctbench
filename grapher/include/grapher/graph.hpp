#pragma once

/// \file
/// Display/draw graphs.

#include <filesystem>

#include <grapher/core.hpp>

namespace grapher {

/// Generates graphs for a collection of categories
/// and outputs them the given directory.
void stacked_graph(category_t const &cat,
                   std::vector<nlohmann::json> const &matcher_set,
                   std::filesystem::path const &dest);

void comparative_graph(category_t const &cat,
                       std::vector<nlohmann::json> const &matcher_set,
                       std::filesystem::path const &dest);

} // namespace grapher
