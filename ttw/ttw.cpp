/// \file
/// This program is supposed to be used as a launcher for clang.
/// It accepts a time-trace export path followed by a compile command.
///
/// If `-ftime-trace` is passed to the compiler, ttw will run the command
/// and copy the time-trace file to the export path.
///
/// Otherwise, it will run the command, measure its execution time with
/// `getrusage`, and generate a time-trace file with only the compiler execution
/// time. This allows comparing compiler execution times between GCC and Clang.

#include <chrono>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string_view>

#include <sys/resource.h>

#include <nlohmann/json.hpp>

inline int get_timetrace_file(std::filesystem::path const time_trace_file_dest,
                              std::string cmd,
                              std::filesystem::path compile_obj_path,
                              bool time_trace_flag) {
  namespace fs = std::filesystem;

  // Run program and measure CPU time
  rusage children_rusage_begin, children_rusage_end;
  getrusage(RUSAGE_CHILDREN, &children_rusage_begin);
  std::system(cmd.c_str()); // TODO: Bypass shell
  getrusage(RUSAGE_CHILDREN, &children_rusage_end);

  // Create destination directory
  if (auto const out_parent = time_trace_file_dest.parent_path();
      !out_parent.empty()) {
    fs::create_directories(time_trace_file_dest.parent_path());
  }

  if (time_trace_flag) {
    // Copy trace-time file if already generated
    fs::copy_file(compile_obj_path.replace_extension(".json"),
                  time_trace_file_dest, fs::copy_options::overwrite_existing);

  } else {
    // Generate time-trace file if not already generated
    namespace nl = nlohmann;

    std::size_t const time_micros = children_rusage_end.ru_utime.tv_usec -
                                    children_rusage_begin.ru_utime.tv_usec;

    nl::json time_trace_json;
    time_trace_json["traceEvents"] = nlohmann::json::array();
    time_trace_json["traceEvents"].push_back(
        nl::json({{"name", "ExecuteCompiler"}, {"dur", time_micros}}));

    // Write file
    std::ofstream(time_trace_file_dest) << time_trace_json;
  }

  return 0;
}

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

  // Building command and finding obj_path
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

  return get_timetrace_file(argv[path_id], cmd_builder.str(),
                            std::move(obj_path), has_time_trace_flag);
}
