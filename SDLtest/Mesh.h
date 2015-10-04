#pragma once

#include <GL/glew.h>
#include "VAO.h"
#include "ShaderProgram.h"

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
};

