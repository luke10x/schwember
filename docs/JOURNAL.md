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
- I should rename my mesh to primitive following the GLTF structure
- They will be endered separatelly
