/*  by Javi Agenjo 2013 UPF  javi.agenjo@gmail.com
	This contains a texture wrapper to use textures. It allow to load TGAs.
*/
#ifndef TEXTURE_H
#define TEXTURE_H

#include "includes.h"
#include "framework.h"
#include <map>
#include <string>
#include <cassert>

class Shader;
class FBO;

//Simple class to handle images
class Image
{
public:
	unsigned int width;
	unsigned int height;
	unsigned int bytes_per_pixel; //bits per pixel
	bool origin_topleft;
	Uint8* data; //bytes with the pixel information

	Image() { width = height = 0; data = NULL; bytes_per_pixel = 3; }
	Image(int w, int h, int bytes_per_pixel = 3) { width = w; height = h; this->bytes_per_pixel = bytes_per_pixel; data = new uint8[w*h*bytes_per_pixel]; memset(data, 0, w*h*bytes_per_pixel);  }
	~Image() { if (data) delete []data; data = NULL; }
	void clear() { if (data) delete[]data; data = NULL; width = height = 0; }
	void flipY();

	Vector4 getPixel(int x, int y) {
		assert(x >= 0 && x < (int)width && y >= 0 && y < (int)height && "reading out of memory");
		int pos = y*width*bytes_per_pixel + x*bytes_per_pixel;
		return Vector4((float)data[pos], (float)data[pos + 1], (float)data[pos + 2], bytes_per_pixel == 4 ? 255.0f : (float)data[pos + 3]);
	};
	void setPixel(int x, int y, Vector4 v) { 
		assert(x >= 0 && x < (int)width && y >= 0 && y < (int)height && "writing out of memory");
		int pos = y*width*bytes_per_pixel + x*bytes_per_pixel;
		data[pos] = (uint8)v.x; data[pos + 1] = (uint8)v.y; data[pos + 2] = (uint8)v.z; if (bytes_per_pixel == 4) data[pos + 3] = (uint8)v.w; };

	bool loadTGA(const char* filename);
	bool loadPNG(const char* filename, bool flip_y = true);
};


// TEXTURE CLASS
class Texture
{
public:
	static int default_mag_filter;
	static int default_min_filter;
	static FBO* global_fbo;

	//a general struct to store all the information about a TGA file

	//textures manager
	static std::map<std::string, Texture*> sTexturesLoaded;

	GLuint texture_id; // GL id to identify the texture in opengl, every texture must have its own id
	float width;
	float height;
	std::string filename;

	unsigned int format; //GL_RGB, GL_RGBA
	unsigned int type; //GL_UNSIGNED_INT, GL_FLOAT
	unsigned int texture_type; //GL_TEXTURE_2D, GL_TEXTURE_CUBE, GL_TEXTURE_2D_ARRAY
	bool mipmaps;

	//original data info
	Image image;

	Texture();
	Texture(unsigned int width, unsigned int height, unsigned int format = GL_RGB, unsigned int type = GL_UNSIGNED_BYTE, bool mipmaps = true, Uint8* data = NULL, unsigned int data_format = 0);
	Texture(Image* img);
	~Texture();

	void create(unsigned int width, unsigned int height, unsigned int format = GL_RGB, unsigned int type = GL_UNSIGNED_BYTE, bool mipmaps = true, Uint8* data = NULL, unsigned int data_format = 0);

	void bind();
	void unbind();
	static void UnbindAll();

	void upload(Image* img);
	void upload(unsigned int width, unsigned int height, unsigned int format = GL_RGB, unsigned int type = GL_UNSIGNED_BYTE, bool mipmaps = true, Uint8* data = NULL);
	void uploadAsArray(unsigned int texture_size, bool mipmaps = true);

	//load without using the manager
	bool load(const char* filename, bool mipmaps = true, bool wrap = true, bool upload_to_vram = true);

	//load using the manager (caching loaded ones to avoid reloading them)
	static Texture* Get(const char* filename, bool mipmaps = true, bool wrap = true, bool upload_to_vram = true);
	void setName(const char* name) { sTexturesLoaded[name] = this; }

	void generateMipmaps();

	//show the texture on the current viewport
	void toViewport( Shader* shader = NULL );
	void blit(Texture* destination, Shader* shader = NULL);

	static FBO* getGlobalFBO(Texture* texture);
};

bool isPowerOfTwo(int n);

#endif