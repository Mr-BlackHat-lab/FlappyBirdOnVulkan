# TODO — Next concrete tasks

This file lists prioritized, actionable tasks to make the project render a frame and progress toward a playable Flappy Bird prototype.

## High priority (render a frame)
- Implement swapchain header and creation
  - Files: `src/Vulkan/Swapchain.h`, `src/Vulkan/Swapchain.cpp`
  - Deliverables: create swapchain, image views, choose surface format/present mode
  - Est: 1–2 hours

- Create render pass, framebuffers, and basic pipeline
  - Files: `src/Vulkan/RenderPass.h/.cpp`, `src/Vulkan/Pipeline.h/.cpp`
  - Deliverables: simple render pass that clears the screen; pipeline to draw a triangle/quad
  - Est: 1–2 hours

- Command pools, command buffers, and synchronization
  - Files: `src/Vulkan/CommandPool.h/.cpp`, update `VulkanState.hpp` as needed
  - Deliverables: allocate command buffers, submit present flow, fences/semaphores
  - Est: 1–2 hours

- Integrate a minimal render loop
  - Files: modify `src/main.cpp` or add `src/Vulkan/Renderer.*`
  - Deliverables: acquire image, record commands, submit, present (frame clears to solid color)
  - Est: 30–60 minutes

## Medium priority (textured rendering & resources)
- Implement a textured quad renderer
  - Files: `src/Renderer.h/.cpp`, `src/Resources/Texture.h/.cpp`, `src/Resources/Sprite.h/.cpp`
  - Deliverables: load texture from `assets/textures`, upload to GPU, draw quad using `shaders/sprite.*`
  - Est: 2–4 hours

- Asset loading helpers and atlas support
  - Files: under `src/Resources/`
  - Deliverables: simple image loader (stb_image), texture atlas support for sprite batching
  - Est: 1–3 hours

## Low priority (game logic & polish)
- Add core game systems (stubs)
  - Files: `src/Game/Game.h/.cpp`, `src/Game/Bird.h/.cpp`, `src/Game/Pipe.h/.cpp`, `src/Game/Collision.h/.cpp`, `src/Game/Score.h/.cpp`
  - Deliverables: game loop integration, input handling, physics stubs
  - Est: 2–6 hours

- Audio integration
  - Files: `src/Resources/Audio.h/.cpp`
  - Deliverables: simple audio playback for flap/hit/point (use a lightweight lib)
  - Est: 1–3 hours

- UI & scoring, high score persistence
  - Files: `src/UI/*`
  - Deliverables: on-screen score, save high score to file
  - Est: 1–2 hours

## Build / CI / Misc
- Add Windows build script (`build-windows.bat`) and a minimal `build.sh` for *nix
- Add a simple GitHub Actions workflow to build on push
- Add `CMake` option checks for Vulkan and GLFW and helpful error messages
