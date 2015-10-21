#include "Engine.h"



Engine::Engine(float ratio) :
	m_camera(vec3(5, 5, 1.7), vec3(-1, -1, 0)), m_scene(ratio), m_deltaX(0), m_deltaY(0)
{
	for (int i(0); i < 5; ++i) {
		m_keys[i] = false;
	}
}


Engine::~Engine()
{
}

void Engine::init()
{
	m_scene.addEntity("cube", translate(mat4(), vec3(2, 0, 0)));
}

void Engine::update(Uint32 delta, SDL_Event& events)
{
	switch (events.type)
	{
	case SDL_KEYDOWN:
	case SDL_KEYUP:

		switch (events.key.keysym.scancode)
		{
		case SDL_SCANCODE_W:
			m_keys[FORWARD] = events.type == SDL_KEYDOWN;
			break;

		case SDL_SCANCODE_A:
			m_keys[LEFT] = events.type == SDL_KEYDOWN;
			break;

		case SDL_SCANCODE_S:
			m_keys[BACKWARD] = events.type == SDL_KEYDOWN;
			break;

		case SDL_SCANCODE_D:
			m_keys[RIGHT] = events.type == SDL_KEYDOWN;
			break;
		}

		break;

	case SDL_MOUSEMOTION:
		//m_deltaX = events.motion.xrel - m_xRel;
		//m_deltaY = events.motion.yrel - m_yRel;
		//m_xRel = events.motion.xrel;
		//m_yRel = events.motion.yrel;
		SDL_GetRelativeMouseState(&m_deltaX, &m_deltaY);

		break;
	}

	vec2 dir = vec2(m_keys[FORWARD] - m_keys[BACKWARD], m_keys[LEFT] - m_keys[RIGHT]);

	if (length(dir)) {
		dir = normalize(dir);
	}

	m_camera.moveForward(delta / 1000.f * 2 * dir.x);
	m_camera.moveLeft(delta / 1000.f * 2 * dir.y);
	m_camera.rotate(-m_deltaX / 10.f, m_deltaY / 10.f);
}

void Engine::render()
{
	m_scene.render(m_camera);
}
