#pragma once

#include <map>
#include "Shader.h"
#include "VAO.h"
#include "Texture.h"

class ShaderProgram
{
public:
	ShaderProgram(std::string vertexFilePath, std::string fragmentFilePath);
	~ShaderProgram();

	void use();
	void stop();

	bool isProgram();
	void deleteProgram();

	void updateUniform(std::string name, Texture::Type type);
	void updateUniform(std::string name, glm::vec3 vector);

	void updateUniform(std::string name, glm::mat4& matrix);
	void updateUniform(GLint location, glm::mat4& matrix);
	void updateProjectionMatrix(glm::mat4& matrix);
	void updateViewMatrix(glm::mat4& matrix);
	void updateModelMatrix(glm::mat4& matrix);

private:
	bool compile();
	void attach(Shader& shader);
	bool link();
	void bindAttribLocation(VAO::Prop attribute, char* name);
	std::string getLog();

	GLint getUniformLocation(std::string name);

private:
	GLuint m_id;
	Shader m_vertexShader;
	Shader m_fragmentShader;
	GLint m_projectionMatrix;
	GLint m_viewMatrix;
	GLint m_modelMatrix;
	std::map<std::string, GLint> m_uniforms;
};

