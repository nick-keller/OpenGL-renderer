#pragma once

#include <GL/glew.h>
#include <string>
#include <fstream>
#include <iostream>

class Shader
{
public:
	Shader(std::string filePath, GLenum type);
	~Shader();

	GLuint getId();
	bool isShader();
	void deleteShader();

private:
	bool compile();
	bool loadFromFile();
	bool compileShader();
	std::string getLog();

private:
	std::string m_filePath;
	GLenum m_type;
	GLuint m_id;
};

