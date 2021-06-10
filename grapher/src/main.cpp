#include <cctype>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <tuple>

#include <grapher/core.hpp>
#include <grapher/display.hpp>
#include <grapher/iostream.hpp>

int main(int argc, char const *argv[]) {
  namespace fs = std::filesystem;

  constexpr int exec_id = 0;
  constexpr int out_id = 1;
  constexpr int res_begin_id = 2;
  const int res_end_id = argc;

  if (res_end_id <= res_begin_id) {
    std::cout << "Usage: " << argv[0] << " [OUTPUT] [INPUT FILES...]\n";
    return 1;
  }

  grapher::category_t category;

  // Iterating through directories
  for (int i = res_begin_id; i < res_end_id; i++) {
    fs::path bench_path(argv[i]);

    if (!fs::is_directory(bench_path)) {
      std::cout << "[WARNING] Not a directory: " << bench_path << '\n';
      continue;
    }

    grapher::benchmark_t bench;
    bench.name = bench_path.filename();

    // Reading files
    for (auto const &f : fs::directory_iterator(bench_path)) {
      grapher::entry_t entry;

      // Basic file property check
      if (!fs::is_regular_file(f)) {
        std::cout << "[INFO] Irregular file: " << f << '\n';
        continue;
      }

      // Getting size from stem
      if (std::istringstream maybe_size(f.path().stem());
          !(maybe_size >> entry.size)) {
        std::cout << "[INFO] Invalid file name: " << f << '\n';
        continue;
      }

      // Getting data from file
      std::ifstream file(f.path());
      if (!file || !(file >> entry.data)) {
        std::cout << "[INFO] Invalid data: " << f << '\n';
        continue;
      }

      bench.entries.push_back(std::move(entry));
    }
  }

  // Output
  grapher::graph(category, { "Total ParseClass" }, argv[out_id]);

  return 0;
}
