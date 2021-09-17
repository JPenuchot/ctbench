#include <regex>

#include <grapher/constraints.hpp>

#include <llvm/ADT/SmallString.h>
#include <llvm/Support/raw_ostream.h>

#include <nlohmann/json.hpp>

namespace grapher::constraints {

/// \defgroup constraints
/// Constraints...

/// \ingroup constraints
/// Implements the "regex" constraint.
bool regex(nlohmann::json const &constraint, nlohmann::json const &value) {
  if (!constraint.contains("pointer") || !constraint["pointer"].is_string()) {
    llvm::errs() << "[ERROR] Regex constraint has no pointer string value:\n"
                 << constraint.dump(2) << '\n';
    return false;
  }

  if (!constraint.contains("regex") || !constraint["regex"].is_string()) {
    llvm::errs() << "[ERROR] Regex constraint has no regex string value:\n"
                 << constraint.dump(2) << '\n';
    return false;
  }

  nlohmann::json::json_pointer pointer(constraint["pointer"]);

  if (!value.contains(pointer) || !value[pointer].is_string()) {
    llvm::errs() << "[ERROR] Given value has no string field at given pointer ("
                 << pointer.to_string() << "):\n"
                 << value.dump(2) << '\n';
    return false;
  }

  return std::regex_match(
      value[pointer].get_ref<nlohmann::json::string_t const &>(),
      std::regex(constraint["regex"]));
}

/// \ingroup constraints
/// Implements the "match" constraint.
bool match(nlohmann::json const &constraint, nlohmann::json const &value) {
  return true;
}

/// \ingroup constraints
/// Implements the "streq" constraint.
bool streq(nlohmann::json const &constraint, nlohmann::json const &value) {
  return true;
}

} // namespace grapher::constraints

namespace grapher {

/// \ingroup constraints
/// Checks whether a value satisfies an arbitrary constraint.
bool check_constraint(nlohmann::json const &constraint,
                      nlohmann::json const &value) {
  auto dump = [&]() { llvm::errs() << constraint.dump(2); };

  if (!constraint.contains("type")) {
    llvm::errs() << "[ERROR] No constraint type:\n";
    dump();

    return false;
  }

  if (!constraint["type"].is_string()) {
    llvm::errs() << "[ERROR] Constraint type is not a string:\n";
    dump();
    return false;
  }

  llvm::SmallString<16> type = constraint["type"];

  // Dispatch
  if (type == "regex") {
    return constraints::regex(constraint, value);
  }
  if (type == "match") {
    return constraints::match(constraint, value);
  }

  llvm::errs() << "[ERROR] Invalid constraint type:\n";
  dump();
  return false;
}

} // namespace grapher
