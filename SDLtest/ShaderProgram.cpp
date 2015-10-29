#include "ShaderProgram.h"



ShaderProgram::ShaderProgram(string pVertexFilePath, string pFragmentFilePath) :
	m_vertexShader(pVertexFilePath, GL_VERTEX_SHADER), m_fragmentShader(pFragmentFilePath, GL_FRAGMENT_SHADER), m_id(0)
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
	updateUniform("textureDiffuse"	, Texture::DIFFUSE);
	updateUniform("textureSpecular"	, Texture::SPECULAR);
	updateUniform("textureNormal"	, Texture::NORMAL);
	updateUniform("textureBump"		, Texture::BUMP);
	updateUniform("skybox"			, Texture::CUBE);
	updateUniform("gPosition"		, Texture::GPOSITION);
	updateUniform("gNormal"			, Texture::GNORMAL);
	updateUniform("gAlbedo"			, Texture::GALBEDO);
	updateUniform("ssaoNoise"		, Texture::NOISE);
	updateUniform("ssaoLevel"		, Texture::SSAO);
	updateUniform("bloom"			, Texture::BLOOM);
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
	bindAttribLocation(VAO::COLORS  , "in_Color");
	bindAttribLocation(VAO::NORMALS , "in_Normal");
	bindAttribLocation(VAO::UV      , "in_TexCoord");
	bindAttribLocation(VAO::TANGENTS, "in_Tangent");

	if (!link())
	{
		cout << getLog() << endl;
		return false;
	}

	return true;
}

void ShaderProgram::attach(Shader & pShader)
{
	glAttachShader(m_id, pShader.getId());
}

bool ShaderProgram::link()
{
	glLinkProgram(m_id);

	// Check err
	GLint isLinked = 0;
	glGetProgramiv(m_id, GL_LINK_STATUS, &isLinked);

	return isLinked == GL_TRUE;
}

void ShaderProgram::bindAttribLocation(VAO::Prop pAttribute, char * pName)
{
	glBindAttribLocation(m_id, pAttribute, pName);
}

string ShaderProgram::getLog()
{
	GLint maxLength = 0;
	glGetProgramiv(m_id, GL_INFO_LOG_LENGTH, &maxLength);

	char *errorLog = new char[maxLength + 1];
	glGetShaderInfoLog(m_id, maxLength, &maxLength, errorLog);
	errorLog[maxLength] = '\0';

	string log(errorLog);
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

void ShaderProgram::updateUniform(string pName, Texture::Type pType)
{
	glUniform1i(getUniformLocation(pName), pType);
}

void ShaderProgram::updateUniform(string pName, vec3 pVector)
{
	glUniform3f(getUniformLocation(pName), pVector.x, pVector.y, pVector.z);
}

void ShaderProgram::updateUniform(string pName, int pInt)
{
	glUniform1i(getUniformLocation(pName), pInt);
}

void ShaderProgram::updateUniform(string pName, vector<vec3> pVectorArray)
{
	for (int i(0); i < pVectorArray.size(); ++i) {
		glUniform3fv(getUniformLocation(pName + "[" + std::to_string(i) + "]"), 1, &pVectorArray[i][0]);
	}
}

void ShaderProgram::updateUniform(string pName, mat4& pMatrix)
{
	updateUniform(getUniformLocation(pName), pMatrix);
}

void ShaderProgram::updateUniform(GLint pLocation, mat4& pMatrix)
{
	glUniformMatrix4fv(pLocation, 1, GL_FALSE, value_ptr(pMatrix));
}

void ShaderProgram::updateProjectionMatrix(mat4& pMatrix)
{
	updateUniform(m_projectionMatrix, pMatrix);
}

void ShaderProgram::updateViewMatrix(mat4& pMatrix)
{
	updateUniform(m_viewMatrix, pMatrix);
}

void ShaderProgram::updateModelMatrix(mat4& pMatrix)
{
	updateUniform(m_modelMatrix, pMatrix);
}

GLint ShaderProgram::getUniformLocation(string pName)
{
	GLint location;

	if (!m_uniforms.count(pName)) {
		location = glGetUniformLocation(m_id, pName.c_str());
		m_uniforms[pName] = location;
	}
	else {
		location = m_uniforms[pName];
	}

	return location;
}
