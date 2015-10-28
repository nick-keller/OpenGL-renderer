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

void FBO::bind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_id);
}

void FBO::unbind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FBO::addColorBuffer(Texture::Type attachement, GLint internalFormat, GLenum format, GLenum type)
{
	bind();

	int attachment = m_colorBuffers.size();
	m_colorBuffers.push_back(new Texture(attachement, m_width, m_height, internalFormat, format, type));
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
