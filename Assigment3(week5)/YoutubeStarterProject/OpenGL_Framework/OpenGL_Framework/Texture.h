#pragma once
#include <GL\glew.h>
#include <string>

class Texture
{
public:
	Texture() = default;
	~Texture();

	bool LoadFromFile(const std::string &file);
	void Unload();

	void Bind();
	void UnBind();

	//The handle to the texture object
	GLuint TexObj = 0;


};