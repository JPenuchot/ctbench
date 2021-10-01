#include "grapher/constraints.hpp"

#include <functional>

#include <llvm/ADT/SmallVector.h>

#include <nlohmann/json.hpp>

#include "grapher/predicates.hpp"

namespace grapher {

llvm::SmallVector<nlohmann::json, 16>
extract(llvm::SmallVector<nlohmann::json, 16> const &events,
        llvm::SmallVector<nlohmann::json, 4> const &constraints) {
  std::vector<predicate_t> predicates;
  for(auto const& constraint : constraints)
  {
    //predicates.push_back()
  }

  return {};
}

} // namespace grapher
