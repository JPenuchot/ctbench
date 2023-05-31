FROM archlinux/archlinux:base-devel

USER root

# Base Arch package installation

RUN pacman -Syu --noconfirm
RUN pacman -S --noconfirm \
  boost \
  boost-libs \
  catch2 \
  clang \
  cmake \
  curl \
  fmt \
  git \
  gnuplot \
  llvm \
  llvm-libs \
  ninja \
  nlohmann-json \
  onetbb \
  tar \
  unzip \
  zip

WORKDIR /tmp

# Run tests

WORKDIR /
COPY entrypoint.sh /entrypoint.sh
ARG CXX_COMPILER=clang++
ENTRYPOINT ["/entrypoint.sh"]
