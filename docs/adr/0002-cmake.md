# 2. Use CMake, with presets, Ninja and CTest, to build and run tests

> Draft: the reasons below come from the repository contents, not from the team's board. Replace or confirm them before accepting.

## Context

We need one description of the build that works on every platform and in every editor, instead of
long compiler commands that differ per operating system. We also need one standard command to run
the tests, whatever test library is used.

Alternatives:

- Compiler commands or shell scripts written by hand.
- Make, Meson or Bazel.
- Project files of a single IDE, such as Visual Studio solutions.

## Decision

We use CMake (3.24 or newer) to describe the build in `CMakeLists.txt` files. `CMakePresets.json`
stores the common configurations (`debug`, `release`, `sanitize`) so that a command such as
`cmake --preset debug` replaces a long list of options. Presets use the Ninja generator.

We use CTest, which is bundled with CMake, as the single way to run tests (`ctest`).

## Status

Proposed

## Consequences

- One build description for all platforms. CMake is widely supported by editors and continuous
  integration services.
- Presets make the build commands short and identical for everyone.
- `ctest` runs every registered test and prints a summary, independent of the test library.
- CMake has its own scripting language, which is one more thing for C++ beginners to learn.
- CMake 3.24 or newer is required, because the test dependency uses `FIND_PACKAGE_ARGS`.
- The presets use Ninja. With the Visual Studio generator on Windows, the README gives plain
  `cmake` commands instead, and this path has not been tested.
- CMake only generates build files. Ninja and a compiler are still needed, and the dev container
  provides them.
