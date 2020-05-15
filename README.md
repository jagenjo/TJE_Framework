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

It comes with a Visual Studio 2010 solution, but the code should compile in any platform that supports SDL.
The libs and DLLs are not provided with the source code, please download SDL2 from https://www.libsdl.org/download-2.0.php and copy the libs in the libs/lib folder
The DLLs should be copyed in the root folder.

Feedback
--------

You can write any feedback to javi.agenjo@gmail.com




