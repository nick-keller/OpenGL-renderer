#include "Shader.h"

Shader::Shader(string pFilePath, GLenum pType) :
	m_filePath(pFilePath), m_type(pType), m_id(0)
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
		cout << "File " << m_filePath << " not found." << endl;
		return false;
	}

	if (!compileShader()) {
		cout << getLog() << endl;
		return false;
	}

	return true;
}

bool Shader::loadFromFile()
{
	// Load file
	ifstream file(m_filePath);

	if (!file) {
		return false;
	}

	string source = string((istreambuf_iterator<char>(file)), (istreambuf_iterator<char>()));
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

string Shader::getLog()
{
	GLint maxLength = 0;
	glGetShaderiv(m_id, GL_INFO_LOG_LENGTH, &maxLength);

	char *errorLog = new char[maxLength + 1];
	glGetShaderInfoLog(m_id, maxLength, &maxLength, errorLog);
	errorLog[maxLength] = '\0';

	string log(errorLog);
	delete[] errorLog;

	return log;
}
