# 1. Use a dev container as the shared build environment

> Draft: the reasons below come from the repository contents, not from the team's board. Replace or confirm them before accepting.

## Context

The project is written in C++, and the team works on Windows, macOS and Linux. C++ has no single
toolchain: compiler, compiler version, CMake and debugger all differ from one machine to another,
so the same code can build on one laptop and fail on another.

The team has already used dev containers with VS Code and Zed.

Alternatives:

- Each person installs a compiler, CMake and Ninja natively (MSVC on Windows, Xcode tools on
  macOS, GCC or Clang on Linux).
- A shared remote machine or virtual machine.

## Decision

We use a dev container (`.devcontainer/Dockerfile` and `devcontainer.json`) as the supported way
to build, run and test. It is based on Ubuntu 24.04 and provides GCC 13, Clang 18, CMake, Ninja,
gdb, clang-format, clang-tidy and clangd.

Everyone, on any operating system, builds with the same tools. The environment can also be used
without an editor integration, with plain `docker build` and `docker run` (see the README).

## Status

Accepted

## Consequences

- Same compiler and tool versions for everyone, on every operating system.
- A new person only needs Docker and an editor, and does not install a C++ toolchain by hand.
- Docker (Docker Desktop on Windows and macOS) must be installed and running.
- The first start builds the image, which takes several minutes.
- Tool versions are fixed by the Ubuntu 24.04 release. Upgrading a compiler means changing the
  base image.
- The container adds `SYS_PTRACE` and disables the default seccomp profile so that gdb works,
  which makes it less isolated than a default container.
- The code is compiled with Linux compilers only. A native Windows build with MSVC is documented
  in the README but has not been tested.
- The build has been checked with `docker run`. The editor integrations (VS Code Dev Containers
  and Zed) have not been tested against this `devcontainer.json`.
