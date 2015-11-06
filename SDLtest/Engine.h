#pragma once

#include <SDL2/SDL.h>
#include <random>
#include "Scene.h"
#include "EngineUtils.h"
#include "FBO.h"
#include "ShaderProgram.h"
#include "VAO.h"

using namespace EngineUtils;

class Engine
{
private:
	struct Size {
		int w;
		int h;
	} typedef Size;

public:
	enum Key {FORWARD, BACKWARD, LEFT, RIGHT, JUMP};

	Engine(int width, int height);
	~Engine();

	void init();
	void update(Uint32 delta, SDL_Event& events);
	void render();

private:
	void generateScreenQuad();
	void generateWaterQuad();
	void generateSsaoKernel();
	void generateSsaoNoise();
	GLfloat lerp(GLfloat a, GLfloat b, GLfloat f);

private:
	Camera m_camera;
	Scene m_scene;
	bool m_keys[5];
	int m_xRel, m_yRel;
	int m_deltaX, m_deltaY;
	Size m_screen;

	FBO m_gBuffer;
	VAO m_screenQuad;
	FBO m_postFx;
	ShaderProgram m_shaderFx;
	ShaderProgram m_shaderLighting;

	FBO m_ssaoBuffer;
	FBO m_ssaoBlurBuffer;
	vector<vec3> m_ssaoKernel;
	Texture* m_ssaoNoise;
	ShaderProgram m_shaderSsao;
	ShaderProgram m_shaderBlur;

	FBO* m_pingPong[2];
	ShaderProgram m_shaderGaussian;

	VAO m_waterQuad;
	ShaderProgram m_shaderWater;
	FBO m_waterReflexion;
	FBO m_waterRefraction;
	float m_waterHeight;
	Texture m_waterDuDvMap;
	Texture m_waterNormalMap;
	float m_waterMoveFactor;
};

