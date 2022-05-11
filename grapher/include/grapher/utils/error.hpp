#pragma once

#include <cstdint>
#include <cstdlib>
#include <experimental/source_location>
#include <source_location>
#include <string_view>

#include <llvm/Support/raw_ostream.h>

#include <fmt/core.h>

namespace grapher {

enum error_level_t : std::uint8_t { error_v, warning_v, info_v };

inline std::string to_string(error_level_t error_level) {
  switch (error_level) {
  case error_v:
    return "ERROR";
  case warning_v:
    return "WARNING";
  case info_v:
    return "INFO";
  }
  return "";
}

/// Prints a warning
inline void warn(std::string_view explain,
                 error_level_t error_level = warning_v,
                 std::experimental::source_location loc =
                     std::experimental::source_location::current()) {
  llvm::errs() << fmt::format("{} {}, {}:{}:{} - {}\n", to_string(error_level),
                              loc.file_name(), loc.function_name(), loc.line(),
                              loc.column(), explain);
}

/// Error management: if the condition is false, it will print a warning or
/// error message and termineate the program if error_level is error_v.
inline void check(bool condition, std::string_view explain,
                  error_level_t error_level = error_v, int err_code = -1,
                  std::experimental::source_location loc =
                      std::experimental::source_location::current()) {
  if (!condition) {
    warn(explain, error_level, loc);
    if (error_level) {
      std::exit(err_code);
    }
  }
}

} // namespace grapher
