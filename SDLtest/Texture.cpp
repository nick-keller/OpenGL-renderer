#include "Texture.h"



Texture::Texture(string pFilePath, Type pType) :
	m_type(pType)
{
	SDL_Surface *surface;

	if (loadFromFile(pFilePath, &surface)) {
		glGenTextures(1, &m_id);

		store(surface);
		setParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		setParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		SDL_FreeSurface(surface);
	}
}


Texture::~Texture()
{
	glDeleteTextures(1, &m_id);
}

GLuint Texture::getId()
{
	return m_id;
}

void Texture::bind()
{
	glActiveTexture(GL_TEXTURE0 + m_type);
	glBindTexture(GL_TEXTURE_2D, m_id);
}

void Texture::unbind()
{
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::setParameter(GLenum pName, GLint pParam)
{
	bind();

	glTexParameteri(GL_TEXTURE_2D, pName, pParam);

	unbind();
}

bool Texture::loadFromFile(std::string pFilePath, SDL_Surface **pSurface)
{
	*pSurface = IMG_Load(pFilePath.c_str());

	if (*pSurface == 0)
	{
		std::cout << "Could not load texture : " << SDL_GetError() << std::endl;
		return false;
	}

	m_hasAlpha = (*pSurface)->format->BytesPerPixel == 4;

	if (m_hasAlpha) {
		m_format = (*pSurface)->format->Rmask == 0xff ? GL_RGBA : GL_BGRA;
	}
	else {
		m_format = (*pSurface)->format->Rmask == 0xff ? GL_RGB : GL_BGR;
	}

	return true;
}

void Texture::store(SDL_Surface * pSurface)
{
	bind();

	glTexImage2D(GL_TEXTURE_2D, 0, m_hasAlpha ? GL_RGBA : GL_RGB, pSurface->w, pSurface->h, 0, m_format, GL_UNSIGNED_BYTE, pSurface->pixels);
	glGenerateMipmap(GL_TEXTURE_2D);

	unbind();
}
