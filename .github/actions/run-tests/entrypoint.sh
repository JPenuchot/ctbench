#!/bin/sh -l

pacman -Ql llvm

mkdir build
cmake -S . -B build -G Ninja
cmake --build build --target test-all
