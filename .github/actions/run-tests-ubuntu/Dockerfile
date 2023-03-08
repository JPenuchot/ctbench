FROM ubuntu:23.04

ENV DEBIAN_FRONTEND noninteractive

# Base Arch package installation

RUN apt update -y
RUN apt upgrade -y

RUN apt install -y \
  catch2 \
  clang \
  cmake \
  git \
  libboost-all-dev \
  libbtbb-dev \
  libclang-dev \
  libfmt-dev \
  libllvm15 \
  libtbb-dev \
  libtbb12 \
  llvm \
  llvm-dev \
  ninja-build \
  nlohmann-json3-dev

WORKDIR /tmp

# Sciplot installation
ARG SCIPLOT_VERSION=v0.3.1

RUN git clone https://github.com/sciplot/sciplot.git -b $SCIPLOT_VERSION sciplot
WORKDIR /tmp/sciplot
RUN mkdir build
RUN cmake -S . -B build \
  -DSCIPLOT_BUILD_EXAMPLES=OFF \
  -DSCIPLOT_BUILD_TESTS=OFF \
  -DSCIPLOT_BUILD_DOCS=OFF
RUN cmake --build build --target install

# Run tests

WORKDIR /
COPY entrypoint.sh /entrypoint.sh
ARG CXX_COMPILER=clang++
ENTRYPOINT ["/entrypoint.sh"]
