/// -ftime-trace wrapper for clang.

#include <iostream>
#include <sstream>
#include <string>
#include <string_view>

#include <time-trace-wrapper.hpp>

/// Wrapper for a given clang command.

int main(int argc, char const *argv[]) {
  namespace fs = std::filesystem;

  constexpr int exec_id = 0;
  constexpr int path_id = 1;
  constexpr int cmd_start_id = 2;

  if (argc < 3) {
    std::cout << "Usage: " << argv[exec_id]
              << "time_trace_export_path.json COMMAND [ARGS]...\n";
    return 1;
  }

  // Building path and finding obj_path
  std::ostringstream cmd_builder;
  fs::path obj_path;
  bool has_time_trace_flag = false;

  cmd_builder << argv[cmd_start_id];
  for (auto beg = &argv[cmd_start_id + 1], end = &argv[argc]; beg < end;
       beg++) {
    // Object path finding
    if (*beg == std::string_view("-o") && (beg + 1) != end) {
      obj_path = *(beg + 1);
    }

    if (*beg == std::string_view("-ftime-trace") ||
        *beg == std::string_view("--ftime-trace")) {
      has_time_trace_flag = true;
    }
    cmd_builder << ' ' << *beg;
  }

  if (!has_time_trace_flag) {
    cmd_builder << " -ftime-trace";
  }

  return get_timetrace_file(argv[path_id], std::move(cmd_builder.str()),
                            std::move(obj_path));
}
