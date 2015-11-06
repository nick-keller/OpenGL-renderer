#include "Engine.h"



Engine::Engine(int width, int height) :

	m_camera(vec3(5, 5, 0), vec3(-1, -1, 0)), 
	m_scene((float) width / (float) height), 
	m_screen({ width, height }),
	m_deltaX(0), m_deltaY(0), 
	m_postFx(width, height, GL_RENDERBUFFER),
	m_gBuffer(width, height, GL_RENDERBUFFER),
	m_ssaoBuffer(width, height),
	m_ssaoBlurBuffer(width, height),
	m_shaderFx("basic.vert", "basic.frag"),
	m_shaderLighting("deferredLighting.vert", "deferredLighting.frag"),
	m_shaderSsao("ssao.vert", "ssao.frag"),
	m_shaderBlur("blur.vert", "blur.frag"),
	m_ssaoNoise(NULL)
{
	for (int i(0); i < 5; ++i) {
		m_keys[i] = false;
	}

	generateScreenQuad();
	generateSsaoKernel();
	generateSsaoNoise();

	m_postFx.addColorBuffer(Texture::DIFFUSE, GL_RGB16F, GL_RGB, GL_FLOAT);
	m_postFx.addColorBuffer(Texture::BLOOM);

	m_gBuffer.addColorBuffer(Texture::GPOSITION, GL_RGBA16F, GL_RGBA, GL_FLOAT);
	m_gBuffer.addColorBuffer(Texture::GNORMAL, GL_RGB16F, GL_RGB, GL_FLOAT);
	m_gBuffer.addColorBuffer(Texture::GALBEDO);

	m_ssaoBuffer.addColorBuffer(Texture::SSAO, GL_RED, GL_RGB, GL_FLOAT);
	m_ssaoBlurBuffer.addColorBuffer(Texture::SSAO, GL_RED, GL_RGB, GL_FLOAT);
}


Engine::~Engine()
{
	delete m_ssaoNoise;
}

void Engine::init()
{
	
	//m_scene.addEntity("road1", translate(mat4(), vec3(-6, 0, 0)));
	//m_scene.addEntity("road2");
	//m_scene.addEntity("road3", translate(rotate(mat4(), 180.f, vec3(0, 0, 1)), vec3(12, 0, 0)));
	
	
	Loader& vLoader = Loader(&m_scene);
	vLoader.load();
	
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
	glClearColor(0, 0, 0, 0);
	m_gBuffer.clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	m_gBuffer.enableDepthTest();
	m_scene.render(m_camera);
	m_gBuffer.unbind();

	//**
	// SSAO
	m_ssaoBuffer.bind();
	m_ssaoBuffer.clear(GL_COLOR_BUFFER_BIT);
	m_ssaoBuffer.enableDepthTest(false);

	m_shaderSsao.use();
	m_shaderSsao.updateUniform("ssaoKernel", m_ssaoKernel);
	m_shaderSsao.updateUniform("screenWidth", m_screen.w);
	m_shaderSsao.updateUniform("screenHeight", m_screen.h);
	m_shaderSsao.updateProjectionMatrix(m_scene.getProjectionMatrix());

	m_screenQuad.bind();
	m_gBuffer.getColorBuffer(FBO::POSITION)->bind();
	m_gBuffer.getColorBuffer(FBO::NORMAL)->bind();
	m_ssaoNoise->bind();
	m_screenQuad.drawTriangles();
	m_screenQuad.unbind();

	m_shaderSsao.stop();

	m_ssaoBuffer.unbind();

	// Blur SSAO
	m_ssaoBlurBuffer.bind();
	m_shaderBlur.use();

	m_screenQuad.bind();
	m_ssaoBlurBuffer.enableDepthTest(false);
	m_ssaoBuffer.getColorBuffer()->bind();
	m_screenQuad.drawTriangles();
	m_screenQuad.unbind();

	m_shaderBlur.stop();
	m_ssaoBlurBuffer.unbind();
	//*/

	// Lighting pass
	m_postFx.bind();
	m_postFx.enableDepthTest(false);
	m_postFx.clear(GL_COLOR_BUFFER_BIT);

	m_scene.drawSky(m_camera);

	m_shaderLighting.use();
	m_shaderLighting.updateUniform("in_EyePos", m_camera.getEyePos());
	m_shaderLighting.updateViewMatrix(m_camera.getViewMatrix());
	m_screenQuad.bind();
	m_gBuffer.getColorBuffer(FBO::POSITION)->bind();
	m_gBuffer.getColorBuffer(FBO::NORMAL)->bind();
	m_gBuffer.getColorBuffer(FBO::COLOR)->bind();
	m_ssaoBlurBuffer.getColorBuffer()->bind();
	m_screenQuad.drawTriangles();
	m_screenQuad.unbind();
	m_shaderLighting.stop();

	m_postFx.unbind();

	// Post FX
	glClear(GL_COLOR_BUFFER_BIT);
	glDisable(GL_DEPTH_TEST);
	m_shaderFx.use();

	m_screenQuad.bind();
	m_postFx.getColorBuffer(0)->bind();
	//m_postFx.getColorBuffer(1)->bind();
	m_screenQuad.drawTriangles();
	m_screenQuad.unbind();

	m_shaderFx.stop();
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

void Engine::generateSsaoKernel()
{
	uniform_real_distribution<GLfloat> randomFloats(0.0, 1.0);
	default_random_engine generator;

	for (GLuint i(0); i < 16; ++i)
	{
		// Random direction
		vec3 sample(
			randomFloats(generator) * 2.0 - 1.0,
			randomFloats(generator) * 2.0 - 1.0,
			randomFloats(generator)
			);
		sample = normalize(sample);

		// Random magnitude
		sample *= randomFloats(generator);

		// More samples in the center
		GLfloat scale = GLfloat(i) / 64.0;
		scale = lerp(0.1f, 1.0f, scale * scale);
		sample *= scale;

		m_ssaoKernel.push_back(sample);
	}
}

void Engine::generateSsaoNoise()
{
	uniform_real_distribution<GLfloat> randomFloats(0.0, 1.0);
	default_random_engine generator;

	vector<vec3> ssaoNoise;
	for (GLuint i = 0; i < 16; i++)
	{
		vec3 noise(
			randomFloats(generator) * 2.0 - 1.0,
			randomFloats(generator) * 2.0 - 1.0,
			0.0f);
		ssaoNoise.push_back(noise);
	}

	m_ssaoNoise = new Texture(Texture::NOISE, 4, 4, GL_RGB16F, GL_RGB, GL_FLOAT, GL_NEAREST, GL_REPEAT, &ssaoNoise[0]);
}

GLfloat Engine::lerp(GLfloat a, GLfloat b, GLfloat f)
{
	return a + f * (b - a);
}
