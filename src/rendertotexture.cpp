#include "rendertotexture.h"
#include <iostream>

//typedef void (APIENTRY * glGenFramebuffers_func)(GLsizei n, GLuint *framebuffers); glGenFramebuffers_func glGenFramebuffersEXT = NULL;
bool RenderToTexture::init()
{
	static bool firsttime = true;

	firsttime = false;
	return true;
}

RenderToTexture::RenderToTexture()
{
	width = 0;
	height = 0;
	fbo = 0;
	depthbuffer = 0;
	texture_id = 0;
	generate_mipmaps = false;

	RenderToTexture::init();
}

RenderToTexture::~RenderToTexture()
{
	if (fbo)
	{
		glDeleteFramebuffersEXT(1, &fbo);
		glDeleteRenderbuffersEXT(1, &depthbuffer);
	}
}

bool RenderToTexture::create(int width, int height, bool generate_mipmaps)
{
	this->width = (float)width;
	this->height = (float)height;
	this->generate_mipmaps = generate_mipmaps;

	glGenFramebuffersEXT(1, &fbo);
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fbo);

	glGenRenderbuffersEXT(1, &depthbuffer);
	glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, depthbuffer);

	glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT, width, height);
	glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, depthbuffer);

	glGenTextures(1, &texture_id);
	glBindTexture(GL_TEXTURE_2D, texture_id);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8,  width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, ( generate_mipmaps ? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR) );
	if (generate_mipmaps)
		this->generateMipmaps(); //glGenerateMipmapEXT(GL_TEXTURE_2D);

	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, texture_id, 0);

	GLenum status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
	if (status != GL_FRAMEBUFFER_COMPLETE_EXT)
	{
		std::cout << "Error: Framebuffer object is not completed" << std::endl;
		return false;
	}
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
	return true;
}

void RenderToTexture::enable()
{
	glBindFramebufferEXT( GL_FRAMEBUFFER_EXT, fbo);
	glPushAttrib(GL_VIEWPORT_BIT);
	glViewport(0,0, (int)width, (int)height);

}

void RenderToTexture::disable()
{
	// output goes to the FBO and it’s attached buffers
	glPopAttrib();
	glBindFramebufferEXT( GL_FRAMEBUFFER_EXT, 0);
	if (generate_mipmaps)
	{
		glBindTexture(GL_TEXTURE_2D, texture_id);
		this->generateMipmaps();
		//glGenerateMipmapEXT(GL_TEXTURE_2D);
	}
}

