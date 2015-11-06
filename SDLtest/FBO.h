#pragma once

#include <GL/glew.h>
#include <vector>
#include "Texture.h"
#include "RBO.h"

class FBO
{
public:

	enum Attachement {POSITION, NORMAL, COLOR};

	FBO(int width, int height, GLenum depthBuffer = GL_NONE);
	~FBO();

	GLuint getId();
	int getWidth();
	int getHeight();
	void bind();
	void unbind();
	void clear(GLbitfield mask);
	void enableDepthTest(bool enable = true);
	void copyDepthTo(FBO & buffer);
	void copyColorBufferTo(FBO & buffer);

	void addColorBuffer(Texture::Type attachement, GLint internalFormat = GL_RGB, GLenum format = GL_RGB, GLenum type = GL_UNSIGNED_BYTE, GLint interpolation = GL_NEAREST);
	Texture* getColorBuffer(int index = 0);

private:
	GLuint m_id;
	int m_width, m_height;
	vector<Texture*> m_colorBuffers;
};

