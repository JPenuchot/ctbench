#include <time-trace-wrapper.hpp>

// auto parse_args(int argc, char const *argv[])
void test_parse_args() {
  // TODO
}

// int get_timetrace_file(std::filesystem::path const out, std::string cmd,
//                        std::filesystem::path obj_path)
void test_get_timetrace_file() {
  // TODO
}

void test_all() {
  test_parse_args();
  test_get_timetrace_file();
}

int main(int argc, char const *argv[]) { test_all(); }
