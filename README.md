# TJE Framework #

TJE Framework is a C++ layer on top of SDL and OpenGL to help create games or visual applications.
It only provides the basic GPU abstraction (Meshes, Textures, Shaders, Application).
It has been used for many years in the Videogame's development courses I give at the Universitat Pompeu Fabra.

It contains the basics to do something nice:
- Mesh, Texture, Shader and FBO classes
- Vector2,Vector3,Vector4,Matrix44 and Quaternion classes
- Meshes have a ray-mesh and sphere-mesh collision method (thanks to library Coldet)
- Parser to load OBJ, PNG and TGA. Re-stores meshes in binary for faster load.
- Supports skinned animated meshes using own format (you must use a web tool to convert them).

Enough to make gamejam games pretty easily.


## Installation ##

It comes with a Visual Studio solution, but the code should compile in any platform that supports SDL.


## Compiling in Linux

Remember to install the libraries:

sudo apt-get install libsdl2-dev freeglut3 freeglut3-dev

and then just use the makefile:

make

## Feedback
--------

You can write any feedback to javi.agenjo@gmail.com




