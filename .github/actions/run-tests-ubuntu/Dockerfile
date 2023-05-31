FROM ubuntu:23.04

ENV DEBIAN_FRONTEND noninteractive

USER root

# Base Arch package installation

RUN apt update -y
RUN apt upgrade -y

RUN apt install -y \
  catch2 \
  clang \
  cmake \
  curl \
  git \
  gnuplot-nox \
  libboost-all-dev \
  libclang-dev \
  libfmt-dev \
  libllvm15 \
  libtbb-dev \
  libtbb12 \
  llvm \
  llvm-dev \
  ninja-build \
  nlohmann-json3-dev \
  pkg-config \
  tar \
  unzip \
  zip

WORKDIR /tmp

# Run tests

WORKDIR /
COPY entrypoint.sh /entrypoint.sh
ARG CXX_COMPILER=clang++
ENTRYPOINT ["/entrypoint.sh"]
