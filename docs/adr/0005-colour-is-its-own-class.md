# 5. Make Colour a class of its own, not a Vector

> Draft: written by the assistant from the pinhole spike session. Replace or confirm the reasons before accepting.

## Context

A colour is three numbers (red, green, blue), like a `Vector` is three numbers (x, y, z). Each
pixel of an `Image` holds a colour, and the program computes colours to put in it.

Alternatives:

- Use `Vector` for colours too (an alias or the same class).
- A separate `Colour` class with its own folder.

## Decision

`Colour` is a class of its own, in `colour/`. It holds three private doubles `_r`, `_g` and `_b`
(from 0 to 1) and gives them back with `r()`, `g()` and `b()`.

- Colours will be mixed, kept between 0 and 1, and turned into 0 to 255 for an image file. A
  direction needs none of that, and a colour has no use for `dot` or `normalised`.
- With two classes, the compiler stops anyone adding a colour to a position.
- The same three numbers hold different knowledge. DRY is about knowledge, not similar lines.
- `colour/` is a folder of its own, so that the arrows between folders go one way
  (`render/` → `geometry/` → `math/`, and `render/` → `colour/`). If a shape in `geometry/` holds
  a colour later, `Colour` in `render/` would make a loop.

## Status

Accepted

## Consequences

- Colour and `Vector` cannot be mixed by mistake, and each can grow its own functions.
- Some code looks the same in both classes (constructor, getters).
- Colour does not check that r, g and b are between 0 and 1. What should happen when they are
  not (clamp them, or refuse them) is not decided yet.
- The folder `colour/` is one more folder to know about.
