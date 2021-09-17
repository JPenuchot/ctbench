#pragma once

#include <nlohmann/json_fwd.hpp>

#include <llvm/ADT/StringRef.h>

namespace grapher {
bool check_constraint(nlohmann::json const &constraint,
                      nlohmann::json const &value);
}
