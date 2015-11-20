#include "Engine.h"



Engine::Engine(int width, int height) :
	m_camera(vec3(5, 5, 0), vec3(-1, -1, 0)),
	m_scene((float)width / (float)height),

	m_screen({ width, height }),
	m_deltaX(0), m_deltaY(0),
	m_postFx(width, height, GL_RENDERBUFFER),
	m_gBuffer(width, height, GL_RENDERBUFFER),
	m_ssaoBuffer(width, height),
	m_ssaoBlurBuffer(width, height),
	m_waterReflexion(width, height, GL_RENDERBUFFER),
	m_waterRefraction(width, height),
	m_shaderFx("basic.vert", "basic.frag"),
	m_shaderLighting("deferredLighting.vert", "deferredLighting2.frag"),
	m_shaderSsao("ssao.vert", "ssao.frag"),
	m_shaderBlur("blur.vert", "blur.frag"),
	m_shaderGaussian("gaussian.vert", "gaussian.frag"),
	m_shaderWater("water.vert", "water.frag"),
	m_shaderRain("rain.vert", "rain.frag"),
	m_ssaoNoise(NULL),
	m_waterHeight(-.05),
	m_rain("rain.obj"),
	m_waterDuDvMap("water_dudv.jpg", Texture::DUDV),
	m_waterNormalMap("water_normal.jpg", Texture::NORMAL),
	m_waterMoveFactor(0),
	m_rainMoveFactor(0),
	m_rainTexture("rain.jpg", Texture::DIFFUSE)
{
	for (int i(0); i < 5; ++i) {
		m_keys[i] = false;
	}

	generateScreenQuad();
	generateWaterQuad();
	generateSsaoKernel();
	generateSsaoNoise();

	m_postFx.addColorBuffer(Texture::DIFFUSE, GL_RGB16F, GL_RGB, GL_FLOAT);
	m_postFx.addColorBuffer(Texture::BLOOM, GL_RGB, GL_RGB, GL_UNSIGNED_BYTE, GL_LINEAR);

	m_gBuffer.addColorBuffer(Texture::GPOSITION, GL_RGBA16F, GL_RGBA, GL_FLOAT);
	m_gBuffer.addColorBuffer(Texture::GNORMAL, GL_RGB16F, GL_RGB, GL_FLOAT);
	m_gBuffer.addColorBuffer(Texture::GALBEDO);

	m_ssaoBuffer.addColorBuffer(Texture::SSAO, GL_RED, GL_RGB, GL_FLOAT);
	m_ssaoBlurBuffer.addColorBuffer(Texture::SSAO, GL_RED, GL_RGB, GL_FLOAT);

	m_waterReflexion.addColorBuffer(Texture::REFLEXION);
	m_waterRefraction.addColorBuffer(Texture::DIFFUSE);

	int f = 1;
	m_pingPong[0] = new FBO(width /f, height /f);
	m_pingPong[1] = new FBO(width /f, height /f);
	m_pingPong[0]->addColorBuffer(Texture::BLOOM, GL_RGB16F, GL_RGB, GL_FLOAT, GL_LINEAR);
	m_pingPong[1]->addColorBuffer(Texture::BLOOM, GL_RGB16F, GL_RGB, GL_FLOAT, GL_LINEAR);
}


Engine::~Engine()
{
	delete m_ssaoNoise;
}

void Engine::init()
{
	
	
	
	Loader& vLoader = Loader(&m_scene);
	vLoader.load();
	/*
	m_scene.init();
	
	m_scene.addEntity("road1", translate(mat4(), vec3(-6, 0, 0)));
	m_scene.addEntity("elbox", translate(mat4(), vec3(3.6, 0, 0)));
	m_scene.addEntity("road2");
	m_scene.addEntity("aldente", rotate(translate(mat4(), vec3(0, 0, 0)), -90.f, vec3(0, 0, 1)));
	m_scene.addEntity("front1", rotate(translate(mat4(), vec3(-5, -3.6, 0)), -90.f, vec3(0, 0, 1)));
	m_scene.addEntity("lamp", rotate(translate(mat4(), vec3(0, 3.5, 0)), 180.f, vec3(0, 0, 1)));
	m_scene.addEntity("stop", rotate(translate(mat4(), vec3(0, 3.5, 0)), 180.f, vec3(0, 0, 1)));
	m_scene.addEntity("lamp", rotate(translate(mat4(), vec3(-2, 3.5, 0)), 180.f, vec3(0, 0, 1)));
	m_scene.addEntity("lamp", rotate(translate(mat4(), vec3(-4, 3.5, 0)), 180.f, vec3(0, 0, 1)));
	m_scene.addEntity("lamp", rotate(translate(mat4(), vec3(-6, 3.5, 0)), 180.f, vec3(0,0,1)));
	m_scene.addEntity("road3", translate(rotate(mat4(), 180.f, vec3(0, 0, 1)), vec3(12, 0, 0)));
	*/
	m_toggleCollision = false;
}



void Engine::update(Uint32 delta, SDL_Event& events)
{
	switch (events.type)
	{
	case SDL_KEYDOWN:
		switch (events.key.keysym.scancode)
		{
			case SDL_SCANCODE_F:
				m_toggleCollision = ( (events.type == SDL_KEYDOWN) == m_toggleCollision ) ? false : true ;
			break;
		}

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

	m_waterMoveFactor += delta / 1000.f * 0.03f;
	if (m_waterMoveFactor >= 1) {
		m_waterMoveFactor -= 1;
	}

	m_rainMoveFactor += delta / 1000.f * 0.6f;
	if (m_rainMoveFactor >= 1) {
		m_rainMoveFactor -= 1;
	}

	// Check collisions
	if (m_toggleCollision) {
		vector<Entity*>* entities = m_scene.getEntities();
		for (int i(0); i < entities->size(); ++i) {
			if (intersects(m_camera.getBoundingBox(), entities->at(i)->getBoundingBox())) {
				m_camera.move(minDisplacement(entities->at(i)->getBoundingBox(), m_camera.getBoundingBox()));
			}
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

	// Water reflexion
	m_waterReflexion.bind();
	glClearColor(0, 0, 0, 0);
	glEnable(GL_CLIP_DISTANCE0);
	m_waterReflexion.clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	m_waterReflexion.enableDepthTest();
	m_scene.fastRender(m_camera.getReflexted(m_waterHeight));
	glDisable(GL_CLIP_DISTANCE0);
	m_waterReflexion.unbind();

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

	// Render extra details
	m_gBuffer.copyDepthTo(m_postFx);
	m_postFx.copyColorBufferTo(m_waterRefraction);
	m_postFx.enableDepthTest();
	//m_scene.drawAxis();
	//m_scene.drawNormals();

	// Render water
	m_shaderWater.use();
	m_shaderWater.updateViewMatrix(m_camera.getViewMatrix());
	m_shaderWater.updateProjectionMatrix(m_scene.getProjectionMatrix());
	m_shaderWater.updateUniform("moveFactor", m_waterMoveFactor);
	m_waterReflexion.getColorBuffer()->bind();
	m_waterRefraction.getColorBuffer()->bind();
	m_gBuffer.getColorBuffer(FBO::POSITION)->bind();
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	m_waterDuDvMap.bind();
	m_waterNormalMap.bind();
	m_waterQuad.bind();
	m_waterQuad.drawTriangles();
	m_waterQuad.unbind();
	m_shaderWater.stop();

	// Render rain
	m_shaderRain.use();
	m_shaderRain.updateViewMatrix(m_camera.getViewMatrix());
	m_shaderRain.updateProjectionMatrix(m_scene.getProjectionMatrix());
	m_shaderRain.updateUniform("moveFactor", m_rainMoveFactor);
	m_shaderRain.updateUniform("offset", m_camera.getPosition());
	m_gBuffer.getColorBuffer(FBO::POSITION)->bind();
	glDepthMask(false);

	m_rain.bind();
	m_rainTexture.bind();
	m_rain.draw();
	m_rain.unbind();

	m_shaderRain.stop();
	glDisable(GL_BLEND);
	glDepthMask(true);

	m_postFx.unbind();

	// Bloom
	GLboolean horizontal = true, first_iteration = true;
	m_shaderGaussian.use();
	for (GLuint i = 0; i < 4; i++)
	{
		m_pingPong[horizontal]->bind();
		m_shaderGaussian.updateUniform("horizontal", horizontal);

		if (first_iteration) {
			m_postFx.getColorBuffer(1)->bind();
		}
		else {
			m_pingPong[!horizontal]->getColorBuffer()->bind();
		}

		m_screenQuad.bind();
		m_screenQuad.drawTriangles();
		m_screenQuad.unbind();

		horizontal = !horizontal;
		first_iteration = false;
	}
	m_pingPong[0]->unbind();

	// Post FX
	glClear(GL_COLOR_BUFFER_BIT);
	glDisable(GL_DEPTH_TEST);
	glViewport(0, 0, m_screen.w, m_screen.h);
	m_shaderFx.use();

	m_screenQuad.bind();
	m_postFx.getColorBuffer(0)->bind();
	m_pingPong[horizontal]->getColorBuffer()->bind();
	m_gBuffer.getColorBuffer(FBO::POSITION)->bind();
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

void Engine::generateWaterQuad()
{
	vec3 vertices[] = {
		vec3( 100, 100, m_waterHeight),
		vec3( 100,-100, m_waterHeight),
		vec3(-100, 100, m_waterHeight),
		vec3(-100,-100, m_waterHeight)
	};

	uvec3 faces[] = {
		uvec3(0, 2, 1), 
		uvec3(2, 3, 1)
	};

	m_waterQuad.storeVertices(vector<vec3>(vertices, vertices + 4));
	m_waterQuad.storeFaces(vector<uvec3>(faces, faces + 2));
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
