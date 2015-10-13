#include "VBO.h"



VBO::VBO(GLenum pTarget) :
	m_target(pTarget)
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

void VBO::setData(const GLvoid * pData, GLsizeiptr pSize, GLenum pUsage)
{
	bind();

	glBufferData(m_target, pSize, pData, pUsage);
}

void VBO::bind()
{
	glBindBuffer(m_target, m_id);
}

void VBO::unbind()
{
	glBindBuffer(m_target, 0);
}
