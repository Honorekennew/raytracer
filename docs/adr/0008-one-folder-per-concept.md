# 8. One folder per concept, with the public headers in include/

> Draft: written by the assistant from the structure of the repository. Replace or confirm the
> reasons before accepting.

## Context

The code is growing: vectors, colours, rays, shapes, materials, scenes, a camera, an image and its
savers. One flat folder would make it hard to find a class, its header and its tests.

Alternatives:

- **One flat folder.** Simple at the start, and it does not scale.
- **Headers next to the sources** in `src/`, included by their path from `src/`.
- **Headers in `include/raytracer/`, sources in `src/`, tests in `tests/`**, each with one folder
  per concept.

## Decision

Each concept has one folder, with the same name in three trees that mirror each other:

- `include/raytracer/<concept>/<thing>.hpp`: the public header.
- `src/<concept>/<thing>.cpp`: the implementation.
- `tests/<concept>/<thing>_test.cpp`: the tests.

The concepts are `colour`, `geometry`, `io`, `material`, `math`, `render` and `scene`. The code is in
the namespace `raytracer`, and headers are included from `include/`, for example
`#include "raytracer/geometry/sphere.hpp"`. All the sources go in one library, `raytracer_core`,
used by the program and by the tests. `main.cpp` stays in `src/`.

## Status

Accepted

## Consequences

- A class, its header and its tests are found from its concept, by the same path in three trees.
- Adding a class means a header, a source, a test, and a line in `src/CMakeLists.txt` and in
  `tests/CMakeLists.txt`.
- `include/` holds what the library shows to its users and `src/` what it hides, which is the layout
  that a library used by other projects will need.
- The arrows between folders should go one way. `Colour` has a folder of its own for this reason
  (ADR 0005). They do not fully go one way today: `geometry/` includes `material/` (a shape has a
  material) and `material/` includes `geometry/` (a material reads a `Hit`). `hit.hpp` only declares
  `Material`, which breaks the loop between the two headers, but the two folders still depend on each
  other.
- We kept one library for all the folders. One library per folder is possible later, and the loop
  above would have to be removed first.
- Three trees to keep in step, and a folder per concept that may hold a single class.
