#include "ShaderProgram.h"



ShaderProgram::ShaderProgram(std::string vertexFilePath, std::string fragmentFilePath) :
	m_vertexShader(vertexFilePath, GL_VERTEX_SHADER), m_fragmentShader(fragmentFilePath, GL_FRAGMENT_SHADER), m_id(0)
{
	if (!compile()) {
		deleteProgram();
		return;
	}

	// Cache those for quick access
	m_projectionMatrix = getUniformLocation("projection");
	m_viewMatrix = getUniformLocation("view");
	m_modelMatrix = getUniformLocation("model");

	// Bind texture units
	use();
	updateUniform("textureDiffuse", Texture::DIFFUSE);
	updateUniform("textureSpecular", Texture::SPECULAR);
	updateUniform("textureNormal", Texture::NORMAL);
	updateUniform("textureBump", Texture::BUMP);
	stop();
}


ShaderProgram::~ShaderProgram()
{
	deleteProgram();
}

bool ShaderProgram::compile()
{
	// Check if program already exist
	if (isProgram()) {
		deleteProgram();
	}

	m_id = glCreateProgram();

	// If one shader did not compile stop here
	if (!m_vertexShader.isShader() || !m_fragmentShader.isShader()) {
		return false;
	}

	attach(m_vertexShader);
	attach(m_fragmentShader);

	bindAttribLocation(VAO::VERTICES, "in_Vertex");
	bindAttribLocation(VAO::COLORS, "in_Color");
	bindAttribLocation(VAO::NORMALS, "in_Normal");
	bindAttribLocation(VAO::UV, "in_TexCoord");
	bindAttribLocation(VAO::TANGENTS, "in_Tangent");

	if (!link())
	{
		std::cout << getLog() << std::endl;
		return false;
	}

	return true;
}

void ShaderProgram::attach(Shader & shader)
{
	glAttachShader(m_id, shader.getId());
}

bool ShaderProgram::link()
{
	glLinkProgram(m_id);

	// Check err
	GLint isLinked = 0;
	glGetProgramiv(m_id, GL_LINK_STATUS, &isLinked);

	return isLinked == GL_TRUE;
}

void ShaderProgram::bindAttribLocation(VAO::Prop attribute, char * name)
{
	glBindAttribLocation(m_id, attribute, name);
}

std::string ShaderProgram::getLog()
{
	GLint maxLength = 0;
	glGetProgramiv(m_id, GL_INFO_LOG_LENGTH, &maxLength);

	char *errorLog = new char[maxLength + 1];
	glGetShaderInfoLog(m_id, maxLength, &maxLength, errorLog);
	errorLog[maxLength] = '\0';

	std::string log(errorLog);
	delete[] errorLog;

	return log;
}

void ShaderProgram::use()
{
	glUseProgram(m_id);
}

void ShaderProgram::stop()
{
	glUseProgram(0);
}

bool ShaderProgram::isProgram()
{
	return glIsProgram(m_id) == GL_TRUE;
}

void ShaderProgram::deleteProgram()
{
	glDeleteProgram(m_id);
}

void ShaderProgram::updateUniform(std::string name, Texture::Type type)
{
	glUniform1i(getUniformLocation(name), type);
}

void ShaderProgram::updateUniform(std::string name, glm::vec3 vector)
{
	glUniform3f(getUniformLocation(name), vector.x, vector.y, vector.z);
}

void ShaderProgram::updateUniform(std::string name, glm::mat4& matrix)
{
	updateUniform(getUniformLocation(name), matrix);
}

void ShaderProgram::updateUniform(GLint location, glm::mat4& matrix)
{
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
}

void ShaderProgram::updateProjectionMatrix(glm::mat4& matrix)
{
	updateUniform(m_projectionMatrix, matrix);
}

void ShaderProgram::updateViewMatrix(glm::mat4& matrix)
{
	updateUniform(m_viewMatrix, matrix);
}

void ShaderProgram::updateModelMatrix(glm::mat4& matrix)
{
	updateUniform(m_modelMatrix, matrix);
}

GLint ShaderProgram::getUniformLocation(std::string name)
{
	GLint location;

	if (!m_uniforms.count(name)) {
		location = glGetUniformLocation(m_id, name.c_str());
		m_uniforms[name] = location;
	}
	else {
		location = m_uniforms[name];
	}

	return location;
}
