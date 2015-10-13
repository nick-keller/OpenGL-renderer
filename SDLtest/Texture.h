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
	enum Type {DIFFUSE, SPECULAR, NORMAL, BUMP};

	Texture(string pFilePath, Type pType);
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

