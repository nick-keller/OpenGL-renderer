#pragma once

#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <string>

using namespace std;

class CubeMap
{
public:
	enum Face { RIGHT, LEFT, TOP, BOTTOM, FRONT, BACK};

	CubeMap(string filePath);
	~CubeMap();

	void bind();
	void unbind();

private:
	bool loadFromFile(string pFilePath, SDL_Surface **pSurface);
	void store(SDL_Surface *pSurface, Face face);

private:
	GLuint m_id;
	bool m_hasAlpha;
	GLenum m_format;
};

