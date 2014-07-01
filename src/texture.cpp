#include "texture.h"
#include "utils.h"

#include <iostream> //to output
#include <cmath>



//function to create mipmaps using the GPU (much faster)
typedef void (APIENTRY *glGenerateMipmapEXT_func)( GLenum target );
glGenerateMipmapEXT_func glGenerateMipmapEXT = NULL;


Texture::Texture()
{
	width = 0;
	height = 0;

	if(glGenerateMipmapEXT == NULL) //get the extension
		glGenerateMipmapEXT = (glGenerateMipmapEXT_func) SDL_GL_GetProcAddress("glGenerateMipmapEXT");
}

bool Texture::load(const char* filename, bool mipmaps)
{
	std::string str = filename;
	std::string ext = str.substr( str.size() - 4,4 );

	if (ext == ".tga" || ext == ".TGA")
	{
		TGAInfo* tgainfo = loadTGA(filename);
		if (tgainfo == NULL)
			return false;

		this->filename = filename;

		//How to store a texture in VRAM
		glGenTextures(1, &texture_id); //we need to create an unique ID for the texture
		glBindTexture(GL_TEXTURE_2D, texture_id);	//we activate this id to tell opengl we are going to use this texture

		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);	//set the min filter
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,mipmaps ? GL_LINEAR_MIPMAP_LINEAR : GL_NEAREST ); //set the mag filter
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 4);

		if(!mipmaps) //no mipmaps
		{
			glTexImage2D(GL_TEXTURE_2D, 0, ( tgainfo->bpp == 24 ? 3 : 4), tgainfo->width, tgainfo->height, 0, ( tgainfo->bpp == 24 ? GL_BGR : GL_BGRA), GL_UNSIGNED_BYTE, tgainfo->data); //upload without mipmaps
		}
		else
		{
			if (glGenerateMipmapEXT) //extension of GL3.0 (I guess faster)
			{
				glTexImage2D(GL_TEXTURE_2D, 0, ( tgainfo->bpp == 24 ? 3 : 4), tgainfo->width, tgainfo->height, 0, ( tgainfo->bpp == 24 ? GL_BGR : GL_BGRA), GL_UNSIGNED_BYTE, tgainfo->data); //upload without mipmaps
				this->generateMipmaps(); //glGenerateMipmapEXT(GL_TEXTURE_2D);
			}
			else //use old way
			{
				#ifdef GL_VERSION_1_4
					glTexParameteri( GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE );
					glTexImage2D(GL_TEXTURE_2D, 0, ( tgainfo->bpp == 24 ? 3 : 4), tgainfo->width, tgainfo->height, 0, ( tgainfo->bpp == 24 ? GL_BGR : GL_BGRA), GL_UNSIGNED_BYTE, tgainfo->data); //upload without mipmaps
					glTexParameteri( GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_FALSE );
				#else
					//the wrong and slow way
					gluBuild2DMipmaps(GL_TEXTURE_2D, ( tgainfo->bpp == 24 ? 3 : 4), tgainfo->width, tgainfo->height, ( tgainfo->bpp == 24 ? GL_BGR : GL_BGRA) , GL_UNSIGNED_BYTE, tgainfo->data); //upload the texture and create their mipmaps
				#endif
			}
		}

		width = tgainfo->width;
		height = tgainfo->height;

		delete tgainfo->data;
		delete tgainfo;
		return true;
	}
	return false;
}

void Texture::bind()
{
	glEnable( GL_TEXTURE_2D ); //enable the textures 
	glBindTexture( GL_TEXTURE_2D, texture_id );	//enable the id of the texture we are going to use
}

void Texture::unbind()
{
	glDisable( GL_TEXTURE_2D ); //disable the textures 
	glBindTexture( GL_TEXTURE_2D, 0 );	//disable the id of the texture we are going to use
}

void Texture::UnbindAll()
{
	glDisable( GL_TEXTURE_CUBE_MAP );
	glDisable( GL_TEXTURE_2D );
	glBindTexture( GL_TEXTURE_2D, 0 );
	glBindTexture( GL_TEXTURE_CUBE_MAP, 0 );
}

void Texture::generateMipmaps()
{
	if(!glGenerateMipmapEXT) return;

	glBindTexture( GL_TEXTURE_2D, texture_id );	//enable the id of the texture we are going to use
	glGenerateMipmapEXT(GL_TEXTURE_2D);
}

Texture::TGAInfo* Texture::loadTGA(const char* filename)
{
    GLubyte TGAheader[12] = {0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    GLubyte TGAcompare[12];
    GLubyte header[6];
    GLuint bytesPerPixel;
    GLuint imageSize;
    GLuint type = GL_RGBA;

    FILE * file = fopen(filename, "rb");
    
    if ( file == NULL || fread(TGAcompare, 1, sizeof(TGAcompare), file) != sizeof(TGAcompare) ||
        memcmp(TGAheader, TGAcompare, sizeof(TGAheader)) != 0 ||
        fread(header, 1, sizeof(header), file) != sizeof(header))
    {
        if (file == NULL)
            return NULL;
        else
        {
            fclose(file);
            return NULL;
        }
    }

	TGAInfo* tgainfo = new TGAInfo;
    
    tgainfo->width = header[1] * 256 + header[0];
    tgainfo->height = header[3] * 256 + header[2];
    
    if (tgainfo->width <= 0 || tgainfo->height <= 0 || (header[4] != 24 && header[4] != 32))
    {
        fclose(file);
		delete tgainfo;
        return NULL;
    }
    
    tgainfo->bpp = header[4];
    bytesPerPixel = tgainfo->bpp / 8;
    imageSize = tgainfo->width * tgainfo->height * bytesPerPixel;
    
    tgainfo->data = (GLubyte*)malloc(imageSize);
    
    if (tgainfo->data == NULL || fread(tgainfo->data, 1, imageSize, file) != imageSize)
    {
        if (tgainfo->data != NULL)
            free(tgainfo->data);
            
        fclose(file);
		delete tgainfo;
        return NULL;
    }
    
	/* swap?
    for (GLuint i = 0; i < int(imageSize); i += bytesPerPixel)
    {
        temp = tgainfo->data[i];
        tgainfo->data[i] = tgainfo->data[i + 2];
        tgainfo->data[i + 2] = temp;
    }
	//*/
    
    fclose(file);

	return tgainfo;
}
