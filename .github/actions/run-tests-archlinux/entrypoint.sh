#!/bin/sh -le

export VCPKG_DISABLE_METRICS=1

git clone https://github.com/Microsoft/vcpkg.git
./vcpkg/bootstrap-vcpkg.sh
./vcpkg/vcpkg install sciplot fmt

cmake --preset release -G Ninja \
  -DCTBENCH_ENABLE_TESTS=ON \
  -DCTBENCH_ENABLE_TRACY=OFF \
  -DCTBENCH_ENABLE_DOCS=OFF \
  -DCMAKE_TOOLCHAIN_FILE=vcpkg/scripts/buildsystems/vcpkg.cmake

cmake --build --preset release --target test-all

cmake --build --preset release --target install

cd example

cmake --preset release
cmake --preset release-clang
cmake --preset release-clang-tt

cmake --build --preset release
cmake --build --preset release-clang
cmake --build --preset release-clang-tt
