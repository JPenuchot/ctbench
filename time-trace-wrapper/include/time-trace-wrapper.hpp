#pragma once

#include <filesystem>

int get_timetrace_file(std::filesystem::path const out, std::string cmd,
                       std::filesystem::path obj_path) {
  namespace fs = std::filesystem;

  // Run program
  std::system(cmd.c_str());
  if (auto const out_parent = out.parent_path(); !out_parent.empty()) {
    fs::create_directories(out.parent_path());
  }

  fs::copy_file(obj_path.replace_extension(".json"), out,
                fs::copy_options::overwrite_existing);

  return 0;
}

auto parse_args(int argc, char const *argv[]) {
  namespace fs = std::filesystem;

  constexpr int exec_id = 0;
  constexpr int path_id = 1;
  constexpr int cmd_beg_id = 2;

  std::ostringstream cmd_builder;
  fs::path obj_path;
  bool has_time_trace_flag = false;

  cmd_builder << argv[cmd_beg_id];
  for (auto beg = &argv[cmd_beg_id + 1], end = &argv[argc]; beg < end; beg++) {
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

  return std::tuple(cmd_builder.str(), obj_path);
}
