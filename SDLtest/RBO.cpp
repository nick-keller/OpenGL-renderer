#include "RBO.h"



RBO::RBO(GLenum internalFormat, int width, int height)
{
	glGenRenderbuffers(1, &m_id);
	bind();

	glRenderbufferStorage(GL_RENDERBUFFER, internalFormat, width, height);

	unbind();
}


RBO::~RBO()
{
	glDeleteRenderbuffers(1, &m_id);
}

void RBO::bind()
{
	glBindRenderbuffer(GL_RENDERBUFFER, m_id);
}

void RBO::unbind()
{
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

GLuint RBO::getId()
{
	return m_id;
}
