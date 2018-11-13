#include "Texture.h"
#include "SOIL/SOIL.h"

#include <iostream>

Texture::~Texture()
{
	Unload();
}

bool Texture::LoadFromFile(const std::string &file)
{
	TexObj = SOIL_load_OGL_texture(file.c_str(), SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);
	if (TexObj == 0)
	{
		std::cout << "Texture failed to load. \n" << SOIL_last_result << '\n';
		return false;

		//modify the texture
		glBindTexture(GL_TEXTURE_2D, TexObj);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); //if texture is really small
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // u axis
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); // v axis

		return true;
	}
}
void Texture::Unload()
{
	if (TexObj != 0)
	{
		//remove data from GPU
		glDeleteTextures(1, &TexObj);
		TexObj = 0;
	}
}

void Texture::Bind()
{
	glBindTexture(GL_TEXTURE_2D, TexObj);
}
void Texture::UnBind()
{
	glBindTexture(GL_TEXTURE_2D, 0);
}

//The handle to the texture object
GLuint TexObj = 0;
