#include "fbo.h"
#include <cassert>
FBO::FBO()
{
	fbo_id = 0;
	color_textures[0] = color_textures[1] = color_textures[2] = color_textures[3] = NULL;
	depth_texture = NULL;

	renderbuffer_color = 0;
	renderbuffer_depth = 0;
	num = 0;
	owns_textures = false;
}

FBO::~FBO()
{
	if (owns_textures)
	{
		for (int i = 0; i < 4; ++i)
		{
			Texture* t = color_textures[i];
			if (!t)
				continue;
			delete t;
		}
		if (depth_texture)
			delete depth_texture;
	}

	if(fbo_id)
		glDeleteFramebuffers(1, &fbo_id);
	if (renderbuffer_color)
		glDeleteRenderbuffersEXT(1, &renderbuffer_color);
	if (renderbuffer_depth)
		glDeleteRenderbuffersEXT(1, &renderbuffer_depth);
}

bool FBO::create( int width, int height, int format, int type, int num_textures )
{
	assert(glGetError() == GL_NO_ERROR);
	assert(width &&& height);
	assert(num_textures < 5);

	if (this->color_textures[0] && this->width == width && this->height == height && this->color_textures[0]->format == format && this->color_textures[0]->type == type)
		return true;

	this->width = width;
	this->height = height;
	owns_textures = true;

	//create textures
	for (int i = 0; i < num_textures; ++i)
		color_textures[i] = new Texture(width, height, format, type, false );
	depth_texture = new Texture(width, height, GL_DEPTH_COMPONENT, GL_UNSIGNED_INT, false);

	glGenFramebuffersEXT(1, &fbo_id);
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fbo_id);

	//glFramebufferParameteri(GL_DRAW_FRAMEBUFFER, GL_FRAMEBUFFER_DEFAULT_WIDTH, width);
	//glFramebufferParameteri(GL_DRAW_FRAMEBUFFER, GL_FRAMEBUFFER_DEFAULT_HEIGHT, height);
	//glFramebufferParameteri(GL_DRAW_FRAMEBUFFER, GL_FRAMEBUFFER_DEFAULT_SAMPLES, 4);

	memset(bufs, 0, sizeof(bufs));

	for (int i = 0; i < num_textures; ++i)
	{
		Texture* color_texture = color_textures[i];
		glFramebufferTexture2DEXT( GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT + i, GL_TEXTURE_2D, color_texture->texture_id, 0);
		bufs[i] = GL_COLOR_ATTACHMENT0_EXT + i;
	}

	glFramebufferTexture2DEXT( GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depth_texture->texture_id, 0 );

	GLenum status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
	if (status != GL_FRAMEBUFFER_COMPLETE_EXT)
	{
		std::cout << "Error: Framebuffer object is not completed: " << status  << std::endl;
		return false;
	}
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
	assert(glGetError() == GL_NO_ERROR);
	return true;
}

bool FBO::createFromTextures(Texture* color_texture, Texture* color_textureB, Texture* depth_texture)
{
	assert(color_texture);
	assert(glGetError() == GL_NO_ERROR);
	width = (int)color_texture->width;
	height = (int)color_texture->height;
	owns_textures = false;

	if(fbo_id == 0)
		glGenFramebuffersEXT(1, &fbo_id);

	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fbo_id);

	memset(bufs, 0, sizeof(bufs));

	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT + 0, GL_TEXTURE_2D, color_texture->texture_id, 0);
	bufs[0] = GL_COLOR_ATTACHMENT0_EXT;
	color_textures[0] = color_texture;

	if (color_textureB)
	{
		glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT + 1, GL_TEXTURE_2D, color_textureB->texture_id, 0);
		bufs[1] = GL_COLOR_ATTACHMENT0_EXT + 1;
		color_textures[1] = color_textureB;
	}
	else
		glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT + 1, GL_TEXTURE_2D, NULL, 0);

	if (depth_texture)
	{
		glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depth_texture->texture_id, 0);
		this->depth_texture = depth_texture;
	}
	else
		glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, NULL, 0);

	GLenum status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
	if (status != GL_FRAMEBUFFER_COMPLETE_EXT)
	{
		std::cout << "Error: Framebuffer object is not completed: " << status << std::endl;
		return false;
	}
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
	assert(glGetError() == GL_NO_ERROR);
	return true;
}

bool FBO::createDepthOnly(int width, int height)
{
	owns_textures = true;

	glGenFramebuffersEXT(1, &fbo_id);
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fbo_id);

	glGenRenderbuffersEXT(1, &renderbuffer_color);
	glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, renderbuffer_color);

	glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_RGBA, width, height);
	glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER_EXT, renderbuffer_color);

	depth_texture = new Texture(width, height, GL_DEPTH_COMPONENT, GL_UNSIGNED_INT, false);
	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depth_texture->texture_id, 0);

	GLenum status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
	if (status != GL_FRAMEBUFFER_COMPLETE_EXT)
	{
		std::cout << "Error: Framebuffer object is not completed" << std::endl;
		return false;
	}
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
	return true;
}

void FBO::bind()
{
	assert(glGetError() == GL_NO_ERROR);
	Texture* tex = color_textures[0] ? color_textures[0] : depth_texture;
	assert(tex && "framebuffer without texture");
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fbo_id);
	glPushAttrib(GL_VIEWPORT_BIT);

	glDrawBuffers(4, bufs);

	glViewport(0, 0, (int)tex->width, (int)tex->height);
	assert(glGetError() == GL_NO_ERROR);
}

GLenum one_buffer = GL_BACK;

void FBO::unbind()
{
	// output goes to the FBO and it’s attached buffers
	glPopAttrib();
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
	//glDrawBuffers(1, &one_buffer);
	assert(glGetError() == GL_NO_ERROR);
}