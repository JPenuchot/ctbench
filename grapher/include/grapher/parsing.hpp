#pragma once

#include <grapher/core.hpp>

namespace grapher {

/// Extracts either a single time-trace report or a directory into a vector
/// of time-trace entries.
std::vector<entry_t> extract_category(std::filesystem::path const &cat);

} // namespace grapher
