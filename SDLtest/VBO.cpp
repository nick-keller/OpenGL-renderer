#include "VBO.h"



VBO::VBO(GLenum target) :
	m_target(target)
{
	glGenBuffers(1, &m_id);
}


VBO::~VBO()
{
	glDeleteBuffers(1, &m_id);
}

GLuint VBO::getId()
{
	return m_id;
}

void VBO::setData(const GLvoid * data, GLsizeiptr size, GLenum usage)
{
	bind();

	glBufferData(m_target, size, data, usage);
}

void VBO::bind()
{
	glBindBuffer(m_target, m_id);
}

void VBO::unbind()
{
	glBindBuffer(m_target, 0);
}
