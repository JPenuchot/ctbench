#pragma once

#include <filesystem>
#include <optional>
#include <tuple>
#include <vector>

#include <nlohmann/json.hpp>

#include <grapher/core.hpp>

namespace grapher {

/// Raw entry (before proper extraction in an entry_t type)
using raw_entry_t = std::tuple<std::filesystem::path, nlohmann::json>;

/// Raw category
using raw_category_t = std::vector<entry_t>;

/// Extracts either a single time-trace report or a directory into a vector
/// of time-trace entries.
std::optional<category_t> extract_category(std::filesystem::path const &cat);

} // namespace grapher
