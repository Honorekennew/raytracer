# 7. A shape has a material

> Draft: written by the assistant from the code of the shapes session. Replace or confirm the
> reasons before accepting.

## Context

A shape (`Sphere`, `Plane`) says where a ray hits it. The colour of the hit also depends on the
surface: a flat colour, or Lambert's law with the sun. Both shapes need a surface.

Alternatives:

- **Inheritance.** One subclass of `Sphere` per surface (`FlatSphere`, `LambertSphere`) and the same
  for `Plane`. Every new shape and every new surface multiplies the classes: shapes x materials.
- **Composition.** A `Material` is an object of its own, and a shape *has* one.

## Decision

A shape has a material. `Material` is an abstract class with one function,
`shade(ray, hit, toLight)`, and `FlatShader` and `LambertShader` are two ways of colouring (the
Strategy pattern, like `ImageSaver` for saving).

`Sphere` and `Plane` each hold a `std::shared_ptr<const Material>`, given at construction, and put a
reference to it in the `Hit` they return. A shared pointer lets several shapes share one material.
The shapes know a `Material`, never a `LambertShader`: only `main` names the concrete ones.

## Status

Accepted

## Consequences

- Shapes and materials grow separately: a new shape or a new material is one new class, and no
  existing class changes. There are shapes + materials classes, not shapes x materials.
- A `Hit` carries the material, so `trace` shades the nearest hit without knowing which shape it
  came from.
- Each shape holds the material and passes it to its `Hit`: the same few lines are in `Sphere` and
  in `Plane`. We accept it for two shapes and will look again with a third.
- A `Hit` points to a material that a shape owns. It must not outlive the shapes of the scene.
  This is true today: a hit lives for the time of one pixel.
- `Sphere` refuses a null material (it throws `std::invalid_argument`). `Plane` does not check it
  yet.
- A pointer to an abstract class costs an indirection at each hit, and `std::shared_ptr` adds a
  reference count. We accept it: a ray tracer spends its time in the shape maths.
