#ifndef FBO_H
#define FBO_H

#include "includes.h"
#include "texture.h"

//FrameBufferObject
//helps rendering the scene inside a texture

class FBO {
public:
	GLuint fbo_id; 
	Texture* color_textures[4];
	Texture* depth_texture;
	int num;
	GLenum bufs[4];
	int width;
	int height;
	bool owns_textures;

	GLuint renderbuffer_color;
	GLuint renderbuffer_depth;//not used

	FBO();
	~FBO();

	bool create(int width, int height, int format = GL_RGB, int type = GL_UNSIGNED_BYTE, int num_textures = 1);
	bool createFromTextures(Texture* color, Texture* colorB = NULL, Texture* depth = NULL);
	bool createDepthOnly(int width, int height); //use this for shadowmaps
	
	void bind();
	void unbind();
};

#endif