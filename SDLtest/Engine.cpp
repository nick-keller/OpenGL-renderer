#include "Engine.h"



Engine::Engine(int width, int height) :
	m_camera(vec3(5, 5, 0), vec3(-1, -1, 0)), 
	m_scene((float) width / (float) height), 
	m_deltaX(0), m_deltaY(0), 
	m_postFx(width, height, GL_RENDERBUFFER),
	m_gBuffer(width, height, GL_RENDERBUFFER),
	m_shaderFx("basic.vert", "basic.frag"),
	m_shaderLighting("deferredLighting.vert", "deferredLighting.frag")
{
	m_screen = {width, height};

	for (int i(0); i < 5; ++i) {
		m_keys[i] = false;
	}

	generateScreenQuad();

	m_postFx.addColorBuffer(Texture::DIFFUSE);

	m_gBuffer.addColorBuffer(Texture::GPOSITION, GL_RGB16F, GL_RGB, GL_FLOAT);
	m_gBuffer.addColorBuffer(Texture::GNORMAL, GL_RGB16F, GL_RGB, GL_FLOAT);
	m_gBuffer.addColorBuffer(Texture::GALBEDO);
}


Engine::~Engine()
{
}

void Engine::init()
{
	m_scene.addEntity("cube", translate(mat4(), vec3(2, 0, 0)));
	m_scene.addEntity("cube", translate(scale(mat4(), vec3(2, 2, 2)), vec3(0, 0, -1.4)));
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

	m_camera.moveForward(delta / 1000.f * 2.5f * dir.x);
	m_camera.moveLeft(delta / 1000.f * 2.5f * dir.y);
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
	// Geometry pass
	m_gBuffer.bind();
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	m_scene.render(m_camera);
	m_gBuffer.unbind();
	
	glClearColor(.1f, .1f, .1f, 1);
	glClear(GL_COLOR_BUFFER_BIT);

	// Sky
	m_scene.drawSky(m_camera);

	// Lighting pass
	m_shaderLighting.use();
	m_shaderLighting.updateUniform("in_EyePos", m_camera.getEyePos());
	m_screenQuad.bind();
	glDisable(GL_DEPTH_TEST);
	m_gBuffer.getColorBuffer(FBO::POSITION)->bind();
	m_gBuffer.getColorBuffer(FBO::NORMAL)->bind();
	m_gBuffer.getColorBuffer(FBO::COLOR)->bind();
	m_screenQuad.drawTriangles();
	m_screenQuad.unbind();
	m_shaderLighting.stop();
}

void Engine::generateScreenQuad()
{
	vec3 vertices[] = {
		vec3(-1, -1, 0),
		vec3(-1, 1, 0),
		vec3(1, -1, 0),
		vec3(1, 1, 0)
	};

	vec2 uvs[] = {
		vec2(0, 0),
		vec2(0, 1),
		vec2(1, 0),
		vec2(1, 1)
	};

	uvec3 faces[] = {
		uvec3(3, 1, 0),
		uvec3(3, 0, 2)
	};

	m_screenQuad.storeVertices(vector<vec3>(vertices, vertices + 4));
	m_screenQuad.storeUvs(vector<vec2>(uvs, uvs + 4));
	m_screenQuad.storeFaces(vector<uvec3>(faces, faces + 2));
}
