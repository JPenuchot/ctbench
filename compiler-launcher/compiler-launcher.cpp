/// \file
/// This program is supposed to be used as a launcher for clang.
/// It accepts a time-trace export path followed by a compile command.
///
/// If `-ftime-trace` is passed to the compiler, compiler-launcher will run the
/// command and copy the time-trace file to the export path.
///
/// Otherwise, it will run the command, measure its execution time with
/// `getrusage`, and generate a time-trace file with only the compiler execution
/// time. This allows comparing compiler execution times between GCC and Clang.
///
/// Additionally, compiler-launcher accepts an `--override-compiler=<COMPILER>`
/// flag to override the current compiler, allowing CMake targets to be compiled
/// with different compilers within a single CMake build.

#include <algorithm>
#include <chrono>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <string>
#include <string_view>

#include <boost/process.hpp>

#include <nlohmann/json.hpp>

#include <fmt/core.h>

/// Concatenates an argument list into a command string
/// in which the arguments are separated by whitespaces.
inline std::string
to_command_string(std::vector<std::string> const &command_args) {
  return std::accumulate(
      command_args.begin(), command_args.end(), std::string{},
      [](std::string accumulator, std::string_view argument) {
        return std::move(accumulator) + ' ' + argument.data();
      });
}

/// Invokes the compiler, measures the execution time, and either
/// copies the time-trace file or generates one to the desired location.
/// The return value is the exit code of the compiler.
/// If the exit code is not zero, then no time-trace file is copied or
/// generated.
inline int get_timetrace_file(std::filesystem::path const time_trace_file_dest,
                              std::vector<std::string> command_args,
                              std::filesystem::path compile_obj_path,
                              bool time_trace_flag) {
  namespace fs = std::filesystem;
  namespace ch = std::chrono;

  // Run program and measure CPU time

  using exec_clock_t = ch::steady_clock;

  exec_clock_t::time_point const exec_t0 = exec_clock_t::now();
  int const exit_code = boost::process::system(command_args);
  exec_clock_t::time_point const exec_t1 = exec_clock_t::now();

  // Check child exit code
  if (exit_code != 0) {
    fmt::print("Following compile command exited with status {}: `{}`.\n",
               exit_code, to_command_string(command_args));

    // Forward exit code
    return exit_code;
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

    std::size_t const time_micros =
        ch::duration_cast<ch::microseconds>(exec_t1 - exec_t0).count();

    nl::json time_trace_json;
    time_trace_json["traceEvents"] = nlohmann::json::array();
    time_trace_json["traceEvents"].push_back(
        nl::json({{"name", "ExecuteCompiler"}, {"dur", time_micros}}));

    // Write file
    std::ofstream(time_trace_file_dest) << time_trace_json;
  }

  // Forward exit code
  return exit_code;
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

  // Help display
  if (argc < 3 ||
      std::any_of(argv, argv + argc, [](std::string_view arg) -> bool {
        return arg == "--help" || arg == "-h" || arg == "-help";
      })) {
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
  // std::string compiler_executable = argv[compiler_id];
  std::vector<std::string> command_args({argv[compiler_id]});
  fs::path obj_path;
  bool has_time_trace_flag = false;

  for (auto beg = &argv[args_start_id], end = &argv[argc]; beg < end; beg++) {
    // Current argument as a string_view
    std::string_view current_arg{*beg};

    // Handling -o flag
    if (current_arg == "-o" && beg + 1 != end) {
      obj_path = *(beg + 1);
    }

    // Handling Clang -ftime-trace flag
    else if (current_arg == "-ftime-trace" || current_arg == "--ftime-trace") {
      has_time_trace_flag = true;
    }

    // Handling --override-compiler flag
    else if (current_arg.starts_with(override_flag_prefix)) {
      current_arg.remove_prefix(override_flag_prefix.size());
      command_args[0] = current_arg;

      // Do not pass argument to the compiler
      continue;
    }

    command_args.push_back(*beg);
  }

  if (std::getenv("CTBENCH_TTW_VERBOSE") != nullptr) {
    fmt::print("[CTBENCH_TTW] Compile command: {}\n",
               to_command_string(command_args));
  }

  return get_timetrace_file(argv[path_id], std::move(command_args),
                            std::move(obj_path), has_time_trace_flag);
}
