#pragma once

#include <map>
#include "Shader.h"
#include "VAO.h"
#include "Texture.h"

using namespace std;
using namespace glm;

class ShaderProgram
{
public:
	ShaderProgram(string pVertexFilePath, string pFragmentFilePath);
	~ShaderProgram();

	void use();
	void stop();

	bool isProgram();
	void deleteProgram();

	void updateUniform(string pName, Texture::Type pType);
	void updateUniform(string pName, vec3 pVector);
	void updateUniform(string pName, int pInt);
	void updateUniform(string pName, vector<vec3> pVectorArray);

	void updateUniform(string pName, mat4& pMatrix);
	void updateUniform(GLint pLocation, mat4& pMatrix);
	void updateProjectionMatrix(mat4& pMatrix);
	void updateViewMatrix(mat4& pMatrix);
	void updateModelMatrix(mat4& pMatrix);

private:
	bool compile();
	void attach(Shader& shader);
	bool link();
	void bindAttribLocation(VAO::Prop pAttribute, char* pName);
	string getLog();

	GLint getUniformLocation(string pName);

private:
	GLuint m_id;
	Shader m_vertexShader;
	Shader m_fragmentShader;
	GLint m_projectionMatrix;
	GLint m_viewMatrix;
	GLint m_modelMatrix;
	map<string, GLint> m_uniforms;
};

