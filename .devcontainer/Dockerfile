FROM ubuntu:24.04

ENV DEBIAN_FRONTEND=noninteractive

# Toolchain: GCC 13 and Clang 18 (both C++20), CMake 3.28, Ninja,
# plus debugging and formatting tools. Versions are fixed by the Ubuntu release.
RUN apt-get update \
    && apt-get install -y --no-install-recommends \
        build-essential \
        ca-certificates \
        clang \
        clang-format \
        clang-tidy \
        clangd \
        cmake \
        gdb \
        git \
        ninja-build \
        openssh-client \
    && rm -rf /var/lib/apt/lists/*

# The ubuntu:24.04 image ships a non-root user "ubuntu" (uid 1000).
USER ubuntu
WORKDIR /workspace
CMD ["bash"]
