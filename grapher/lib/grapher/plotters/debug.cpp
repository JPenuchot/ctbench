#include "grapher/plotters/debug.hpp"
#include <llvm/Support/raw_ostream.h>
#include <string_view>

namespace grapher::plotters {

std::string_view plotter_debug_t::get_help() const {
  return "Debug plotter. Outputs various statistics on benchmark categories to "
         "debug category building or traversal issues.";
}

nlohmann::json plotter_debug_t::get_default_config() const { return {}; }

void plotter_debug_t::plot(const category_t &cat,
                           const std::filesystem::path &dest,
                           const nlohmann::json &config) const {
  llvm::outs() << "Output path: " << dest << '\n';
  llvm::outs() << "Category size: " << cat.size() << '\n';

  for (benchmark_t const &bench : cat) {
    llvm::outs() << "\tBenchmark name: " << bench.name << '\n';
    llvm::outs() << "\tEntries: " << bench.entries.size() << '\n';

    for (entry_t const &entry : bench.entries) {
      llvm::outs() << "\t\tEntry size: " << entry.size << '\n';
      llvm::outs() << "\t\tIterations: " << entry.data.size() << '\n';
      llvm::outs() << '\n';
    }
    llvm::outs() << '\n';
  }
}

} // namespace grapher::plotters
