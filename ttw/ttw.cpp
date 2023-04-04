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
///
/// Additionally, ttw accepts an `--override-compiler=<COMPILER>` flag to
/// override the current compiler, allowing CMake targets to be compiled with
/// different compilers within a single CMake build.

#include <chrono>
#include <cstdlib>
#include <ctime>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <string_view>

#include <sys/resource.h>
#include <sys/wait.h>

#include <nlohmann/json.hpp>

#include <fmt/core.h>

inline int get_timetrace_file(std::filesystem::path const time_trace_file_dest,
                              std::string compile_command,
                              std::filesystem::path compile_obj_path,
                              bool time_trace_flag) {
  namespace fs = std::filesystem;

  // Run program and measure CPU time
  rusage children_rusage_begin;
  rusage children_rusage_end;

  getrusage(RUSAGE_CHILDREN, &children_rusage_begin);
  // TODO: Bypass shell call?
  int const ret = std::system(compile_command.c_str());
  getrusage(RUSAGE_CHILDREN, &children_rusage_end);

  // Check child exit status
  if (int const exit_status = WEXITSTATUS(ret); exit_status != 0) {
    fmt::print("Following compile command exited with status {}: `{}`.\n",
               exit_status, compile_command);
    exit(exit_status);
  };

  // Create destination directory
  if (std::filesystem::path const out_parent =
          time_trace_file_dest.parent_path();
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

  // Forward return value
  return ret;
}

/// Wrapper for a compiler command.
/// It allows catching compiler flags to retrieve Clang's time-trace reports or
/// override the current compiler.

int main(int argc, char const *argv[]) {
  namespace fs = std::filesystem;

  // Argument ranks
  constexpr int exec_id = 0;
  constexpr int path_id = 1;
  constexpr int compiler_id = 2;
  constexpr int args_start_id = 3;

  // Override flag prefix
  constexpr std::string_view override_flag_prefix = "--override-compiler=";

  if (argc < 3) {
    fmt::print("Usage: {} time_trace_export_path.json COMPILER [ARGS]...\n\n"
               "{} <COMPILER> - Override previously set compiler\n\n"
               "If CTBENCH_TTW_VERBOSE is set, "
               "the program will display the compile command.\n",
               argv[exec_id], override_flag_prefix);
    return 1;
  }

  // Building command and finding obj_path

  // Compiler exec comes after the wrapper exec and the destination for the
  // time-trace file, as these two are set as compiler launcher by the CMake
  // boilerplate.
  std::string compiler_executable = argv[compiler_id];
  std::ostringstream args_builder;
  fs::path obj_path;
  bool has_time_trace_flag = false;

  for (auto beg = &argv[args_start_id], end = &argv[argc]; beg < end; beg++) {
    // Current argument as a string_view
    std::string_view current_argument{*beg};

    // Handling -o flag
    if (current_argument == std::string_view("-o") && beg + 1 != end) {
      obj_path = *(beg + 1);
    }

    // Handling Clang -ftime-trace flag
    else if (current_argument == "-ftime-trace" ||
             current_argument == "--ftime-trace") {
      has_time_trace_flag = true;
    }

    // Handling --override-compiler flag
    else if (current_argument.starts_with(override_flag_prefix)) {
      current_argument.remove_prefix(override_flag_prefix.size());
      compiler_executable = current_argument;

      // Do not pass argument to the compiler
      continue;
    }

    args_builder << ' ' << *beg;
  }

  std::string compile_command =
      std::move(compiler_executable) + args_builder.str();

  if (std::getenv("CTBENCH_TTW_VERBOSE") != nullptr) {
    fmt::print("[CTBENCH_TTW] Compile command: {}\n", compile_command);
  }

  return get_timetrace_file(argv[path_id], std::move(compile_command),
                            std::move(obj_path), has_time_trace_flag);
}
