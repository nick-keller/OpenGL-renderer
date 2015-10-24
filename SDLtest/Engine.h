#pragma once

#include <SDL2/SDL.h>
#include "Scene.h"
#include "EngineUtils.h"

using namespace EngineUtils;

class Engine
{
public:
	enum Key {FORWARD, BACKWARD, LEFT, RIGHT, JUMP};

	Engine(float ratio);
	~Engine();

	void init();
	void update(Uint32 delta, SDL_Event& events);
	void render();

private:
	Camera m_camera;
	Scene m_scene;
	bool m_keys[5];
	int m_xRel, m_yRel;
	int m_deltaX, m_deltaY;
};

