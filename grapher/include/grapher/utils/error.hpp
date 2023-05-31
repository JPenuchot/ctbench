#pragma once

#include <cstdint>
#include <cstdlib>
#include <source_location>
#include <string_view>

#include <llvm/Support/raw_ostream.h>

#include <fmt/core.h>

namespace grapher {

enum error_level_t : std::uint8_t { error_v, warning_v, info_v, log_v };

inline std::string to_string(error_level_t error_level) {
  switch (error_level) {
  case error_v:
    return "ERROR";
  case warning_v:
    return "WARNING";
  case info_v:
    return "INFO";
  case log_v:
    return "LOG";
  }
  return "";
}

/// Prints a warning
inline void warn(std::string_view explain,
                 error_level_t error_level = warning_v,
                 std::source_location loc = std::source_location::current()) {
  llvm::errs() << fmt::format(
      "[{}] {}:{}:{} ({}) - {}\n", to_string(error_level), loc.file_name(),
      loc.line(), loc.column(), loc.function_name(), explain);
}

/// Error management: if the condition is false, it will print a warning or
/// error message and termineate the program if error_level is error_v.
/// Condition value is then returned (useful for warnings).
inline bool check(bool condition, std::string_view explain,
                  error_level_t error_level = error_v, int err_code = -1,
                  std::source_location loc = std::source_location::current()) {
  if (!condition) {
    warn(explain, error_level, loc);
    if (error_level == error_v) {
      std::exit(err_code);
    }
  }
  return condition;
}

} // namespace grapher
