#!/bin/sh -l

mkdir build

cmake --preset release -G Ninja \
  -DCTBENCH_ENABLE_TESTS=ON \
  -DCTBENCH_ENABLE_TRACY=OFF \
  -DCTBENCH_ENABLE_DOCS=OFF

cmake --build --preset release --target test-all
