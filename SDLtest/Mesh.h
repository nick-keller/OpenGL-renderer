#pragma once

#include <GL/glew.h>
#include "VAO.h"

class Mesh
{
public:
	Mesh();
	~Mesh();

	GLuint getVaoId();
	VAO getVao();

	void render();

private:
	VAO m_data;
	GLuint m_vao;
	GLuint m_vboVertices;
	GLuint m_vboFaces;
};

