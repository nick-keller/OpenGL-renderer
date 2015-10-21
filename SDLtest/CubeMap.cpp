#include "CubeMap.h"



CubeMap::CubeMap(string filePath)
{
	glGenTextures(1, &m_id);
	bind();

	string faces[] = {
		filePath + "_right.jpg",
		filePath + "_left.jpg",
		filePath + "_top.jpg",
		filePath + "_bottom.jpg",
		filePath + "_front.jpg",
		filePath + "_back.jpg",
	};

	SDL_Surface *surface;

	for (int i(0); i < 6; ++i) {

		if (loadFromFile(faces[i], &surface)) {
			store(surface, (Face) i);
			SDL_FreeSurface(surface);
		}
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	unbind();
}


CubeMap::~CubeMap()
{
}

void CubeMap::bind()
{
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_id);
}

void CubeMap::unbind()
{
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

bool CubeMap::loadFromFile(string pFilePath, SDL_Surface **pSurface)
{
	*pSurface = IMG_Load(pFilePath.c_str());
	const char* err = SDL_GetError();
	err;

	if (*pSurface == 0)
	{
		cout << "Could not load texture : " << SDL_GetError() << endl;
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

void CubeMap::store(SDL_Surface * pSurface, Face face)
{
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + face, 0, m_hasAlpha ? GL_RGBA : GL_RGB, pSurface->w, pSurface->h, 0, m_format, GL_UNSIGNED_BYTE, pSurface->pixels);
}
