FROM archlinux/archlinux:base-devel

# Base Arch package installation

RUN pacman -Syu --noconfirm
RUN pacman -S --noconfirm \
  boost \
  boost-libs \
  catch2 \
  clang \
  cmake \
  fmt \
  git \
  llvm \
  llvm-libs \
  ninja \
  nlohmann-json \
  tbb

WORKDIR /tmp

# Run tests

WORKDIR /
COPY entrypoint.sh /entrypoint.sh
ARG CXX_COMPILER=clang++
ENTRYPOINT ["/entrypoint.sh"]
