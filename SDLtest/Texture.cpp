#include "Texture.h"



Texture::Texture(std::string filePath, Type type) :
	m_type(type)
{
	SDL_Surface *surface;

	if (loadFromFile(filePath, &surface)) {
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

void Texture::setParameter(GLenum pname, GLint param)
{
	bind();

	glTexParameteri(GL_TEXTURE_2D, pname, param);

	unbind();
}

bool Texture::loadFromFile(std::string filePath, SDL_Surface **surface)
{
	*surface = IMG_Load(filePath.c_str());

	if (*surface == 0)
	{
		std::cout << "Could not load texture : " << SDL_GetError() << std::endl;
		return false;
	}

	m_hasAlpha = (*surface)->format->BytesPerPixel == 4;

	if (m_hasAlpha) {
		m_format = (*surface)->format->Rmask == 0xff ? GL_RGBA : GL_BGRA;
	}
	else {
		m_format = (*surface)->format->Rmask == 0xff ? GL_RGB : GL_BGR;
	}

	return true;
}

void Texture::store(SDL_Surface * surface)
{
	bind();

	glTexImage2D(GL_TEXTURE_2D, 0, m_hasAlpha ? GL_RGBA : GL_RGB, surface->w, surface->h, 0, m_format, GL_UNSIGNED_BYTE, surface->pixels);
	glGenerateMipmap(GL_TEXTURE_2D);

	unbind();
}
