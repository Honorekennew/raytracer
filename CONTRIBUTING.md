# Contributing

Thank you for your interest in this project. This page explains how to set up your environment,
which commands to use, how we write code, and how we record decisions.

## 1. Set up the dev container

We build and test inside a dev container, so everyone uses the same compiler and tools whatever
their operating system. A native build is possible but not tested (see the
[README](README.md#building-without-the-dev-container)).

1. Install [Docker Desktop](https://www.docker.com/products/docker-desktop/) (or Docker Engine on
   Linux) and make sure it is running.
2. Fork and clone the repository.
3. Open the folder in the dev container:
   - **VS Code**: install the *Dev Containers* extension and run
     *Dev Containers: Reopen in Container*.
   - **Zed**: accept the dev container prompt.
   - **Any other editor**: build and enter the container from a terminal, from the repository root:

     ```sh
     docker build -t raytracer-dev .devcontainer
     docker run --rm -it -v "$(pwd):/workspace" raytracer-dev     # PowerShell: "${PWD}:/workspace"
     ```

The first start builds the image, which takes a few minutes. All commands below are run **inside
the container** (the shell prompt looks like `ubuntu@<id>:/workspace$`).

## 2. Build, run and test

The `Makefile` gives short names to the CMake commands. See the [README](README.md) for the full
`cmake` commands.

| Command | What it does |
|---|---|
| `make build` | Compile. |
| `make run` | Compile, then start the program (`make run ARGS=Ada` passes an argument). |
| `make test` | Compile, then run the tests. |
| `make clean` | Delete the `build/` folder. |

Add `PRESET=release` or `PRESET=sanitize` to use another preset, for example
`make test PRESET=sanitize`.

## 3. Write the test first (TDD)

We work in short cycles:

1. **Red**: write a test for the next small behaviour and run it. It must fail.
2. **Green**: write the simplest code that makes it pass.
3. **Refactor**: clean up while the tests stay green.

Tests use [Catch2](https://github.com/catchorg/Catch2) and live in `tests/`. To add one, create
`tests/<thing>_test.cpp`, add the file to `add_executable(raytracer_tests ...)` in
`tests/CMakeLists.txt`, then run `make test`. The README explains this step by step, with an
example.

A change that adds behaviour should come with the tests that describe it. Before opening a pull
request, run `make test PRESET=sanitize` as well, to catch memory errors and undefined behaviour.

## 4. Architecture decisions (ADRs)

Our decisions about the build, the tools and the test setup are recorded in
[`docs/adr/`](docs/adr/), one short file per decision, with a status (`Proposed`, `Accepted`, ...).

- **Read them before changing the build, the tools or the test setup.**
- Do not go against an `Accepted` ADR.
- To change a decision, or to introduce a new tool or service, add a **new** ADR with the status
  `Proposed`, following the format of the existing ones (Title, Context, Decision, Status,
  Consequences), and open a pull request or an issue to discuss it. Do not edit an existing ADR
  to reverse it.
- Do not set an ADR to `Accepted` yourself. The maintainers do that after reading it.

## 5. Submit a change

1. Create a branch in your fork.
2. Make your change, tests first, and check that `make test` passes in the dev container.
3. Open a pull request and fill in the template.
4. The CI workflow (`.github/workflows/ci.yml`) builds the project and runs the tests in the same
   dev container. It must pass.

To report a bug or suggest an improvement, open an issue and pick the matching template.
