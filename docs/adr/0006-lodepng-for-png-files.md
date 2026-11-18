# 6. Use LodePNG to encode and save PNG files

> Draft: written by the assistant from the decision the team gave in the chat. The survey of the
> alternatives is not in the repository: replace the context with the team's own notes before
> accepting.

## Context

The ray tracer produces an `Image`, a grid of colours in memory. To look at it, we need to save it
as a file that common viewers open. PNG is a good format for this: it is lossless and everywhere.
Encoding a PNG means compressing the pixels (zlib/deflate), splitting them in chunks and adding
checksums, which is too much to write by hand for this project.

Alternatives:

- Write a simpler format by hand (PPM), with no library.
- libpng, with zlib.
- stb_image_write.
- Write the PNG encoder ourselves.

## Decision

We use LodePNG to encode the PNG files. It is used only by `PngSaver`, the PNG implementation of
the `ImageSaver` interface (see `include/raytracer/io/`).

- LodePNG is downloaded with CMake's `FetchContent`, like Catch2 (ADR 3), and **pinned to the
  commit `7b2893e35891c0c85bee5595caafaf3c2044b409`** (version 20261102). The repository has no
  tags, so a commit is the only way to pin it.
- LodePNG has no `CMakeLists.txt`. `src/CMakeLists.txt` builds its single source file, `lodepng.cpp`,
  as a static library named `lodepng`. That library does not use our warning flags: they stay
  for our own code.
- `raytracer_core` links `lodepng` privately: no header of ours includes `<lodepng.h>`, so the rest
  of the code does not depend on it.
- The tests link `lodepng` too, to read the saved file back with its decoder.

## Status

Proposed

## Consequences

- We get PNG files with a few lines of our own code, and a library that is one `.cpp` file and one
  `.h` file, with no other dependency.
- The first configuration needs internet access to download LodePNG, as for Catch2. The CI
  downloads it on every run, because nothing is cached.
- The version is pinned, so upgrading LodePNG is a manual change of the commit.
- LodePNG is third-party code compiled without our warnings, so it can warn or hide problems that
  our own code would not be allowed to.
- Because of the library, `PngSaver` only handles file names in plain `char` strings (no wide
  characters), and overwrites an existing file without asking.
- LodePNG writes a file for an image with no pixels, and that file is not a valid PNG, so
  `PngSaver` refuses empty images itself.
- Other formats will be other `ImageSaver` classes, and this decision does not apply to them.
- The licence of LodePNG (in its `LICENSE` file) was not reviewed here.
