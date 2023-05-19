# Default options for all ctbench-related targets
add_library(ctbench-compile-opts INTERFACE)

target_compile_options(
  ctbench-compile-opts
  INTERFACE -Wall
            -Wextra
            -Werror
            -Wnull-dereference
            -Wold-style-cast
            -Wdouble-promotion
            -Wshadow
            -march=native
            -fno-rtti
            -flto
            -fPIC)

target_compile_features(ctbench-compile-opts INTERFACE cxx_std_20)
