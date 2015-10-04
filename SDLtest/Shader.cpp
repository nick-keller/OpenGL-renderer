#include "Shader.h"

Shader::Shader(std::string filePath, GLenum type) :
	m_filePath(filePath), m_type(type), m_id(0)
{
	compile();
}


Shader::~Shader()
{
}

bool Shader::compile()
{
	// Check if shader already exist
	if (isShader()) {
		deleteShader();
	}

	m_id = glCreateShader(m_type);

	// Read file
	std::ifstream file(m_filePath);

	if (!file) {
		std::cout << "File " << m_filePath << " not found" << std::endl;
		deleteShader();

		return false;
	}

	std::string content((std::istreambuf_iterator<char>(file)), (std::istreambuf_iterator<char>()));
	file.close();

	// Send and compile
	setSource(content);
	compileShader();


	// Check err
	GLint isCompiled = 0;
	glGetShaderiv(m_id, GL_COMPILE_STATUS, &isCompiled);

	if (isCompiled == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetShaderiv(m_id, GL_INFO_LOG_LENGTH, &maxLength);

		char *errorLog = new char[maxLength + 1];
		glGetShaderInfoLog(m_id, maxLength, &maxLength, errorLog);
		errorLog[maxLength] = '\0';

		std::cout << errorLog << std::endl;

		delete[] errorLog;
		glDeleteShader(m_id);

		return false;
	}

	return true;
}

bool Shader::readFile(std::string & content)
{
	std::ifstream file(m_filePath);

	if (!file) {
		std::cout << "File " << m_filePath << " not found" << std::endl;
		return false;
	}

	content = std::string((std::istreambuf_iterator<char>(file)), (std::istreambuf_iterator<char>()));
	file.close();

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

void Shader::compileShader()
{
	glCompileShader(m_id);
}

void Shader::setSource(std::string source)
{
	const GLchar* sourceCode = source.c_str();
	glShaderSource(m_id, 1, &sourceCode, 0);
}
