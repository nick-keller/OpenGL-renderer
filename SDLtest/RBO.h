#pragma once

#include <GL/glew.h>

class RBO
{
public:
	RBO(GLenum internalFormat, int width, int height);
	~RBO();

	void bind();
	void unbind();

	GLuint getId();

private:
	GLuint m_id;
};

