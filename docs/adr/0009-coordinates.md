# 9. x to the right, y up, z forward, pixels counted from the top left

> Draft: written by the assistant from the code of the camera and the image. Replace or confirm the
> reasons before accepting.

## Context

A ray tracer places everything with three numbers, and the same numbers mean different things if the
axes are not agreed. The camera, the shapes and the image all use them, and a mistake shows as a
picture that is mirrored or upside down, not as an error.

Alternatives:

- **z forward, away from the eye** (a left-handed system). The direction of the view is +z.
- **z towards the eye, looking along -z** (a right-handed system). It is the choice of *Ray Tracing
  in One Weekend* and of OpenGL.

## Decision

- x grows to the right, y grows up, and z grows forward, away from the camera.
- The camera is at a negative z, `(0, 0, -1)`, and looks towards +z. The screen is in the plane
  `z = 0`, centred on `(0, 0, 0)`. The shapes are at a positive z.
- In an `Image`, the pixel `(0, 0)` is at the top left. The column `x` grows to the right and the row
  `y` grows downwards, like in image files.
- The camera turns the pixel into a point of the screen: the column gives x directly, and the row is
  turned round, so the top row looks up (positive y).

## Status

Accepted

## Consequences

- A ray that goes straight ahead has the direction `(0, 0, 1)`, and a surface that faces the camera
  has a normal with a negative z. The tests and the course use these numbers.
- The row is turned round in one place, `Camera::rayThroughPixel`. A mistake there gives an upside
  down picture, and only the tests of pixels that are not in the middle row notice it.
- A reader used to OpenGL or to *Ray Tracing in One Weekend* has to flip the z axis.
- With y up and z away from the eye, the system is left-handed. If a cross product is needed later,
  the order of its operands has to be checked against this choice.
