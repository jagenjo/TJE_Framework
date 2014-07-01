/*  by Javi Agenjo 2013 UPF  javi.agenjo@gmail.com
	This contains a texture wrapper to use textures. It allow to load TGAs.
*/
#ifndef TEXTURE_H
#define TEXTURE_H

#include "includes.h"
#include <map>
#include <string>



// TEXTURE CLASS
class Texture
{
	typedef struct sTGAInfo //a general struct to store all the information about a TGA file
	{
		GLuint width;
		GLuint height;
		GLuint bpp; //bits per pixel
		GLubyte* data; //bytes with the pixel information
	} TGAInfo;

public:
	GLuint texture_id; // GL id to identify the texture in opengl, every texture must have its own id
	float width;
	float height;
	std::string filename;

	Texture();
	void bind();
	void unbind();
	static void UnbindAll();

	bool load(const char* filename, bool mipmaps = true);
	void generateMipmaps();

protected:
	TGAInfo* loadTGA(const char* filename);
};

#endif