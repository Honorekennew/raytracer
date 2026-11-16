# Discovery report

## Problem statement

The studio needs a ray tracer its own team can maintain and extend, because commercial licences are paid per artist and per machine and its last in-house renderer failed in ways nobody could explain, but the first delivery only has to render spheres and planes by 5 January.

## Client interview summary

### Licence cost

**Need:** Stop paying a licence for every artist and every render machine.

> **Q (Classmate):** Why not keep the commercial renderer?
>
> **A:** "The licence is seat-based: we pay per artist, plus every node. On the last project the licence budget reached about two million. We could not afford it, and it still does not cover future shows. We want to spend no more than five hundred thousand euros, in-house."

**My conclusion:** One sentence said "two billion". The later figure, two million dollars, is the one to keep. The target budget of five hundred thousand is in euros, so the two amounts are not in the same currency.

### When people leave

**Need:** The studio must maintain the program after the authors leave, and the remaining people must still understand it.

> **Q (Classmate):** What happens when the person who wrote the software leaves?
>
> **A:** "Two cases. You deliver the first version and leave: can we maintain it without you? Or only some of you leave: can the others still understand and use it? Last time, that person left, and nobody could explain the code."

**My conclusion:** On that last failure, five people could not debug the program after the author left. The notes do not say if the author was one of the five. The size of the team that will own the code was not given.

### Failures must be explainable

**Need:** The same scene always gives the same image. If a render breaks, the cause is found the same day.

> **Q (Classmate):** What went wrong on the last show?
>
> **A:** "Six months ago we got black dots in the images and the render time doubled. Nobody could say why. We bought a commercial licence and finished three weeks late."

### Scope and deadline

**Need:** For this project, render spheres and planes only. The final image is HD, 1920×1080. The film is 90 minutes and its render must not exceed 60 days. Delivery of the software is 5 January. The tool must still be able to grow, because every show adds requirements.

> **Q (Classmate):** What must the first version render, and by when? How long is the film? What is the final image size?
>
> **A:** "Spheres and planes, and that is it for now. The software must be delivered by 5 January so production can start. The film is 90 minutes, in HD, 1920 by 1080. We do not want the render to exceed 60 days."

**My conclusion:** The teacher said a real client would want every standard model. He cut the scope so it fits this module. Designer previews stay smaller than 1920×1080. If the film is 24 frames per second, that is about 129,600 frames. On 100 machines for 60 days, the ceiling is about one hour per frame per machine. The client did not say the frame rate or the time per frame. The 60 days is his limit. The one hour is our conclusion.

### Two kinds of users

**Need:** A fast, good-enough preview for the artist. A high-quality render for production, on the CPU, with no graphics card. The render farm has up to 100 machines.

> **Q (Classmate):** What hardware do you have?
>
> **A:** "The designer has a powerful desktop and must see a smaller preview quickly. Production runs on a larger machine, with reasonable specs, and must be rendered by the CPU. There is no graphics card. The farm goes up to 100 machines. For the designer, a fast and pretty accurate image matters. In production, high quality matters."

**My conclusion:** No exact model and no RAM figure were captured. Asked how long one frame may take, the client said "I'm not too sure", and only gave the 60-day cap for the whole film. The client also asked for fast previews and at least ten pre-renders per image.

### From the artist to the film

**Need:** The artist builds the scene in another program, saves a file, and our program writes the final images to disk.

> **Q (Classmate):** How does a shot get from the artist to the final frame?
>
> **A:** "The designer uses third-party software to place the objects and the lights, then saves a file. That file goes to the renderer. The final images are saved to disk, as a sequence for the film. It is a black box: a scene description in, an image out. The format must be a standard one."

**My conclusion:** The commercial tool they fell back on was Maya. "Mayan" was a dictation error. The file extension was not chosen.

### Open source, partners, documentation

**Need:** Partner companies must be able to use the program in their own company. The studio keeps it for years, partners can contribute, and the people who launch renders have a short guide.

> **Q (Classmate):** Does "everything is open source" mean the program we deliver?
>
> **A:** "We want partner companies to be able to use it in their own company. We want to maintain it in-house, long term. Partner studios also render for us and can contribute. A team on our side will own the code and review it. We expect a user guide for the people who set up the renders."

### Open questions

- What is the frame rate? Not given. 24 frames per second is our assumption, not the client's.
- What is the acceptable time for one frame? The client said "I'm not too sure". The only hard limit is 60 days for the whole film.
- What are the exact specs and RAM of the farm machines? Not captured.
- How many developers will take over the code? Not given.

## State of the art

| Solution | Maker | Licence | Pricing (16 November 2026) | Capabilities | Sources |
|---|---|---|---|---|---|
| Arnold | Autodesk | Commercial, per named user | The Autodesk page does not show the price on 16 November 2026 (it is loaded dynamically). One reseller listing showed 430 USD per year, which Autodesk does not confirm. | Path tracer used in film. CPU and GPU. Often used from Maya. | https://www.autodesk.com/products/arnold/buy |
| RenderMan | Pixar | Commercial, per licence, floating or node-locked | 595 USD per licence on Pixar's store, plus 250 USD per licence for annual maintenance. That is 845 USD for the first year (our sum). | Production renderer behind Pixar films. | https://renderman.pixar.com/store |
| Cycles | Blender Foundation | Open source (Apache 2.0) | 0 | Path tracer inside Blender. Free to use in commercial films. | https://www.blender.org/about/license/ |

Commercial renderers already calculate film images. The client already fell back on Maya, and the cost is the problem: one licence per artist and per machine. Free renderers exist, and the client still wants a program its own team and its partner studios can read, change and explain. For this module the first version is only spheres and planes.

## What is ray tracing?

A scene and a camera go in. An image comes out. For each pixel, the program shoots a ray, finds the first sphere or plane it hits, and sets the pixel colour from the light there. Extra rays simulate reflections. Film studios use a heavier variant, path tracing, which follows many light paths. This course starts with the simple version.

**Sources:** https://www.scratchapixel.com/lessons/3d-basic-rendering/introduction-to-ray-tracing/ray-tracing-practical-example.html and https://pbrt.org/

## What is C++?

### Programming paradigms

C++ allows several styles at once: procedural (functions), object-oriented (classes) and generic (templates). Languages such as Python or JavaScript hide more of this. In C++ you choose the style, and you also choose how memory is handled.

### What to watch out for

C++ does not free memory for you, and it does not check every invalid access. Classic traps: using a freed object, forgetting to free one, mixing up a copy and a reference, and numbers that silently overflow. The same scene must still give the same image, so those traps matter.

### Shaping the domain model

The client's words (ray, sphere, plane, camera, scene, image) should become the names in the code. A sphere and a plane are two shapes that both answer "does this ray hit you?". That is a base type plus two variants, not a pile of special cases. New shapes can then be added when a later show needs them.

**Sources:** https://www.learncpp.com/ and https://en.cppreference.com/
