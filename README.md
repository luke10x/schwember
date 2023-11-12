# Schwember 3D Simulation Framework

The Schwember 3D Simulation Framework is a versatile and powerful toolset
designed to assist in rendering intricate 3D environments. 
It empowers developers to create interactive C++ programs
that seamlessly interact with these environments,
providing a wide range of possibilities.

## Key Features

- **Rendering 3D Environments**: Schwember simplifies the process
  of rendering complex 3D scenes, allowing you to focus on creating immersive experiences.
- **Web Deployment with WASM**: Schwember is tailored to support
  the delivery of your programs through the web using WebAssembly (WASM).
  This enables a wide audience to access your interactive simulations
  directly from their browsers.
- **Utilizes Common Tools and Libraries**: Schwember leverages a selection of widely-used
  tools and libraries in the field of 3D graphics and simulations, including:
    - **OpenGL**: For high-performance rendering.
    - **GLFW**: To manage windows and input.
    - **Bullet Physics**: Providing realistic physics simulations.
    - **GLM**: A mathematics library for graphics programming.
    - **CGLTF**: Handling GLTF file formats.
    - **Flecs**: A fast and lightweight ECS (Entity-Component-System) framework.
    - **Dear ImGui**: For creating intuitive and customizable user interfaces for dev.
    - **RmlUi**: Allows creating in-game UI using HTML/CSS based components.
    - **Emscripten**: Facilitating the use of WebAssembly for web deployment.

## Development environment

While the primary focus is to ship WASM applications,
for local development native build is the preferred environment.

Download all dependencies with

    git submodule foreach git checkout 

Start native build locally:

    make clean run

# Building for WebAssembly

Start Compose Container environment:

    docker-compose up -d --force-recreate --build

Schell into WASM toolchain container:

    docker-compose exec wasm bash

Build WASM application:

    make clean build

Open [the WASM build](http://localhost:9900/build/) in a Web Browser

## Development procedures

Some most common development procedures:

Add new library:
> Add submodule :
>```
>git submodule add -b v1.89.9 https://github.com/ocornut/imgui.git external/imgui
>git commit
>```
> Update all modules:
>```
>git submodule foreach git checkout 
>```

