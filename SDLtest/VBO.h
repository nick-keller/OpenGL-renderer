#pragma once

#include <GL/glew.h>

class VBO
{
public:
	VBO(GLenum pTarget);
	~VBO();

	GLuint getId();

	void setData(const GLvoid *pData, GLsizeiptr pSize, GLenum pUsage = GL_STATIC_DRAW);

	void bind();
	void unbind();

private:
	GLuint m_id;
	GLenum m_target;
};

