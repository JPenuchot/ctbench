FROM archlinux/archlinux:base-devel

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
  llvm \
  llvm-libs \
  ninja \
  nlohmann-json \
  tar \
  tbb \
  unzip \
  zip

WORKDIR /tmp

# Run tests

WORKDIR /
COPY entrypoint.sh /entrypoint.sh
ARG CXX_COMPILER=clang++
ENTRYPOINT ["/entrypoint.sh"]