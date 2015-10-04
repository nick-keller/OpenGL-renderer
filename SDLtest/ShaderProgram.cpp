#include "ShaderProgram.h"



ShaderProgram::ShaderProgram(std::string vertexFilePath, std::string fragmentFilePath) :
	m_vertexShader(vertexFilePath, GL_VERTEX_SHADER), m_fragmentShader(fragmentFilePath, GL_FRAGMENT_SHADER), m_id(0)
{
	compile();
}


ShaderProgram::~ShaderProgram()
{
	glDeleteProgram(m_id);
}

bool ShaderProgram::compile()
{
	if (glIsProgram(m_id) == GL_TRUE)
		glDeleteProgram(m_id);

	m_id = glCreateProgram();

	glAttachShader(m_id, m_vertexShader.getId());
	glAttachShader(m_id, m_fragmentShader.getId());

	glLinkProgram(m_id);

	// Check err
	GLint isLinked = 0;
	glGetProgramiv(m_id, GL_LINK_STATUS, &isLinked);

	if (isLinked == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetProgramiv(m_id, GL_INFO_LOG_LENGTH, &maxLength);

		char *errorLog = new char[maxLength + 1];
		glGetShaderInfoLog(m_id, maxLength, &maxLength, errorLog);
		errorLog[maxLength] = '\0';

		std::cout << errorLog << std::endl;

		delete[] errorLog;
		glDeleteProgram(m_id);

		return false;
	}

	return true;
}

void ShaderProgram::use()
{
	glUseProgram(m_id);
}

void ShaderProgram::stop()
{
	glUseProgram(0);
}
