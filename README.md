# FlappyVulkan

A small Flappy Bird–style game implemented with Vulkan and GLFW. This repository contains a simple 2D renderer, game logic, and assets to build a playable prototype.

## Features
- Vulkan-based renderer (triangle → textured quad → sprites)
- GLFW windowing and input
- Simple 2D renderer and sprite batching
- Game systems: bird physics, pipes, collision, scoring, audio

## Quick Start
Prerequisites: Vulkan SDK, GLFW, a C++ toolchain, and CMake.

Configure and build (recommended using Ninja):

```bash
cmake -S . -B build -G "Ninja" -DCMAKE_BUILD_TYPE=Debug
cmake --build build
```

Then run the executable produced in the `build` directory (name may vary):

```bash
./build/FlappyBird.exe
```

On Windows with Visual Studio generators:

```powershell
cmake -S . -B build -G "Visual Studio 17 2022" -A x64
cmake --build build --config Release
```

## Project Layout
Top-level layout:

```
CMakeLists.txt
README.md
assets/
  textures/ (bird, pipe, background, ground)
  fonts/    (font.ttf)
  audio/    (flap, hit, point)
shaders/
  sprite.vert
  sprite.frag
src/
  main.cpp
  VulkanState.hpp
  Core/
  Vulkan/
  Game/
  Resources/
```

The code is split into renderer infrastructure (`VulkanContext`, `Swapchain`, `Pipeline`, `Buffer`, `Image`, `Renderer`), game logic (`Game`, `Bird`, `Pipe`, `Collision`, `Score`), and resource helpers (`Texture`, `Sprite`, `Audio`).

Estimated size: ~2,700 LOC across core systems (approximate).

## Build / Run Checklist
- Install the Vulkan SDK and ensure `VK_SDK_PATH` / environment variables are set.
- Install GLFW and make sure CMake can find it (or use provided CMake options to point to GLFW).
- Configure with CMake and build as shown above.

## Development Stages (recommended incremental approach)
You don't need the entire architecture up front — build in stages:

1. Stage 1 — Vulkan triangle
	- Create a GLFW window
	- Initialize Vulkan
	- Render a triangle

2. Stage 2 — Textured quad
	- Add texture support
	- Render a textured quad (the bird sprite)

3. Stage 3 — 2D renderer
	- Implement a `drawSprite(...)` helper and batch rendering

4. Stage 4 — Bird
	- Add gravity, velocity, and flap input

5. Stage 5 — Pipes
	- Implement pipe spawning, movement, and random gaps

6. Stage 6 — Collision
	- Detect bird ↔ pipe, bird ↔ ground, bird ↔ ceiling

7. Stage 7 — Game states
	- READY → PLAYING → GAME OVER → RESTART

8. Stage 8 — Polish
	- Sprites, animation, sound, score, high score, particles, screen shake, menus

This progressive approach keeps the Vulkan surface small and easier to debug.

## Recommended Starting Point
Start with `src/main.cpp`, `src/Vulkan/VulkanContext.*`, and `src/Renderer.*`. Get a single textured quad rendering before expanding game logic.

## Notes
- The repository contains example assets in `assets/` and simple vertex/fragment shaders in `shaders/`.
- The provided structure is intentionally modular; you can replace or simplify subsystems while developing.

If you want, I can also:
- Add a build script for Windows
- Create CI build steps
- Run a quick code scan for TODOs

Enjoy building — Vulkan is low-level, so iterate small and test often.

## Current progress
Automated scan of the `src/` tree shows the following components are implemented or partially implemented:

- Core / Windowing:
	- `src/Core/Application.h` / `src/Core/Application.cpp` — GLFW window creation, event polling, cleanup.

- Vulkan setup:
	- `src/Vulkan/VulkanContext.h` / `src/Vulkan/VulkanContext.cpp` — Vulkan instance creation, surface creation, physical device selection, logical device creation, and cleanup.
	- `src/Vulkan/Swapchain.cpp` exists but the header `Swapchain.h` is currently empty (swapchain implementation pending).
	- `src/Vulkan/VulkanContext.cpp` currently initializes instance/device/queues but does not create swapchain, command pools, or render pass yet.

- Entry point:
	- `src/main.cpp` — initializes `Application` and `VulkanContext`, runs main loop.

- State / helpers:
	- `src/VulkanState.hpp` — central state struct for window, instance, device, queues, and surface.

- Assets & shaders:
	- `assets/` contains textures, audio, and a font.
	- `shaders/` contains `sprite.vert` and `sprite.frag`.

Summary: windowing and basic Vulkan device setup are working. Missing or TODO items to reach a renderable frame include:

- Implement swapchain creation and image views
- Create command pools, command buffers, and synchronization primitives
- Build a render pass, framebuffers, and a pipeline (vertex/index buffers, shaders are present)
- Implement a simple textured quad renderer and sprite batching
- Add game logic files (Game, Bird, Pipe, Collision, Score) — these are not present yet

Would you like me to:

- Implement a minimal swapchain + render loop that clears the screen? (quick win)
- Add a basic textured quad renderer using existing shaders? (next step)
- Generate a TODO file listing concrete next tasks?