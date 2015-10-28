#pragma once

#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <string>

using namespace std;

class Texture
{
public:
	enum Type {DIFFUSE, SPECULAR, NORMAL, BUMP, CUBE, GPOSITION, GNORMAL, GALBEDO};

	Texture(string pFilePath, Type pType);
	Texture(Type pType, int width, int height, GLint internalFormat = GL_RGB, GLenum format = GL_RGB, GLenum type = GL_UNSIGNED_BYTE, GLint interpolation = GL_NEAREST);
	~Texture();

	GLuint getId();

	void bind();
	void unbind();

	void setParameter(GLenum pName, GLint pParam);

private:
	bool loadFromFile(string pFilePath, SDL_Surface **pSurface);
	void store(SDL_Surface *pSurface);

private:
	GLuint m_id;
	Type m_type;
	bool m_hasAlpha;
	GLenum m_format;
};

