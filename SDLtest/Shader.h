#pragma once

#include <GL/glew.h>
#include <string>
#include <fstream>
#include <iostream>

using namespace std;

class Shader
{
public:
	Shader(string pFilePath, GLenum pType);
	~Shader();

	GLuint getId();
	bool isShader();
	void deleteShader();

private:
	bool compile();
	bool loadFromFile();
	bool compileShader();
	string getLog();

private:
	string m_filePath;
	GLenum m_type;
	GLuint m_id;
};

