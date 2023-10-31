#!/bin/bash
today=$(date '+%Y-%m-%d, %a')
echo -e "\n## $today" >> "$0"
exit 0

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
- Raylib is far ahead of Schwember, it maybe a point wherez
  I will be using more Raylib than developing Schwember
- Bullet requires asyncify to work on emcc

## 2023-09-05, Tue

- I added a "pixel perfect" sign textured from this
  [youtube video](https://www.youtube.com/watch?v=RQVAUaSUP-k)
- Except texture surface must be Principal BSDF
- and it must use the texture for the base color
- otherwise gltf loading segfaults right now
- And I did not use suggested plugins from the video

## 2023-09-09, Sat

- After trying it with Raylib, I am starting to port Bullet
  to this project
- It will not be exact Raylib test copy, but instead I
  need to load collission shapes from model/mesh data

## 2023-09-10, Sun

In the morning got working falling pyramyd stopping at floor level.

- but actually it is a bit higher then the floor, needs to debug!
- All members of physics_t structure need to be alligned
  (Otherwise it crashes on WASM, but can also on other platforms)
- Finally, it is best to schift the pyramid down while rendering,
  and not in Bullet, makes it work smoothly.
- There will be a lot of refactoring
- ImGui is a project with no tool driven installation provided
- It will be installed as git module
- I want to install other projects as git modules therefore
  here trying to lay some good foundation
- ImGui is hard to work with but got it working! following
  [This blog](https://technotes.blog/2023/01/24/getting-started-with-imgui-a-beginners-tutorial/)
- imgui/examples/example_glfw_opengl3/Makefile is a usefule example
- imgui officially requires C++11

## 2023-09-11, Mon

- I need to do some cleanup
- and I need to make state resetable without rerunning
- Before I gone to ECS, I feel hesitant,
  What if it will overcomplicate at this stage?
- Also after I adde external submodules section,
  I am thinking whether or not I should make every library this way.
- [MiniAudio](https://miniaud.io/) when I need sound...

## 2023-09-14, Thu

I need to prioritize and plan a little bit,
but I think I need to stop coding and focus on learning.
I need to read the theory behind the GLTF,
including material rendering and animations.

The resource for it will be
[glTF-Tutorials](https://github.khronos.org/glTF-Tutorials/)

## 2023-09-26, Tue

- Renders models without texture too
- Added plain Suzanne the monkey model
- It renders using last used texture
- Next I will work to make it render with a color from VBO
- Also, it is very annoying that I cannot build offline
- Because of CMake fetch allways fetching repos online
- To solve it migrate to git modules instead

## 2023-09-27, Wed

Added another bool uniform to indicate if it should use texture.
Now it displays .color value from VBO, but it does not
seem to have anything set there while loading models.
It prints all possible random colors.

## 2023-09-28, Thu

Attribute of type cgltf_attribute_type_normal is not essential.
Material has its metalic_roughness albedo color, I use that instead.

- Now uses material base color if texture is not provided
- if material is not provided then uses green
- Added sphere and mixamo enabled stickman, which has animation

## 2023-10-02, Mon

Organizing creation of plane collider into separate unit
for last couple of days.

## 2023-10-03, Tue

GPT said:
> The btTransform::setFromOpenGLMatrix method in Bullet Physics
> is designed to convert a 4x4 OpenGL-style matrix into a btTransform.
> However, it doesn't inherently support scaling,
> as Bullet Physics primarily deals
> with translation and rotation components.

I tend to believe it.

- Misused collision shape's local scale
- Perhaps I should use a field in collider_*_t unit
- Perhaps Bullet actually has a capability to persist scale
- I currently need to center mesh vertices (this is bad)

Then after quick sphere collider and force applied

## 2023-10-04, Wed

I could add custom collider,
but it only owrks with some faces not all.

## 2023-10-05, Thu

Trying to make small triangles on btBvhTriangleMeshShape
did not give any results.
My suspicion is it forms wrong triangles out of the vertices.


bellow is not true:
> The suspicion was correct

- Three vertices at the time loop had a bug;
- After fixing this bug btBvhTriangleMeshShape collides well;
- I am not sure if I even need custom collision handlers

## 2023-10-06, Fri

- On WASM it is too slow if it prints on every detected collision;
- IO is likely the bottleneck, but Bullet surelly adds to it;

I finally settle down with the idea of the local scale of the 
collision shape. In seems it does apply effect on collision shape,
and on top of it I want to use it for rescalling trans matrix before rendering.

- Bullet physics however wants box vector to be just half
  of the box dimensions
- But I may need to add center adjustment for non-centered meshes
- But I can leave this for later

To make from bouncing to swicth to rolling it loses speed
- tied setting damping, rolling friction, CCD motion threshold, and CCD swept sphere radius
- but no significant results, but restitution works

## 2023-10-07, Sat

PC is introduced. It will consist of several building blocks.
Renderable is there to replace mesh eventually.
Printing PC position as a matrix in Dear IMGUI.

- I want to start working on PC movement
- For that I need to refactor input/controlls code
- Also thing about event based state of the application

## 2023-10-09, Mon

Idea: use formatting rules of VSCode

- Introduce control unit, that issues events
- Camera handles issued events
- Control does not issue camera events when it is in PC mode

## 2023-10-11, Wed

I found on a forum somebody wrote like this:
> The collision object was using motionstates,
> so when the box came to a stationary state,
> it's activation state was changed to ISLAND_SLEEPING.
> In order to get the box moving again, I had to run
> `setActivationState(1)`

This was exactly my issue, and same solution.
Finally...

- Maybe I should use elipsis collision shape;
- Maybe it is possible to make my ellipsis
  to rotate accross Y axis only;
- Maybe it is possible to get vertices of collision shape,
  so that meshes or renderables could be created.

Reasons to move to VSCode formatting rules:

- VSCode uses 4 spaces, and it is more readable
  and Vim works better with 4 spaces;
- VSCode is important and means a lot to me

To format file: `clang-format  - src/mesh.h`

## 2023-10-13, Fri

- Bullet constraints are not useful to make capsule stand
- But angularFactor is
- also it is more practical to use linear velocity
- and not applying any forces 

Capsule can "climb" the stairs.

Github page JamesTKhan/libgdx-bullet-tutorials has some useful

Enabled lldb, as gdb does not work

## 2023-10-18, Wed

With some examples started to move along 
https://github.com/fubark/cosmic/blob/15f85e4bf5c6618c237d6f11f540717f7df16eb6/graphics/src/graphics.zig#L2083

## 2023-10-29, Sun

Asset now has "nose", so I confirm that
I cannot see odd indexing of the nodes anymore.

But there is a weird artefact, where the model position seems
to be out of space at the moment shader is selected
(and later slowly it returns back)

It will be some memory access bug, as I can only reproducedd it
after cahnging seemingly unrelated code.

There was another issue causing quite a bit of problem
with accessing some of struct properties as if it was not a pointer
but inlined struct, which was not the case.

Printing sampler details

## 2023-10-30, Mon

The name of the controlled bone is actually accessible from
channel samplers. (I had a bug before that made it look
it is not there, but it actually is). Just need to see if 
for rotation type I can get vec4 quarternion, and for translation
vec3.

- Maybe I should look to other projects
- I need to port simple outputs to Dear Imgui browser
- Also don't forget to colorcode the weights of bones
- and why does input sampler count is higher than output by 1?


