#!/bin/bash
current_date=$(date '+%Y-%m-%d, %a')
echo -e "\n## $current_date" >> "$0"
exit()

## 2023-08-12, Sat

This file is here to document the development of this project,
including its progress, and decission logs and other notes and
observations.

The file is executable script which appends current date to
the end of the file itself. It is just to facilitate ceremony
to record the date of the entry.

There is the entry for today:

- Tried to get Assimp working again. But it seems like it is an
  know issue that it does not work on macOS with clang.
- Assimp does allegedly works on macOs with g++, but Apple is 
  keen to deprecate all other compilers and alias them all to
  clang.
- Possibly, cgltf could be a better way to go. Just quite possibly
  it is much more work to implement.
- [in python](https://github.com/fury-gl/fury/pull/685/files)
- [to test gltf files](https://sandbox.babylonjs.com/)
- [Looks like a big project](http://guillaumeblanc.github.io/ozz-animation/)
- I reported some issues to assimp issue tracker on GH: #5203 and #5204
- I observed to compile assimp takes time which is still fine
- I observed to compile programs using asimp, takes long time which is bad
- I would rather look into libraries that not add that much to
  the buildtime of my project, especialy in WASM
- I am worried that sceletal animation has to be applied in shader code,
  which makes it hard, and I am trying to keep my shaders both
  300 ES and 330 Core compatible.
- And if to apply skeletal transformations outside of the shader in CPU,
  would it be fast enough?

## 2023-08-13, Sun

- make shaders more aware of uniforms
- still looking on ways to play animations
- in not too much detail is described in FloatyMonke's YT
- also ozz-animation seems to may be useful
- all day very hard trying ot implement camera controls
- all controls working except yawing to sides does not stay
  on floor plane, which makes camera roll
- trying very hard just to get that right
- I changed view_matrix to be the main source of camera position data
- before it was tracking the angles and recalculating it

## 2023-08-14, Mon

- got the camera movement vorking in consistent way
- it supports 3 degree of freedom of movement and 3 of rotation
- it would be more practical to always keep camera horizontally aligned
  but then it is difficult to do it directly iterracting to view matrix.
- actually in the end of the day it is sometheing that makes sense
  (direct control on camera), in practical applications there 
  will have to be different controllers. but it is not necessary
  to have them figured now

    W     - Move forward   UP     - Pitch up
    S     - Move backward  DOWN   - Pitch down
    A     - Strafe left    LEFT   - Yaw left
    D     - Strafe right   RIGHT  - Yaw right
    Shift - Move up        Comma  - Roll left
    Space - Move down      Period - Roll right 

## 2023-08-15, Tue

- Reading about structure of GLTF, apparently what I call mesh
  is referred as primitive.
- My shaders contain of up to 4 tectures which is just a base color
  I will have to redo shader system to only support one texture per
  mesh (or primitive).
- I will try to keep mesh object containing data for entire mesh,
  but it wil have to render each primitives data separatelly

## 2023-08-20, Sun

- Trying to remember what progress was done
- I have been learning some OpenGL 3.0 ES meanwhile
- Looks like I have a plan to	call glDrawElements with more args,
  to define the offset and render just a part of the triangles;

## 2023-08-22, Tue

- GPT did not give any valuable advice how to parse vertices and indexes from gltf
- But CgltfLoader.cpp in MaterialX project has a good example
- use GH/search page is very good
- I still need to find how to load textures using cgltf
- finally found how image data is located in raylib project
- also looked at projects like Defold and MeshOptimizer
- apparently somethimes gltf encode image data in image->uri base64 encoded
- but this time my gltf generated in Blender has it in buffer_view


## 2023-09-01, Fri

- I was off sick, now trying to go back to the project
- Amount of work left to do to load models look overwhelming
- although I know exactly what to do.

## 2023-09-03, Sun

- Got some time today, and got model vertex loaded
- No colors yet, and all in a dirty way
- model position is not applied, where is it in gltf?
- Yes, finally I had to recurse to parents to apply
  parent transformations too.
- Transformations should be applied in SRT order:
  Scale, Rotation, Translaion


## 2023-09-04, Mon

- Today, I took a look around to Raylib and got an example
  of Raylib+Bullet working on WASM in another repo
- It makes 30FPS
- Raylib is far ahead of Schwember, it maybe a point where
  I will be using more Raylib than developing Schwember
- Bullet requires asyncify to work on emcc


## 2023-09-05, Tue

- I added a "pixel perfect" sign textured from this
  [youtube video](https://www.youtube.com/watch?v=RQVAUaSUP-k)
- Except texture surface must be Principal BSDF
- and it must use the texture for the base color
- otherwise gltf loading segfaults right now
- And I did not use suggested plugins from the video
