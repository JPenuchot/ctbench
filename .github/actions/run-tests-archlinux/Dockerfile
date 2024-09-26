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
  openssh \
  tar \
  unzip \
  zip

WORKDIR /tmp

#
# AUR packages installation
#

RUN useradd -u 24 -m builder
RUN usermod -aG wheel builder
RUN echo '%wheel ALL=(ALL:ALL) NOPASSWD: ALL' >> /etc/sudoers

USER builder

RUN mkdir -p -m 0700 ~/.ssh && ssh-keyscan github.com >> ~/.ssh/known_hosts

RUN git clone https://aur.archlinux.org/paru-git.git /tmp/paru
WORKDIR /tmp/paru
RUN --mount=type=cache,sharing=locked,target=/var/cache/pacman \
  ulimit -n 128 && makepkg -si --noconfirm

USER builder

RUN --mount=type=cache,sharing=locked,target=/var/cache/pacman \
  ulimit -n 128 && paru -S --noconfirm \
  sciplot-git

# Run tests

USER root

WORKDIR /
COPY entrypoint.sh /entrypoint.sh
ARG CXX_COMPILER=clang++
ENTRYPOINT ["/entrypoint.sh"]
