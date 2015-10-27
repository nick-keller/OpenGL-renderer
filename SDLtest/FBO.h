#pragma once

#include <GL/glew.h>
#include "Texture.h"
#include "RBO.h"

class FBO
{
public:
	FBO(int width, int height, bool colorBuffer, GLenum depthBuffer);
	~FBO();

	void bind();
	void unbind();

	Texture* getColorBuffer();

private:
	GLuint m_id;
	int m_width, m_height;
	Texture* m_colorBuffer;
};

