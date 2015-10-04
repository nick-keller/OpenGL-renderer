#pragma once

#include "Shader.h"

class ShaderProgram
{
public:
	ShaderProgram(std::string vertexFilePath, std::string fragmentFilePath);
	~ShaderProgram();

	void use();
	void stop();

private:
	bool compile();

private:
	GLuint m_id;
	Shader m_vertexShader;
	Shader m_fragmentShader;
};

