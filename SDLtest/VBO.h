#pragma once

#include <GL/glew.h>

class VBO
{
public:
	VBO(GLenum target);
	~VBO();

	GLuint getId();

	void setData(const GLvoid *data, GLsizeiptr size, GLenum usage = GL_STATIC_DRAW);

	void bind();
	void unbind();

private:
	GLuint m_id;
	GLenum m_target;
};

