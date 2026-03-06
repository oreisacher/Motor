# Motor Game Engine

A small OpenGL game engine that began as a uni project.

## Features
- Load and render 3D models
- Scene graph with hierarchical nodes
- Component-based nodes for easy extension
- PBR materials
- Lights (Spot, Point, Directional)
- Multi-Window support
- Automatic mesh instancing
- Basic animation support
- Input manager for handling keyboard and mouse controls
- Custom resource file format to bundle and ship game assets

## Planned
- Physics and collision system
- Editor tools and debugging utilities
- Optimized node tree updates for better performance
- Improved animation system with support for separate animation files

## Build Motor Engine
```bash
git clone https://github.com/oreisacher/Motor.git
cd Motor
mkdir build && cd build
cmake ..
make Motor
```

## Build Example Project
```bash
git clone https://github.com/oreisacher/Motor.git
cd Motor/example
mkdir build && cd build
cmake ..
make Example
```
