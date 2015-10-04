#include "Shader.h"

Shader::Shader(std::string filePath, GLenum type) :
	m_filePath(filePath), m_type(type), m_id(0)
{
	if (!compile()) {
		deleteShader();
	}
}


Shader::~Shader()
{
	deleteShader();
}

bool Shader::compile()
{
	// Check if shader already exist
	if (isShader()) {
		deleteShader();
	}

	m_id = glCreateShader(m_type);

	if (!loadFromFile()) {
		std::cout << "File " << m_filePath << " not found." << std::endl;
		return false;
	}

	if (!compileShader()) {
		std::cout << getLog() << std::endl;
		return false;
	}

	return true;
}

bool Shader::loadFromFile()
{
	// Load file
	std::ifstream file(m_filePath);

	if (!file) {
		return false;
	}

	std::string source = std::string((std::istreambuf_iterator<char>(file)), (std::istreambuf_iterator<char>()));
	file.close();

	// Upload source to GPU
	const GLchar* sourceCode = source.c_str();
	glShaderSource(m_id, 1, &sourceCode, 0);

	return true;
}

GLuint Shader::getId()
{
	return m_id;
}

bool Shader::isShader()
{
	return glIsShader(m_id) == GL_TRUE;
}

void Shader::deleteShader()
{
	glDeleteShader(m_id);
}

bool Shader::compileShader()
{
	glCompileShader(m_id);

	// Check err
	GLint isCompiled = 0;
	glGetShaderiv(m_id, GL_COMPILE_STATUS, &isCompiled);

	return isCompiled == GL_TRUE;
}

std::string Shader::getLog()
{
	GLint maxLength = 0;
	glGetShaderiv(m_id, GL_INFO_LOG_LENGTH, &maxLength);

	char *errorLog = new char[maxLength + 1];
	glGetShaderInfoLog(m_id, maxLength, &maxLength, errorLog);
	errorLog[maxLength] = '\0';

	std::string log(errorLog);
	delete[] errorLog;

	return log;
}
