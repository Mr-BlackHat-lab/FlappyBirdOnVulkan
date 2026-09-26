
Project structure
FlappyVulkan/
│
├── CMakeLists.txt
├── README.md
│
├── assets/
│   ├── textures/
│   │   ├── bird.png
│   │   ├── pipe.png
│   │   ├── background.png
│   │   └── ground.png
│   │
│   ├── fonts/
│   │   └── font.ttf
│   │
│   └── audio/
│       ├── flap.wav
│       ├── hit.wav
│       └── point.wav
│
├── shaders/
│   ├── sprite.vert
│   └── sprite.frag
│
└── src/
├── main.cpp
│
├── Core/
│   ├── Application.hpp
│   └── Application.cpp
│
├── Vulkan/
│   ├── VulkanContext.hpp
│   ├── VulkanContext.cpp
│   ├── Swapchain.hpp
│   ├── Swapchain.cpp
│   ├── Pipeline.hpp
│   ├── Pipeline.cpp
│   ├── Buffer.hpp
│   ├── Buffer.cpp
│   ├── Image.hpp
│   ├── Image.cpp
│   ├── Renderer.hpp
│   └── Renderer.cpp
│
├── Game/
│   ├── Game.hpp
│   ├── Game.cpp
│   ├── Bird.hpp
│   ├── Bird.cpp
│   ├── Pipe.hpp
│   ├── Pipe.cpp
│   ├── Collision.hpp
│   ├── Collision.cpp
│   ├── Score.hpp
│   └── Score.cpp
│
└── Resources/
├── Texture.hpp
├── Texture.cpp
├── Sprite.hpp
├── Sprite.cpp
├── Audio.hpp
└── Audio.cpp





main.cpp                    30
Application                 150
────────────────────────────────
VulkanContext               400
Swapchain                   250
Pipeline                    250
Buffer                      200
Image                       200
Renderer                    250
────────────────────────────────
Game                        250
Bird                         80
Pipe                        150
Collision                    80
Score                        80
────────────────────────────────
Texture                     150
Sprite                      100
Audio                       100
────────────────────────────────
TOTAL                     ~2,700 LOC


create GLFW window
initialize Vulkan
initialize renderer
initialize game
main loop
process input
update game
render game
cleanup



The really important part
You don't actually need all of this to make the game.

I'd build it in stages:

Stage 1 — Vulkan triangle
GLFW
↓
Vulkan
↓
Triangle

~500–1,000 LOC depending on how much you abstract.

Stage 2 — textured quad
GLFW
↓
Vulkan
↓
Texture
↓
Quad

Now you can render the bird.

Stage 3 — 2D renderer
Make:

drawSprite(...)

work.

Now rendering the game becomes easy.

Stage 4 — Bird
Add:

gravity
velocity
flap

Stage 5 — Pipes
Add:

pipe spawning
movement
random gaps

Stage 6 — Collision
Add:

bird ↔ pipe
bird ↔ ground
bird ↔ ceiling

Stage 7 — Game states
READY
↓
PLAYING
↓
GAME OVER
↓
RESTART

Stage 8 — Polish
sprites
animation
sound
score
high score
particles
screen shake
menus

At that point you've got a legit little Vulkan game.

My recommendation: don't start by trying to write a 2,700-line architecture. Start with main.cpp + VulkanContext + Renderer, get one textured quad rendering, and then build the game on top of that. That's much easier to debug with Vulkan.