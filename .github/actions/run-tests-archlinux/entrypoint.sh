#!/bin/sh -l

export VCPKG_DISABLE_METRICS=1

git clone https://github.com/Microsoft/vcpkg.git
./vcpkg/bootstrap-vcpkg.sh
./vcpkg/vcpkg install sciplot

cmake --preset release -G Ninja \
  -DCTBENCH_ENABLE_TESTS=ON \
  -DCTBENCH_ENABLE_TRACY=OFF \
  -DCTBENCH_ENABLE_DOCS=OFF \
  -DCMAKE_TOOLCHAIN_FILE=vcpkg/scripts/buildsystems/vcpkg.cmake \
  -DVCPKG_OVERLAY_PORTS=""

cmake --build --preset release --target test-all
