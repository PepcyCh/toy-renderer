# toy-renderer

`toy-renderer` is a project for me to learn OpenGL. It wraps many operations and objects in OpenGL to make the code more clear.

It seems that the current version is not clear enough. I tried to integrate many into one, but I can't finish it more gracefully. Maybe I should read some others' codes.

But it is still a good try for me. And I have learned a lot during working on this project.

## Third party

* glfw
* glad
* stb image

## Features

* simple math library written by my self
* load models from `.obj` and material from `.mtl`, some PBR extensions are supported
* Phong and Blinn-Phong
* off-screen rendering
* deferred rendering
* SSAO
* PBR (Cook-Torrance)

## Build

`toy-renderer` uses CMake to build.

I have only built it on Mac OS. Maybe it can't be built successfully on other platforms.
