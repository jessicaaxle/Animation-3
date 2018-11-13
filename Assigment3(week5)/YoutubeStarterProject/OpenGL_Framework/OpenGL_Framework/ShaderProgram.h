#pragma once
#include <GL\glew.h>

#include <MiniMath\Core.h>
#include <string>

class ShaderProgram
{
public:
	ShaderProgram();
	~ShaderProgram();

	//- Loads a vertex shader amd a fragment shader, and places them in a program
	bool Load(const std::string &vertFile, const std::string &fragFile);
	bool IsLoaded() const;
	//- Clear all data from OpenGL
	void UnLoad();
	bool LinkProgram();

	//- Use / detach the shader from use
	void Bind() const;
	void UnBind();

	//- Requires a re-link before OpenGL will register the change
	void AddAttribute(unsigned int index, const std::string &attribName);

	//- Returns -1 if the attribute does not exist
	int GetAttribLoacation(const std::string &attribName); //Attributes are the slots in which your VBOs feed in
	//- Returns -1 if uniform does not exist
	int GetUniformLocation(const std::string &uniformName); //Any variables that you can set on the fly (colour info, matrices, etc.)

	//- Send data to shaders
	void SendUniform(const std::string &name, int integer);
	void SendUniform(const std::string &name, unsigned int unsigned_nteger);
	void SendUniform(const std::string &name, float scalar);
	void SendUniform(const std::string &name, const vec2 &vector);
	void SendUniform(const std::string &name, const vec3 &vector);
	void SendUniform(const std::string &name, const vec4 &vector);
	void SendUniformMat3(const std::string &name, float *matrix, bool transpose);
	void SendUniformMat4(const std::string &name, float *matrix, bool transpose);
	

private:
	bool _IsInit = false;
	GLuint _VertexShader	= 0;
	GLuint _FragShader		= 0;
	GLuint _Program			= 0;

	std::string ReadFile(const std::string &fileName) const;
	//Take one of the two shaders in the class and compile them, if there is an error it will output those errors
	bool CompileShader(GLuint shader) const;
	//Will output the error list of the shader in the event that the shader did not compile properly **super useful for workflow**
	void OutputShaderLog(GLuint shader) const;
	//Outputs debug information relative to the entire program
	void OutputProgramLog() const; 


};