#include "FBO.h"



FBO::FBO(int width, int height, bool colorBuffer, GLenum depthBuffer) :
	m_width(width), m_height(height), m_colorBuffer(NULL)
{
	glGenFramebuffers(1, &m_id);
	bind();

	if (colorBuffer) {
		m_colorBuffer = new Texture(width, height);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_colorBuffer->getId(), 0);
	}

	if (depthBuffer == GL_RENDERBUFFER) {
		RBO* depth = new RBO(GL_DEPTH24_STENCIL8, width, height);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, depth->getId());
	}

	unbind();
}


FBO::~FBO()
{
	glDeleteFramebuffers(1, &m_id);
}

void FBO::bind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_id);
}

void FBO::unbind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

Texture * FBO::getColorBuffer()
{
	return m_colorBuffer;
}
