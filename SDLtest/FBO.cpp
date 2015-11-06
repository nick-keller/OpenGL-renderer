#include "FBO.h"



FBO::FBO(int width, int height, GLenum depthBuffer) :
	m_width(width), m_height(height)
{
	glGenFramebuffers(1, &m_id);
	bind();

	if (depthBuffer == GL_RENDERBUFFER) {
		RBO* depth = new RBO(GL_DEPTH24_STENCIL8, width, height);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, depth->getId());
	}

	unbind();
}


FBO::~FBO()
{
	glDeleteFramebuffers(1, &m_id);

	for (int i(0); i < m_colorBuffers.size(); ++i) {
		delete m_colorBuffers[i];
	}
}

GLuint FBO::getId()
{
	return m_id;
}

int FBO::getWidth()
{
	return m_width;
}

int FBO::getHeight()
{
	return m_height;
}

void FBO::bind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_id);
	glViewport(0, 0, m_width, m_height);
}

void FBO::unbind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FBO::clear(GLbitfield mask)
{
	glClear(mask);
}

void FBO::enableDepthTest(bool enable)
{
	if (enable) {
		glEnable(GL_DEPTH_TEST);
	}
	else {
		glDisable(GL_DEPTH_TEST);
	}
}

void FBO::copyDepthTo(FBO & buffer)
{
	glBindFramebuffer(GL_READ_FRAMEBUFFER, m_id);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, buffer.getId());
	glBlitFramebuffer(0, 0, m_width, m_height, 0, 0, buffer.getWidth(), buffer.getHeight(), GL_DEPTH_BUFFER_BIT, GL_NEAREST);
	buffer.bind();
}

void FBO::copyColorBufferTo(FBO & buffer)
{
	glBindFramebuffer(GL_READ_FRAMEBUFFER, m_id);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, buffer.getId());
	glBlitFramebuffer(0, 0, m_width, m_height, 0, 0, buffer.getWidth(), buffer.getHeight(), GL_COLOR_BUFFER_BIT, GL_NEAREST);
	bind();
}

void FBO::addColorBuffer(Texture::Type attachement, GLint internalFormat, GLenum format, GLenum type, GLint interpolation)
{
	bind();

	int attachment = m_colorBuffers.size();
	m_colorBuffers.push_back(new Texture(attachement, m_width, m_height, internalFormat, format, type, interpolation));
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + attachment, GL_TEXTURE_2D, m_colorBuffers[attachment]->getId(), 0);

	vector<GLuint> attachments;
	for (int i(0); i < m_colorBuffers.size(); ++i) {
		attachments.push_back(GL_COLOR_ATTACHMENT0 + i);
	}

	glDrawBuffers(m_colorBuffers.size(), &attachments[0]);

	unbind();
}

Texture * FBO::getColorBuffer(int index)
{
	return m_colorBuffers[index];
}
