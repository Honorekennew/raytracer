# 10. Short commands with a Makefile

> Draft: written by the assistant from the `Makefile` of the repository. Replace or confirm the
> reasons before accepting.

## Context

Building, running and testing take long commands (`cmake --preset debug`,
`cmake --build --preset debug`, `./build/debug/src/raytracer`, `ctest --preset debug`). They are used
all day, and are hard to remember.

Alternatives:

- **Type the CMake commands every time.** Nothing to maintain, and long.
- **Scripts** (a shell script per task). They are more code than the commands they wrap.
- **A Makefile.** `make` is installed in the dev container, and its short commands are known to many
  developers.

## Decision

A `Makefile` at the root gives short names to the commands. It only calls the CMake presets: the
real build stays in `CMakeLists.txt`.

- `make build`: configure if needed, then compile.
- `make run`: compile, then start the program (`make run ARGS=...` passes arguments).
- `make test`: compile, then run the tests.
- `make clean`: delete the `build/` folder.
- `PRESET=release` or `PRESET=sanitize` picks another preset (the default is `debug`).

The commands run inside the dev container (ADR 0001).

## Status

Accepted

## Consequences

- The day-to-day commands are four short words, and the CI uses the same `make test` (ADR 0004).
- `make` is not available by default on Windows. A native Windows build uses the CMake commands of
  the README.
- The commands are written in the README, in `AGENTS.md` and in the `Makefile`. They have to be kept
  in step by hand.
- `make clean` deletes the whole `build/` folder, for every preset, and the next build downloads
  Catch2 and LodePNG again.
- A Makefile must be indented with tabs, and its line endings must be LF, otherwise it fails in the
  container. `.gitattributes` forces LF for every file for this reason.
