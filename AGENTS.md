# AGENTS.md

C++20 ray tracer, built with CMake and tested with Catch2 through CTest.

## Commands

Run these inside the dev container (see `README.md`). The shell there is bash.

```sh
cmake --preset debug             # configure
cmake --build --preset debug     # build
./build/debug/src/raytracer      # run (prints "Hello, World!")
ctest --preset debug             # run the tests
```

Other presets: `release` and `sanitize` (same three steps, replace `debug`).

Shortcuts (`Makefile`, they only call the commands above and configure automatically):

```sh
make build                # compile
make run                  # compile and run (make run ARGS=Ada to pass arguments)
make test                 # compile and run the tests
make clean                # delete build/
make test PRESET=sanitize # use another preset (default: debug)
```

## Code style

- Private variables (data members) start with `_` followed by a lowercase letter: `_x`, `_count`.
  Never use `_` followed by an uppercase letter, or a double underscore (`__`): C++ reserves them.
- Do not use a `get` prefix for accessors, it is redundant. Name the function after the value:
  `x()`, not `getX()`.

## Contributing and CI

`CONTRIBUTING.md` describes the workflow for contributors (dev container, `make` commands,
test-first, ADRs). `.github/workflows/ci.yml` builds the dev container image and runs `make test`
on every pull request: keep that command passing.

## Architecture decisions

Our architecture decisions are in `docs/adr/`. Read them before changing the build, the tools or
the test setup.

- Never go against an accepted ADR. To change a decision, write a new ADR with status
  `Proposed` and ask the user.
- Never set an ADR to `Accepted`. Only the user does that.
