#pragma once

#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <string>

class Texture
{
public:
	enum Type {DIFFUSE, SPECULAR, NORMAL, BUMP};

	Texture(std::string filePath, Type type);
	~Texture();

	GLuint getId();

	void bind();
	void unbind();

	void setParameter(GLenum pname, GLint param);

private:
	bool loadFromFile(std::string filePath, SDL_Surface **surface);
	void store(SDL_Surface *surface);

private:
	GLuint m_id;
	Type m_type;
	bool m_hasAlpha;
	GLenum m_format;
};

