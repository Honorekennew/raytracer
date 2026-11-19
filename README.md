# Raytracer

Ray tracer for a film studio, written in C++20.

Discovery notes: [`docs/discovery/discovery-report.md`](docs/discovery/discovery-report.md).

At this stage the project is only a **bootstrapped C++ skeleton**: a build system, a Hello World
program and a test framework. There is no ray tracing code yet.

## Build strategy

Everyone builds with the same tools, whatever their operating system.

| Piece | Choice | Why |
|---|---|---|
| Environment | **Dev container** (Ubuntu 24.04, GCC 13, Clang 18, CMake, Ninja, gdb, clang-format) | Same compiler and tools on Windows, macOS and Linux. Works with VS Code and Zed. |
| Build system | **CMake** (3.24 or newer) with **presets** | Standard for C++, understood by every editor and CI. |
| Generator | **Ninja** | Fast, same on every platform. |
| Tests | **Catch2 v3** through **CTest** | Readable test syntax, minimal setup. |

The dev container is the supported way to work. A native build is possible too
(see [Building without the dev container](#building-without-the-dev-container)).

## Getting started

### 1. Open the project in the dev container

Install [Docker Desktop](https://www.docker.com/products/docker-desktop/) (or Docker Engine on
Linux) and make sure it is running.

- **VS Code**: install the *Dev Containers* extension, open this folder, then run
  *Dev Containers: Reopen in Container* from the command palette.
- **Zed**: open this folder and accept the dev container prompt it shows
  (see [Zed's dev container documentation](https://zed.dev/docs/dev-containers) if it does not appear).
- **Any other editor**: use the terminal commands below.

The first start builds the image and takes a few minutes. It then configures the `debug` preset
automatically.

**Without an editor integration**, from the repository root:

```sh
# Build the image once
docker build -t raytracer-dev .devcontainer

# Open a shell in the container, with the repository mounted
docker run --rm -it -v "$(pwd):/workspace" raytracer-dev        # Linux, macOS, Git Bash
docker run --rm -it -v "${PWD}:/workspace" raytracer-dev        # Windows PowerShell
```

The commands in the rest of this file are run **inside the container** (the editor's terminal or
the shell opened above). If your prompt looks like `PS C:\...>`, you are on Windows, not in the
container: `cmake` is not installed there, so the commands will fail. The container prompt looks
like `ubuntu@<id>:/workspace$`.

The container uses `bash`, so `&&` works there. If you build natively on Windows with
Windows PowerShell 5.1, run each command on its own line instead, since that version does not
support `&&`.

### 2. Build and run the program

```sh
cmake --preset debug             # configure (once, or when CMake files change)
cmake --build --preset debug     # compile
./build/debug/src/raytracer      # prints nothing, writes output.png
```

### 3. Run the tests

```sh
ctest --preset debug
```

Expected result:

```text
100% tests passed, 0 tests failed out of 2
```

### Shortcuts

The `Makefile` gives short names to the commands above, like npm scripts. It only calls the
CMake presets, and it configures automatically the first time.

| Command | What it does |
|---|---|
| `make build` | Compile. |
| `make run` | Compile, then start the program. `make run ARGS=Ada` passes an argument. |
| `make test` | Compile, then run the tests. |
| `make clean` | Delete the `build/` folder. The next build downloads Catch2 again. |

Add `PRESET=release` or `PRESET=sanitize` to use another preset, for example
`make test PRESET=sanitize`. Run `make help` to see this list in the terminal.

`make` is installed in the dev container. It is not available by default on Windows, so for a
native Windows build use the `cmake` commands directly.

## Test-driven development

We write the test **before** the code, in short cycles:

1. **Red**: write a test for the next small behaviour. Build and run: it must fail.
2. **Green**: write the simplest code that makes it pass.
3. **Refactor**: clean up the code while the tests stay green.

### Adding a test

1. Create `tests/<concept>/<thing>_test.cpp`, in the folder of the concept being tested (for
   example `tests/math/vector_test.cpp`):

   ```cpp
   #include <catch2/catch_test_macros.hpp>

   #include "raytracer/math/vector.hpp"
   #include "test_helpers.hpp"

   TEST_CASE("Vector add", "[vector]") {
     raytracer::Vector a(1, 2, 3);
     raytracer::Vector b(4, 5, 6);

     requireVectorsEqual(a + b, raytracer::Vector(5, 7, 9));
   }
   ```

2. Add the file to `add_executable(raytracer_tests ...)` in `tests/CMakeLists.txt`
   (for example `math/vector_test.cpp`).
3. Build, then run the tests:

   ```sh
   cmake --build --preset debug
   ctest --preset debug
   ```

Every `TEST_CASE` is registered with CTest automatically; there is nothing else to declare.
Floating point comparisons (which a ray tracer will need) use `Catch::Approx`
(`#include <catch2/catch_approx.hpp>`): `REQUIRE(x == Catch::Approx(1.0).margin(1e-9));`.

### Useful commands

```sh
ctest --preset debug -R Vector            # run tests whose name matches "Vector"
./build/debug/tests/raytracer_tests "[vector]"     # run by tag, with Catch2's own output
./build/debug/tests/raytracer_tests --list-tests   # list all tests
ctest --preset sanitize                   # tests with AddressSanitizer + UBSan (after configuring it)
```

### Presets

| Preset | Use |
|---|---|
| `debug` | Daily work. Warnings are errors. |
| `release` | Optimised build, for timing renders. |
| `sanitize` | Debug build that detects memory errors and undefined behaviour. Run it before merging. |

Each preset has a configure, build and test step. For `sanitize` and `release`, configure first:

```sh
cmake --preset sanitize
cmake --build --preset sanitize
ctest --preset sanitize
```

## Project layout

```text
.devcontainer/      Dockerfile and devcontainer.json (the shared environment)
.github/            CI workflow, pull request template, issue templates
include/raytracer/  Public headers, one folder per concept (colour/, geometry/, io/, light/, material/, math/, render/)
src/                Library sources, one folder per concept, and the executable (main.cpp)
tests/              Catch2 tests, one folder per concept
docs/adr/           Architecture decision records
docs/               Other project documents
CMakeLists.txt      Build definition
CMakePresets.json   debug / release / sanitize presets
Makefile            Shortcuts: make build / run / test / clean
CONTRIBUTING.md     How to contribute
AGENTS.md           Instructions for AI coding agents
.clang-format       Code style
```

The logic lives in the `raytracer_core` library. `main.cpp` stays thin, so tests can exercise
everything without running the program.

Files are grouped by concept, and the three trees mirror each other. For example, the `Vector`
class is in `include/raytracer/math/vector.hpp`, `src/math/vector.cpp` and
`tests/math/vector_test.cpp`, and is included with `#include "raytracer/math/vector.hpp"`.
To add a concept, create the matching folder in each tree and list the new files in
`src/CMakeLists.txt` and `tests/CMakeLists.txt`.

To format code (in the dev container):

```sh
find src include tests \( -name '*.cpp' -o -name '*.hpp' \) | xargs clang-format -i
```

## Contributing

See [`CONTRIBUTING.md`](CONTRIBUTING.md): setting up the dev container, the `make` commands, how we
work test-first, and how we use the architecture decisions in [`docs/adr/`](docs/adr/).

Every pull request is built and tested in the dev container by GitHub Actions
(`.github/workflows/ci.yml`). Run `make test` locally first to get the same result.

## Building without the dev container

Install a C++20 compiler, CMake 3.24+ and Ninja:

- **Windows**: Visual Studio 2022 (*Desktop development with C++*) and CMake. Run the commands
  from the *Developer PowerShell for VS*.
- **macOS**: `xcode-select --install` and `brew install cmake ninja`.
- **Linux**: `sudo apt install build-essential cmake ninja-build` (or your distribution's equivalent).

Then use the same presets as above. With Visual Studio's generator, configure without presets:

```sh
cmake -S . -B build
cmake --build build --config Debug
ctest --test-dir build -C Debug --output-on-failure
```

Catch2 is downloaded automatically at configure time, so the first configure needs internet access.
