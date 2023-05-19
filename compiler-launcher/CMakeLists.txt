add_executable(ctbench-compiler-launcher compiler-launcher.cpp)

target_link_libraries(ctbench-compiler-launcher PRIVATE
  ctbench-compile-opts
  fmt::fmt
  nlohmann_json::nlohmann_json
  Boost::boost)

target_include_directories(ctbench-compiler-launcher PRIVATE include)
