#include <llvm/Support/raw_ostream.h>

#include <fstream>
#include <memory>

#include <grapher/core.hpp>
#include <grapher/plotters/debug.hpp>

namespace grapher::plotters {

grapher::json_t plotter_debug_t::get_default_config() const {
  grapher::json_t res;
  res["plotter"] = "debug";
  res["disable_console_output"] =
      "Console output is disabled if this field is present.";
  return res;
}

void plotter_debug_t::plot(const benchmark_set_t &input_benchmark_set,
                           const std::filesystem::path &destination_file,
                           const grapher::json_t &config) const {
  grapher::json_t json_result;

  // Writing JSON description fields

  json_result["output_path"] = destination_file.string();
  json_result["category_size"] = input_benchmark_set.size();

  for (benchmark_case_t const &current_bench : input_benchmark_set) {
    // Writing benchmark size

    grapher::json_t &current_bench_json =
        json_result["benchmarks"][current_bench.name];
    current_bench_json["instance_count"] = current_bench.instances.size();

    for (benchmark_instance_t const &current_instance :
         current_bench.instances) {
      // Writing instance properties (repetitions and size)

      grapher::json_t &current_instance_json =
          json_result["benchmarks"][current_bench.name]["instances"]
          [std::to_string(current_instance.size)];

      current_instance_json["repetitions"] =
          current_instance.repetitions.size();
      current_instance_json["instanciation_size"] = current_instance.size;
    }
  }

  // Write debug file
  std::ofstream(destination_file) << json_result.dump(2);

  // Optionally: console output
  if (!config.contains("disable_console_output")) {
    // Console output
    llvm::outs() << json_result.dump(2);
  }
}

} // namespace grapher::plotters
