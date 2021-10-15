#include "grapher/plotters/debug.hpp"

#include <llvm/Support/raw_ostream.h>
#include <string_view>

namespace grapher::plotters {

std::string_view plotter_debug_t::get_help() const {
  return "Debug plotter. Outputs various statistics on benchmark categories to "
         "debug category building or traversal issues.";
}

nlohmann::json plotter_debug_t::get_default_config() const {
  nlohmann::json res;
  res["plotter"] = "debug";
  return res;
}

void plotter_debug_t::plot(const benchmark_set_t &bset,
                           const std::filesystem::path &dest,
                           const nlohmann::json &config) const {
  llvm::outs() << "Output path: " << dest << '\n';
  llvm::outs() << "Category size: " << bset.size() << '\n';

  for (benchmark_case_t const &bench : bset) {
    llvm::outs() << "\tBenchmark case name: " << bench.name << '\n'
                 << "\tIterations: " << bench.iterations.size() << '\n';

    for (benchmark_iteration_t const &iteration : bench.iterations) {
      llvm::outs() << "\t\tIteration size: " << iteration.size << '\n'
                   << "\t\tRepetitions: " << iteration.repetition_paths.size()
                   << "\n\n";
    }
    llvm::outs() << '\n';
  }
}

} // namespace grapher::plotters
