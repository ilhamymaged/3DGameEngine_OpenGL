# 3D Game Engine (OpenGL)

![C++](https://img.shields.io/badge/C++-17-blue.svg)
![OpenGL](https://img.shields.io/badge/OpenGL-4.x-green.svg)
![Platform](https://img.shields.io/badge/Platform-Windows%20%7C%20Linux-success)
![Status](https://img.shields.io/badge/Status-Active%20Development-orange)

<p align="center">
  <img src="docs/images/screenshot.png" alt="Engine Screenshot" width="900">
</p>

A cross-platform 3D game engine built with **C++** and **OpenGL**, focused on providing a modern, modular architecture for real-time rendering and game development.

> 🚧 This project is currently under active development.

---

## Features

### Rendering
- Mesh rendering
- Texture loading and rendering
- Shadow mapping
- Skybox rendering
- Basic lighting
- OpenGL rendering pipeline

### Engine Systems
- Entity Component System (ECS)
- Scene management
- Camera system
- Event system
- Input handling
- Audio system
- Logging system

### Platform
- Cross-platform architecture
- Keyboard and mouse input
- Modular engine design

---

## Current Screenshot

<p align="center">
  <img src="docs/images/screenshot.png" width="900">
</p>

---

## Project Structure

```
Engine/
├── Renderer/
├── ECS/
├── Scene/
├── Camera/
├── Audio/
├── Events/
├── Input/
├── Logger/
├── Resources/
└── ...
```

---

## Technologies

- C++
- OpenGL
- GLSL
- CMake
- GLFW
- GLAD
- stb_image
- assimp
- glm
- entt
- imgui
- spdlog
- miniaudio

---

## Engine Architecture

The engine is organized into independent modules:

- **Renderer** – Rendering pipeline, shaders, meshes, textures, shadows, and skyboxes.
- **ECS** – Entity Component System for flexible object composition.
- **Scene** – Scene loading and object management.
- **Camera** – Free camera movement and view/projection handling.
- **Events** – Event dispatching between engine systems.
- **Input** – Cross-platform keyboard and mouse input.
- **Audio** – Sound playback system.
- **Logger** – Centralized logging for debugging.

---

## Controls

| Key | Action |
|------|--------|
| W A S D | Move Camera |
| Mouse | Look Around |
| Esc | Exit |

---

## Building

```bash
git clone https://github.com/ilhamymaged/3DGameEngine_OpenGL.git

cd 3DGameEngine_OpenGL

mkdir build
cd build

cmake ..
cmake --build .
```

---

## Roadmap

### Completed
- [x] OpenGL renderer
- [x] Mesh rendering
- [x] Texture support
- [x] Shadow mapping
- [x] Skyboxes
- [x] Camera system
- [x] Scene management
- [x] ECS
- [x] Event system
- [x] Audio system
- [x] Logger
- [x] Cross-platform input

### Planned
- [ ] PBR materials
- [ ] Deferred rendering
- [ ] Model importing improvements
- [ ] Physics engine
- [ ] Animation system
- [ ] UI system
- [ ] Scripting
- [ ] Post-processing
- [ ] Serialization
- [ ] Editor

---

## Goals

The goal of this project is to build a complete modern C++ game engine while learning and implementing real-time graphics, engine architecture, and game development techniques.

---

## License

This project is licensed under the MIT License.

---

## Author

**Ilhamy Maged**

Feel free to open issues, submit pull requests, or provide feedback.