#include <llvm/Support/raw_ostream.h>

#include <grapher/plotters/debug.hpp>

namespace grapher::plotters {

grapher::json_t plotter_debug_t::get_default_config() const {
  grapher::json_t res;
  res["plotter"] = "debug";
  return res;
}

void plotter_debug_t::plot(const benchmark_set_t &bset,
                           const std::filesystem::path &dest,
                           const grapher::json_t & /* config */) const {
  llvm::outs() << "Output path: " << dest << '\n'
               << "Category size: " << bset.size() << '\n';

  for (benchmark_case_t const &bench : bset) {
    llvm::outs() << "\tBenchmark case name: " << bench.name << '\n'
                 << "\tIterations: " << bench.iterations.size() << '\n';

    for (benchmark_iteration_t const &iteration : bench.iterations) {
      llvm::outs() << "\t\tIteration size: " << iteration.size << '\n'
                   << "\t\tSamples: " << iteration.samples.size() << "\n\n";
    }
    llvm::outs() << '\n';
  }
}

} // namespace grapher::plotters
