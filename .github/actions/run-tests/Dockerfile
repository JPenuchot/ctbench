FROM archlinux/archlinux:base-devel

# Base Arch package installation

RUN pacman -Syu --noconfirm
RUN pacman -S --noconfirm \
  git \
  cmake \
  ninja \
  clang \
  llvm \
  llvm-libs \
  nlohmann-json \
  fmt \
  boost \
  boost-libs \
  tbb \
  catch2

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
