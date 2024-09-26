#!/bin/sh -le

cmake --preset release \
  -DCTBENCH_ENABLE_TESTS=ON \
  -DCTBENCH_ENABLE_TRACY=OFF \
  -DCTBENCH_ENABLE_DOCS=OFF

cmake --build --preset release --target test-all
cmake --build --preset release --target install

cd example

cmake --preset release
cmake --preset release-clang
cmake --preset release-clang-tt

cmake --build --preset release
cmake --build --preset release-clang
cmake --build --preset release-clang-tt
