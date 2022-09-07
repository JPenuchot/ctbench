#!/bin/sh -l

mkdir build

cmake -S . -B build -G Ninja \
  -DBUILD_TESTING=ON \
  -DENABLE_DOCS=OFF

cmake --build build --target test-all
