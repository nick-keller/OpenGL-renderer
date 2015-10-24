#include "Engine.h"



Engine::Engine(float ratio) :
	m_camera(vec3(5, 5, 0), vec3(-1, -1, 0)), m_scene(ratio), m_deltaX(0), m_deltaY(0)
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

		case SDL_SCANCODE_C:
			m_camera.setCrouched(events.type == SDL_KEYDOWN);
			break;
		}

		break;

	case SDL_MOUSEMOTION:
		SDL_GetRelativeMouseState(&m_deltaX, &m_deltaY);
		break;
	}

	vec2 dir = vec2(m_keys[FORWARD] - m_keys[BACKWARD], m_keys[LEFT] - m_keys[RIGHT]);

	if (length(dir)) {
		dir = normalize(dir);
	}

	m_camera.moveForward(delta / 1000.f * 2.5 * dir.x);
	m_camera.moveLeft(delta / 1000.f * 2.5 * dir.y);
	m_camera.rotate(-m_deltaX / 10.f, m_deltaY / 10.f);
	m_camera.update(delta);

	// Check collisions
	vector<Entity*>* entities = m_scene.getEntities();
	for (int i(0); i < entities->size(); ++i) {
		if (intersects(m_camera.getBoundingBox(), entities->at(i)->getBoundingBox())) {
			m_camera.move(minDisplacement(entities->at(i)->getBoundingBox(), m_camera.getBoundingBox()));
		}
	}
}

void Engine::render()
{
	m_scene.render(m_camera);
}
